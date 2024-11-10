#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QBitmap>
#include <QImage>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QTextBrowser>
#include <QDebug>
#include <QFrame>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);

signals:
    void mousePressed();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
