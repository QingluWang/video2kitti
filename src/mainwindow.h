#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>
#include "converter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = NULL);
    ~MainWindow();

private:
    QLabel *qlabel_1,*qlabel_2,*qlabel_3,*qlabel_4,*qlabel_5,*qlabel_6,*qlabel_7;
    QRadioButton *qrbtn_1,*qrbtn_2,*qrbtn_3,*qrbtn_4,*qrbtn_5;
    QLineEdit *qledit_1,*qledit_2,*qledit_3;
    QPushButton *qpbtn_1,*qpbtn_2;
    QButtonGroup *qbg_1,*qbg_2;
};
#endif // MAINWINDOW_H
