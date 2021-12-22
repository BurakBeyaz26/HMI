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

    QString app = jsonObject.value(QString("App")).toString();
    QString systemname = jsonObject.value(QString("SystemName")).toString();
    QString devicename = jsonObject.value(QString("DeviceName")).toString();
    QString devicetype = jsonObject.value(QString("DeviceType")).toString();

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
    if(systemname == "PC")
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

void DataManagement::sendSocketMessageJson(QString type,QString topic,QString message)
{

    int order = -1;
    QString ordername = "noone";
    bool is_confirm = false;
    int iteratorindex = 0;

    auto clientsiterator = ClientStatesStore.begin();
    while(clientsiterator != ClientStatesStore.end())
    {
        iteratorindex = std::distance(ClientStatesStore.begin(), clientsiterator);

        if(clientsiterator->Id == type)
        {
            for(int i = 0;i < server->m_clients.size();i++)
            {
                QString objectmyname = server->m_clients.at(i)->objectName();
                if(objectmyname == type)
                {
                    qDebug() << objectmyname;
                    order = i;
                    ordername = clientsiterator->Id;
                    is_confirm = clientsiterator->State;
                }
            }
            break;
        }
        else
        {
            *clientsiterator++;
        }
    }

    if(is_confirm)
    {
        QString SocketMessage;

            QJsonObject JsonMessagePackage;
            JsonMessagePackage["topic"] = topic;
            JsonMessagePackage["message"] = message;
            QString strFromObj = QJsonDocument(JsonMessagePackage).toJson(QJsonDocument::Compact);
            server->m_clients.at(order)->sendTextMessage(strFromObj);
            if(!server->m_clients.at(order)->errorString().isNull())
            {
                log.logWork("SendSocketMessageJson - stringmessage - containmessage","true",SocketMessage,"send to --> "+ordername);
                qDebug() << "send to --> "<< ordername;
            }
            else
            {
                log.logWork("SendSocketMessageJson - stringmessage - containmessage","false",SocketMessage,"error on send to --> "+ordername);
                qDebug() << "error on send to --> "+ordername;
            }

    }
    else
    {
        QJsonObject JsonMessagePackage;
        JsonMessagePackage["topic"] = topic;
        JsonMessagePackage["message"] = message;
        QString strFromObj = QJsonDocument(JsonMessagePackage).toJson(QJsonDocument::Compact);
        serialComm.sendMessage(strFromObj);
        log.logWork("SendSocketMessageJson - stringmessage","false","Confirm: "+is_confirm,"error on send to --> "+ordername+" socket could be off --> undelivered topic: "+topic);
        qDebug() << ordername << " socket could be off is_confirm: " << is_confirm << "undelivered topic: " << topic;
    }
}

void DataManagement::MessageReceived(QString message)
{
        QString socketmessage;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8());
        QJsonObject jsonObject = jsonDoc.object();
        QString topic = jsonObject.value(QString("topic")).toString();
        QJsonObject messageObject =  jsonObject.value("message").toObject();

        //socketmessage = jsonObject.value(QString("message")).toString();

        if(topic == "identification/SystemControl")
        {
            Identification_SystemControl(topic,messageObject);
        }

        else{

            if(topic == "ping")
            {
                //Ping(socketmessage);
            }

        }
}
