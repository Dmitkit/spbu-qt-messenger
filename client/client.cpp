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

void Client::sendMessage(const QString& message, const QString& recipient)
{
    if (!recipient.isEmpty() && !message.isEmpty()) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);

        out << QString("MESSAGE") << myLogin << recipient << message;
        socket->write(data);
        qDebug() << "Message sent to:" << recipient;
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
        emit loginSuccess();
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
    }else {
        qDebug() << "Server response:";
    }
}

QString Client::getLogin(){
    return(myLogin);
};
// void Client::loginSuccess(){}

// // Ошибка при входе (например, неправильный логин или пароль)
// void Client::loginError(const QString &errorMessage){}

// // Успешная регистрация
// void Client::registrationSuccess(){}

// // Ошибка при регистрации
// void Client::registrationError(const QString &errorMessage){}

// // Получено новое сообщение
// void Client::newMessage(const QString &sender, const QString &message){}

// // Подключение к серверу установлено
// void Client::connected(){}

// // Ошибка подключения
// void Client::connectionError(const QString &errorMessage){}

// // Соединение с сервером потеряно
// void Client::disconnected(){}
