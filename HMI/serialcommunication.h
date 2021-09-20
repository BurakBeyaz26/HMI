#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QIODevice>
#include <QDebug>

class SerialCommunication : public QObject
{
    Q_OBJECT
public:
    explicit SerialCommunication(QObject *parent = nullptr);
    ~SerialCommunication();
    void initSerial();
    void closeSerial();


signals:
private:
    QSerialPort *serial;

};

#endif // SERIALCOMMUNICATION_H
