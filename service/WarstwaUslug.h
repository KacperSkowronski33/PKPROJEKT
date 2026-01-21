#pragma once
#include "ProstyUAR.h"
#include "GeneratorWartosci.h"
#include <QTimer>
#include <QObject>

class WarstwaUslug : public QObject
{
    Q_OBJECT
public:
    explicit WarstwaUslug(QObject *parent = nullptr);
    void startSym(int interwal);
    void stopSym();
    void setInterwal(int interwal);

	enum class RodzajSygnalu { Brak, Sinusoida, Prostokatny };
    struct Wykres { double wartZad, wartReg, uchyb, sterowanie, p, i, d; };  //wartzad i reg -> 1 wykres, p,i,d -> 2 wykres, uchyb -> 3 wykres, ster -> 4 wykres
    Wykres wykonajKrokSym(); //do tworzenia wykresow -> teraz sygnal aktDanychUslugi robi to samo wiec te metode mozna usunac jak wszystko bedzie dzialac

	//settery
	void ustawParametryPID(double k, double Ti, double Td);
	void resetPID();
	void ustawParametryARX(const std::vector<double>& A, const std::vector<double>& B, int k);
	void ustawOdchylenie(double odchylenie);
    void ustawLimity(double minU, double maxU, double minY, double maxY, bool czyLimit);
	void ustawRodzajSygnalu(RodzajSygnalu rodzaj);
    void ustawParametrySin(double okres, double amplituda, double skladowaStala);
    void ustawParametryProst(double okres, double wypelnienie, double amplituda, double skladowaStala);
    void ustawTrybCalkowania(bool czyPrzedSuma);
	void resetSymulacji();
    void resetujCalke();
    void resetujRozniczke();

signals:
    void aktDanychUslugi(WarstwaUslug::Wykres dane);

private slots:
    void sygnalSymulacji(); //glowna petla symulacji

private:
    QTimer *m_timer;
    int m_interwalUslugi; //do timera
	ProstyUAR m_uar;
	RodzajSygnalu m_rodzajSyg;
	GeneratorSinusoida m_sygSin; //do generowania sinusoidy
	GeneratorProstokat m_sygPro; //do generowania prostokata
	int m_krokSym;				 //krok symulacji
    int okresDoProbki(double okres);

};

