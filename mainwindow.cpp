#include "mainwindow.h"
#include "chatwindow.h"
#include "client/client.h"
#include <QPalette>
#include <QString>
#include <QTimer>
#include <QMessageBox>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    loginEdit(new QLineEdit()),
    passwordEdit(new QLineEdit()),
    loginButton(new QPushButton("Войти")),
    registerButton(new QPushButton("Регистрация")),
    mainLayout(new QVBoxLayout()),
    buttonLayout(new QHBoxLayout()),
    inputLayout(new QVBoxLayout()),
    helpButton(new QPushButton("?")),
    myClient(new Client(this))
{
    connect(myClient, &Client::loginSuccess, this, &MainWindow::onLoginSuccess);
    connect(myClient, &Client::loginError, this, &MainWindow::onLoginError);
    connect(myClient, &Client::registrationSuccess, this, &MainWindow::onRegistrationSuccess);

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
    connect(registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterButtonClicked);

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
    QString login = loginEdit->text();
    QString password = passwordEdit->text();

    myClient->login(login, password);
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

void MainWindow::onRegisterButtonClicked() {
    // Скрываем виджеты авторизации
    loginEdit->hide();
    passwordEdit->hide();
    loginButton->hide();
    registerButton->hide();
    disconnect(helpButton, &QPushButton::clicked, this, nullptr);
    connect(helpButton, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, "Справка", "Это регистрация в мессенджере. Введите имя, логин и пароль.");
    });

    // Проверяем, создавались ли уже виджеты регистрации
    if (!centralWidget->property("registrationWidgetsCreated").toBool()) {
        // Поля регистрации
        QLineEdit* nameEdit = new QLineEdit();
        QLineEdit* regLoginEdit = new QLineEdit();
        QLineEdit* regPasswordEdit = new QLineEdit();
        QPushButton* submitRegisterButton = new QPushButton("Зарегистрироваться");
        QPushButton* backButton = new QPushButton("Назад");

        // Настройки виджетов
        nameEdit->setPlaceholderText("Введите имя");
        regLoginEdit->setPlaceholderText("Введите логин");
        regPasswordEdit->setPlaceholderText("Введите пароль");
        regPasswordEdit->setEchoMode(QLineEdit::Password);

        QString styleSheet = "QLineEdit {background-color: #ADD8E6;"
                             "padding: 6px;"
                             "border-radius: 5px;}";
        nameEdit->setStyleSheet(styleSheet);
        regLoginEdit->setStyleSheet(styleSheet);
        regPasswordEdit->setStyleSheet(styleSheet);

        // Добавляем в макет
        inputLayout->addWidget(nameEdit);
        inputLayout->addWidget(regLoginEdit);
        inputLayout->addWidget(regPasswordEdit);
        inputLayout->addWidget(submitRegisterButton);
        inputLayout->addWidget(backButton);

        QString buttonStyle = "QPushButton {"
                              "background-color: #1E90FF;"
                              "color: white;"
                              "border-radius: 8px;"
                              "padding: 8px 12px;}"
                              "QPushButton:hover{"
                              "background-color: #1C86EE;}";
        submitRegisterButton->setStyleSheet(buttonStyle);
        backButton->setStyleSheet(buttonStyle);

        // Сохраняем виджеты как свойства центрального виджета
        centralWidget->setProperty("nameEdit", QVariant::fromValue(static_cast<QWidget*>(nameEdit)));
        centralWidget->setProperty("regLoginEdit", QVariant::fromValue(static_cast<QWidget*>(regLoginEdit)));
        centralWidget->setProperty("regPasswordEdit", QVariant::fromValue(static_cast<QWidget*>(regPasswordEdit)));
        centralWidget->setProperty("submitRegisterButton", QVariant::fromValue(static_cast<QWidget*>(submitRegisterButton)));
        centralWidget->setProperty("backButton", QVariant::fromValue(static_cast<QWidget*>(backButton)));

        // Отмечаем, что виджеты созданы
        centralWidget->setProperty("registrationWidgetsCreated", true);

        // Подключаем кнопку "Назад"
        connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackButtonClicked);
        connect(submitRegisterButton, &QPushButton::clicked, this, [this, regLoginEdit, regPasswordEdit, nameEdit](){
            //выглядит ужасно, но мне лень переписывать всё к чёртовой матери
            //здесь подключается кнопка "Зарегистрироваться"
            if (!((regLoginEdit->text()).isEmpty())&&!((regPasswordEdit->text()).isEmpty())&&!((nameEdit->text()).isEmpty()))
                this->myClient->registerUser(regLoginEdit->text(), regPasswordEdit->text(), nameEdit->text());
        });
    }

    // Показываем виджеты регистрации
    // так коряво потому что мне лень добавлять все эти поля в h файл
    // и инициализировать их
    centralWidget->property("nameEdit").value<QWidget*>()->show();
    centralWidget->property("regLoginEdit").value<QWidget*>()->show();
    centralWidget->property("regPasswordEdit").value<QWidget*>()->show();
    centralWidget->property("submitRegisterButton").value<QWidget*>()->show();
    centralWidget->property("backButton").value<QWidget*>()->show();
}


