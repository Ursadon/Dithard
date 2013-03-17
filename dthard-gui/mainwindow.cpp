#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
    x_coord = 0;
    y_coord = 0;
    port_opened = FALSE;
    btn_UP.load(":/graphics/img/circle_blue.png");
    btn_DOWN.load(":/graphics/img/circle_green.png");
    connect(&serial, SIGNAL(readyRead()), this, SLOT(readRequest()));

    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList) {
        if(serialPortInfo.isBusy()) {
            ui->comPortList->addItem(serialPortInfo.portName().append("*"));
        } else {
            ui->comPortList->addItem(serialPortInfo.portName());
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Up)
    {
        ui->lb_UP->setPixmap(btn_UP);
    }
    if(event->key() == Qt::Key_Down)
    {
        ui->lb_DOWN->setPixmap(btn_UP);
    }
    if(event->key() == Qt::Key_Left)
    {
        ui->lb_LEFT->setPixmap(btn_UP);
    }
    if(event->key() == Qt::Key_Right)
    {
        ui->lb_RIGHT->setPixmap(btn_UP);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Up)
    {
        ui->lb_UP->setPixmap(btn_DOWN);
        if(x_coord < 100 ){
            x_coord++;
        }
    }
    if(event->key() == Qt::Key_Down)
    {
        ui->lb_DOWN->setPixmap(btn_DOWN);
        if(x_coord > 0){
            x_coord--;
        }
    }
    if(event->key() == Qt::Key_Left)
    {
        ui->lb_LEFT->setPixmap(btn_DOWN);
        y_coord++;
    }
    if(event->key() == Qt::Key_Right)
    {
        ui->lb_RIGHT->setPixmap(btn_DOWN);
        y_coord--;
    }
    ui->battery_main->setValue(x_coord);
}

void MainWindow::on_pbComPortOpen_clicked()
{
    if(!port_opened) {
        ui->comPortList->setEnabled(false);
        port_opened = TRUE;
        serial.setPortName(ui->comPortList->currentText());
        if (!serial.open(QIODevice::ReadWrite)) {
            qDebug() << tr("Can't open %1, error code %2").arg(serial.portName()).arg(serial.error()) << endl;
            return;
        }

        if (!serial.setBaudRate(QSerialPort::Baud9600)) {
            //            processError(tr("Can't set baud rate 9600 baud to port %1, error code %2")
            //                         .arg(serial.portName()).arg(serial.error()));
            return;
        }

        if (!serial.setDataBits(QSerialPort::Data8)) {
            //            processError(tr("Can't set 8 data bits to port %1, error code %2")
            //                         .arg(serial.portName()).arg(serial.error()));
            return;
        }

        if (!serial.setParity(QSerialPort::NoParity)) {
            //            processError(tr("Can't set no patity to port %1, error code %2")
            //                         .arg(serial.portName()).arg(serial.error()));
            return;
        }

        if (!serial.setStopBits(QSerialPort::OneStop)) {
            //            processError(tr("Can't set 1 stop bit to port %1, error code %2")
            //                         .arg(serial.portName()).arg(serial.error()));
            return;
        }

        if (!serial.setFlowControl(QSerialPort::NoFlowControl)) {
            //            processError(tr("Can't set no flow control to port %1, error code %2")
            //                         .arg(serial.portName()).arg(serial.error()));
            return;
        }
        qDebug() << "PORT OPENED" << endl;
        ui->pbComPortOpen->setText("Close");
        ui->battery_main->setEnabled(TRUE);
        ui->lbBattery_main->setEnabled(TRUE);
    } else {
        ui->comPortList->setEnabled(true);
        port_opened = FALSE;
        serial.close();
        qDebug() << "PORT CLOSED" << endl;
        ui->pbComPortOpen->setText("Open");
        ui->battery_main->setValue(0);
        ui->signal_strenght->setValue(0);
        ui->battery_main->setEnabled(FALSE);
        ui->lbBattery_main->setEnabled(FALSE);
    }

}

void MainWindow::readRequest() {
    QByteArray rx_buffer = serial.readAll();
    foreach (unsigned char rx_byte, rx_buffer) {
        if (packet_started) {
            // Bytes destuffing
            if(rx_byte == TFEND && bytes.endsWith(FESC)){
                bytes.chop(1);
                bytes.append(FEND);
            } else if (rx_byte == TFESC && bytes.endsWith(FESC)) {
                bytes.chop(1);
                bytes.append(FESC);
            } else {
                bytes.append(rx_byte);
            }
            // We received full header?
            if (bytes.size() == 4) {
                // TODO: implement ADDR & CMD check
                num_of_bytes = bytes.at(n);
                data_started = TRUE;
            }
            if(data_started && bytes.size() == 1 + 1 + 1 + 1 + num_of_bytes + 1) { // FEND + ADDR + CMD + N + DATA + CRC
                // TODO: implement CRC16
                qDebug() << "[RX] FEND" << endl
                         << "[RX] ADDR: " << bytes.at(addr) << endl
                         << "[RX] CMD: " << bytes.at(cmd) << endl
                         << "[RX] N: " << QString::number(bytes.at(n)) << endl
                         << "[RX] DATA: " << bytes.mid(datastream).toHex() << endl
                         << "----------------------------" << endl;
                process_packet(bytes);
                data_started = FALSE;
                packet_started = FALSE;
                bytes.clear();
            }
        } else if (rx_byte == FEND) {
            bytes.append(rx_byte);
            packet_started = TRUE;
        }
    }
}

int MainWindow::process_packet(QByteArray packet) {
    unsigned char command = packet.at(cmd);
    QByteArray pkt = packet.mid(datastream);

    pkt.chop(1);
    int val;

    switch (command) {
    case 51:
        val = ((static_cast<unsigned int>(pkt.at(0)) & 0xFF) << 8)
                + (static_cast<unsigned int>(pkt.at(1)) & 0xFF);
        ui->battery_main->setValue(val);
        break;
    default:
        break;
    }
    return 0;
}

void MainWindow::on_battery_main_valueChanged(int value)
{
    int value_maximum = ui->battery_main->maximum();
    int value_percent = value * 100 / value_maximum * 10000;
    QString st_red = QString (
                "QProgressBar::chunk {"
                "background-color: #FF0000;"
                "}"
                "QProgressBar {"
                "border: 1px solid grey;"
                "border-radius: 2px;"
                "text-align: center;"
                "background: #eeeeee;"
                "}"
                );
    QString st_yellow = (
                "QProgressBar::chunk {"
                "background-color: #FFFF00;"
                "}"
                "QProgressBar {"
                "border: 1px solid grey;"
                "border-radius: 2px;"
                "text-align: center;"
                "background: #eeeeee;"
                "}"
                );
    QString st_green = QString (
                "QProgressBar::chunk {"
                "background-color: #00FF00;"
                "}"
                "QProgressBar {"
                "border: 1px solid grey;"
                "border-radius: 2px;"
                "text-align: center;"
                "background: #eeeeee;"
                "}"
                );
    if (value_percent < 33) {
        ui->battery_main->setStyleSheet(st_red);
    } else if (value_percent >= 33 && value_percent <= 65) {
        ui->battery_main->setStyleSheet(st_yellow);
    } else if (value_percent > 65) {
        ui->battery_main->setStyleSheet(st_green);
    }
}

void MainWindow::on_pushButton_clicked() {
    char SOH[] = {'\1'};
    serial.write(SOH);
}
