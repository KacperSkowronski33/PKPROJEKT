#include "GeneratorWartosci.h"

void GeneratorWartosci::setOkres(int okres)
{
	m_T = okres;
}

void GeneratorWartosci::setAmplituda(double amplituda)
{
	m_A = amplituda;
}

void GeneratorWartosci::setStala(double stala)
{
	m_S = stala;
}

int GeneratorWartosci::getOkres()
{
	return m_T;
}

double GeneratorWartosci::getAmplituda()
{
	return m_A;
}

double GeneratorWartosci::getStala()
{
	return m_S;
}

void GeneratorProstokat::setWypelnienie(double wyp)
{
	m_p = wyp;
}

double GeneratorProstokat::getWypelnienie()
{
	return m_p;
}

void GeneratorProstokat::setAmplituda(double amplituda)
{
	if (amplituda < 0) m_A = 0.0;
	else m_A = amplituda;
}

double GeneratorProstokat::wynikProstokat(int krok)
{
	if ((krok % getOkres()) < getWypelnienie() * getOkres()) {
        return getStala() + getAmplituda();
	}
	else {
        return getStala() - getAmplituda();
	}
}

double GeneratorSinusoida::wynikSin(int krok)
{
    int T = getOkres();
    if (T <= 0) return getStala();
	double wynik = 0.0;
	double mod = ((krok % getOkres()) / static_cast<double>(getOkres())) * 2 * M_PI;
	wynik = getAmplituda() * sin(mod) + getStala();
	return wynik;
}
