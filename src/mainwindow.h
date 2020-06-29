#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>
#include <QSpinBox>
#include "converter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = NULL);
    ~MainWindow();

private:
    QLabel *qlabel_video,*qlabel_choice,*qlabel_img_type \
    ,*qlabel_img_size,*qlabel_img_width,*qlabel_img_height,*qlabel_sampling_rate;
    QRadioButton *qrbtn_crop,*qrbtn_full,*qrbtn_png,*qrbtn_jpg;
    QLineEdit *qledit_video,*qledit_width,*qledit_height;
    QPushButton *qpbtn_choose_file,*qpbtn_enter,*qpbtn_clear;
    QButtonGroup *qbg_1,*qbg_2;
    QSpinBox *qspinbox_sampling_rate;
    Converter *converter_;
private slots:
    void on_pushbtn_choose_file_clicked();
    void on_pushbtn_enter_clicked();
    void on_pushbtn_clear_clicked();
};
#endif // MAINWINDOW_H
