#include "console.h"
#include "rs232.h"
Console::Console(QObject *parent) :
    QThread(parent)
{
    qDebug() << "Console thread started" << endl;
    wp = new Wakeproto();
    rs232thread = new RS232();
    connect(wp,SIGNAL(packetReceived(QByteArray)),this,SLOT(packet_rcvd(QByteArray)));
    connect(rs232thread, SIGNAL(wakepacket_sn(const QByteArray &)), SLOT(wakepacket(const QByteArray &)));
}

Console::~Console(){

}

void Console::run()
{
    // Сравниваем адрес текущего потока с адресом главного потока приложения
    //qDebug() << "Different thread started... current thread is" << QThread::currentThread() << "main thread is" << QCoreApplication::instance()->thread();
    QTextStream qtin(stdin);
    QTextStream qtout(stdout);
    QString line;
    QStringList command;
    int i = 0;
    qtout << "> " << flush;

    while(true) {

        line = qtin.readLine();
        i = 0;

        command = line.split(" ", QString::SkipEmptyParts);
        if(line == "") {
            qtout << "> " << flush;
            continue;
        }

        while (i < command.count()) {
            if (command.at(i) == "quit") {
                qtout << "Good bye!" << endl;
                QCoreApplication::exit();
            } else if (command.at(i) == "spi") {
                if ((++i) < command.count()) {
                    if(command.at(i) == "read") {
                        if ((++i) < command.count()) {
                             qtout << "Reading value at: 0x" << QString::number(command.at(i).toInt(),16) << endl;
                        } else {
                             qtout << "Need value to read" << endl;
                        }
                    } else if(command.at(i) == "write") {

                    } else if(command.at(i) == "dump") {

                    } else {
                        qtout << "Unknown command" << endl;
                    }
                } else {
                    qtout << "Incomplete command. spi [read/write/dump] {value}" << endl;
                }
            } else {
                qtout << "Command not found" << endl;
            }
            i++;
        }
        qtout << "> " << flush;
    }
}

void Console::wakepacket(const QByteArray &packet)
{
    rx_packet = packet;
    wp->dump_packet(rx_packet);
}
void Console::packet_rcvd(QByteArray packet) {
    qDebug() << "[CONSOLE][INFO]: Packet received!" << endl;
    //wp->dump_packet(packet);
    //emit wakepacket(packet);
}
