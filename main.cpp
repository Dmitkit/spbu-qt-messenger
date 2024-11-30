#include "mainwindow.h"
<<<<<<< HEAD
=======
// #include "chatwindow.h"
#include "server.h"
>>>>>>> fd4b0ca (Added users, dialogs)

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
<<<<<<< HEAD
    w.show();
=======
    // MainWindow ww;
    Server s;
    w.show();
    // ww.show();
>>>>>>> fd4b0ca (Added users, dialogs)
    return a.exec();
}
