#include <QtNetwork>
#include <QDebug>

int client(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Создание и подключение 6 клиентов
    QList<QTcpSocket*> clients;
    for (int i = 0; i < 6; i++) {
        QTcpSocket* client = new QTcpSocket();
        client->connectToHost("127.0.0.1", 5555); // IP и порт сервера

        if (client->waitForConnected()) {
            qDebug() << "Client" << i+1 << "connected to server";
            clients.append(client);
        } else {
            qDebug() << "Client" << i+1 << "failed to connect to server:" << client->errorString();
            delete client;
        }
    }

    // Отправка сообщения от одного клиента и получение ответа от сервера
    if (clients.count() > 0) {
        QTcpSocket* senderClient = clients.at(0);
        QString message = "heLlo woRld!";
        senderClient->write(message.toUtf8());
        senderClient->flush();

        if (senderClient->waitForReadyRead()) {
            QByteArray response = senderClient->readAll();
            qDebug() << "Received response from server:" << response;
        }
    }

    // Отключение и удаление клиентов
    for (QTcpSocket* client : clients) {
        client->disconnectFromHost();
        client->waitForDisconnected();
        qDebug() << "Client disconnected from server";
        delete client;
    }

    return a.exec();
}
