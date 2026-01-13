#pragma once
#include <cmath>
#define M_PI 3.14

class GeneratorWartosci
{
protected:
	int m_T; //okres
	double m_A; //amplituda
	double m_S; //skladowa stala
public:
	void setOkres(int okres);
	virtual void setAmplituda(double amplituda);
	void setStala(double stala);

	int getOkres();
	double getAmplituda();
	double getStala();
};

class GeneratorSinusoida : public GeneratorWartosci
{
public:
	double wynikSin(int krok);
};

class GeneratorProstokat : public GeneratorWartosci
{
private:
	double m_p; //wypelnienie
public:
	void setWypelnienie(double wyp);
	double getWypelnienie();
	void setAmplituda(double amplituda) override;
	double wynikProstokat(int krok);
};
