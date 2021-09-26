#include "datamanagement.h"

DataManagement::DataManagement(QObject *parent) : QObject(parent)
{
    serverValues.ServerIp = "192.168.152.128";
    serverValues.ServerPort = 4001;
    serverValues.SocketDebug = true;
}
void DataManagement::onConnected()
{
    connect(server->pSocket,&QWebSocket::textMessageReceived,this,&DataManagement::MessageReceived);
    connect(server->pSocket,&QWebSocket::disconnected,this,&DataManagement::onDisconnected);

}
void DataManagement::SocketStart()
{
    qDebug() << "SystemBase SocketStart";

    bool debug = serverValues.SocketDebug;
    int port = serverValues.ServerPort;
    QString ip = serverValues.ServerIp;

    server = new TcpSocketCommunication(ip,port, debug);
    connect(server->m_pWebSocketServer, &QWebSocketServer::newConnection,this, &DataManagement::onConnected);
    connect(server, &TcpSocketCommunication::closed, this, &QCoreApplication::quit);

    log.logWork("SocketStart","","","Socket başlatıldı.");
}

void DataManagement::SerialStart()
{
    serialComm.initSerial();
    log.logWork("SerialStart","","","Seri haberleşme başlatıldı.");
}
void DataManagement::onDisconnected()
{
    isSocketOpened = false;
     auto iter = ClientStatesStore.begin();
     while(iter !=  ClientStatesStore.end()){
         ClientStatesStateText = iter->State ? "true" : "false";
         if(iter->Id == server->ClosedConnectionName){
             iter->State = isSocketOpened;
             log.logWork("onDisconnected","Connection Lost",iter->Id," Disconnected informations...");
         }

     iter++;
     }

     ClientStateList();

}

void DataManagement::Identification_SystemControl(QString socketmessage, QJsonObject jsonObject)
{
    qDebug() << "Appidentification SystemControl geldim";

    QJsonObject identificationparameters =  jsonObject.value("message").toObject();
    QString app = identificationparameters.value(QString("App")).toString();
    QString systemname = identificationparameters.value(QString("SystemName")).toString();
    QString devicename = identificationparameters.value(QString("DeviceName")).toString();
    QString devicetype = identificationparameters.value(QString("DeviceType")).toString();

    isSocketOpened = true;
    ClientStates.Id = server->ConnectedClient.identification;
    ClientStates.State = isSocketOpened;
    ClientStates.DeviceName = devicename;
    ClientStates.DeviceType = devicetype;
    ClientStatesStateText = ClientStates.State ? "true" : "false";

    auto iter = ClientStatesStore.begin();
    while(iter !=  ClientStatesStore.end())
    {
        ClientStatesStateText = iter->State ? "true" : "false";
        if(iter->Id == server->ConnectedClient.identification)
        {
            isSocketalreadyOpened = true;
            iter->State = isSocketOpened;
        }
        iter++;
    }

    ClientStatesStateText = ClientStates.State ? "true" : "false";

    if(isSocketalreadyOpened == false)
    {
        ClientStatesStore.push_back(ClientStates);
        RemoveableClientStatesStore.push_back(ClientStates);
        log.logWork("identification/App","New Connection",ClientStates.Id," New Connected informations...");
        if(ClientStates.Id == to_pis)
        {
            pisconnected = true;
        }
    }
    else
    {
        isSocketalreadyOpened = false;
        log.logWork("identification/App","Reconnect",ClientStates.Id," ReConnected informations...");
        if(ClientStates.Id == to_pis)
        {
            pisconnected = true;
        }
    }

    qDebug() << "System Model Checking...";
    if(systemname == "Ankara")
    {
        CorrectSystemFlag = true;
        qDebug() << "Correct System, System Model: " << systemname;
    }
    else
    {
        CorrectSystemFlag = false;
        qDebug() << "Wrong System, System Model: " << systemname << " Client will be closing";
        for(int i=0;i<server->m_clients.size();i++)
        {
            if(server->m_clients.at(i)->objectName() == server->ConnectedClient.identification)
            {
                server->m_clients.at(i)->close();
                qDebug() << "Client Closed..";

                auto iter = ClientStatesStore.begin();
                while(iter !=  ClientStatesStore.end())
                {
                    ClientStatesStateText = iter->State ? "true" : "false";
                    if(iter->Id == server->ConnectedClient.identification)
                    {
                        isSocketalreadyOpened = false;
                        iter->State = false;
                    }
                    iter++;
                }
            }
        }
    }

    ClientStateList();
}

void DataManagement::ClientStateList()
{
    auto iterlist = ClientStatesStore.begin();
    while(iterlist !=  ClientStatesStore.end())
    {
        ClientStatesStateText = iterlist->State ? "true" : "false";
        qDebug() << "Current Client States of " << iterlist->Id << ": " << iterlist->State;
        log.logWork("ClientStateList","",iterlist->Id+"="+ClientStatesStateText,"List of Current Client States");
        iterlist++;
    }

}


void DataManagement::MessageReceived(QString message)
{
        QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8());
        QJsonObject jsonObject = jsonDoc.object();
        QString topic = jsonObject.value(QString("topic")).toString();
        QString socketmessage = jsonObject.value(QString("message")).toString();

        if(topic == "identification/SystemControl")
        {
            Identification_SystemControl(socketmessage,jsonObject);
        }

        else{

            if(topic == "ping")
            {
                //Ping(socketmessage);
            }

        }
}
