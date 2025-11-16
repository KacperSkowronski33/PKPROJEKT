#include "ModelARX.h"

void ModelARX::setLimit(double minSter, double maxSter, double minReg, double maxReg)
{
	m_minSter = minSter;
	m_maxSter = maxSter;
	m_minReg = minReg;
	m_maxSter = maxReg;
}

void ModelARX::czyLimit(bool czyLimit)
{
	m_czyLimit = czyLimit;
}

bool ModelARX::getCzyLimit()
{
	return m_czyLimit;
}

void ModelARX::setCzyWlasnyLim(bool wlasnyLim)
{
	m_czyWlasnyLimit = wlasnyLim;
}

bool ModelARX::getCzyWlasnyLim()
{
	return m_czyWlasnyLimit;
}

double ModelARX::getMinSter()
{
	return m_minSter;
}

double ModelARX::getMaxSter()
{
	return m_maxSter;
}

double ModelARX::getMinReg()
{
	return m_minReg;
}

double ModelARX::getMaxReg()
{
	return m_maxReg;
}
double ModelARX::symuluj(double aktualnaWartSter)
{
	if (getCzyLimit()) {
		aktualnaWartSter = setLimitWart(aktualnaWartSter);
	}

	double aktBufor = 0.0;
	if (m_buforOpoznienia.size() >= m_opoznienie) {
		aktBufor = m_buforOpoznienia[m_opoznienie - 1];
	}

	m_buforOpoznienia.push_front(aktualnaWartSter);
	if (m_buforOpoznienia.size() > m_opoznienie) m_buforOpoznienia.pop_back();

	m_kolejkaWej.push_front(aktBufor);
	if (m_kolejkaWej.size() > m_wspWektorB.size()) m_kolejkaWej.pop_back();

	double sumaB = 0.0;
	for (int i = 0; i < m_wspWektorB.size(); i++) {
		if (i < m_kolejkaWej.size()) {
			sumaB += m_wspWektorB[i] * m_kolejkaWej[i];
		}
	}

	double sumaA = 0.0;
	for (int i = 0; i < m_wspWektorA.size(); i++) {
		if (i < m_kolejkaWyj.size()) {
			sumaA += m_wspWektorA[i] * m_kolejkaWyj[i];
		}
	}

	double wartReg = sumaB - sumaA;

	if (getCzyLimit()) {
		wartReg = setLimitWart(wartReg);
	}

	m_kolejkaWyj.push_front(wartReg);
	if (m_kolejkaWyj.size() > m_wspWektorA.size()) m_kolejkaWyj.pop_back();

	return wartReg;
}

double ModelARX::setLimitWart(double wartSter)
{
	double wartPoLim;
	if (!getCzyWlasnyLim()) {
		if (wartSter > 10) wartPoLim = 10;
		else if (wartSter < -10) wartPoLim = -10;
		else wartPoLim = wartSter;
	}
	else {
		if (wartSter > getMaxSter()) wartPoLim = getMaxSter();
		else if (wartSter < getMinSter()) wartPoLim = getMinSter();
		else wartPoLim = wartSter;
	}
	return wartPoLim;
}

