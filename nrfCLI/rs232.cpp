#include "rs232.h"

RS232::RS232(QObject *parent) :
    QThread(parent)
{

}

RS232::~RS232()
{

}
void RS232::run()
{
    qDebug() << "Created RS232 Thread" << endl;
    //this->start();
    wp = new Wakeproto();
    wp->moveToThread(this);
    serial.moveToThread(this);
    connect(wp,SIGNAL(packetReceived(QByteArray)),this,SLOT(packet_rcvd(QByteArray)));
    connect(&serial, SIGNAL(readyRead()), this, SLOT(readRequest()));
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setPortName("/dev/tty0");
    if (!serial.open(QIODevice::ReadWrite)) {
        qDebug() << tr("Can't open %1, error code %2").arg(serial.portName()).arg(serial.error()) << endl;
        //return;
    }
    qDebug() << "Different thread started... current thread is" << QThread::currentThread() << "main thread is" << QCoreApplication::instance()->thread();
    const unsigned char data[] = {0xC0, 0xC9, 0x02, 0x08, 0x73, 0x65, 0x6E, 0x64, 0x64, 0x61, 0x74, 0x61, 0xF0};
    QByteArray packet1;
    packet1 = QByteArray::fromRawData((char*)data, 13);
    msleep(1000);
    emit wakepacket_sn(packet1);
}

void RS232::readRequest() {
    wp->getpacket(serial.readAll());
}
void RS232::packet_rcvd(QByteArray packet) {
    qDebug() << "[RS232][INFO]: Packet received!" << endl;
    wp->dump_packet(packet);
    emit wakepacket_sn(packet);
}
