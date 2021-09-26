#ifndef TCPSOCKETCOMMUNICATION_H
#define TCPSOCKETCOMMUNICATION_H


#include <QObject>
#include <QList>
#include <QByteArray>
#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>

#include <iostream>
#include <vector>
#include <random>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n
#include <time.h>
#include <sstream>
#include <string>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>
#include <list>
#include <iterator>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

struct ClientValues{
    QString identification;
};

class TcpSocketCommunication : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocketCommunication(const QString &ip, quint16 port, bool debug = false, QObject *parent = nullptr);
    ~TcpSocketCommunication();

    QWebSocket *pSocket;
    QWebSocket *pClient;

    QWebSocketServer *m_pWebSocketServer;
    QVector<QWebSocket *> m_clients;
    bool m_debug;
    ClientValues ConnectedClient;
    std::vector<ClientValues> ConnectedClientStore;

    int NumberofConnectedClient = 0;
    QString ClosedConnectionName;

    int ConnectionOrder = 0;


Q_SIGNALS:
    void closed();

public Q_SLOTS:

    void processTextMessage(const QString &message);
    void onNewConnection();
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

private:

};

#endif // TCPSOCKETCOMMUNICATION_H
