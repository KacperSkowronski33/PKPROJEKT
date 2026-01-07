#pragma once
//wartosci domyslene k=1 Ti=0 Td=0 T=0.2
class RegulatorPID
{
private:

	double m_k;
	double m_Td;
	double m_Ti;
	double m_okresprobkowania;
	double m_poprzedniuchyb;
	double m_sumauchyb;
	//do wykresow
	double m_skladowaP;
	double m_skladowaI;
	double m_skladowaD;
	//true = pod suma, false = przed suma
	bool m_trybcalkowania;
public:

	enum class LiczCalk { Wew, Zew };

	RegulatorPID(double k = 1.0, double Ti = 0.0, double Td = 0.0, double T = 0.2);

	//settery
	void ustawParametry(double k, double Ti, double Td);
	void ustawczasprobkowania(double T);
	void ustawTrybCalkowania(bool tryb);
	void setStalaCalk(double Ti);
	void setLiczCalk(LiczCalk tryb);

	//metody
	void resetujcalke();
	void resetujrozniczke();
	double symuluj(double uchyb);


	//gettery
	double pobierzP() const { return m_skladowaP; }
	double pobierzI() const { return m_skladowaI; }
	double pobierzD() const { return m_skladowaD; }

};



