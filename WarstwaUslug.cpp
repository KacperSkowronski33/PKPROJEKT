#include "WarstwaUslug.h"

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

