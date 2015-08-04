#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
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
    rx_crc_error_count = 0;
    ack_num = 0;
    port_opened = FALSE;
    socket_opened = FALSE;
    sending = FALSE;
    btn_UP.load(":/graphics/img/circle_blue.png");
    btn_DOWN.load(":/graphics/img/circle_green.png");
    connection_type = conn_ip;
    udp_socket = new QUdpSocket(this);
    wproto = new Wakeproto();
    QObject::connect(&crc_error_timer, SIGNAL(timeout()), this, SLOT(crc_error_timeout()));
    QObject::connect(&tx_timer, SIGNAL(timeout()), this, SLOT(send_queue()));
    QObject::connect(&adc_timer, SIGNAL(timeout()), this, SLOT(get_adc1()));
    QObject::connect(&range_timer, SIGNAL(timeout()), this, SLOT(get_range()));
    QObject::connect(&T1_timer, SIGNAL(timeout()), this, SLOT(T1_timeout()));
    QObject::connect(&T2_timer, SIGNAL(timeout()), this, SLOT(T2_timeout()));
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
    QByteArray command;
    if(event->key() == Qt::Key_Up)
    {
        ui->lb_UP->setPixmap(btn_DOWN);
        if(x_coord < 100 ){
            x_coord++;
        }
        command.append(100);
        send_packet(5,1,command,0);
    }
    if(event->key() == Qt::Key_Down)
    {
        ui->lb_DOWN->setPixmap(btn_DOWN);
        if(x_coord > 0){
            x_coord--;
        }
        command.append(100);
        send_packet(5,2,command,0);
    }
    if(event->key() == Qt::Key_Left)
    {
        ui->lb_LEFT->setPixmap(btn_DOWN);
        y_coord++;
        command.append(100);
        send_packet(5,3,command,0);
    }
    if(event->key() == Qt::Key_Right)
    {
        ui->lb_RIGHT->setPixmap(btn_DOWN);
        y_coord--;
        command.append(100);
        send_packet(5,4,command,0);
    }
    command.clear();
    //ui->battery_main->setValue(x_coord);
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

        crc_error_timer.start(300);
        tx_timer.start(50);
        //adc_timer.start(500);
        if(ui->cbRange->isChecked()){
            range_timer.start(1000);
        }
        T1_timer.start(1000);
        ui->pbComPortOpen->setText("Close");
        ui->battery_main->setEnabled(TRUE);
        ui->lbBattery_main->setEnabled(TRUE);
        ui->signal_strenght->setEnabled(TRUE);
        ui->lbSignal_Strenght->setEnabled(TRUE);
    } else {
        ui->comPortList->setEnabled(true);
        port_opened = FALSE;
        serial.close();
        crc_error_timer.stop();
        tx_timer.stop();
        range_timer.stop();
        T1_timer.stop();
        T2_timer.stop();
        qDebug() << "PORT CLOSED" << endl;
        ui->pbComPortOpen->setText("Open");
        ui->battery_main->setValue(0);
        ui->signal_strenght->setValue(0);
        ui->battery_main->setEnabled(FALSE);
        ui->lbBattery_main->setEnabled(FALSE);
        ui->signal_strenght->setEnabled(FALSE);
        ui->lbSignal_Strenght->setEnabled(FALSE);
    }

}

void MainWindow::readRequest() {
    wproto->getpacket(serial.readAll());
}
void MainWindow::packet_rcvd(QByteArray packet)
{
    qDebug() << "[PDU][INFO]: Packet received!" << endl;
    wproto->dump_packet(packet);
    process_packet(packet.at(cmd), packet);
}

