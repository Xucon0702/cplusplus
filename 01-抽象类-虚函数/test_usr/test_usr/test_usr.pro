TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ctesttransfer.cpp \
        main.cpp

HEADERS += \
    ctesttransfer.h \
    lib/ctestlib.h \
    lib/interface_base.h

win32: LIBS += -L$$PWD/lib/ -ltest_lib

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
