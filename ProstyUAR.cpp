#include "ProstyUAR.h"

void ProstyUAR::setPID(double k, double Ti, double Td)
{
    m_regulatorPID.ustawParametry(k, Ti, Td);
}

void ProstyUAR::setARX(const std::vector<double>& A, const std::vector<double>& B, int opoznienie, double zaklocenie)
{

}

double ProstyUAR::getKrok()
{
    return m_krokPoprzedni;
}

double ProstyUAR::getUchyb()
{
    return m_uchybPoprzedni;
}

double ProstyUAR::getWartSter()
{
    return m_WartSterPoprzedni;
}

double ProstyUAR::getP() const
{
    return m_regulatorPID.pobierzP();
}

double ProstyUAR::getI() const
{
    return m_regulatorPID.pobierzI();
}

double ProstyUAR::getD() const
{
    return m_regulatorPID.pobierzD();
}

double ProstyUAR::symuluj(double sygWej)
{
    double wynik = sygWej - m_krokPoprzedni; //akt uchyb
    m_uchybPoprzedni = wynik;
    double aktWartSter = m_regulatorPID.symuluj(wynik); //sterowanie u
    m_WartSterPoprzedni = aktWartSter;
    double obliczonaWartSter = m_modelARX.symuluj(aktWartSter);
    m_krokPoprzedni = obliczonaWartSter;
    return obliczonaWartSter;
}
