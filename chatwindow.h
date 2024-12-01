#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QString>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>
#include "client/client.h"
#include <QTextBrowser>

QT_BEGIN_NAMESPACE
namespace Ui {
class ChatWindow;
}
QT_END_NAMESPACE

class Person : public QWidget {
    Q_OBJECT

public:
    explicit Person(QWidget *parent = nullptr);
    int userId;      // ID пользователя
    int dialogId;    // ID диалога
    QString userName; // Имя пользователя

    int getChatId();
    int getUserId(){return userId;};
    // QJsonObject messages;


signals:
    void mousePressed();
protected:
    void mousePressEvent(QMouseEvent *event) override;
public slots:
};

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChatWindow(Client* client, QWidget *parent = nullptr);
    ~ChatWindow();

    int getCurrentChat() {return currentChatId;};
    int getCurrentUser() {return currentUserId;};
    void setCurrentChat(int chatId) {currentChatId = chatId;}
    void setCurrentUser(int userId) {currentUserId = userId;}

private:
    Ui::ChatWindow *ui;
    QStackedWidget *stacked_wid; // правый многостраничный виджет
    QVBoxLayout *left_layout;// лэйаут списка чатов и кнопки добавления чата
    Client* myClient;
    QTextBrowser *messageHistory;
    Person* createChatWithUser(const QString &name, const QString &photo_path, QVBoxLayout* layout);
    void displayMessages(Person* person, const QJsonArray &messages, QTextBrowser *messageHistory);
    void addChatMessage(const QString &time, const QString &text, bool isIncoming = false, QString sender = "");

    int currentChatId;
    int currentUserId; //userId того, с кем сейчас ведётся диалог
private slots:
    void onNewMessage(QString& sender, int chatId, QString& messageText, QString& time);
    void onChatsAcquired(const QJsonArray &chats);
    void createChat(const QJsonObject &chatInfo); // и слот и просто функция
};
#endif // CHATWINDOW_H

