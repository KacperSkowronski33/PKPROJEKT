#include "ProstyUAR.h"

void ProstyUAR::setPID(double k, double Ti, double Td)
{
    m_regulatorPID.ustawParametry(k, Ti, Td);
}

void ProstyUAR::setARX(const std::vector<double>& A, const std::vector<double>& B, int opoznienie, double zaklocenie)
{
    m_modelARX.setWekA(A);
    m_modelARX.setWekB(B);
    m_modelARX.setOpoznienie(opoznienie);
    m_modelARX.setOdchylenie(zaklocenie);
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

void ProstyUAR::resetPID()
{
    m_regulatorPID.resetujcalke();
    m_regulatorPID.resetujrozniczke();
}

void ProstyUAR::resetSymulacji()
{
    m_krokPoprzedni = 0.0;
    m_uchybPoprzedni = 0.0;
    m_WartSterPoprzedni = 0.0;
    m_regulatorPID.resetujcalke();
    m_regulatorPID.resetujrozniczke();
    m_modelARX.resetARX();
}

void ProstyUAR::setOdchylenie(double odchylenie)
{
    // Komenda do ModelARX: ustawienie tylko odchylenia dla Test�w 9 i 10
    m_modelARX.setOdchylenie(odchylenie);
}

void ProstyUAR::setLimity(double minU, double maxU, double minY, double maxY)
{
    // Komenda do ModelARX: ustawienie wszystkich 4 limit�w
    m_modelARX.setLimit(minU, maxU, minY, maxY);
}
ProstyUAR::ProstyUAR()
    : m_modelARX({ 0.0 }, { 0.0 }, 1, 0.0), // Inicjalizacja ModelARX zerami
    m_regulatorPID(0.0, 0.0, 0.0),    // Inicjalizacja RegulatorPID zerami
    m_krokPoprzedni(0.0),
    m_uchybPoprzedni(0.0),
    m_WartSterPoprzedni(0.0) {
}
void ProstyUAR::ustawTryb(bool czyPrzedSuma) {
    m_czyPrzedSuma = czyPrzedSuma;
}
