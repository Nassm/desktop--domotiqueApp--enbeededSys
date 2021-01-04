#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QWidget>
#include <QMainWindow>
#include <QSerialPort>
#include <QDebug>
#include "rommstmmodel.h"
#include <QEventLoop>



class MySerialPort : public QWidget
{
    Q_OBJECT

public:
    explicit MySerialPort(QString name, QWidget *parent = nullptr);
    ~MySerialPort();

public slots:
    bool openSerialPort();
    void closeSerialPort();
    void readData();
    void handleError(QSerialPort::SerialPortError error);

signals :
    void sendDataChambre (RoomStmModel*);

private:

    void parseBuffer(QString line);
    void sendBuffer(QString name, double temp, double pressure);

    QString stm32Name;
    QSerialPort *m_serial = nullptr;
    RoomStmModel* mChambre = nullptr;
    QString mBuffer1;
};

#endif // MYSERIALPORT_H
