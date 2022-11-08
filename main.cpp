#include "mainwindow.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QDesktopWidget *desktop = QApplication::desktop();
    w.setGeometry(desktop->screenGeometry(1));
    w.show();
    return a.exec();
}
