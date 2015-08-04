#-------------------------------------------------
#
# Project created by QtCreator 2015-08-04T12:47:36
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = Dithard-server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp

HEADERS += \
    server.h

CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../libwakeproto/src/debug/ -lwakeproto
        DESTDIR = debug

} else {
        LIBS += -L$$PWD/../libwakeproto/src/release/ -lwakeproto
        DESTDIR = release

}

INCLUDEPATH += $$PWD/../libwakeproto/src/
DEPENDPATH += $$PWD/../libwakeproto/src/
