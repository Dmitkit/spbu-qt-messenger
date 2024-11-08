#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool addUser(const QString& name, const QString& login, const QString& pas); // добавляет пользователя
    bool userExists(const QString& login); // проверяет существование пользователя по логину
    bool addChat(int user1_id, int user2_id); // создает чат с проверкой существования чата между этими пользователями
    bool isChatExists(int user1_id,int user2_id); // проверка существования чата между собеседниками
    bool addMessage(const QString& time,int sender,int tochat,const QString& text); // добавление сообщения
    //get-функции
    QList<QMap<QString, QVariant>> autorization(const QString& login, const QString& password); // функция авторизации, возвращает id, имя, имеющиеся чаты
    const QString& GetUserName(int id); // возвращает имя по id
    int GetSecond(int chatid, int firstuser); // возвращает собеседника по id чата и указанию имеющегося собеседника
    QJsonArray GetMessages(int chatid); // возвращает список сообщений по id чата
    QList<QMap<QString, QVariant>> GetText(int mesid); // возвращает время отправления, от кого и текст сообщения по id сообщения
    // удаление
    void DeleteUser(int id); // стирает данные о пользователе без удаления чатов и всего остального

    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QSqlDatabase userdb;
    QSqlQuery *query;
    QSqlTableModel *tableuser;

    QSqlDatabase chatsdb;
    QSqlQuery *querychat;
    QSqlTableModel *tablechats;

    QSqlDatabase messagesdb;
    QSqlQuery *querymessages;
    QSqlTableModel *tablemessages;

};
#endif // MAINWINDOW_H
