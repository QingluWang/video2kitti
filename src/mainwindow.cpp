#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("tools");
    QDesktopWidget *deskdop=QApplication::desktop();
    this->resize(400,500);
    this->move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);
}

MainWindow::~MainWindow()
{
}

