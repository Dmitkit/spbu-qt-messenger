#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDataStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    socket->connectToHost("127.0.0.1", 3333);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    login = ui->loginInput->text();
    if (!login.isEmpty()) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << QString("LOGIN") << login;
        socket->write(data);
        ui->loginInput->setEnabled(false);
        ui->loginButton->setEnabled(false);
    }
}

void MainWindow::on_sendButton_clicked()
{
    QString recipient = ui->recipientInput->text();
    QString message = ui->messageInput->text();

    if (!recipient.isEmpty() && !message.isEmpty()) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << QString("MESSAGE") << login << recipient << message;
        socket->write(data);
        ui->messageInput->clear();
    }
}

void MainWindow::onReadyRead()
{
    QDataStream in(socket);
    QString message;
    in >> message;
    ui->chatDisplay->append(message);
}
