#include "myserialport.h"

#include <QLabel>
#include <QMessageBox>

MySerialPort::MySerialPort(QString name, QWidget *parent) :
    QWidget(parent),
    stm32Name(name),
    m_serial(new QSerialPort(this))

{
    stm32Name = "ttyACM0";
    bool bOpened = openSerialPort();
    if(bOpened)
    {
        connect(m_serial, &QSerialPort::errorOccurred, this, &MySerialPort::handleError);
        connect(m_serial, &QSerialPort::readyRead, this, &MySerialPort::readData);
    }
    else
        this->setEnabled(false);
}


MySerialPort::~MySerialPort()
{

}




bool MySerialPort::openSerialPort()
{
    m_serial->setPortName(stm32Name);
    m_serial->setBaudRate(QSerialPort::BaudRate::Baud9600);
    m_serial->setDataBits(QSerialPort::DataBits::Data7);
    m_serial->setParity(QSerialPort::Parity::OddParity);
    m_serial->setStopBits(QSerialPort::StopBits::OneStop);
    m_serial->setFlowControl(QSerialPort::FlowControl::NoFlowControl);

    if (m_serial->open(QIODevice::ReadOnly)) {
        return true;
    } else {
        if(stm32Name == "ttyACM0")
        {
            stm32Name = "ttyACM1";
            openSerialPort();
        }
        else if(stm32Name == "ttyACM1")
        {
            stm32Name = "ttyACM2";
            openSerialPort();
        }
        else if(stm32Name == "ttyACM0")
        {
            stm32Name = "ttyACM3";
            openSerialPort();
        }
        else
            return false;

    }
}

void MySerialPort::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
}

void MySerialPort::readData()
{
    disconnect(m_serial, &QSerialPort::readyRead, this, &MySerialPort::readData);

        QByteArray data = m_serial->readAll();
        QString line = data;
        if(line.indexOf("\r") == 0)
        {
           parseBuffer(mBuffer1);
           mBuffer1.clear();
        }
        else
        {
             mBuffer1 = mBuffer1 + data;
        }

    connect(m_serial, &QSerialPort::readyRead, this, &MySerialPort::readData);
}


void MySerialPort::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());
        closeSerialPort();
    }
}




void MySerialPort::parseBuffer(QString line)
{
    QStringList listLine = line.split(" ");
    QString nom = listLine.value(1);
    double temp = listLine.value(3).toDouble();
    double pressure = listLine.value(5).toDouble();
    sendBuffer(nom, temp, pressure);
}




void MySerialPort::sendBuffer(QString name, double temp, double pressure)
{
    if (mChambre == NULL)
    {
        mChambre = new RoomStmModel(name, temp, pressure);
    }
    else
    {
        mChambre->setName(name);
        mChambre->setTemp(temp);
        mChambre->setPressure(pressure);
    }
    emit sendDataChambre (mChambre);

}

