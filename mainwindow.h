#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

<<<<<<< HEAD
=======
#include "client/client.h"

>>>>>>> fd4b0ca (Added users, dialogs)
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *centralWidget;
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *helpButton;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QVBoxLayout *inputLayout;

<<<<<<< HEAD
=======
    Client *myClient;

>>>>>>> fd4b0ca (Added users, dialogs)
    void colorLines(QString color);

public slots:
    void onLoginBtnClicked();
<<<<<<< HEAD
    // void onRegisterBtnClicked();
    // void passwordAccepted();
    // void passwordDenied();
=======
    void onRegisterButtonClicked();
    void onBackButtonClicked();

private slots:
    void onLoginSuccess();
    void onLoginError(const QString &errorMessage);
    void onRegistrationSuccess();
    void onRegistrationError(const QString &errorMessage);
    void onNewMessage(const QString &sender, const QString &message);
    void onConnected();
    void onConnectionError(const QString &errorMessage);
    void onDisconnected();
>>>>>>> fd4b0ca (Added users, dialogs)

};

#endif // MAINWINDOW_HPP
