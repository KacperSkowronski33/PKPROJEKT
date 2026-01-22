#include "WarstwaUslug.h"
#include "GeneratorWartosci.h"

WarstwaUslug::Wykres WarstwaUslug::wykonajKrokSym()
{
	double wartZadana = 0.0;
	switch (m_rodzajSyg)
	{
	case RodzajSygnalu::Brak:
		wartZadana = 1.0;
		break;
	case RodzajSygnalu::Sinusoida:
		wartZadana = m_sygSin.wynikSin(m_krokSym);
		break;
	case RodzajSygnalu::Prostokatny:
		wartZadana = m_sygPro.wynikProstokat(m_krokSym);
		break;
	}

	m_krokSym++;
	double wynik = m_uar.symuluj(wartZadana);
	
	Wykres wykresy;
	wykresy.wartZad = wartZadana;
	wykresy.wartReg = wynik;
	wykresy.uchyb = m_uar.getUchyb();
	wykresy.sterowanie = m_uar.getWartSter();
	wykresy.p = m_uar.getP();
	wykresy.i = m_uar.getI();
	wykresy.d = m_uar.getD();

	return wykresy;
}

void WarstwaUslug::resetSymulacji() 
{
	m_krokSym = 0;
	m_uar.resetSymulacji();
}

void WarstwaUslug::sygnalSymulacji()
{
    double wartZadana = 0.0;
    switch (m_rodzajSyg)
    {
    case RodzajSygnalu::Brak:
        wartZadana = 1.0;
        break;
    case RodzajSygnalu::Sinusoida:
        wartZadana = m_sygSin.wynikSin(m_krokSym);
        break;
    case RodzajSygnalu::Prostokatny:
        wartZadana = m_sygPro.wynikProstokat(m_krokSym);
        break;
    }

    m_krokSym++;
    double wynik = m_uar.symuluj(wartZadana);

    Wykres dane;
    dane.wartZad = wartZadana;
    dane.wartReg = wynik;
    dane.uchyb = m_uar.getUchyb();
    dane.sterowanie = m_uar.getWartSter();
    dane.p = m_uar.getP();
    dane.i = m_uar.getI();
    dane.d = m_uar.getD();

    emit aktDanychUslugi(dane);
}

int WarstwaUslug::okresDoProbki(double okres)
{
    int okresProbka = static_cast<int>((okres * 1000.0) /m_interwalUslugi);
    if(okresProbka < 1) okresProbka = 1;
    return okresProbka;
}

void WarstwaUslug::ustawParametryPID(double k, double Ti, double Td)
{
	m_uar.setPID(k, Ti, Td);
}

void WarstwaUslug::resetPID()
{
	m_uar.resetPID();
}

void WarstwaUslug::ustawParametryARX(const std::vector<double>& A, const std::vector<double>& B, int k)
{
	m_uar.setARX(A, B, k, 0.0);
}

void WarstwaUslug::ustawOdchylenie(double odchylenie) 
{
	m_uar.setOdchylenie(odchylenie);
}

void WarstwaUslug::ustawLimity(double minU,double maxU, double minY, double maxY, bool czyLimit)
{
    m_uar.setLimity(minU, maxU, minY, maxY, czyLimit);
}

void WarstwaUslug::ustawParametrySin(double okres, double amplituda, double skladowaStala)
{
    int okresProbka = okresDoProbki(okres);
    m_sygSin.setOkres(okresProbka);
	m_sygSin.setAmplituda(amplituda);
	m_sygSin.setStala(skladowaStala);
}

void WarstwaUslug::ustawParametryProst(double okres, double wypelnienie, double amplituda, double skladowaStala)
{
    int okresProbka = okresDoProbki(okres);
    m_sygPro.setOkres(okresProbka);
	m_sygPro.setWypelnienie(wypelnienie);
	m_sygPro.setAmplituda(amplituda);
	m_sygPro.setStala(skladowaStala);
}

void WarstwaUslug::ustawRodzajSygnalu(RodzajSygnalu rodzaj)
{
	m_rodzajSyg = rodzaj;
}

WarstwaUslug::WarstwaUslug(QObject *parent) : QObject(parent), m_interwalUslugi(200), m_uar(), m_rodzajSyg(RodzajSygnalu::Brak) ,m_krokSym(0)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &WarstwaUslug::sygnalSymulacji);
}

void WarstwaUslug::startSym(int interwal)
{
    m_timer->start(interwal);
}

void WarstwaUslug::stopSym()
{
    m_timer->stop();
}

void WarstwaUslug::setInterwal(int interwal)
{
    m_timer->setInterval(interwal);
}

void WarstwaUslug::ustawTrybCalkowania(bool czyPrzedSuma) {
    m_uar.ustawTryb(czyPrzedSuma);
}

void WarstwaUslug::resetujCalke()
{
    m_uar.resetCalki();
};

void WarstwaUslug::resetujRozniczke()
{
    m_uar.resetRozniczki();
};
