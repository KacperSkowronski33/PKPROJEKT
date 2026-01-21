#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include <QFile>
#include "WarstwaUslug.h"
#include "arx_set_param.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void aktSym();

    void on_start_button_clicked();

    void on_stop_button_clicked();

    void on_paramARX_button_clicked();

    void on_reset_button_clicked();

    void on_actionZapisz_konfiguracj_triggered();

    void on_actionWczytaj_konfiguracj_triggered();

    void getDaneSym(WarstwaUslug::Wykres dane);

    void on_resest_cal_button_clicked();

    void on_reset_roz_button_clicked();

private:
    Ui::MainWindow *ui;
    WarstwaUslug *m_warstwaUslug;
    int m_interwal;
    double m_czasSym;

    //wykres 1
    void generujWykres_ZadReg();
    QLineSeries *m_wykres_Zad;
    QLineSeries *m_wykres_Reg;
    QValueAxis *m_X_wykres_1;
    QValueAxis *m_Y_wykres_1;

    //wykres 2
    void generujWykres_uchyb();
    QLineSeries *m_wykres_uchyb;
    QValueAxis *m_X_wykres_2;
    QValueAxis *m_Y_wykres_2;

    //wykres 3
    void generujWykres_ster();
    QLineSeries *m_wykres_ster;
    QValueAxis *m_X_wykres_3;
    QValueAxis *m_Y_wykres_3;

    //wykres 4
    void generujWykres_PID();
    QLineSeries *m_wykres_P;
    QLineSeries *m_wykres_I;
    QLineSeries *m_wykres_D;
    QValueAxis *m_X_wykres_4;
    QValueAxis *m_Y_wykres_4;

    arx_set_param *m_arxWindow;
    void skalowanieY(QValueAxis *oy, const QList<QLineSeries*> &dane);

    void ZapiszDoPliku();
    void WczytajZPliku();
    void utworzOknoARX();

};


#endif // MAINWINDOW_H
