#include "tcpsocketcommunication.h"
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QVariant>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QT_USE_NAMESPACE

TcpSocketCommunication::TcpSocketCommunication(const QString &ip, quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Modernisation Server"),
                                            QWebSocketServer::NonSecureMode, this)),

    m_debug(debug)
{

    if(m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        if (m_debug)
            qDebug() << "Modernisation Server listening on ip: "<< "Any" << " port: " << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &TcpSocketCommunication::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &TcpSocketCommunication::closed);

    }
}

TcpSocketCommunication::~TcpSocketCommunication()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void TcpSocketCommunication::onNewConnection()
{
    pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &TcpSocketCommunication::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &TcpSocketCommunication::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &TcpSocketCommunication::socketDisconnected);

    m_clients << pSocket;
    qDebug() << "New Connection...";
    qDebug() << pSocket->peerAddress().toString();
}

void TcpSocketCommunication::processTextMessage(const QString &message)
{
    pClient = qobject_cast<QWebSocket *>(sender());

    QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObject = jsonDoc.object();
    QString topic = jsonObject.value(QString("topic")).toString();

    if(topic == "identification/SystemControl"){
         QJsonObject clientmessage = jsonObject["message"].toObject();
         ConnectedClient.identification = clientmessage.value(QString("App")).toString();

         ConnectedClientStore.push_back(ConnectedClient);
         pClient->setObjectName(ConnectedClient.identification);

         m_clients.at(ConnectionOrder)->setObjectName(ConnectedClient.identification);
         ConnectionOrder = ConnectionOrder + 1;

         NumberofConnectedClient = ConnectedClientStore.size();

         qDebug() << " *** " << ConnectedClient.identification << " Connected...\n" << "Number of connected client: " << QString::number(NumberofConnectedClient);
    }
    if (m_debug)
        //qDebug() << "Message received:" << message;
    if (pClient)
    {
        //pClient->sendTextMessage(message);
    }
}

void TcpSocketCommunication::processBinaryMessage(QByteArray message)
{
    pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

void TcpSocketCommunication::socketDisconnected()
{
    pClient = qobject_cast<QWebSocket *>(sender());

    auto iterator = ConnectedClientStore.begin();
    ClosedConnectionName = pClient->objectName();
    while(iterator != ConnectedClientStore.end())
    {
        if(iterator->identification == ClosedConnectionName)
        {
            ConnectedClientStore.erase(iterator);
            ConnectionOrder = ConnectionOrder - 1;
        }
        else
        {
            *iterator++;
        }
    }
    NumberofConnectedClient = ConnectedClientStore.size();

    if (m_debug)
        qDebug() << pClient->objectName() << " client socket bağlantısı koptu...\n" << "Number of connected client: "+QString::number(NumberofConnectedClient);
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
