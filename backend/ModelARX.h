#pragma once
#include <vector>
#include <deque>
#include <random>

class ModelARX
{
private:
	std::vector<double> m_wspWektorA;
	std::vector<double> m_wspWektorB;
	std::deque<double> m_kolejkaWej;
	std::deque<double> m_kolejkaWyj;
	std::deque<double> m_buforOpoznienia;
	double m_minSter, m_maxSter;
	double m_minReg, m_maxReg;
	int m_opoznienie;
	double m_zaklocenie;
	bool m_czyLimit;
	bool m_czyWlasnyLimit;

	std::mt19937 m_generator; 
	std::normal_distribution<double> m_rozkladNormalny;
	double m_odchStd;
public:
	ModelARX(const std::vector<double>& wektorA, const std::vector<double>& wektorB, int opoznienie = 1, double zaklocenie = 0.0) : m_wspWektorA(wektorA), m_wspWektorB(wektorB), m_opoznienie(opoznienie), m_zaklocenie(zaklocenie) {
		m_kolejkaWej = std::deque<double>(wektorB.size(), 0.0);
		m_kolejkaWyj = std::deque<double>(wektorA.size(), 0.0);
		m_buforOpoznienia = std::deque<double>(opoznienie, 0.0);
		setOdchylenie(zaklocenie);
	}
	void setLimit(double minSter, double maxSter, double minReg, double maxReg);
	void czyLimit(bool czyLimit);
	void setOdchylenie(double odchylenie);
	void setCzyWlasnyLim(bool wlasnyLim);
	void setWekA(const std::vector<double>& A);
	void setWekB(const std::vector<double>& B);
	void setOpoznienie(int opoznienie);

	void resetARX();

	bool getCzyLimit();
	bool getCzyWlasnyLim();
	double getMinSter();
	double getMaxSter();
	double getMinReg();
	double getMaxReg();

	double symuluj(double aktualnaWartSter);
	double setLimitWart(double wartSter);
	double genZaklocen();
};