int MainWindow::process_packet(char command, QByteArray packet) {
    unsigned int val;
    switch (command) {
    case 10:
        // ADC value
        val = ((static_cast<unsigned int>(packet.at(0)) & 0xFF) << 8)
                + (static_cast<unsigned int>(packet.at(1)) & 0xFF);
        ui->battery_main->setValue(val);
        ack_num = 10;
        sending = FALSE;
        ui->tableStatus->item(3,1)->setText(QString::number(T2_timer.interval() - T2_timer.remainingTime()));
        T2_timer.stop();
        break;
    case 1:
        // Motor UP feedback
        //ui->tableStatus->item(2,1)->setText(QString::number(packet.at(0)));
        //ack_num = 11;
        sending = FALSE;
        qDebug() << "ACK 1 received (" << QString::number(packet.at(0)) << ") T2: " << QString::number(T2_timer.interval() - T2_timer.remainingTime());
        T2_timer.stop();
        break;
    case 2:
        // Motor DOWN feedback
        sending = FALSE;
        qDebug() << "ACK 2 received (" << QString::number(packet.at(0)) << ") T2: " << QString::number(T2_timer.interval() - T2_timer.remainingTime());
        T2_timer.stop();
        break;
    case 3:
        // Motor LEFT feedback
        sending = FALSE;
        qDebug() << "ACK 3 received (" << QString::number(packet.at(0)) << ") T2: " << QString::number(T2_timer.interval() - T2_timer.remainingTime());
        T2_timer.stop();
        break;
    case 4:
        // Motor RIGHT feedback
        sending = FALSE;
        qDebug() << "ACK 4 received (" << QString::number(packet.at(0)) << ") T2: " << QString::number(T2_timer.interval() - T2_timer.remainingTime());
        T2_timer.stop();
        break;
    case 5:
        // Range
        val = ((static_cast<unsigned int>(packet.at(0)) & 0xFF) << 8)
                + (static_cast<unsigned int>(packet.at(1)) & 0xFF);
        ui->tableStatus->item(1,1)->setText(QString::number(int(val * 0.63)));
        ack_num = 5;
        sending = FALSE;
        qDebug() << "ACK 5 received (" << QString::number(int(val * 0.63)) << ") T2: " << QString::number(T2_timer.interval() - T2_timer.remainingTime());
        T2_timer.stop();
        break;
    default:
        ui->tableStatus->item(1,1)->setText("N/A");
        break;
    }
    return 0;
}

int MainWindow::send_packet(char addr, unsigned char command, QByteArray data, bool urgency = 0) {
    QByteArray packet;
    packet = wproto->createpacket(addr,command,data);
    if (ui->cbNoQueue->isChecked()){
        if (connection_type == conn_serial){
            serial.write(packet);
        } else if (connection_type == conn_ip) {
            udp_socket->writeDatagram(packet, QHostAddress(ui->leIPaddress->text()), 42000);
        } else {
            qDebug() << "Error: unknown connection type";
        }
    } else if (urgency) {
        tx_queue_urgency.enqueue(packet);
    } else {
        tx_queue.enqueue(packet);
    }
    wproto->dump_packet(packet);
    packet.clear();
    return 0;
}

