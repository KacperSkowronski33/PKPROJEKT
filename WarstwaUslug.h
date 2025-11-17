#pragma once
#include "ProstyUAR.h"


class WarstwaUslug
{
public:
	enum class RodzajSygnalu { Brak, Sinusoida, Prostokatny };
	struct Wykres { double wartZad, wartReg, uchyb, sterowanie, p, i, d; };  //wartzad i reg -> 1 wykres, p,i,d -> 2 wykres, uchyb -> 3 wykres, ster -> 4 wykres

	Wykres wykonajKrokSym(); //do tworzenia wykresow

	void zapiszKonfig();
	void wczytajKonfig();
private:
	ProstyUAR m_uar;
	RodzajSygnalu m_rodzajSyg;
	GeneratorSinusoida m_sygSin; //do generowania sinusoidy
	GeneratorProstokat m_sygPro; //do generowania prostokata
	int m_krokSym;				 //krok symulacji

	const char* m_nazwaPliku;
};

