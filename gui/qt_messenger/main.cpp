#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    //установка шрифта
    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont(":/font/TT Neoris Medium.ttf");
    QFont font("TT Neoris Medium");
    a.setFont(font);

    MainWindow w;
    w.show();
    return a.exec();
}
