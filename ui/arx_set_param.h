#ifndef ARX_SET_PARAM_H
#define ARX_SET_PARAM_H

#include <QDialog>

namespace Ui {
class arx_set_param;
}

class arx_set_param : public QDialog
{
    Q_OBJECT

public:
    explicit arx_set_param(QWidget *parent = nullptr);
    ~arx_set_param();
    void resetui();

    int getopoznienie();
    double getzaklocenia();
    double getWektorA1();
    double getWektorA2();
    double getWektorA3();
    double getWektorB1();
    double getWektorB2();
    double getWektorB3();
    double getUmin();
    double getUmax();
    double getYmin();
    double getYmax();

    void setopoznienie(int value);
    void setzaklocenia(double value);
    void setWektorA1(double value);
    void setWektorA2(double value);
    void setWektorA3(double value);
    void setWektorB1(double value);
    void setWektorB2(double value);
    void setWektorB3(double value);
    void setUmin(double value);
    void setUmax(double value);
    void setYmin(double value);
    void setYmax(double value);



signals:
    void daneZatwierdzone(std::vector<double> A, std::vector<double> B, int opoznienie, double odchylenie, double umin, double umax, double ymin , double ymax);

private slots:
    void on_zatwierdz_button_clicked();
    void on_anuluj_button_clicked();

private:
    Ui::arx_set_param *ui;


};

#endif
