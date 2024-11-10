#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.addUser("Ivan","romac","1515151");
    //w.addUser("Max","deroma","qwertya");
    //w.addChat(1,2);
    //w.addMessage("13:50",2,1,"шепчу");
    //QList<QMap<QString, QVariant>> t = w.autorization("romac","1515151");
    //w.DeleteUser(1);
    //qDebug() << alpha;

    w.show();
    return a.exec();
}
