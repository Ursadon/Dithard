#ifndef RS232_H
#define RS232_H

#include <QCoreApplication>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <wakeproto.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class RS232 : public QThread
{
    Q_OBJECT
public:
    explicit RS232(QObject *parent = 0);
    ~RS232();
signals:
    void new_wake_packet(const QByteArray packet);

private:
    Wakeproto *wp;
    QSerialPort *serial;
public slots:

private slots:
    void readRequest();
    void packet_rcvd(QByteArray packet);
protected:
    void run();
};

#endif // RS232_H
