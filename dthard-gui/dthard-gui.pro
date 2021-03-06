#-------------------------------------------------
#
# Project created by QtCreator 2013-02-28T10:19:51
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dthard-gui
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

OTHER_FILES +=

RESOURCES += \
    graphics.qrc

CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../libwakeproto/src/debug/ -lwakeproto
        DESTDIR = debug

} else {
        LIBS += -L$$PWD/../libwakeproto/src/release/ -lwakeproto
        DESTDIR = release

}

INCLUDEPATH += $$PWD/../libwakeproto/src/
DEPENDPATH += $$PWD/../libwakeproto/src/
