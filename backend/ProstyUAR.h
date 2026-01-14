#pragma once
#include "GeneratorWartosci.h"
#include "ModelARX.h"
#include "RegulatorPID.h"

class ProstyUAR
{
private:
	double m_sygWe;
	double m_krokPoprzedni; // yi-1
	ModelARX m_modelARX;
	RegulatorPID m_regulatorPID;
	double m_uchybPoprzedni; //pola potrzebne do wykresów
	double m_WartSterPoprzedni;
    bool m_czyPrzedSuma = true;
public:
	ProstyUAR();
	ProstyUAR(ModelARX modelARX, RegulatorPID regulatorPID) : m_modelARX(modelARX), m_regulatorPID(regulatorPID), m_krokPoprzedni(0.0)
	{

	}


	void setOdchylenie(double odchylenie);
	void setLimity(double minU, double maxU, double minY, double maxY);
	void resetPID();
    void resetARX();
	void resetSymulacji();

	void setPID(double k, double Ti, double Td);
	void setARX(const std::vector<double>& A, const std::vector<double>& B, int opoznienie, double zaklocenie);

	double getKrok();
	double getUchyb();
	double getWartSter();

	double getP() const;
	double getI() const;
	double getD() const;

    void ustawTryb(bool czyPrzedSuma);

	double symuluj(double sygWej);

};

