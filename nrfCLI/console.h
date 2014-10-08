#ifndef CONSOLE_H
#define CONSOLE_H

#include <QCoreApplication>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <iostream>

class Console : public QThread
{
    Q_OBJECT
public:
    explicit Console(QObject *parent = 0);
    ~Console();
signals:

public slots:

protected:
    void run();

};

#endif // CONSOLE_H
