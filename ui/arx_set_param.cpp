#include "arx_set_param.h"
#include "ui_arx_set_param.h"

arx_set_param::arx_set_param(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::arx_set_param)
{
    ui->setupUi(this);

    limityWartoscDomyslna();
    on_czyLimit_check_toggled(false);
    this->setWindowTitle("PARAMETRY ARX");
}

arx_set_param::~arx_set_param()
{
    delete ui;
}

void arx_set_param::on_zatwierdz_button_clicked() {
    std::vector<double> A = { ui->a1_param->value(), ui->a2_param->value(), ui->a3_param->value() };
    std::vector<double> B = { ui->b1_param->value(), ui->b2_param->value(), ui->b3_param->value() };
    int opoznienie = ui->opoznienie_param->value();
    double zaklocenia = ui->zaklocenia_param->value();
    double umin = ui->umin_param->value();
    double umax = ui->umax_param->value();
    double ymin = ui->ymin_param->value();
    double ymax = ui->ymax_param->value();
    bool czyLimit = ui->czyLimit_check->isChecked();

    emit daneZatwierdzone(A, B, opoznienie, zaklocenia, umin, umax, ymin, ymax, czyLimit);
    this->hide();
}

void arx_set_param::on_anuluj_button_clicked(){
    //
    this -> hide();
}

void arx_set_param::resetui(){
    ui->a1_param->setValue(0.0);
    ui->a2_param->setValue(0.0);
    ui->a3_param->setValue(0.0);
    ui->b1_param->setValue(0.0);
    ui->b2_param->setValue(0.0);
    ui->b3_param->setValue(0.0);
    ui->opoznienie_param->setValue(0);
    ui->umax_param->setValue(0.0);
    ui->umin_param->setValue(0.0);
    ui->ymax_param->setValue(0.0);
    ui->ymin_param->setValue(0.0);
    ui->zaklocenia_param->setValue(0);


}
int arx_set_param::getopoznienie() {return ui->opoznienie_param->value();}
double arx_set_param::getzaklocenia(){return ui->zaklocenia_param->value();}
double arx_set_param::getWektorA1(){return ui->a1_param->value();}
double arx_set_param::getWektorA2(){return ui->a2_param->value();}
double arx_set_param::getWektorA3(){return ui->a3_param->value();}
double arx_set_param::getWektorB1(){return ui->b1_param->value();}
double arx_set_param::getWektorB2(){return ui->b2_param->value();}
double arx_set_param::getWektorB3(){return ui->b3_param->value();}
double arx_set_param::getUmax(){return ui->umax_param->value();}
double arx_set_param::getUmin(){return ui->umin_param->value();}
double arx_set_param::getYmax(){return ui->ymax_param->value();}
double arx_set_param::getYmin(){return ui->ymin_param->value();}

void arx_set_param::setopoznienie(int value){ui->opoznienie_param->setValue(value);}
void arx_set_param::setzaklocenia(double value){ui->zaklocenia_param->setValue(value);}
void arx_set_param::setWektorA1(double value){ui->a1_param->setValue(value);}
void arx_set_param::setWektorA2(double value){ui->a2_param->setValue(value);}
void arx_set_param::setWektorA3(double value){ui->a3_param->setValue(value);}
void arx_set_param::setWektorB1(double value){ui->b1_param->setValue(value);}
void arx_set_param::setWektorB2(double value){ui->b2_param->setValue(value);}
void arx_set_param::setWektorB3(double value){ui->b3_param->setValue(value);}
void arx_set_param::setUmin(double value){ui->umin_param->setValue(value);}
void arx_set_param::setUmax(double value){ui->umax_param->setValue(value);}
void arx_set_param::setYmin(double value){ui->ymin_param->setValue(value);}
void arx_set_param::setYmax(double value){ui->ymax_param->setValue(value);}




void arx_set_param::on_czyLimit_check_toggled(bool checked)
{
    ui->umax_param->setEnabled(checked);
    ui->umin_param->setEnabled(checked);
    ui->ymax_param->setEnabled(checked);
    ui->ymin_param->setEnabled(checked);

    if(!checked) {
        limityWartoscDomyslna();
    }
}

void arx_set_param::limityWartoscDomyslna()
{
    ui->umin_param->setValue(-10.0);
    ui->umax_param->setValue(10.0);
    ui->ymin_param->setValue(-10.0);
    ui->ymax_param->setValue(10.0);
}

