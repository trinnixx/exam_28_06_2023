#include <QDebug>
#include <QCoreApplication>
#include "mytcpserver.h"

MyTcpServer::~MyTcpServer()
{
    for (auto iter = mTcpSocket.begin(); iter != mTcpSocket.end(); iter++){
        iter->second->close();
    }
    mTcpSocket.clear();
    mTcpServer->close();
    server_status=0;
}
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);
    if(!mTcpServer->listen(QHostAddress::Any, 5555)){
        qDebug() << "server is not started";
    } else {
        server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    if (server_status == 1 && connectedClientsCount < 6) {
        QTcpSocket* curr_socket = mTcpServer->nextPendingConnection();
        connect(curr_socket, &QTcpSocket::readyRead, this, [=]() { slotServerRead(curr_socket); });
        connect(curr_socket, &QTcpSocket::disconnected, this, [=]() { slotClientDisconnected(curr_socket); });
        mTcpSocket.insert(std::make_pair(curr_socket->socketDescriptor(), curr_socket));
        connectedClientsCount++;
    }
    else {
        QTcpSocket* curr_socket = mTcpServer->nextPendingConnection();
        curr_socket->close();
    }
}

void MyTcpServer::slotServerRead(QTcpSocket* active_socket){
    QByteArray array;
    QString tmp;
    while(active_socket->bytesAvailable()>0)
    {
        array = active_socket->readAll();
        tmp.append(array);
    }
    for (int i = 0; i < tmp.length(); i++) {
        if ((i+1)%3 == 0) {
            for (auto socket_pair : this->mTcpSocket) {
                if (socket_pair.first != active_socket->socketDescriptor()) socket_pair.second->write(QString(tmp[i]).toUtf8());
            }
        }
    }
}

void MyTcpServer::slotClientDisconnected(QTcpSocket* active_socket){
    {
        long long act_descr = std::find_if(std::begin(mTcpSocket), std::end(mTcpSocket),
        [&active_socket](const auto& p)
        {
        return p.second == active_socket;
        })->first;
        active_socket->close();
        mTcpSocket.erase(act_descr);
        connectedClientsCount--;
    }
}