void MainWindow::onBackButtonClicked() {
    // Скрываем виджеты регистрации
    // так коряво потому что мне лень добавлять все эти поля в h файл
    // и инициализировать их
    centralWidget->property("nameEdit").value<QWidget*>()->hide();
    centralWidget->property("regLoginEdit").value<QWidget*>()->hide();
    centralWidget->property("regPasswordEdit").value<QWidget*>()->hide();
    centralWidget->property("submitRegisterButton").value<QWidget*>()->hide();
    centralWidget->property("backButton").value<QWidget*>()->hide();

    // Показываем виджеты авторизации
    loginEdit->show();
    passwordEdit->show();
    loginButton->show();
    registerButton->show();
    disconnect(helpButton, &QPushButton::clicked, this, nullptr);
    connect(helpButton, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, "Справка", "Это авторизация в мессенджере. Введите логин и пароль.");
    });
}

void MainWindow::onLoginSuccess(int userId, QString& userName)
{
    // Обновляем интерфейс после успешного входа
    colorLines("green");
    QTimer::singleShot(1000, this, [=]() {colorLines("blue");});
    // Скрываем текущее окно
    this->hide();
    this->myClient->setUserId(userId);
    this->myClient->setUserName(userName);

    // Открываем окно чата
    ChatWindow* chatWindow = new ChatWindow(myClient, this);
    chatWindow->show();
}

void MainWindow::onLoginError(const QString &errorMessage)
{
    // Выводим ошибку в интерфейс
    colorLines("red");
    QTimer::singleShot(1000, this, [=](){colorLines("blue");});
}
void MainWindow::onRegistrationSuccess()
{
    // Показать подтверждающее сообщение
    QMessageBox::information(this, "Регистрация успешна", "Вы успешно зарегистрировались! Теперь вы можете войти в систему.");

    // Скрываем виджеты регистрации
    centralWidget->property("nameEdit").value<QWidget*>()->hide();
    centralWidget->property("regLoginEdit").value<QWidget*>()->hide();
    centralWidget->property("regPasswordEdit").value<QWidget*>()->hide();
    centralWidget->property("submitRegisterButton").value<QWidget*>()->hide();
    centralWidget->property("backButton").value<QWidget*>()->hide();

    // Показываем виджеты авторизации
    loginEdit->show();
    passwordEdit->show();
    loginButton->show();
    registerButton->show();

    // Восстанавливаем функциональность кнопки "?"
    disconnect(helpButton, &QPushButton::clicked, this, nullptr);
    connect(helpButton, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, "Справка", "Это авторизация в мессенджере. Введите логин и пароль.");
    });

    // Сбрасываем поля ввода
    loginEdit->clear();
    passwordEdit->clear();
}
