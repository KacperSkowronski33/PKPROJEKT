#include "RegulatorPID.h"

RegulatorPID::RegulatorPID(double k, double Ti, double Td, double T)
{
	m_k = k;
	m_Ti = Ti;
	m_Td = Td;
	m_okresprobkowania = T;
	m_poprzedniuchyb = 0.0;
	m_sumauchyb = 0.0;
	m_skladowaP = 0.0;
	m_skladowaI = 0.0;
	m_skladowaD = 0.0;
    m_trybcalkowania = false; //domyslnie przed suma
}
void RegulatorPID::ustawParametry(double k, double Ti, double Td)
{
	m_k = k;
	m_Ti = Ti;
	m_Td = Td;
}
void RegulatorPID::ustawczasprobkowania(double T)
{
	m_okresprobkowania = T;
}
void RegulatorPID::ustawTrybCalkowania(bool tryb)
{
	m_trybcalkowania = tryb;
}
void RegulatorPID::resetujcalke()
{
	m_sumauchyb = 0.0;
}
void RegulatorPID::resetujrozniczke()
{
	m_poprzedniuchyb = 0.0;
}

void RegulatorPID::setLiczCalk(LiczCalk tryb)
{
	bool nowyTrybWew = (tryb == LiczCalk::Wew);

	if (nowyTrybWew != m_trybcalkowania)
	{
		if (m_Ti != 0.0)
		{
			if (nowyTrybWew)
				m_sumauchyb /= m_Ti;
			else
				m_sumauchyb *= m_Ti;
		}
	}
	m_trybcalkowania = nowyTrybWew;
}

void RegulatorPID::setStalaCalk(double Ti)
{
	if (Ti == 0.0)
	{
		m_Ti = 0.0;
		return;
	}

	m_Ti = Ti;
}

double RegulatorPID::symuluj(double uchyb)
{
	m_skladowaP = m_k * uchyb;
	if (m_Ti != 0.0)
	{
        double dt = m_okresprobkowania;

		if (m_trybcalkowania)
            //m_sumauchyb += uchyb / m_Ti;
            m_sumauchyb += (uchyb) / m_Ti;
		else
            //m_sumauchyb += uchyb;
            m_sumauchyb += uchyb;

		if (m_trybcalkowania)
			m_skladowaI = m_sumauchyb;
		else
			m_skladowaI = m_sumauchyb / m_Ti;
	}
	else
		m_skladowaI = 0.0;

    // m_skladowaD = m_Td * (uchyb - m_poprzedniuchyb);
    // m_poprzedniuchyb = uchyb; //wersja poprzednia

    double dt =m_okresprobkowania;
    if(dt > 0.0) {
        m_skladowaD = m_Td * ((uchyb - m_poprzedniuchyb));
    } else {
        m_skladowaD = 0.0;
    }
    m_poprzedniuchyb = uchyb;

	return m_skladowaP + m_skladowaI + m_skladowaD;
}
