#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "arx_set_param.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::aktSym);
    m_arxWindow = new arx_set_param(this);

    //wykres 1
    generujWykres_ZadReg();
    //wykres 2
    generujWykres_uchyb();
    //wykres 3
    generujWykres_ster();
    //wykres 4
    generujWykres_PID();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ZapiszDoPliku(){

    QString sciezka = QFileDialog::getSaveFileName(this,"Zapisz konfigurację","","JSON(*.json)");


    QJsonObject pid;
    pid["P"] = ui->param_P->value();
    pid["I"] = ui->param_I->value();
    pid["D"] = ui->param_D->value();

    QJsonObject gen;
    gen["amplituda"] = ui->param_amplituda->value();
    sadas;



}

void MainWindow::aktSym()
{
    m_interwal = ui->param_interwal->value();

    if (m_timer->interval() != m_interwal) {
        m_timer->setInterval(m_interwal);
    }

    m_warstwaUslug.ustawParametryPID(ui->param_P->value(), ui->param_I->value(), ui->param_D->value());

    if (ui->typ_syg_sin_button->isChecked()) {
        m_warstwaUslug.ustawRodzajSygnalu(WarstwaUslug::RodzajSygnalu::Sinusoida);
        m_warstwaUslug.ustawParametrySin(ui->param_okres->value(), ui->param_amplituda->value(), ui->param_skladowa->value());
    }
    else if (ui->typ_syg_prostokat_button->isChecked()) {
        m_warstwaUslug.ustawRodzajSygnalu(WarstwaUslug::RodzajSygnalu::Prostokatny);
        m_warstwaUslug.ustawParametryProst(ui->param_okres->value(), ui->param_wypelnienie->value(), ui->param_amplituda->value(), ui->param_skladowa->value());
    }
    else {
        m_warstwaUslug.ustawRodzajSygnalu(WarstwaUslug::RodzajSygnalu::Brak);
    }

    m_warstwaUslug.ustawTrybCalkowania(ui->tryb_calk_przed_suma_button->isChecked());

    WarstwaUslug::Wykres dane_wykres = m_warstwaUslug.wykonajKrokSym();

    m_wykres_Zad->append(m_czasSym, dane_wykres.wartZad);
    m_wykres_Reg->append(m_czasSym, dane_wykres.wartReg);
    m_wykres_uchyb->append(m_czasSym, dane_wykres.uchyb);
    m_wykres_ster->append(m_czasSym, dane_wykres.sterowanie);
    m_wykres_P->append(m_czasSym, dane_wykres.p);
    m_wykres_I->append(m_czasSym, dane_wykres.i);
    m_wykres_D->append(m_czasSym, dane_wykres.d);

    m_czasSym += (m_interwal / 1000.0);

    double czasPrzesunieciaOsi = 10.0;
    if(m_czasSym > czasPrzesunieciaOsi) {
        m_X_wykres_1->setRange(m_czasSym - czasPrzesunieciaOsi, m_czasSym);
        m_X_wykres_2->setRange(m_czasSym - czasPrzesunieciaOsi, m_czasSym);
        m_X_wykres_3->setRange(m_czasSym - czasPrzesunieciaOsi, m_czasSym);
        m_X_wykres_4->setRange(m_czasSym - czasPrzesunieciaOsi, m_czasSym);
    }

    int liczbaProbek = static_cast<int>(czasPrzesunieciaOsi*1000)/ui->param_interwal->value();
    if(m_wykres_Reg->count() > liczbaProbek) {
        m_wykres_Reg->remove(0);
        m_wykres_Zad->remove(0);
        m_wykres_uchyb->remove(0);
        m_wykres_ster->remove(0);
        m_wykres_P->remove(0);
        m_wykres_I->remove(0);
        m_wykres_D->remove(0);
    }

    skalowanieY_ZadReg();
    skalowanieY_uchyb();
    skalowanieY_ster();
    skalowanieY_PID();
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
    m_wykres_uchyb->setName("Uchyb");
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

void MainWindow::generujWykres_ster()
{
    m_wykres_ster = new QLineSeries();
    QChart *wykres_ster = new QChart();
    wykres_ster->addSeries(m_wykres_ster);
    m_wykres_ster->setName("Sterowanie");

    m_X_wykres_3 = new QValueAxis();
    m_Y_wykres_3 = new QValueAxis();
    m_X_wykres_3->setRange(0, 10);

    wykres_ster->addAxis(m_X_wykres_3, Qt::AlignBottom);
    wykres_ster->addAxis(m_Y_wykres_3, Qt::AlignLeft);
    m_wykres_ster->attachAxis(m_X_wykres_3);
    m_wykres_ster->attachAxis(m_Y_wykres_3);

    wykres_ster->layout()->setContentsMargins(0,0,0,0);
    wykres_ster->setMargins(QMargins(0,0,0,0));
    wykres_ster->setBackgroundRoundness(5);

    ui->wykres_ster->setChart(wykres_ster);
}

void MainWindow::skalowanieY_ster()
{
    double minY = 1.0, maxY = -1.0;
    QList<QPointF> pZad = m_wykres_ster->points();
    for(QPointF &p : pZad) {
        if(p.y() < minY) minY = p.y();
        if(p.y() > maxY) maxY = p.y();
    }
    if(maxY - minY < 0.001) {maxY += 1.0; minY -= 1.0;}

    double margines = (maxY - minY) * 0.1;
    m_Y_wykres_3->setRange(minY - margines, maxY + margines); //margines +10%
}

void MainWindow::generujWykres_PID()
{
    m_wykres_P = new QLineSeries();
    m_wykres_P->setName("P");
    m_wykres_I = new QLineSeries();
    m_wykres_I->setName("I");
    m_wykres_D = new QLineSeries();
    m_wykres_D->setName("D");
    QChart *wykres_PID = new QChart();
    wykres_PID->addSeries(m_wykres_P);
    wykres_PID->addSeries(m_wykres_I);
    wykres_PID->addSeries(m_wykres_D);

    m_X_wykres_4 = new QValueAxis();
    m_Y_wykres_4 = new QValueAxis();
    m_X_wykres_4->setRange(0, 10);

   wykres_PID->addAxis(m_X_wykres_4, Qt::AlignBottom);
   wykres_PID->addAxis(m_Y_wykres_4, Qt::AlignLeft);
    m_wykres_P->attachAxis(m_X_wykres_4);
    m_wykres_P->attachAxis(m_Y_wykres_4);
    m_wykres_I->attachAxis(m_X_wykres_4);
    m_wykres_I->attachAxis(m_Y_wykres_4);
    m_wykres_D->attachAxis(m_X_wykres_4);
    m_wykres_D->attachAxis(m_Y_wykres_4);

    wykres_PID->layout()->setContentsMargins(0,0,0,0);
    wykres_PID->setMargins(QMargins(0,0,0,0));
    wykres_PID->setBackgroundRoundness(5);

    ui->wykres_skladowePID->setChart(wykres_PID);
}

void MainWindow::skalowanieY_PID()
{
    double minY = 1.0, maxY = -1.0;
    QList<QPointF> pP = m_wykres_P->points();
    QList<QPointF> pI = m_wykres_I->points();
    QList<QPointF> pD = m_wykres_D->points();
    for(QPointF &p : pP) {
        if(p.y() < minY) minY = p.y();
        if(p.y() > maxY) maxY = p.y();
    }
    for(QPointF &p : pI) {
        if(p.y() < minY) minY = p.y();
        if(p.y() > maxY) maxY = p.y();
    }
    for(QPointF &p : pD) {
        if(p.y() < minY) minY = p.y();
        if(p.y() > maxY) maxY = p.y();
    }
    if(maxY - minY < 0.001) {maxY += 1.0; minY -= 1.0;}
    double margines = (maxY - minY) * 0.1;
    m_Y_wykres_4->setRange(minY - margines, maxY + margines);
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


void MainWindow::on_paramARX_button_clicked()
{
        connect(m_arxWindow, &arx_set_param::daneZatwierdzone, this, [this](auto A, auto B, int opoznienia, double zaklocenia){
            m_warstwaUslug.ustawParametryARX(A, B, opoznienia);
            m_warstwaUslug.ustawOdchylenie(zaklocenia);
        });

    m_arxWindow->show();


}

void MainWindow::on_reset_button_clicked()
{
    m_timer->stop();
    m_czasSym = 0.0;
    m_wykres_Zad->clear();
    m_wykres_Reg->clear();
    m_wykres_uchyb->clear();
    m_wykres_ster->clear();
    m_wykres_P->clear();
    m_wykres_I->clear();
    m_wykres_D->clear();
    m_warstwaUslug.resetSymulacji();
    skalowanieY_ZadReg();
    skalowanieY_uchyb();
    skalowanieY_ster();
    skalowanieY_PID();
    m_X_wykres_1->setRange(0,10);
    m_X_wykres_2->setRange(0,10);
    m_X_wykres_3->setRange(0,10);
    m_X_wykres_4->setRange(0,10);
    ui->param_P->setValue(0.0);
    ui->param_I->setValue(0.0);
    ui->param_D->setValue(0.0);
    ui->param_amplituda->setValue(0.0);
    ui->param_interwal->setValue(200);
    ui->param_okres->setValue(1);
    ui->param_skladowa->setValue(0.0);
    ui->param_wypelnienie->setValue(0.0);
    ui->tryb_calk_przed_suma_button->setChecked(true);
    ui->typ_syg_prostokat_button->setChecked(true);
    m_arxWindow->resetui();
};

