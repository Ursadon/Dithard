#include <QCoreApplication>
#include <QCommandLineParser>
#include "console.h"
#include "rs232.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("nrf24-control");
    QCoreApplication::setApplicationVersion("1.0");

    Console *wThread = new Console();
    RS232 *sThread = new RS232();
    wThread->start();
    sThread->start();
    return a.exec();
}
