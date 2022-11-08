#include "mainwindow.h"
#include "imageextractwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(1920, 2160);
    ImageExtractWidget *w = new ImageExtractWidget(this);
    w->setGeometry(0, 0, 1920, 2160);
    w->show();
}

MainWindow::~MainWindow()
{
}

