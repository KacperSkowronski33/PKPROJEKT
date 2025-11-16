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
public:
	ProstyUAR(ModelARX modelARX, RegulatorPID regulatorPID) : m_modelARX(modelARX), m_regulatorPID(regulatorPID), m_krokPoprzedni(0.0)
	{

	}
	double symuluj(double sygWej);

};

