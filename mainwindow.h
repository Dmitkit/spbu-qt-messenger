#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

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

    void colorLines(QString color);

public slots:
    void onLoginBtnClicked();
    // void onRegisterBtnClicked();
    // void passwordAccepted();
    // void passwordDenied();

};

#endif // MAINWINDOW_HPP
