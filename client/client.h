#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QString>
#include <QList>
#include <QMap>
#include <QVariant>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);   // Явный конструктор
    Client(Client* client);                       // Конструктор копирования клиента
    void sendMessage(const int recepientId, const QString& message); // Отправка сообщения
    void registerUser(const QString& login, const QString& password, const QString& name);      // Регистрация пользователя
    void login(QString& login, const QString& password); // Логин пользователя
    void makeNewChatWith(QString& login); // Создание нового диалога
    void getChats();
    void getMessages(int dialogID);
    ~Client();                                    // Деструктор

    QString getLogin();
    int getUserId();
    QString getUserName(){return myName;};
    void setUserId(int userId);
    void setLogin(QString login);
    void setUserName(QString userName){this->myName = userName;};
private slots:
    void onReadyRead();                           // Обработка данных от сервера

private:
    QString myLogin;
    QTcpSocket* socket;
    int myUserId;
    QString myName;
signals:
    void loginSuccess(int userId, QString &username);
    void loginError(const QString &errorMessage);
    void registrationSuccess();
    void registrationError(const QString &errorMessage);
    void newMessage(const QString &sender, const QString &message);
    void connected();
    void connectionError(const QString &errorMessage);
    void disconnected();
    void chatsAcquired(QJsonArray &chats);
    void chatExists();
    void chatCreated(QJsonObject &chatInfo);
    void messagesAcquired(QJsonArray &messages);
    void messageSent(QString &sender, int senderId, QString &messageText, QString &time);

};

#endif // CLIENT_H
