#ifndef DATAMANAGEMENT_H
#define DATAMANAGEMENT_H

#include <QObject>
#include <QCoreApplication>
#include "serialcommunication.h"
#include "tcpsocketcommunication.h"
#include "log.h"


struct ClientSocketStates
{
    QString Id;
    bool State;
    QString DeviceName;
    QString DeviceType;
};
struct ServerConfigValues
{
    QString ServerIp;
    int ServerPort;
    bool SocketDebug;
};

class DataManagement : public QObject
{
    Q_OBJECT
public:
    explicit DataManagement(QObject *parent = nullptr);

    void SocketStart();
    void SerialStart();
    Log log;


    ServerConfigValues serverValues;

    QString to_pis = "PassengerInformationSystem";
    bool pisconnected = false;
    bool CorrectSystemFlag;

    Q_INVOKABLE void sendSocketMessageJson(QString type,QString topic,QString message);

public slots:
    void onConnected();
    void onDisconnected();
    void MessageReceived(QString message);


private:
    bool isSocketOpened;
    bool isSocketalreadyOpened = false;
    QString ClientStatesStateText;
    ClientSocketStates ClientStates;
    std::vector<ClientSocketStates> ClientStatesStore;
    std::vector<ClientSocketStates> RemoveableClientStatesStore;

    void Identification_SystemControl(QString socketmessage, QJsonObject jsonObject);
    void ClientStateList();

protected:

    TcpSocketCommunication *server;
    SerialCommunication serialComm;

signals:

};

#endif // DATAMANAGEMENT_H
