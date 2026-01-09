#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::aktSym);

    //wykres 1
    generujWykres_ZadReg();
    //wykres 2
    generujWykres_uchyb();
    //wykres 3

    //wykres 4
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::aktSym()
{
    m_interwal = ui->param_interwal->value();

    WarstwaUslug::Wykres dane_wykres = m_warstwaUslugowa.wykonajKrokSym();
    m_wykres_Zad->append(m_czasSym, dane_wykres.wartZad);
    m_wykres_Reg->append(m_czasSym, dane_wykres.wartReg);

    m_wykres_uchyb->append(m_czasSym, dane_wykres.uchyb);

    m_czasSym += (ui->param_interwal->value() /1000.0);

    double czasPrzesunieciaOsi = 10.0;
    if(m_czasSym > czasPrzesunieciaOsi) {
        m_X_wykres_1->setRange(m_czasSym - czasPrzesunieciaOsi, m_czasSym);
        m_X_wykres_2->setRange(m_czasSym - czasPrzesunieciaOsi, m_czasSym);
    }
    int liczbaProbek = static_cast<int>(czasPrzesunieciaOsi) * 5;
    if(m_wykres_Reg->count() > liczbaProbek) {
        m_wykres_Reg->remove(0);
        m_wykres_Zad->remove(0);
        m_wykres_uchyb->remove(0);
    }

    skalowanieY_ZadReg();
    skalowanieY_uchyb();
}

void MainWindow::generujWykres_ZadReg()
{
    m_wykres_Zad = new QLineSeries();
    m_wykres_Zad->setName("Wartość Zadana");
    m_wykres_Reg = new QLineSeries();
    m_wykres_Reg->setName("Wartość Regulowana");
    QChart *wykres_Zad_Reg = new QChart();
    wykres_Zad_Reg->addSeries(m_wykres_Zad);
    wykres_Zad_Reg->addSeries(m_wykres_Reg);

    m_X_wykres_1 = new QValueAxis();
    m_Y_wykres_1 = new QValueAxis();
    m_X_wykres_1->setRange(0, 10);
    //m_Y_wykres_1->setRange(-1,1);

    wykres_Zad_Reg->addAxis(m_X_wykres_1, Qt::AlignBottom);
    wykres_Zad_Reg->addAxis(m_Y_wykres_1, Qt::AlignLeft);
    m_wykres_Zad->attachAxis(m_X_wykres_1);
    m_wykres_Zad->attachAxis(m_Y_wykres_1);
    m_wykres_Reg->attachAxis(m_X_wykres_1);
    m_wykres_Reg->attachAxis(m_Y_wykres_1);

    wykres_Zad_Reg->layout()->setContentsMargins(0,0,0,0);
    wykres_Zad_Reg->setMargins(QMargins(0,0,0,0));
    wykres_Zad_Reg->setBackgroundRoundness(5);

    ui->wykres_zad_reg->setChart(wykres_Zad_Reg);
}

void MainWindow::skalowanieY_ZadReg()
{
    double minY = 1.0, maxY = -1.0;
    QList<QPointF> pZad = m_wykres_Zad->points();
    QList<QPointF> pReg = m_wykres_Reg->points();
    for(QPointF &p : pZad) {
        if(p.y() < minY) minY = p.y();
        if(p.y() > maxY) maxY = p.y();
    }
    for(QPointF &p : pReg) {
        if(p.y() < minY) minY = p.y();
        if(p.y() > maxY) maxY = p.y();
    }
    double margines = (maxY - minY) * 0.1;
    m_Y_wykres_1->setRange(minY - margines, maxY + margines); //margines +10%
}

void MainWindow::generujWykres_uchyb()
{
    m_wykres_uchyb = new QLineSeries();
    //m_wykres_uchyb->setName("Uchyb");
    QChart *wykres_uchyb = new QChart();
    wykres_uchyb->addSeries(m_wykres_uchyb);

    m_X_wykres_2 = new QValueAxis();
    m_Y_wykres_2 = new QValueAxis();
    m_X_wykres_2->setRange(0, 10);

    wykres_uchyb->addAxis(m_X_wykres_2, Qt::AlignBottom);
    wykres_uchyb->addAxis(m_Y_wykres_2, Qt::AlignLeft);
    m_wykres_uchyb->attachAxis(m_X_wykres_2);
    m_wykres_uchyb->attachAxis(m_Y_wykres_2);

    wykres_uchyb->layout()->setContentsMargins(0,0,0,0);
    wykres_uchyb->setMargins(QMargins(0,0,0,0));
    wykres_uchyb->setBackgroundRoundness(5);

    ui->wykres_uchyb->setChart(wykres_uchyb);
}

void MainWindow::skalowanieY_uchyb()
{
    double minY = 1.0, maxY = -1.0;
    QList<QPointF> pZad = m_wykres_uchyb->points();
    for(QPointF &p : pZad) {
        if(p.y() < minY) minY = p.y();
        if(p.y() > maxY) maxY = p.y();
    }
    if(maxY - minY < 0.001) {maxY += 0.1; minY -= 0.1;}

    double margines = (maxY - minY) * 0.1;
    m_Y_wykres_2->setRange(minY - margines, maxY + margines); //margines +10%
}

void MainWindow::on_start_button_clicked()
{
    int interwal = ui->param_interwal->value();
    m_timer->start(interwal);
}


void MainWindow::on_stop_button_clicked()
{
    m_timer->stop();
}

