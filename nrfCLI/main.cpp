#include <QCoreApplication>
#include <QCommandLineParser>
#include "console.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("nrf24-control");
    QCoreApplication::setApplicationVersion("1.0");

    Console *wThread = new Console();
    wThread->start();

    return a.exec();
}
