#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtGui>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
const unsigned char
  FEND  = 0xC0,        // Frame END
  FESC  = 0xDB,        // Frame ESCape
  TFEND = 0xDC,        // Transposed Frame END
  TFESC = 0xDD;        // Transposed Frame ESCape
enum packet_offset { fend = 0, addr, cmd, n, datastream, crc };

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
    QPixmap btn_UP;
    QPixmap btn_DOWN;
    int x_coord, y_coord;
    unsigned int rx_ping_error_count, rx_crc_error_count;
    bool data_started;
    bool packet_started;
    bool port_opened;
    QList<QSerialPortInfo> serialPortInfoList;
    QByteArray bytes;
    QSerialPort serial;
    QTimer rx_timer;
    QString st_yellow;
    QString st_green;
    QString st_red;
    unsigned char num_of_bytes;
    int process_packet(char command, QByteArray packet);
    int send_packet(char addr, unsigned char command, QByteArray data);

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private slots:
    void on_pbComPortOpen_clicked();
    void readRequest();
    void on_battery_main_valueChanged(int value);
    void on_pushButton_clicked();
    void rx_timer_timeout();
};

#endif // MAINWINDOW_H
