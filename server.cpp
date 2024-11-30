#include "server.h"
#include <QDataStream>
#include <QDebug>
#include <QString>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
#include "database/database.h"

Server::Server(QObject *parent) : QTcpServer(parent), db(new Database)
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

    if (type == "MESSAGE") {
        handleMessage(clientSocket, in);
    } else if (type == "LOGIN") {
        handleLogin(clientSocket, in);
    } else if (type == "REGISTER") {
        handleRegister(clientSocket, in);
    } else if (type == "GET_CHATS"){
        handleGetChats(clientSocket, in);
    } else if (type == "ADD_CHAT"){
        handleAddChat(clientSocket, in);
    } else if (type == "GET_MESSAGES"){
        handleGetMessages(clientSocket, in);
    }
}

void Server::handleMessage(QTcpSocket *clientSocket, QDataStream &in)
{
    QString sender, recipient, message;
    in >> sender >> recipient >> message;

    if (clients.contains(recipient)) {
        QTcpSocket *recipientSocket = clients[recipient];
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << QString("From %1: %2").arg(sender).arg(message);
        recipientSocket->write(data);
    } else {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << QString("ERROR: Recipient not connected.");
        clientSocket->write(data);
    }
}

void Server::handleLogin(QTcpSocket *clientSocket, QDataStream &in)
{
    QString login, password;
    in >> login >> password;

    if (db->Authorization(login, password)) {
        clients[login] = clientSocket;
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << QString("LOGIN_SUCCESS");
        clientSocket->write(data);
        qDebug() << "User logged in:" << login;
    } else {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << QString("LOGIN_FAILED");
        clientSocket->write(data);
        qDebug() << "Login failed for user:" << login;
    }
}

void Server::handleRegister(QTcpSocket *clientSocket, QDataStream &in)
{
    QString login, password, name;
    in >> login >> password >> name;

    if (db->addUser(name, login, password)) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << QString("REGISTER_SUCCESS");
        clientSocket->write(data);
        qDebug() << "User registered:" << login;
    } else {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << QString("REGISTER_FAILED");
        clientSocket->write(data);
        qDebug() << "Registration failed for user:" << login;
    }
}

void Server::handleGetChats(QTcpSocket *clientSocket, QDataStream &in)
{
    QString login;
    in >> login;

    int userId = db->GetUserId(login);
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    if (userId == -1) {
        out << QString("ERROR: User not found.");
        clientSocket->write(data);
        return;
    }

    QList<QMap<QString, QVariant>> chats = db->GetUserChats(login);
    QJsonArray chatsArray;

    for (const auto &dialog : chats) {
        QJsonArray chatIds = dialog["chats"].toJsonArray();
        for (const auto &chatIdValue : chatIds) {
            int chatId = chatIdValue.toInt();
            int secondUserId = db->GetSecond(chatId, userId);
            if (secondUserId == -1) continue;

            QString secondUserName = db->GetUserName(secondUserId);
            QJsonObject dialogObject{
                {"chatId", chatId},
                {"userId", secondUserId},
                {"userName", secondUserName}
            };
            chatsArray.append(dialogObject);
        }
    }

    out << QString("CHATS_ACQUIRED") << chatsArray;
    clientSocket->write(data);
}


void Server::handleGetMessages(QTcpSocket *clientSocket, QDataStream &in)
{
    qDebug() << "Server::handleGetMessages";
    int dialogID;
    in >> dialogID; // Читаем ID диалога из входных данных
    qDebug() << dialogID;

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    // Получаем идентификаторы сообщений для данного диалога
    QJsonArray messageIDs = db->GetMessages(dialogID);

    if (messageIDs.isEmpty()) {
        // Если сообщений нет, отправляем пустой ответ
        out << QString("ERROR: Сообщения не найдены.");
        clientSocket->write(data);
        return;
    }

    QJsonArray messagesArray;

    // Проходим по всем идентификаторам сообщений
    for (const QJsonValue &id : messageIDs) {
        int messageId = id.toInt();
        QList<QMap<QString, QVariant>> messageData = db->GetText(messageId);

        if (!messageData.isEmpty()) {
            // Если данные для сообщения найдены, формируем JSON-объект
            QMap<QString, QVariant> message = messageData.first();
            QJsonObject messageObject{
                {"time", message["time"].toString()},
                {"sender", message["sender"].toInt()},
                {"text", message["text"].toString()}
            };
            messagesArray.append(messageObject);
        }
        else{
            qDebug() << "0 messages";
        }
    }

    // Формируем итоговый JSON-ответ
    QJsonObject response{
        {"messages", messagesArray}
    };

    QByteArray jsonData = QJsonDocument(response).toJson(QJsonDocument::Compact);

    qDebug() << "Server retrieved messages for dialog" << dialogID;

    // Отправляем данные клиенту
    out << QString("MESSAGES_ACQUIRED") << jsonData;
    clientSocket->write(data);
    clientSocket->flush();
}


void Server::handleAddChat(QTcpSocket *clientSocket, QDataStream &in)
{
    QString firstLogin, secondLogin;
    in >> firstLogin >> secondLogin;  // Читаем логины пользователей из входных данных

    int firstUserId = db->GetUserId(firstLogin);
    int secondUserId = db->GetUserId(secondLogin);
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    // Проверка, что оба пользователя существуют
    if (firstUserId == -1 || secondUserId == -1) {
        out << QString("ERROR: Один или оба пользователя не найдены.");
        clientSocket->write(data);
        return;
    }

    // Проверяем, существует ли чат между этими пользователями
    if (db->chatExists(firstUserId, secondUserId)) {
        out << QString("CHAT_EXISTS");
        clientSocket->write(data);
    } else {
        // Если чат не существует, создаем новый
        if (db->addChat(firstUserId, secondUserId)) {
            int chatId = db->GetChatId(firstUserId, secondUserId);
            QString secondUserName = db->GetUserName(secondUserId);

            // Если чат создан, отправляем информацию о чате
            QJsonObject response{
                {"chatId", chatId},
                {"userId", secondUserId},
                {"userName", secondUserName}
            };

            QByteArray jsonData = QJsonDocument(response).toJson(QJsonDocument::Compact);

            qDebug() << "Server added chat";
            out << QString("CHAT_CREATED") << jsonData;

            clientSocket->write(data); // отправляем тому кто запрашивал новый чат
            if (clients[secondLogin] != nullptr){ // если тот с кем новый чат открыт - в сети
                clients[secondLogin]->write(data);} // то отправляем и ему тоже

        } else {
            // Если не удалось создать чат
            qDebug("Server error");
            out << QString("ERROR: Не удалось создать чат.");
            clientSocket->write(data);
        }
    }
}


void Server::onDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    QString clientLogin = clients.key(clientSocket);
    clients.remove(clientLogin);

    clientSocket->deleteLater();
    qDebug() << "Client disconnected:" << clientLogin;
}
