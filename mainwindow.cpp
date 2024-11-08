#include "mainwindow.h"
#include <QPalette>
#include <QString>
#include <QTimer>
#include <QMessageBox>
#include <QIcon>
#include "chatwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    loginEdit(new QLineEdit()),
    passwordEdit(new QLineEdit()),
    loginButton(new QPushButton("Войти")),
    registerButton(new QPushButton("Регистрация")),
    mainLayout(new QVBoxLayout()),
    buttonLayout(new QHBoxLayout()),
    inputLayout(new QVBoxLayout())
{
    setWindowTitle("");
    setWindowIcon(QIcon());
    setCentralWidget(centralWidget);


    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(70, 130, 180));
    setPalette(pal);

    loginButton->setStyleSheet("QPushButton {"
                                    "background-color: #1E90FF;"
                                    "color: white;"
                                    "border-radius: 8px;"
                                    "padding: 8px 12px;"
                                    "outline: none;}"
                               "QPushButton:hover {"
                                    "background-color: #1C86EE; }");

    registerButton->setStyleSheet("QPushButton {"
                                    "background-color: #1E90FF;"
                                    "color: white;"
                                    "border-radius: 8px;"
                                    "padding: 8px 12px;"
                                    "outline: none;}"
                                  "QPushButton:hover{"
                                    "background-color: #1C86EE;}");

    loginEdit->setPlaceholderText("Введите логин");
    passwordEdit->setPlaceholderText("Введите пароль");
    colorLines("blue");
    passwordEdit->setEchoMode(QLineEdit::Password);

    inputLayout->addWidget(loginEdit);
    inputLayout->addWidget(passwordEdit);

    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);

    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);

    centralWidget->setLayout(mainLayout);

    loginEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    passwordEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    loginButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    registerButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginBtnClicked);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &MainWindow::onLoginBtnClicked);


    helpButton = new QPushButton("?");
    helpButton->setStyleSheet("QPushButton {"
                                  "background-color: #1E90FF;"
                                  "color: white;"
                                  "border-radius: 8px;"
                                  "padding: 8px 12px;}"
                                  "QPushButton:hover{"
                                  "background-color: #1C86EE;"
                                  "outline: none;}");
    helpButton->setFixedSize(30, 30);
    mainLayout->addWidget(helpButton, 0, Qt::AlignRight);
    connect(helpButton, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, "Справка", "Это авторизация в мессенджере. Введите свой логин и пароль.");
    });
}

void MainWindow::onLoginBtnClicked() {
    QString correctLogin = "Dima";
    QString correctPassword = "123";

    if (loginEdit->text() == correctLogin && passwordEdit->text() == correctPassword) {
        colorLines("green");
        QTimer::singleShot(1000, this, [=]() {colorLines("blue");});

    } else {
        colorLines("red");
        QTimer::singleShot(1000, this, [=](){colorLines("blue");});
    }
}



void MainWindow::colorLines(QString color){
    QString styleSheetRed = "QLineEdit {background-color: #FFB6C1;"
                         "padding: 6px;"
                         "border-radius: 5px;}";
    QString styleSheetBlue = "QLineEdit {background-color: #ADD8E6;"
                              "padding: 6px;"
                              "border-radius: 5px;}";
    QString styleSheetGreen = "QLineEdit {background-color: #90EE90;"
                              "padding: 6px;"
                              "border-radius: 5px;}";

    if (color == "red"){
        passwordEdit->setStyleSheet(styleSheetRed);
        loginEdit->setStyleSheet(styleSheetRed);
    }
    else if (color == "blue"){
        passwordEdit->setStyleSheet(styleSheetBlue);
        loginEdit->setStyleSheet(styleSheetBlue);
    }
    else if (color == "green"){
        passwordEdit->setStyleSheet(styleSheetGreen);
        loginEdit->setStyleSheet(styleSheetGreen);
    }
}

MainWindow::~MainWindow() {

}

