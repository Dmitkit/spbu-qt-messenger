#include "chatwindow.h"
<<<<<<< HEAD

ChatWindow::ChatWindow(QWidget *parent)
    : QMainWindow{parent}
{
}
=======
#include "./ui_chatwindow.h"
#include "client/client.h"
#include <QJsonArray>
#include <QMessageBox>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPainterPath>
#include <QListWidget>
#include <QLabel>
#include <QBitmap>
#include <QImage>
#include <QStackedLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QTextBrowser>
#include <QDebug>
#include <QFrame>
#include <QJsonObject>

Person::Person(QWidget *parent) : QWidget(parent) {}

void Person::mousePressEvent(QMouseEvent *event) { //кликабельность виджета
    QWidget::mousePressEvent(event);
    emit mousePressed();
}

QPixmap createRoundPixmap(const QPixmap &pixmap) { //округление фото
    QPixmap roundedPixmap(pixmap.size());
    roundedPixmap.fill(Qt::transparent); // Устанавливаем прозрачный фон

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing); // Включаем сглаживание
    painter.setBrush(QBrush(pixmap));
    painter.drawRoundedRect(roundedPixmap.rect(), roundedPixmap.width() / 2, roundedPixmap.height() / 2);

    return roundedPixmap;
}

void addChatMessage(const QString &text, bool isIncoming, QTextBrowser *chatBrowser) {
    // Создание HTML кода для сообщения
    QString html = "<div style=\"";
    if (isIncoming) {
        html += "background-color: #9A7E6F; border-radius: 10px; padding: 10px; margin-bottom: 20px; margin-right: 10px; margin-left: 80px; text-align: right;\"";
    } else {
        html += "background-color: #7e6f9a; border-radius: 10px; padding: 10px; margin-bottom: 20px; margin-right: 60px; margin-left: 10px; text-align: left\"";
    }
    html += ">";
    html += text;
    html += "</div>";

    // Добавление HTML в QTextBrowser
    chatBrowser->append(html);
}

