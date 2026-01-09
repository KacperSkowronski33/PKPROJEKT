#include "mainwindow.h"

#include <QApplication>

void uruchom_test();
//#define test_mode

int main(int argc, char *argv[])
{
#ifdef test_mode
    uruchom_test();
#else
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
#endif
}
