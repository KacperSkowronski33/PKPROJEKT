#include "ModelARX.h"

void ModelARX::setLimit(double minSter, double maxSter, double minReg, double maxReg)
{
	m_minSter = minSter;
	m_maxSter = maxSter;
	m_minReg = minReg;
	m_maxReg = maxReg;
	m_czyWlasnyLimit = true;
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

void ModelARX::setWekA(const std::vector<double>& A)
{
	m_wspWektorA = A;
	m_kolejkaWyj.resize(A.size());
}

void ModelARX::setWekB(const std::vector<double>& B)
{
	m_wspWektorB = B;
	m_kolejkaWej.resize(B.size());
}

void ModelARX::setOpoznienie(int opoznienie)
{
	m_opoznienie = opoznienie;
	m_buforOpoznienia.resize(opoznienie); //automatycznie sie dostosowywuje wiec nie trzeba sprawdzac warunku 
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

	if (m_odchStd != 0.0) { //jezeli 0.0 to wart domyslna - nie ma sensu uruchamiac generatora liczb losowych 
		wartReg += genZaklocen();
	}

	if (getCzyLimit()) {
		double maxY = getCzyWlasnyLim() ? getMaxReg() : 10.0;
		double minY = getCzyWlasnyLim() ? getMinReg() : -10.0;

		if (wartReg > maxY) wartReg = maxY;
		else if (wartReg < minY) wartReg = minY;
		//wartReg = setLimitWart(wartReg);
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

void ModelARX::setOdchylenie(double odchylenie)
{
	m_odchStd = odchylenie;
}

double ModelARX::genZaklocen()
{
	double wynik = m_rozkladNormalny(m_generator) * m_odchStd;
	return wynik;
}

void ModelARX::resetARX()
{
	std::fill(m_kolejkaWej.begin(), m_kolejkaWej.end(), 0.0);
	std::fill(m_kolejkaWyj.begin(), m_kolejkaWyj.end(), 0.0);
	std::fill(m_buforOpoznienia.begin(), m_buforOpoznienia.end(), 0.0);
}
