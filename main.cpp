#include "mainwindow.h"
// #include "chatwindow.h"
#include "server.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // MainWindow ww;
    Server s;
    w.show();
    // ww.show();
    return a.exec();
}
