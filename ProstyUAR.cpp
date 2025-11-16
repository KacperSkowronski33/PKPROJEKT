#include "ProstyUAR.h"

double ProstyUAR::symuluj(double sygWej)
{
    double wynik = sygWej - m_krokPoprzedni; //akt uchyb
    double aktWartSter = m_regulatorPID.symuluj(wynik);
    double obliczonaWartSter = m_modelARX.symuluj(aktWartSter);
    m_krokPoprzedni = obliczonaWartSter;
    return obliczonaWartSter;
}
