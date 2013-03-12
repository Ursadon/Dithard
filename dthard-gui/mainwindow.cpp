#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
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
    parital_packet = FALSE;
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
    QTextStream out(stdout);
    if(!port_opened) {
        ui->comPortList->setEnabled(false);
        port_opened = TRUE;
        serial.setPortName(ui->comPortList->currentText());
        if (!serial.open(QIODevice::ReadWrite)) {
            out << tr("Can't open %1, error code %2").arg(serial.portName()).arg(serial.error()) << endl;
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
        out << "PORT OPENED" << endl;
        ui->pbComPortOpen->setText("Close");
        ui->battery_main->setEnabled(TRUE);
        ui->lbBattery_main->setEnabled(TRUE);
    } else {
        ui->comPortList->setEnabled(true);
        port_opened = FALSE;
        serial.close();
        out << "PORT CLOSED" << endl;
        ui->pbComPortOpen->setText("Open");
        ui->battery_main->setValue(0);
        ui->signal_strenght->setValue(0);
        ui->battery_main->setEnabled(FALSE);
        ui->lbBattery_main->setEnabled(FALSE);
    }

}

void MainWindow::readRequest()
{
    QTextStream out(stdout);
    QByteArray temp_data = serial.readAll(); // Заполняем массив данными
    int fend_offset = temp_data.indexOf(0xC0);
    int fend_count = temp_data.count(0xC0);
    char fend_header_start, fend_header_end, fend_header_size;
    char x,y = 0;
    QByteArray fend_offset_array;

    if(fend_count > 1) {
        fend_offset_array.append(temp_data.indexOf(0xC0));
        out << "multi-packet RX total:" << QString::number(fend_count) << endl;
        for (int var = 0; var < fend_count; var++) {
            y = fend_offset_array.at(var)+1;
            x = temp_data.indexOf(0xC0,y);
            fend_offset_array.append(x);
            //out << "FEND at: " << QString::number(fend_offset_array.at(var)) << endl;
        }
        // Теперь у нас есть массив с позициями заголовков
        for (int var = 0; var < fend_count; var++) {
            fend_header_start = fend_offset_array.at(var);
            fend_header_end = fend_offset_array.at(var+1);
            fend_header_size = fend_header_end - fend_header_start;
            out << "FEND boundaries at: " << QString::number(fend_header_start) << " + " << QString::number(fend_header_end) << endl;
            if (fend_header_size >= 4) {
                bytes = temp_data.mid(fend_header_start,fend_header_size);
                if(temp_data.at(fend_header_start+1) <= bytes.size()) {
                    adc.append(bytes.right(2));
                    int size = ((static_cast<unsigned int>(adc.at(0)) & 0xFF) << 8)
                            + (static_cast<unsigned int>(adc.at(1)) & 0xFF);
                    out << "RCVD PACKET! " << bytes.size() << " : " <<  QString::number(size) << endl;
                    adc.clear();
                    bytes.clear();
                    parital_packet = FALSE;
                }
            } else {
                parital_packet = TRUE;
            }
        }
        fend_offset_array.clear();
    } else {
        if (fend_offset != -1) {
            // Скинули всё, что идёт после FEND в пакет
            bytes.append(temp_data.mid(fend_offset));
            // Если полностью получили заголовок - то парсим его
            parital_packet = TRUE;
        } else if(parital_packet == TRUE) {
            bytes += temp_data;
        }
        if (bytes.size() >= 4) {
            if(bytes.at(1) <= bytes.size()) {
                adc.append(bytes.right(2));
                int size = ((static_cast<unsigned int>(adc.at(0)) & 0xFF) << 8)
                        + (static_cast<unsigned int>(adc.at(1)) & 0xFF);
                out << "RCVD PACKET! " << bytes.size() << " : " <<  QString::number(size) << endl;
                adc.clear();
                bytes.clear();
                parital_packet = FALSE;
            }
        }
    }
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
