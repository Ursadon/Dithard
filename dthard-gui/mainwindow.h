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
    bool port_opened;
    QList<QSerialPortInfo> serialPortInfoList;
    QByteArray bytes;
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
};

#endif // MAINWINDOW_H
