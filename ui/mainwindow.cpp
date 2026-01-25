#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "arx_set_param.h"
#include <initializer_list>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_arxWindow(nullptr)
{
    ui->setupUi(this);
    m_warstwaUslug = new WarstwaUslug(this);

    connect(ui->param_P, &QDoubleSpinBox::editingFinished, this, &MainWindow::aktSym);
    connect(ui->param_I, &QDoubleSpinBox::editingFinished, this, &MainWindow::aktSym);
    connect(ui->param_D, &QDoubleSpinBox::editingFinished, this, &MainWindow::aktSym);
    connect(ui->param_interwal, &QDoubleSpinBox::editingFinished, this, &MainWindow::aktSym);
    connect(ui->param_amplituda, &QDoubleSpinBox::editingFinished, this, &MainWindow::aktSym);
    connect(ui->param_okres, &QDoubleSpinBox::editingFinished, this, &MainWindow::aktSym);
    connect(ui->param_skladowa, &QDoubleSpinBox::editingFinished, this, &MainWindow::aktSym);
    connect(ui->param_wypelnienie, &QDoubleSpinBox::editingFinished, this, &MainWindow::aktSym);
    connect(m_warstwaUslug, &WarstwaUslug::aktDanychUslugi, this, &MainWindow::getDaneSym);


    m_interwal = 200.0;
    m_czasSym = 0.0;
    aktSym();

    generujWykres_ZadReg();
    generujWykres_uchyb();
    generujWykres_ster();
    generujWykres_PID();
    this->setWindowTitle("STATUS SYMULACJI - ZATRZYMANA");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ZapiszDoPliku(){

    QString sciezka = QFileDialog::getSaveFileName(this,"Zapisz konfigurację","","JSON(*.json)");

    QJsonObject root;

    QJsonObject pid;
    pid["interwal"] = ui->param_interwal->value();
    pid["P"] = ui->param_P->value();
    pid["I"] = ui->param_I->value();
    pid["D"] = ui->param_D->value();
    pid["tryb"] = ui->tryb_calk_w_sumie_button->isChecked() ? "w sumie":"przed suma";

    QJsonObject gen;
    gen["amplituda"] = ui->param_amplituda->value();
    gen["okres"] = ui->param_okres->value();
    gen["skladowa"] = ui->param_skladowa->value();
    gen["wypelnienie"] = ui->param_wypelnienie->value();
    gen["okno_czasowe"] = ui->param_czasObserwacji->value();
    pid["typ"] = ui->typ_syg_sin_button->isChecked() ? "sinus":"prostokat";

    QJsonObject arx;
    arx["zaklocenia"] = m_arxWindow->getzaklocenia();
    arx["opoznienie"] = m_arxWindow->getopoznienie();
    arx["a1"] = m_arxWindow->getWektorA1();
    arx["a2"] = m_arxWindow->getWektorA2();
    arx["a3"] = m_arxWindow->getWektorA3();
    arx["b1"] = m_arxWindow->getWektorB1();
    arx["b2"] = m_arxWindow->getWektorB2();
    arx["b3"] = m_arxWindow->getWektorB3();
    arx["Umin"] = m_arxWindow->getUmin();
    arx["Umax"] = m_arxWindow->getUmax();
    arx["Ymin"] = m_arxWindow->getYmin();
    arx["Ymax"] = m_arxWindow->getYmax();
    arx["czyLimit"] = m_arxWindow->getLimit();


    root["parametry_pid"] = pid;
    root["parametry_generatora"] = gen;
    root["parametry_arx"] = arx;

    QJsonDocument doc(root);
    QFile plik(sciezka);
    plik.open(QFile::ReadWrite);
    plik.write(doc.toJson());
}

void MainWindow::WczytajZPliku(){
    if(!m_arxWindow) { //okno jest tworzone po nacisnieciu przycisku, wiec jak ktos wczyta json przed wcisnieciem przycisku to dane nie wyladuja w oknie, bo takie jeszcze nie istnieje
        utworzOknoARX();
    }

    QString sciezka = QFileDialog::getOpenFileName(this,"Wczytaj konfiguracje","","JSON (*.json)" );
    QFile plik(sciezka);
    plik.open(QIODevice::ReadOnly);
    QByteArray dane = plik.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(dane);
    QJsonObject root = doc.object();

    QJsonObject pid = root["parametry_pid"].toObject();
    ui->param_P->setValue(pid["P"].toDouble());
    ui->param_I->setValue(pid["I"].toDouble());
    ui->param_D->setValue(pid["D"].toDouble());
    ui->param_interwal->setValue(pid["interwal"].toInt());
    if(pid["tryb"].toString()=="w sumie"){
        ui->tryb_calk_w_sumie_button->setChecked(true);
    }else{
        ui->tryb_calk_przed_suma_button->setChecked(true);
    }

    QJsonObject gen = root["parametry_generatora"].toObject();
    ui->param_amplituda->setValue(gen["amplituda"].toDouble());
    ui->param_skladowa->setValue(gen["skladowa"].toDouble());
    ui->param_okres->setValue(gen["okres"].toDouble());
    ui->param_wypelnienie->setValue(gen["wypelnienie"].toDouble());
    ui->param_czasObserwacji->setValue(gen["okno_czasowe"].toDouble());
    if(pid["typ"].toString()=="sinus"){
        ui->typ_syg_sin_button->setChecked(true);
    }else{
        ui->typ_syg_prostokat_button->setChecked(true);
    }

    QJsonObject arx = root["parametry_arx"].toObject();

    m_arxWindow->setopoznienie(arx["opoznienie"].toInt());
    m_arxWindow->setzaklocenia(arx["zaklocenia"].toDouble());
    m_arxWindow->setWektorA1(arx["a1"].toDouble());
    m_arxWindow->setWektorA2(arx["a2"].toDouble());
    m_arxWindow->setWektorA3(arx["a3"].toDouble());
    m_arxWindow->setWektorB1(arx["b1"].toDouble());
    m_arxWindow->setWektorB2(arx["b2"].toDouble());
    m_arxWindow->setWektorB3(arx["b3"].toDouble());
    m_arxWindow->setUmax(arx["Umax"].toDouble());
    m_arxWindow->setUmin(arx["Umin"].toDouble());
    m_arxWindow->setYmax(arx["Ymax"].toDouble());
    m_arxWindow->setYmin(arx["Ymin"].toDouble());
    m_arxWindow->setLimit(arx["czyLimit"].toBool());

    m_arxWindow->on_zatwierdz_button_clicked();


}

void MainWindow::utworzOknoARX()
{
    m_arxWindow = new arx_set_param(this);

    connect(m_arxWindow, &arx_set_param::daneZatwierdzone, this, [this](auto A, auto B, int opoznienia, double zaklocenia, double umin, double umax, double ymin, double ymax, double czyLimit){
        m_warstwaUslug->ustawParametryARX(A, B, opoznienia);
        m_warstwaUslug->ustawOdchylenie(zaklocenia);
        m_warstwaUslug->ustawLimity(umin, umax, ymin, ymax, czyLimit);
    });
}

void MainWindow::on_actionZapisz_konfiguracj_triggered()
{
    ZapiszDoPliku();
}
void MainWindow::on_actionWczytaj_konfiguracj_triggered()
{
    WczytajZPliku();
}

void MainWindow::getDaneSym(WarstwaUslug::Wykres dane)
{
    m_czasSym += (static_cast<double>(m_interwal) / 1000.0);

    m_wykres_Zad->append(m_czasSym, dane.wartZad);
    m_wykres_Reg->append(m_czasSym, dane.wartReg);
    m_wykres_uchyb->append(m_czasSym, dane.uchyb);
    m_wykres_ster->append(m_czasSym, dane.sterowanie);
    m_wykres_P->append(m_czasSym, dane.p);
    m_wykres_I->append(m_czasSym, dane.i);
    m_wykres_D->append(m_czasSym, dane.d);

    double czasPrzesunieciaOsi = ui->param_czasObserwacji->value();
    // if(m_czasSym > czasPrzesunieciaOsi) {
    //     m_X_wykres_1->setRange(m_czasSym - czasPrzesunieciaOsi, m_czasSym);
    //     m_X_wykres_2->setRange(m_czasSym - czasPrzesunieciaOsi, m_czasSym);
    //     m_X_wykres_3->setRange(m_czasSym - czasPrzesunieciaOsi, m_czasSym);
    //     m_X_wykres_4->setRange(m_czasSym - czasPrzesunieciaOsi, m_czasSym);
    // } else {
    //     m_X_wykres_1->setRange(0, czasPrzesunieciaOsi);
    //     m_X_wykres_2->setRange(0, czasPrzesunieciaOsi);
    //     m_X_wykres_3->setRange(0, czasPrzesunieciaOsi);
    //     m_X_wykres_4->setRange(0, czasPrzesunieciaOsi);
    // }

    auto skalowanieOX = [](double aktCzas, double czasPrzesuniecia, std::initializer_list<QValueAxis*> os) {
        double min = 0.0, max = 0.0;

        if(aktCzas > czasPrzesuniecia) {
            min = aktCzas - czasPrzesuniecia;
            max = aktCzas;
        } else {
            max = czasPrzesuniecia;
            min = 0.0;
        }
        for(auto o : os) {
            if(o) o->setRange(min, max);
        }
    };
    skalowanieOX(m_czasSym, czasPrzesunieciaOsi, {m_X_wykres_1, m_X_wykres_2, m_X_wykres_3, m_X_wykres_4 });

    int liczbaProbek = static_cast<int>(czasPrzesunieciaOsi*1000)/m_interwal;
    if(m_wykres_Reg->count() > liczbaProbek) {
        m_wykres_Reg->remove(0);
        m_wykres_Zad->remove(0);
        m_wykres_uchyb->remove(0);
        m_wykres_ster->remove(0);
        m_wykres_P->remove(0);
        m_wykres_I->remove(0);
        m_wykres_D->remove(0);
    }

    skalowanieY(m_Y_wykres_1, {m_wykres_Zad, m_wykres_Reg});
    skalowanieY(m_Y_wykres_2, {m_wykres_uchyb});
    skalowanieY(m_Y_wykres_3, {m_wykres_ster});
    skalowanieY(m_Y_wykres_4, {m_wykres_P, m_wykres_I, m_wykres_D});
}


void MainWindow::aktSym()
{
    int aktInterwal = ui->param_interwal->value();

    if (aktInterwal != m_interwal) {
        m_interwal = aktInterwal;
        m_warstwaUslug->setInterwal(m_interwal);
    }

    m_warstwaUslug->ustawParametryPID(ui->param_P->value(), ui->param_I->value(), ui->param_D->value());

    if (ui->typ_syg_sin_button->isChecked()) {
        m_warstwaUslug->ustawRodzajSygnalu(WarstwaUslug::RodzajSygnalu::Sinusoida);
        m_warstwaUslug->ustawParametrySin(ui->param_okres->value(), ui->param_amplituda->value(), ui->param_skladowa->value());
    }
    else if (ui->typ_syg_prostokat_button->isChecked()) {
        m_warstwaUslug->ustawRodzajSygnalu(WarstwaUslug::RodzajSygnalu::Prostokatny);
        m_warstwaUslug->ustawParametryProst(ui->param_okres->value(), ui->param_wypelnienie->value(), ui->param_amplituda->value(), ui->param_skladowa->value());
    }
    else {
        m_warstwaUslug->ustawRodzajSygnalu(WarstwaUslug::RodzajSygnalu::Brak);
    }

    m_warstwaUslug->ustawTrybCalkowania(ui->tryb_calk_przed_suma_button->isChecked());

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
    m_Y_wykres_1->setRange(-1,1);

    wykres_Zad_Reg->addAxis(m_X_wykres_1, Qt::AlignBottom);
    wykres_Zad_Reg->addAxis(m_Y_wykres_1, Qt::AlignLeft);
    m_wykres_Zad->attachAxis(m_X_wykres_1);
    m_wykres_Zad->attachAxis(m_Y_wykres_1);
    m_wykres_Reg->attachAxis(m_X_wykres_1);
    m_wykres_Reg->attachAxis(m_Y_wykres_1);

    wykres_Zad_Reg->layout()->setContentsMargins(0,0,0,0);
    wykres_Zad_Reg->setMargins(QMargins(0,0,0,0));
    wykres_Zad_Reg->setBackgroundRoundness(5);
    m_X_wykres_1->setTitleText("Czas [s]");
    m_Y_wykres_1->setTitleText("Wartość");

    ui->wykres_zad_reg->setChart(wykres_Zad_Reg);
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
    m_Y_wykres_2->setRange(-1,1);

    wykres_uchyb->addAxis(m_X_wykres_2, Qt::AlignBottom);
    wykres_uchyb->addAxis(m_Y_wykres_2, Qt::AlignLeft);
    m_wykres_uchyb->attachAxis(m_X_wykres_2);
    m_wykres_uchyb->attachAxis(m_Y_wykres_2);

    wykres_uchyb->layout()->setContentsMargins(0,0,0,0);
    wykres_uchyb->setMargins(QMargins(0,0,0,0));
    wykres_uchyb->setBackgroundRoundness(5);
    m_X_wykres_2->setTitleText("Czas [s]");
    m_Y_wykres_2->setTitleText("Wartość uchybu");

    ui->wykres_uchyb->setChart(wykres_uchyb);
}

void MainWindow::generujWykres_ster()
{
    m_wykres_ster = new QLineSeries();
    QChart *wykres_ster = new QChart();
    wykres_ster->addSeries(m_wykres_ster);
    //m_wykres_ster->setName("Sterowanie");

    m_X_wykres_3 = new QValueAxis();
    m_Y_wykres_3 = new QValueAxis();
    m_X_wykres_3->setRange(0, 10);
    m_Y_wykres_1->setRange(-1,1);

    wykres_ster->addAxis(m_X_wykres_3, Qt::AlignBottom);
    wykres_ster->addAxis(m_Y_wykres_3, Qt::AlignLeft);
    m_wykres_ster->attachAxis(m_X_wykres_3);
    m_wykres_ster->attachAxis(m_Y_wykres_3);

    wykres_ster->layout()->setContentsMargins(0,0,0,0);
    wykres_ster->setMargins(QMargins(0,0,0,0));
    wykres_ster->setBackgroundRoundness(5);
    m_X_wykres_3->setTitleText("Czas [s]");
    m_Y_wykres_3->setTitleText("Wartość sterowania");

    ui->wykres_ster->setChart(wykres_ster);
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
    m_Y_wykres_1->setRange(-1,1);

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
    m_X_wykres_4->setTitleText("Czas [s]");
    m_Y_wykres_4->setTitleText("Wartości PID");

    ui->wykres_skladowePID->setChart(wykres_PID);
}

void MainWindow::skalowanieY(QValueAxis *oy, const QList<QLineSeries *> &dane)
{
    if(dane.isEmpty()) return;

    double min = 10000.0, max = -10000.0;
    for(QLineSeries *d : dane) {
        QList<QPointF> punkty = d->points();
        for(QPointF &p : punkty) {
            if(p.y() < min) min = p.y();
            if(p.y() > max) max = p.y();
        }
    }
    double minZakres = 2.0;
    double aktZakres = max - min;
    double srodekZakresu = (max + min) /2.0;

    if(minZakres > aktZakres) {
        max = srodekZakresu + minZakres;
        min = srodekZakresu - minZakres;
    } else {
        double margines = aktZakres * 0.1;
        max += margines;
        min -= margines;
    }
    oy->setRange(min, max);
}

void MainWindow::on_start_button_clicked()
{
    aktSym();
    int interwal = ui->param_interwal->value();
    m_warstwaUslug->startSym(interwal);
    this->setWindowTitle("STATUS SYMULACJI - URUCHOMIONA");
}


void MainWindow::on_stop_button_clicked()
{
    m_warstwaUslug->stopSym();
    this->setWindowTitle("STATUS SYMULACJI - ZATRZYMANA");
}

void MainWindow::on_paramARX_button_clicked()
{
    if(!m_arxWindow) {
        utworzOknoARX();
    }
    m_arxWindow->show();
}

void MainWindow::on_reset_button_clicked()
{
    m_warstwaUslug->stopSym();
    m_czasSym = 0.0;
    m_wykres_Zad->clear();
    m_wykres_Reg->clear();
    m_wykres_uchyb->clear();
    m_wykres_ster->clear();
    m_wykres_P->clear();
    m_wykres_I->clear();
    m_wykres_D->clear();
    m_warstwaUslug->resetSymulacji();
    m_X_wykres_1->setRange(0,10);

    m_Y_wykres_1->setRange(-1,1);
    m_Y_wykres_2->setRange(-1,1);
    m_Y_wykres_3->setRange(-1,1);
    m_Y_wykres_4->setRange(-1,1);

    m_X_wykres_2->setRange(0,10);
    m_X_wykres_3->setRange(0,10);
    m_X_wykres_4->setRange(0,10);
    ui->param_P->setValue(0.0);
    ui->param_I->setValue(0.0);
    ui->param_D->setValue(0.0);
    ui->param_amplituda->setValue(0.0);
    ui->param_interwal->setValue(200);
    ui->param_okres->setValue(1.0);
    ui->param_skladowa->setValue(0.0);
    ui->param_wypelnienie->setValue(0.0);
    ui->param_czasObserwacji->setValue(10.0);
    ui->tryb_calk_przed_suma_button->setChecked(true);
    ui->typ_syg_prostokat_button->setChecked(true);
    if(m_arxWindow) m_arxWindow->resetui();
};


void MainWindow::on_resest_cal_button_clicked()
{
    m_warstwaUslug->resetujCalke();
};

void MainWindow::on_reset_roz_button_clicked()
{
    m_warstwaUslug->resetujRozniczke();
};
