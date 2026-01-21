QT       += core gui widgets charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SymulatorUAR
TEMPLATE = app
CONFIG += c++17

INCLUDEPATH += \
    $$PWD/../backend \
    $$PWD/../service \
    $$PWD/../ui \
    $$PWD/../tests

SOURCES += \
    ../ui/arx_set_param.cpp \
    ../ui/mainwindow.cpp \
    main.cpp \
    ../service/WarstwaUslug.cpp \
    ../backend/ModelARX.cpp \
    ../backend/RegulatorPID.cpp \
    ../backend/ProstyUAR.cpp \
    ../backend/GeneratorWartosci.cpp \
    ../tests/testy_uar.cpp \
    ../tests/run_tests.cpp

HEADERS += \
    ../service/WarstwaUslug.h \
    ../backend/ModelARX.h \
    ../backend/RegulatorPID.h \
    ../backend/ProstyUAR.h \
    ../backend/GeneratorWartosci.h \
    ../ui/arx_set_param.h \
    ../ui/mainwindow.h

FORMS += \
    ../ui/arx_set_param.ui \
    ../ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
