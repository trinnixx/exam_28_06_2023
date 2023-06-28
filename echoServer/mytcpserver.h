#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <QtNetwork>
#include <QByteArray>
#include <QDebug>

#include <map>
#include <iterator>
#include <algorithm>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();
public slots:
    void slotNewConnection();
    void slotClientDisconnected(QTcpSocket* active_socket);
    void slotServerRead(QTcpSocket* active_socket);
private:
    int connectedClientsCount = 0;
    QTcpServer* mTcpServer;
    std::map<long long, QTcpSocket*> mTcpSocket;
    int server_status;
};
#endif // MYTCPSERVER_H







