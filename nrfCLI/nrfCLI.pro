#-------------------------------------------------
#
# Project created by QtCreator 2014-10-08T10:21:49
#
#-------------------------------------------------

QT       += core serialport

QT       -= gui

TARGET = nrfCLI
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    console.cpp \
    rs232.cpp

HEADERS += \
    console.h \
    rs232.h

CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../../libwakeproto/src/debug/ -lwakeproto
} else {
        LIBS += -L$$PWD/../../libwakeproto/src/release/ -lwakeproto
}
INCLUDEPATH += $$PWD/../../libwakeproto/src/
DEPENDPATH += $$PWD/../../libwakeproto/src/
