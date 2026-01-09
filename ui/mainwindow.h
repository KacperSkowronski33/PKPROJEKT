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
    //void aktSym();

private:
    Ui::MainWindow *ui;
    WarstwaUslug m_warstwaUslugowa;
    QTimer *m_timer;
};
#endif // MAINWINDOW_H
