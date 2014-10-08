#ifndef CONSOLE_H
#define CONSOLE_H

#include <QCoreApplication>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <iostream>
#include <wakeproto.h>
#include "rs232.h"

class Console : public QThread
{
    Q_OBJECT
public:
    explicit Console(QObject *parent = 0);
    ~Console();
private:
    Wakeproto *wp;
    QByteArray rx_packet;
    RS232 *rs232thread;
signals:

public slots:
void wakepacket_st(const QByteArray &packet);
protected:
    void run();
private slots:

        void packet_rcvd(QByteArray packet);
};

#endif // CONSOLE_H
