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
