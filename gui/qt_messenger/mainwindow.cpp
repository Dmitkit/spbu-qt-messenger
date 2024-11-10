#include "mainwindow.h"
#include "./ui_mainwindow.h"



MyWidget::MyWidget(QWidget *parent) : QWidget(parent) {}

void MyWidget::mousePressEvent(QMouseEvent *event) { //кликабельность виджета
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    QWidget* dialogsWindow = centralWidget();
    this->setMinimumWidth(700);
    this->setMinimumHeight(900);

    QWidget* chats = new QWidget(dialogsWindow);
    chats->setStyleSheet("background-color: #e2ddd8;");

    // Создаем layout для списка чатов
    QVBoxLayout *left_layout = new QVBoxLayout(chats);
    chats->setLayout(left_layout);
    chats->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    MyWidget *person[5];
    for (int i = 0; i < 5; i++) {
        person[i] = new MyWidget;
        QHBoxLayout *itemLayout = new QHBoxLayout(person[i]);
        itemLayout->setSpacing(15); //расстояние между фото и текстом

        QLabel *nameLabel = new QLabel(person[i]);
        nameLabel->setText(QStringLiteral("Пользователь %1").arg(i));
        nameLabel->setStyleSheet("font-weight: 550;");

        QLabel* photo = new QLabel(person[i]);
        QPixmap ava;
        QString path = QStringLiteral(":/im/icon%1.jpg").arg(i);
        ava.load(path);
        ava = ava.scaled(56, 56, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        ava = createRoundPixmap(ava);

        photo->setPixmap(ava);
        photo->setStyleSheet(
            "border-radius: 28px;"
            "max-height: 56px;"
            "max-width: 56px; ");
        itemLayout->addWidget(photo);
        itemLayout->addWidget(nameLabel);

        QFrame *sep = new QFrame;  //разделитель диалогов
        sep->setFrameShape(QFrame::HLine);
        sep->setFrameShadow(QFrame::Sunken);
        sep->setLineWidth(1);

        left_layout->addWidget(person[i]);
        left_layout->addWidget(sep);
    }

    QStackedWidget *stacked_wid = new QStackedWidget; //стек для смены виджета выбора диалога и выбранного чата
    stacked_wid->setMaximumWidth(670);

    //выбор чатов
    QWidget* choice = new QWidget(dialogsWindow);
    QVBoxLayout* right_layout = new QVBoxLayout(choice);
    QLabel* choice_mes = new QLabel("Выберите чат...", choice);
    choice_mes->setStyleSheet("font-weight: 400;");
    choice_mes->setAlignment(Qt::AlignCenter);
    right_layout->addWidget(choice_mes);

    choice->setStyleSheet(
        "background-image: url(:/im/background.jpg); "
        "background-position: center; ");

    choice->setLayout(right_layout);
    stacked_wid->addWidget(choice);

    //чат
    QWidget *dialogue = new QWidget(dialogsWindow);
    QVBoxLayout *dialogue_layout = new QVBoxLayout(dialogue);
    dialogue->setStyleSheet(
        "background-color: #e2ddd8; ");

    QTextBrowser *message_history = new QTextBrowser(dialogue);
    message_history->setMaximumSize(650, 1200);
    message_history->setStyleSheet("background-image: url(:/im/background.jpg); ");
    dialogue_layout->addWidget(message_history);

    QHBoxLayout *input_layout = new QHBoxLayout;
    dialogue_layout->addLayout(input_layout);

    QTextEdit *text_input = new QTextEdit;
    text_input->setMaximumSize(550, 50);
    text_input->setStyleSheet("background-color: white;"); // Set the background color to white
    input_layout->addWidget(text_input);

    QPushButton *send_button = new QPushButton(dialogue);
    send_button->setFixedSize(60, 48);
    send_button->setStyleSheet
        ("background-image: url(:/im/icon_sent.png); "
        "background-position: center; ");

    input_layout->addWidget(send_button);

    //функционал кнопки
    connect(send_button, &QPushButton::clicked, this, [this, text_input, message_history]() {
        QString message = text_input->toPlainText();
        addChatMessage(message, false, message_history);   //пока что здесь меняется отправка сообщения
        text_input->clear();                              // true - я; false - другой пользователь
        qDebug() << "Отправлено сообщение:" << message;
    });


    dialogue->setLayout(dialogue_layout);
    stacked_wid->addWidget(dialogue);


    QFrame* separator = new QFrame(dialogsWindow); //основной разделитель списка чатов и диалогового окна
    separator->setFrameShape(QFrame::VLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setLineWidth(1);

    QHBoxLayout* main_layout = new QHBoxLayout(dialogsWindow);
    main_layout->addWidget(chats);
    main_layout->addWidget(separator);
    main_layout->addWidget(stacked_wid);
    dialogsWindow->setLayout(main_layout);

    //кликабельность виджета
    for (int i = 0; i < 5; i++){
        connect(person[i], &MyWidget::mousePressed, this, [this, stacked_wid]() {
            stacked_wid->setCurrentIndex(1);
        });
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
