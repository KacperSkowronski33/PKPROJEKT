#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtCharts>
#include "WarstwaUslug.h"

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

private:
    Ui::MainWindow *ui;
    WarstwaUslug m_warstwaUslugowa;
    QTimer *m_timer;
    int m_interwal;
    double m_czasSym;

    //wykres 1
    void generujWykres_ZadReg();
    void skalowanieY_ZadReg();
    QLineSeries *m_wykres_Zad;
    QLineSeries *m_wykres_Reg;
    QValueAxis *m_X_wykres_1;
    QValueAxis *m_Y_wykres_1;

    //wykres 2
    void generujWykres_uchyb();
    void skalowanieY_uchyb();
    QLineSeries *m_wykres_uchyb;
    QValueAxis *m_X_wykres_2;
    QValueAxis *m_Y_wykres_2;

    //wykres 3

    //wykres 4

};
#endif // MAINWINDOW_H
