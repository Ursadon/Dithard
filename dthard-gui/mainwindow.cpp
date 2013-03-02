#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    x_coord = 0;
    y_coord = 0;
    btn_UP.load(":/graphics/img/circle_blue.png");
    btn_DOWN.load(":/graphics/img/circle_green.png");
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
    ui->battery_main->setStyleSheet(st_green);
    ui->signal_strenght->setStyleSheet(st_green);
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
    QString st_yellow = QString (
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
    if(event->key() == Qt::Key_Up)
    {
        ui->lb_UP->setPixmap(btn_DOWN);
        x_coord++;
    }
    if(event->key() == Qt::Key_Down)
    {
        ui->lb_DOWN->setPixmap(btn_DOWN);
        x_coord--;
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
    if (x_coord < 30) {
        ui->battery_main->setStyleSheet(st_red);
    } else if (x_coord >= 30 && x_coord <= 65) {
        ui->battery_main->setStyleSheet(st_yellow);
    } else if (x_coord > 65) {
        ui->battery_main->setStyleSheet(st_green);
    }
}

void MainWindow::on_battery_main_valueChanged(int value)
{

}
