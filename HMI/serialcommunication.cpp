#include "serialcommunication.h"

SerialCommunication::SerialCommunication(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
}

SerialCommunication::~SerialCommunication()
{
    closeSerial();
}

void SerialCommunication::initSerial()
{
    serial->setPortName("/dev/tty2");
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
