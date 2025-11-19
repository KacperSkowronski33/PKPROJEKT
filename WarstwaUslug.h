#pragma once
#include "ProstyUAR.h"
#include "GeneratorWartosci.h"

class WarstwaUslug
{
public:
	enum class RodzajSygnalu { Brak, Sinusoida, Prostokatny };
	struct Wykres { double wartZad, wartReg, uchyb, sterowanie, p, i, d; };  //wartzad i reg -> 1 wykres, p,i,d -> 2 wykres, uchyb -> 3 wykres, ster -> 4 wykres
	WarstwaUslug();
	Wykres wykonajKrokSym(); //do tworzenia wykresow

	//settery
	void ustawParametryPID(double k, double Ti, double Td);
	void resetPID();
	void ustawParametryARX(const std::vector<double>& A, const std::vector<double>& B, int k);
	void ustawOdchylenie(double odchylenie);
	void ustawLimity(double minU, double maxU);
	void ustawRodzajSygnalu(RodzajSygnalu rodzaj);
	void ustawParametrySin(int okres, double amplituda, double skladowaStala);
	void ustawParametryProst(int okres, double wypelnienie, double amplituda, double skladowaStala);


	void resetSymulacji();
private:
	ProstyUAR m_uar;
	RodzajSygnalu m_rodzajSyg;
	GeneratorSinusoida m_sygSin; //do generowania sinusoidy
	GeneratorProstokat m_sygPro; //do generowania prostokata
	int m_krokSym;				 //krok symulacji

	const char* m_nazwaPliku;
};

