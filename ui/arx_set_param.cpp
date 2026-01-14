#include "arx_set_param.h"
#include "ui_arx_set_param.h"

arx_set_param::arx_set_param(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::arx_set_param)
{
    ui->setupUi(this);
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
    emit daneZatwierdzone(A, B, opoznienie, zaklocenia);
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
