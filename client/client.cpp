#include "client.h"
#include <QDataStream>
#include <QByteArray>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

Client::Client(QWidget *parent) :
    socket(new QTcpSocket(this))
{
    socket->connectToHost("127.0.0.1", 3333);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
}

Client::~Client()
{
    if (socket->isOpen()) {
        socket->close();
    }
}
void Client::getChats(){
    if (!myLogin.isEmpty()){
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);

        out << QString("GET_CHATS") << myLogin;
        socket->write(data);
    }
    else
    {
        qDebug() << "Login in empty";
    }
}


void Client::getMessages(int dialogID){
    if (dialogID > 0){
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);

        out << QString("GET_MESSAGES") << dialogID;
        socket->write(data);
    }
    else{
        qDebug() << "DialogID is empty";
    }
}

void Client::sendMessage(const int recipientId, const QString& message)
{
    if (recipientId > 0 && !message.isEmpty()) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);

        out << QString("MESSAGE") << myUserId << recipientId << message;
        socket->write(data);
        qDebug() << "Message sent to:" << recipientId;
    } else {
        qDebug() << "Invalid recipient or message!";
    }
}

void Client::login(QString& login, const QString& password)
{
    if (socket->state() == QTcpSocket::ConnectedState && !login.isEmpty() && !password.isEmpty()) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);

        out << QString("LOGIN") << login << password;
        socket->write(data);
        qDebug() << "Login request sent for user:" << login;
        myLogin = login;
    } else {
        qDebug() << "Connection error or invalid data!";
    }
}

void Client::registerUser(const QString& login, const QString& password, const QString& name)
{
    if (socket->state() == QTcpSocket::ConnectedState && !login.isEmpty() && !password.isEmpty() && !name.isEmpty()) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);

        out << QString("REGISTER") << login << password << name;
        socket->write(data);
        qDebug() << "Register request sent for user:" << login;
    } else {
        qDebug() << "Connection error or invalid login!";
    }
}

void Client::makeNewChatWith(QString& login){
    if (socket->state() == QTcpSocket::ConnectedState && !login.isEmpty()) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);

        out << QString("ADD_CHAT") << myLogin << login;
        socket->write(data);
        qDebug() << "New chat request sent:" << login;
    } else {
        qDebug() << "Connection error or invalid login!";
    }
};

void Client::onReadyRead()
{
    QDataStream in(socket);
    QString response;
    in >> response;

    if (response == "LOGIN_SUCCESS") {
        qDebug() << "Login successful!";
        int userId;
        QString userName;
        in >> userId >> userName;
        qDebug() << userId << userName;
        emit loginSuccess(userId, userName);
    } else if (response == "LOGIN_FAILED") {
        qDebug() << "Login failed!";
        emit loginError("error");
    } else if (response == "REGISTER_SUCCESS") {
        qDebug() << "Registration successful!";
        emit registrationSuccess();
    } else if (response == "REGISTER_FAILED") {
        qDebug() << "Registration failed!";
        emit registrationError("Registration failed");
    } else if (response == "CHATS_ACQUIRED"){
        QJsonArray chatsArray;
        in >> chatsArray;
        emit chatsAcquired(chatsArray);
    }else if (response == "CHAT_EXISTS") {
        qDebug() << "Chat exists!";
        emit chatExists();
    }else if (response == "CHAT_CREATED") {
        QByteArray jsonData;
        in >> jsonData;

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (!doc.isNull() && doc.isObject()) {
            QJsonObject chatInfo = doc.object();
            emit chatCreated(chatInfo);
            qDebug() << "Chat created with details:" << chatInfo;
        } else {
            qDebug() << "Failed to parse chat creation response";
        }
    }else if (response == "MESSAGES_ACQUIRED") {
        QByteArray jsonData;
        in >> jsonData;

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (!doc.isNull() && doc.isObject()) {
            QJsonObject responseObj = doc.object();
            QJsonArray messagesArray = responseObj["messages"].toArray();

            emit messagesAcquired(messagesArray);
            qDebug() << "Messages acquired for dialog";
        } else {
            qDebug() << "Failed to parse messages response";
        }
    }else if (response == "MESSAGE_SENT") {
        QByteArray jsonData;
        in >> jsonData;

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (!doc.isNull() && doc.isObject()) {
            QJsonObject responseObj = doc.object();
            int chatId = responseObj["chatId"].toInt();
            QString sender = responseObj["sender"].toString();
            QString messageText = responseObj["messageText"].toString();
            QString time = responseObj["time"].toString();

            // Обработать полученное сообщение (например, обновить UI или вывести в лог)
            qDebug() << "Message sent by user" << sender << ": " << messageText << "at" << time;

            // Эмитируем сигнал для обновления интерфейса
            emit messageSent(sender, chatId, messageText, time);
        } else {
            qDebug() << "Failed to parse message sent response";
        }
    }
    else {
        qDebug() << "Server response:";
    }
}

QString Client::getLogin(){
    return(myLogin);
};

int Client::getUserId(){
    return(myUserId);
}

void Client::setUserId(int userId){
    this->myUserId = userId;
}

void Client::setLogin(QString login){
    this->myLogin = login;
}
