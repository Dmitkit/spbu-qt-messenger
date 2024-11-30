#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

#include "client/client.h"

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

    Client *myClient;

    void colorLines(QString color);

public slots:
    void onLoginBtnClicked();
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

};

#endif // MAINWINDOW_HPP
