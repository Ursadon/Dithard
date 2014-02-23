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
	QPixmap btn_UP, btn_DOWN;
    int x_coord, y_coord;
	unsigned int rx_crc_error_count, ack_num;
	bool data_started, packet_started, sending;
    bool port_opened;
    QList<QSerialPortInfo> serialPortInfoList;
    QQueue<QByteArray> tx_queue, tx_queue_urgency;
    QByteArray bytes;
    QSerialPort serial;
    QTimer adc_timer, range_timer, crc_error_timer, tx_timer, T1_timer, T2_timer;
	QString st_yellow, st_green, st_red;
    unsigned char num_of_bytes;
    int process_packet(char command, QByteArray packet);
    int send_packet(char addr, unsigned char command, QByteArray data, bool urgency);

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private slots:
    void on_pbComPortOpen_clicked();
    void readRequest();
    void on_battery_main_valueChanged(int value);
    void on_pushButton_clicked();
	void crc_error_timeout();
	void send_queue();
	void get_adc1();
    void get_range();
	void T1_timeout();
	void T2_timeout();
    void on_checkBox_stateChanged(int arg1);
    void on_cb_acks_stateChanged(int arg1);
    void on_checkBox_2_stateChanged(int arg1);
};

#endif // MAINWINDOW_H
