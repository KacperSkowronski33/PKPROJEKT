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

private:
    Ui::arx_set_param *ui;
};

#endif // ARX_SET_PARAM_H
