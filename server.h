#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QDataStream>
#include "database/database.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);  // Явный конструктор
    void incomingConnection(qintptr socketDescriptor) override; // Обработка входящих подключений

private slots:
    void onReadyRead();       // Обработчик входящих данных от клиента
    void onDisconnected();    // Обработчик отключений клиента

private:
    void handleSendMessage(QTcpSocket* clientSocket, QDataStream &in); // Обработка сообщений
    void handleLogin(QTcpSocket* clientSocket, QDataStream &in);   // Обработка логина
    void handleRegister(QTcpSocket* clientSocket, QDataStream &in); // Обработка регистрации
    void handleGetChats(QTcpSocket* clientSocket, QDataStream &in); // Обработка получения диалогов пользователя
    void handleAddChat(QTcpSocket *clientSocket, QDataStream &in);
    void handleGetMessages(QTcpSocket *clientSocket, QDataStream &in);

private:
    QMap<QString, QTcpSocket*> clients; // Карта: логин -> сокет клиента
    Database* db;                       // Указатель на базу данных
};

#endif // SERVER_H
