#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

const unsigned char crc8Table[256] = {
	0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97,
	0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E,
	0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4,
	0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C, 0x6D,
	0x86, 0xB7, 0xE4, 0xD5, 0x42, 0x73, 0x20, 0x11,
	0x3F, 0x0E, 0x5D, 0x6C, 0xFB, 0xCA, 0x99, 0xA8,
	0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52,
	0x7C, 0x4D, 0x1E, 0x2F, 0xB8, 0x89, 0xDA, 0xEB,
	0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA,
	0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22, 0x13,
	0x7E, 0x4F, 0x1C, 0x2D, 0xBA, 0x8B, 0xD8, 0xE9,
	0xC7, 0xF6, 0xA5, 0x94, 0x03, 0x32, 0x61, 0x50,
	0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C,
	0x02, 0x33, 0x60, 0x51, 0xC6, 0xF7, 0xA4, 0x95,
	0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F,
	0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7, 0xD6,
	0x7A, 0x4B, 0x18, 0x29, 0xBE, 0x8F, 0xDC, 0xED,
	0xC3, 0xF2, 0xA1, 0x90, 0x07, 0x36, 0x65, 0x54,
	0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE,
	0x80, 0xB1, 0xE2, 0xD3, 0x44, 0x75, 0x26, 0x17,
	0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B,
	0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3, 0xD2,
	0xBF, 0x8E, 0xDD, 0xEC, 0x7B, 0x4A, 0x19, 0x28,
	0x06, 0x37, 0x64, 0x55, 0xC2, 0xF3, 0xA0, 0x91,
	0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0,
	0xFE, 0xCF, 0x9C, 0xAD, 0x3A, 0x0B, 0x58, 0x69,
	0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93,
	0xBD, 0x8C, 0xDF, 0xEE, 0x79, 0x48, 0x1B, 0x2A,
	0xC1, 0xF0, 0xA3, 0x92, 0x05, 0x34, 0x67, 0x56,
	0x78, 0x49, 0x1A, 0x2B, 0xBC, 0x8D, 0xDE, 0xEF,
	0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15,
	0x3B, 0x0A, 0x59, 0x68, 0xFF, 0xCE, 0x9D, 0xAC
};

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
	sending = FALSE;
	btn_UP.load(":/graphics/img/circle_blue.png");
	btn_DOWN.load(":/graphics/img/circle_green.png");
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
        if(ui->checkBox->isChecked()){
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
	QByteArray rx_buffer = serial.readAll();
	QByteArray rx_data;
	unsigned char rx_crc_calculated = 0xFF;
	unsigned char rx_crc_actual = 0xFF;

	foreach (unsigned char rx_byte, rx_buffer) {
		if (rx_byte == FEND && packet_started == TRUE) {
			// Получили FEND, но пакет ещё не окончен - сбрасываем данные
			bytes.clear();
			bytes.append(rx_byte);
			num_of_bytes = 0;
			rx_data.clear();
			rx_buffer.clear();

		} else if (packet_started) {
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
				rx_data = bytes.mid(datastream,bytes.size()-5);
				foreach (unsigned char k, rx_data) {
					rx_crc_calculated = crc8Table[rx_crc_calculated ^ k];
				}
				rx_crc_actual = bytes.right(1).at(0);
				if (rx_crc_actual != rx_crc_calculated) {
					//qDebug() << "[RX] CRC error" << QString::number(rx_crc_actual) << " (" << QString::number(rx_crc_calculated) << ")" << endl;
					rx_crc_error_count++;
					// Send NACK
					bytes.clear();
					bytes.append(0xAE);
                    send_packet(201,60,bytes,0);
				} else {
					//                    qDebug() << "[RX] FEND" << endl
					//                             << "[RX] ADDR: " << QString::number(static_cast<unsigned char>(bytes.at(addr))) << endl
					//                             << "[RX] CMD: " << QString::number(static_cast<unsigned char>(bytes.at(cmd))) << endl
					//                             << "[RX] N: " << QString::number(static_cast<unsigned char>(bytes.at(n))) << endl
					//                             << "[RX] DATA: " << rx_data.toHex() << endl
					//                             << "[RX] CRC: " << QString::number(rx_crc_actual) << " (" << QString::number(rx_crc_calculated) << ")" << endl
					//                             << "----------------------------" << endl;
					process_packet(bytes.at(cmd), rx_data);
				}
				data_started = FALSE;
				packet_started = FALSE;
				num_of_bytes = 0;
				bytes.clear();
				rx_data.clear();
				rx_buffer.clear();
			}
		} else if (rx_byte == FEND) {
			bytes.append(rx_byte);
			packet_started = TRUE;
		}
	}
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
	unsigned char tx_crc = 0xFF;
	foreach (unsigned char k, data) {
		tx_crc = crc8Table[tx_crc ^ k];
	}
	// TODO: Byte stuffing
	packet.append(FEND);
	packet.append(addr);
	packet.append(command);
	packet.append(data.size()); // N
	packet.append(data);
	packet.append(tx_crc); // CRC
    if (ui->checkBox_2->isChecked()){
        serial.write(packet);
    } else if (urgency) {
        tx_queue_urgency.enqueue(packet);
    } else {
        tx_queue.enqueue(packet);
    }
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

void MainWindow::on_pushButton_clicked() {
    send_packet(201,5,0,0);
}
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
            serial.write(tx_queue_urgency.dequeue());
            sending = TRUE;
        } else {
            serial.write(tx_queue_urgency.dequeue());
        }

    }
    if(!tx_queue.isEmpty() && sending == FALSE) {
        if(ui->cb_acks->isChecked()){
            T2_timer.start(300);
            serial.write(tx_queue.dequeue());
            sending = TRUE;
        } else {
            serial.write(tx_queue.dequeue());
        }
    }
}

void MainWindow::on_checkBox_stateChanged(int arg1)
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

void MainWindow::on_rbSerial_clicked(bool checked)
{
    ui->comPortList->setEnabled(true);
    ui->pbComPortOpen->setEnabled(true);
    ui->pbIpOpen->setEnabled(false);
    ui->leIPaddress->setEnabled(false);
}

void MainWindow::on_rbIP_clicked(bool checked)
{
    ui->comPortList->setEnabled(false);
    ui->pbComPortOpen->setEnabled(false);
    ui->pbIpOpen->setEnabled(true);
    ui->leIPaddress->setEnabled(true);
}
