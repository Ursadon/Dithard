#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtGui>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QUdpSocket>
#include <wakeproto.h>

#define TRUE 1
#define FALSE 0

enum packet_offset { fend = 0, addr, cmd, n, datastream, crc };
enum connection_type {conn_undefined = 0, conn_ip = 1, conn_serial = 2};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPixmap btn_UP, btn_DOWN;
    int x_coord, y_coord;
    unsigned int rx_crc_error_count, ack_num;
    bool data_started, packet_started, sending;
    bool port_opened, socket_opened;
    QList<QSerialPortInfo> serialPortInfoList;
    QQueue<QByteArray> tx_queue, tx_queue_urgency;
    QByteArray bytes;
    QSerialPort serial;
    QTimer adc_timer, range_timer, crc_error_timer, tx_timer, T1_timer, T2_timer;
    QString st_yellow, st_green, st_red;
    unsigned char num_of_bytes;
    unsigned int connection_type;
    int process_packet(char command, QByteArray packet);
    int send_packet(char addr, unsigned char command, QByteArray data, bool urgency);
    QUdpSocket *udp_socket;
    Wakeproto *wproto;

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private slots:
    void on_pbComPortOpen_clicked();
    void readRequest();
    void on_battery_main_valueChanged(int value);
    void crc_error_timeout();
    void send_queue();
    void get_adc1();
    void get_range();
    void T1_timeout();
    void T2_timeout();
    void on_cbRange_stateChanged(int arg1);
    void on_cb_acks_stateChanged(int arg1);
    void on_cbNoQueue_stateChanged(int arg1);
    void on_rbSerial_clicked();
    void on_rbIP_clicked();
    void on_pbIpOpen_clicked();
    void packet_rcvd(QByteArray packet);
    void on_pbCustomSend_clicked();
};

#endif // MAINWINDOW_H
