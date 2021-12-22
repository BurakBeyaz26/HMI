#include "serialcommunication.h"

SerialCommunication::SerialCommunication(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    QList<QSerialPortInfo> serialports = QSerialPortInfo::availablePorts();

    auto iter = serialports.begin();
    while(iter != serialports.end())
    {
        QSerialPortInfo inf = *iter;
        qDebug() << "inf: " << inf.description();
        qDebug() << "manufacturer: " << inf.manufacturer();
        qDebug() << "name: " << inf.portName();
        *iter++;
    }
}

SerialCommunication::~SerialCommunication()
{
    closeSerial();
}

void SerialCommunication::initSerial()
{
    serial->setPortName("COM2");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "Connected";
    }
    else
    {
        qCritical() << "ERROR: " << serial->errorString();
    }
}

void SerialCommunication::closeSerial()
{
     if(serial->isOpen())
         serial->close();
}

void SerialCommunication::sendMessage(QString message)
{
    serial->write(message.toUtf8());
}
