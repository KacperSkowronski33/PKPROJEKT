#include "WarstwaUslug.h"
#include "GeneratorWartosci.h"
#include "ModelARX.h"
#include "RegulatorPID.h"

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

void WarstwaUslug::ustawLimity(double minU,double maxU) 
{
	m_uar.setLimity(minU, maxU, -10.0, 10.0);
}

void WarstwaUslug::ustawParametrySin(int okres, double amplituda, double skladowaStala) 
{
	m_sygSin.setOkres(okres);
	m_sygSin.setAmplituda(amplituda);
	m_sygSin.setStala(skladowaStala);
}

void WarstwaUslug::ustawParametryProst(int okres, double wypelnienie, double amplituda, double skladowaStala)
{
	m_sygPro.setOkres(okres);
	m_sygPro.setWypelnienie(wypelnienie);
	m_sygPro.setAmplituda(amplituda);
	m_sygPro.setStala(skladowaStala);

}

void WarstwaUslug::ustawRodzajSygnalu(RodzajSygnalu rodzaj)
{
	m_rodzajSyg = rodzaj;
}

WarstwaUslug::WarstwaUslug()
	: m_uar(), // Wywo³uje nowy konstruktor domyœlny ProstyUAR
	m_rodzajSyg(RodzajSygnalu::Brak),
	m_krokSym(0) {
}