#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>
#include <wakeproto.h>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void HelloUDP();
signals:

public slots:
    void readyRead();

private:
    QUdpSocket *socket;
    Wakeproto *wproto;
private slots:
    void packet_rcvd(QByteArray packet);


};

#endif // SERVER_H
