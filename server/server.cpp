#include "server.h"
#include <QDataStream>
#include <QDebug>

Server::Server(QObject *parent) : QTcpServer(parent)
{
    listen(QHostAddress::Any, 3333);
}
void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *clientSocket = new QTcpSocket;
    clientSocket->setSocketDescriptor(socketDescriptor);

    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onDisconnected);

    qDebug() << "Client connected:" << socketDescriptor;
}

void Server::onReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    QDataStream in(clientSocket);

    QString type;
    in >> type;

    if (type == "LOGIN") {
        QString login;
        in >> login;
        clients[login] = clientSocket;
        qDebug() << "User logged in:" << login;
    }
    else if (type == "MESSAGE") {
        QString sender, recipient, message;
        in >> sender >> recipient >> message;
        sendMessageToClient(sender, recipient, message);
    }
}

void Server::sendMessageToClient(const QString& sender, const QString& recipient, const QString& message)
{
    if (clients.contains(recipient)) {
        QTcpSocket *recipientSocket = clients[recipient];
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << QString("From %1: %2").arg(sender).arg(message);
        recipientSocket->write(data);
    }
}

void Server::onDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    clients.remove(clients.key(clientSocket));
    clientSocket->deleteLater();
    qDebug() << "Client disconnected";
}
