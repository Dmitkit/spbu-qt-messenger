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
    void sendMessage(const QString& message, const QString& recipient); // Отправка сообщения
    void registerUser(const QString& login, const QString& password, const QString& name);      // Регистрация пользователя
    void login(QString& login, const QString& password); // Логин пользователя
    void makeNewChatWith(QString& login); // Создание нового диалога
    void getChats();
    void getMessages(int dialogID);
    ~Client();                                    // Деструктор

    QString getLogin();
private slots:
    void onReadyRead();                           // Обработка данных от сервера

private:
    QString myLogin;
    QTcpSocket* socket;
signals:
    void loginSuccess();
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

};

#endif // CLIENT_H