void MainWindow::on_battery_main_valueChanged(int value)
{
    int value_maximum = ui->battery_main->maximum();
    int value_percent = value * 100 / value_maximum;
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

//void MainWindow::on_pushButton_clicked() {
//    send_packet(201,5,0,0);
//}
void MainWindow::crc_error_timeout() {
    ui->tableStatus->item(0,1)->setText(QString::number(rx_crc_error_count));
    rx_crc_error_count = 0;
}

void MainWindow::get_adc1() {
    QByteArray tx_data;
    tx_data.append(0xD0);
    send_packet(201,10,tx_data,0);
    tx_data.clear();
}
void MainWindow::get_range() {
    send_packet(201,5,0,1);
}
// Transmit buffer overload
void MainWindow::T1_timeout() {
    if (!tx_queue_urgency.isEmpty() && tx_queue_urgency.size() > 10) {
        qDebug() << "TX urgency buffer overload: " << tx_queue_urgency.size();
        tx_queue_urgency.clear();
    }
    if (!tx_queue.isEmpty() && tx_queue.size() > 2) {
        qDebug() << "TX normal buffer overload: " << tx_queue.size();
        tx_queue.clear();
    }
    ui->tableStatus->item(2,1)->setText(QString::number(tx_queue.size()));
}

// Send packet timeout
void MainWindow::T2_timeout() {
    qDebug() << "T2 timeout";
    sending = FALSE;
    T2_timer.stop();
}

void MainWindow::send_queue() {
    if(!tx_queue_urgency.isEmpty() && sending == FALSE) {
        if(ui->cb_acks->isChecked()){
            T2_timer.start(300);
            if (connection_type == conn_serial){
                serial.write(tx_queue_urgency.dequeue());
            } else if (connection_type == conn_ip) {
                udp_socket->writeDatagram(tx_queue_urgency.dequeue(), QHostAddress(ui->leIPaddress->text()), 42000);
            } else {
                qDebug() << "Error: unknown connection type";
            }

            sending = TRUE;
        } else {
            if (connection_type == conn_serial){
                serial.write(tx_queue_urgency.dequeue());
            } else if (connection_type == conn_ip) {
                udp_socket->writeDatagram(tx_queue_urgency.dequeue(), QHostAddress(ui->leIPaddress->text()), 42000);
            } else {
                qDebug() << "Error: unknown connection type";
            }
        }

    }
    if(!tx_queue.isEmpty() && sending == FALSE) {
        if(ui->cb_acks->isChecked()){
            T2_timer.start(300);
            if (connection_type == conn_serial){
                serial.write(tx_queue.dequeue());
            } else if (connection_type == conn_ip) {
                udp_socket->writeDatagram(tx_queue.dequeue(), QHostAddress(ui->leIPaddress->text()), 42000);
            } else {
                qDebug() << "Error: unknown connection type";
            }
        } else {
            if (connection_type == conn_serial){
                serial.write(tx_queue.dequeue());
            } else if (connection_type == conn_ip) {
                udp_socket->writeDatagram(tx_queue.dequeue(), QHostAddress(ui->leIPaddress->text()), 42000);
            } else {
                qDebug() << "Error: unknown connection type";
            }
        }
    }
}

void MainWindow::on_cbRange_stateChanged(int arg1)
{
    switch (arg1) {
    case 2:
        range_timer.start(1000);
        break;
    case 0:
        range_timer.stop();
        break;
    default:
        break;
    }
}

void MainWindow::on_cb_acks_stateChanged(int arg1)
{
    QByteArray tx_data;
    switch (arg1) {
    case 2:
        tx_data.append(0x50);
        send_packet(201,0,tx_data,1);
        tx_data.clear();
        T1_timer.start(1000);
        break;
    case 0:
        tx_data.append(0x51);
        send_packet(201,0,tx_data,1);
        tx_data.clear();
        sending = FALSE;
        T1_timer.stop();
        break;
    default:
        break;
    }
}

void MainWindow::on_rbSerial_clicked()
{
    ui->comPortList->setEnabled(true);
    ui->pbComPortOpen->setEnabled(true);
    ui->pbIpOpen->setEnabled(false);
    ui->leIPaddress->setEnabled(false);
    connection_type = conn_serial;
}

void MainWindow::on_rbIP_clicked()
{
    ui->comPortList->setEnabled(false);
    ui->pbComPortOpen->setEnabled(false);
    ui->pbIpOpen->setEnabled(true);
    ui->leIPaddress->setEnabled(true);
    connection_type = conn_ip;
}

void MainWindow::on_cbNoQueue_stateChanged(int arg1)
{

}

void MainWindow::on_pbIpOpen_clicked()
{
    if(!socket_opened) {
        ui->leIPaddress->setEnabled(false);
        socket_opened = TRUE;
        udp_socket->bind(QHostAddress::LocalHost, 42001);

        qDebug() << "SOCKET OPENED" << endl;

        crc_error_timer.start(300);
        tx_timer.start(50);
        //adc_timer.start(500);
        if(ui->cbRange->isChecked()){
            range_timer.start(1000);
        }
        T1_timer.start(1000);
        ui->pbIpOpen->setText("Close");
        ui->battery_main->setEnabled(TRUE);
        ui->lbBattery_main->setEnabled(TRUE);
        ui->signal_strenght->setEnabled(TRUE);
        ui->lbSignal_Strenght->setEnabled(TRUE);
    } else {
        ui->leIPaddress->setEnabled(true);
        socket_opened = FALSE;
        udp_socket->close();
        crc_error_timer.stop();
        tx_timer.stop();
        range_timer.stop();
        T1_timer.stop();
        T2_timer.stop();
        qDebug() << "SOCKET CLOSED" << endl;
        ui->pbIpOpen->setText("Open");
        ui->battery_main->setValue(0);
        ui->signal_strenght->setValue(0);
        ui->battery_main->setEnabled(FALSE);
        ui->lbBattery_main->setEnabled(FALSE);
        ui->signal_strenght->setEnabled(FALSE);
        ui->lbSignal_Strenght->setEnabled(FALSE);
    }

}

void MainWindow::on_pbCustomSend_clicked()
{
    send_packet(ui->leAddr->text().toInt(),ui->leCmd->text().toInt(),ui->leData->text().toUtf8() ,1);
}