ChatWindow::ChatWindow(Client* client, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ChatWindow),
    stacked_wid(new QStackedWidget),
    myClient(client)
{
    connect(myClient, &Client::chatsAcquired, this, &ChatWindow::onChatsAcquired);
    myClient->getChats();
    connect(myClient, &Client::chatCreated, this, &ChatWindow::createChat);

    ui->setupUi(this);
    QWidget* dialogsWindow = centralWidget();
    this->setMinimumWidth(700);
    this->setMinimumHeight(900);

    QWidget* chats = new QWidget(dialogsWindow);
    chats->setStyleSheet("background-color: #e2ddd8;");

    // Создаем layout для списка чатов
    left_layout = new QVBoxLayout(chats);
    chats->setLayout(left_layout);
    chats->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    stacked_wid->setMaximumWidth(670);

    QWidget* choice = new QWidget(dialogsWindow);
    QVBoxLayout* right_layout = new QVBoxLayout(choice);

    // Создаём заголовок
    QLabel* login_label = new QLabel("Введите логин пользователя, чтобы начать диалог:", choice);
    login_label->setAlignment(Qt::AlignCenter);
    login_label->setStyleSheet("font-weight: 400;");
    right_layout->addWidget(login_label);

    // Создаём поле ввода логина
    QLineEdit* login_input = new QLineEdit(choice);
    login_input->setPlaceholderText("Логин пользователя...");
    login_input->setStyleSheet("background-color: white; padding: 5px;");
    login_input->setFixedWidth(300);
    login_input->setAlignment(Qt::AlignCenter);
    right_layout->addWidget(login_input);

    // Создаём кнопку подтверждения
    QPushButton* start_chat_button = new QPushButton("Начать диалог", choice);
    start_chat_button->setStyleSheet(
        "background-color: #7e6f9a; color: white; border-radius: 10px; padding: 10px;");
    start_chat_button->setFixedSize(150, 40);
    right_layout->addWidget(start_chat_button);

    // Выравниваем все элементы по центру
    right_layout->setAlignment(Qt::AlignCenter);

    // Обработка нажатия кнопки
    connect(start_chat_button, &QPushButton::clicked, this, [this, login_input]() {
        QString login = login_input->text();
        if (!login.isEmpty()) {
            qDebug() << "Пользователь хочет начать диалог с:" << login;
            myClient->makeNewChatWith(login);
        } else {
            QMessageBox::warning(nullptr, "Ошибка", "Введите логин перед началом диалога.");
        }
    });

    choice->setStyleSheet(
        "background-image: url(:/res/im/background.jpg); "
        "background-position: center; "
        );

    stacked_wid->addWidget(choice);

    // Кнопка для добавления нового чата
    QPushButton* add_chat_button = new QPushButton("Добавить диалог", chats);
    add_chat_button->setStyleSheet(
        "background-color: #7e6f9a; color: white; font-weight: bold; border-radius: 10px; padding: 10px;");
    add_chat_button->setFixedSize(200, 40);

    // Добавляем кнопку в список чатов
    left_layout->addWidget(add_chat_button, 0, Qt::AlignCenter);

    connect(add_chat_button, &QPushButton::clicked, this, [this]() {
         stacked_wid->setCurrentIndex(0);
    });

    QWidget *messageArea = new QWidget(dialogsWindow);
    QVBoxLayout *messageAreaLayout = new QVBoxLayout(messageArea);
    messageArea->setStyleSheet(
        "background-color: #e2ddd8; ");

    messageHistory = new QTextBrowser(messageArea);
    messageHistory->setMaximumSize(650, 1200);
    // messageHistory->setStyleSheet("background-image: url(:/res/im/background.jpg); ");
    messageAreaLayout->addWidget(messageHistory);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    messageAreaLayout->addLayout(inputLayout);

    QTextEdit *textInput = new QTextEdit;
    textInput->setMaximumSize(550, 50);
    textInput->setStyleSheet("background-color: white;"); // Set the background color to white
    inputLayout->addWidget(textInput);

    QPushButton *sendButton = new QPushButton(messageArea);
    sendButton->setFixedSize(60, 48);
    sendButton->setStyleSheet
        ("background-image: url(:/res/im/icon_sent.png); "
         "background-position: center; ");

    inputLayout->addWidget(sendButton);

    //функционал кнопки
    connect(sendButton, &QPushButton::clicked, this, [this, textInput]() {
        QString message = textInput->toPlainText();
        addChatMessage(message, false, messageHistory);   //пока что здесь меняется отправка сообщения
        textInput->clear();                              // true - я; false - другой пользователь
        qDebug() << "Отправлено сообщение:" << message;
    });


    messageArea->setLayout(messageAreaLayout);
    stacked_wid->addWidget(messageArea);

    QFrame* separator = new QFrame(dialogsWindow); //основной разделитель списка чатов и диалогового окна
    separator->setFrameShape(QFrame::VLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setLineWidth(1);

    QHBoxLayout* main_layout = new QHBoxLayout(dialogsWindow);
    main_layout->addWidget(chats);
    main_layout->addWidget(separator);
    main_layout->addWidget(stacked_wid);
    dialogsWindow->setLayout(main_layout);

}


ChatWindow::~ChatWindow()
{
    delete ui;
}


Person* ChatWindow::createChatWithUser(const QString &name, const QString &photo_path, QVBoxLayout* layout){
    Person* person = new Person(this);
    person->setMaximumHeight(80);
    QHBoxLayout *itemLayout = new QHBoxLayout(person);
    itemLayout->setSpacing(15);

    QLabel *nameLabel = new QLabel(person);
    nameLabel->setText(name);
    nameLabel->setStyleSheet("font-weight: 550;");

    // QLabel* photo = new QLabel(person);
    // QPixmap ava;
    // QString path = photo_path;
    // ava.load(path);
    // ava = ava.scaled(56, 56, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    // ava = createRoundPixmap(ava);
    // photo->setPixmap(ava);
    // photo->setStyleSheet("border-radius: 28px; max-height: 56px; max-width: 56px;");

    // itemLayout->addWidget(photo);
    itemLayout->addWidget(nameLabel);

    QFrame *sep = new QFrame;
    sep->setFrameShape(QFrame::HLine);
    sep->setFrameShadow(QFrame::Sunken);
    sep->setLineWidth(1);

    layout->addWidget(person);
    layout->addWidget(sep);
    return person;
}

void ChatWindow::onChatsAcquired(const QJsonArray &chats) {
    // Добавление диалогов
    for (const QJsonValue &chatValue : chats) {
        QJsonObject chatInfo = chatValue.toObject();
        createChat(chatInfo);
    }
}

void ChatWindow::createChat(const QJsonObject &chatInfo)
{
    // Извлекаем данные о чате
    int chatId = chatInfo["chatId"].toInt();
    int userId = chatInfo["userId"].toInt();
    QString userName = chatInfo["userName"].toString();

    qDebug() << chatInfo;

    // Добавляем новый чат в список чатов
    Person* person = createChatWithUser(userName, "", left_layout);

    if (person) {
        person->userId = userId;
        person->dialogId = chatId;
        person->userName = userName;
    }

    connect(person, &Person::mousePressed, this, [this, person](){
        this->myClient->getMessages(person->dialogId);
        connect(this->myClient, &Client::messagesAcquired, person, [this, person](QJsonArray &messages){
            displayMessages(person, messages, messageHistory);
        });
    });


}


void ChatWindow::displayMessages(Person* person, const QJsonArray &messages, QTextBrowser *messageHistory) {
    messageHistory->clear(); // Очистка старых сообщений
    qDebug() << messages;
    for (const QJsonValue &value : messages) {
        QJsonObject messageObj = value.toObject();
        int senderId = messageObj["sender"].toInt();
        QString text = messageObj["text"].toString();
        QString time = messageObj["time"].toString();

        // Определяем, входящее сообщение или исходящее
        bool isIncoming = (senderId == person->userId); // Например, "Me" — это текущий пользователь
        qDebug() << senderId << " " << person->userId << " " << myClient->getLogin();

        QString sender = (isIncoming == true) ? person->userName : myClient->getLogin();

        // Форматируем сообщение
        QString html = "<div style=\"";
        if (isIncoming) {
            qDebug() << "incoming\n";
            html += "background-color: #9A7E6F; border-radius: 10px; padding: 10px; margin-bottom: 10px; margin-right: 10px; margin-left: 80px; text-align: left;\"";
        } else {
            qDebug() << "outcoming\n";
            html += "background-color: #7e6f9a; border-radius: 10px; padding: 10px; margin-bottom: 10px; margin-right: 80px; margin-left: 10px; text-align: right;\"";
        }
        html += ">";
        html += "<b>" + sender + "</b>: " + text + "<br><span style='font-size: 10px;'>" + time + "</span>";
        html += "</div>";

        // Добавляем сообщение в QTextBrowser
        messageHistory->append(html);
    }
    stacked_wid->setCurrentIndex(1);
}


void Person::onMessagesAcquired(QJsonArray &messages){
    qDebug() << messages;
    // ChatWindow::displayMessages(this, messages, *(this->parent())->messageHistory);
};



>>>>>>> fd4b0ca (Added users, dialogs)
