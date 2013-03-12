#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtGui>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

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
    int x_coord;
    int y_coord;
    bool packet_started;
    bool port_opened;
    QList<QSerialPortInfo> serialPortInfoList;
    QByteArray bytes;
    QByteArray adc;
    QSerialPort serial;
    QString st_yellow;
    QString st_green;
    QString st_red;


protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private slots:
    void on_pbComPortOpen_clicked();
    void readRequest();
    void on_battery_main_valueChanged(int value);
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
