#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QJsonArray>
#include <QVariant>
#include <QMap>

class Database {
public:
    Database(); // Конструктор
    ~Database(); // Деструктор

    // Методы управления пользователями
    bool addUser(const QString& name, const QString& login, const QString& pas);
    bool userExists(const QString& login);
    void DeleteUser(int id);
    QList<QMap<QString, QVariant>> GetUserChats(const QString login);

    // Методы управления чатами
    bool addChat(int user1_id, int user2_id);
    bool chatExists(int user1_id, int user2_id);
    QJsonArray GetMessages(int chatid);
    int GetSecond(int chatid, int firstuser);
    int GetChatId(int id_1, int id_2);

    // Методы управления сообщениями
    bool addMessage(const QString& time, int sender, int tochat, const QString& text);
    QList<QMap<QString, QVariant>> GetText(int mesid);

    // Авторизация
    bool Authorization(const QString& login, const QString& password);


    // Утилиты
    const QString& GetUserName(int id);
    const int GetUserId (QString login);
    const QString& GetUserLogin(int id);

private:
    QSqlDatabase userdb;        // База данных пользователей
    QSqlDatabase chatsdb;       // База данных чатов
    QSqlDatabase messagesdb;    // База данных сообщений

    QSqlQuery* query;           // Запросы к базе данных пользователей
    QSqlQuery* querychat;       // Запросы к базе данных чатов
    QSqlQuery* querymessages;   // Запросы к базе данных сообщений

    QSqlTableModel* tableuser;  // Модель для таблицы пользователей
    QSqlTableModel* tablechats; // Модель для таблицы чатов
    QSqlTableModel* tablemessages; // Модель для таблицы сообщений
};

#endif // DATABASE_H

