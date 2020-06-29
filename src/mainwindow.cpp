#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QObject>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("VSLAM tool 1.0.0");
    QDesktopWidget *deskdop=QApplication::desktop();
    this->resize(400,500);
    this->move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);

    qlabel_video=new QLabel("Video Path");
    qlabel_choice=new QLabel("Choice");
    qlabel_img_type=new QLabel("Image Type");
    qlabel_img_size=new QLabel("Image Size");
    qlabel_img_width=new QLabel("width");
    qlabel_img_height=new QLabel("height");
    qlabel_sampling_rate=new QLabel("Sampling Rate");

    qledit_video=new QLineEdit();
    qledit_width=new QLineEdit();
    qledit_height=new QLineEdit();

    qbg_1=new QButtonGroup();
    qbg_2=new QButtonGroup();
    qrbtn_crop=new QRadioButton("crop");
    qrbtn_full=new QRadioButton("full");
    qrbtn_png=new QRadioButton("png");
    qrbtn_jpg=new QRadioButton("jpg");

    qspinbox_sampling_rate=new QSpinBox();
    qspinbox_sampling_rate->setValue(1);

    qpbtn_choose_file = new QPushButton("choose");
    qpbtn_enter=new QPushButton("enter");
    qpbtn_clear=new QPushButton("clear");
    //set button group
    qrbtn_crop->setChecked(true);
    qrbtn_png->setChecked(true);
    qbg_1->addButton(qrbtn_crop);
    qbg_1->addButton(qrbtn_full);
    qbg_2->addButton(qrbtn_png);
    qbg_2->addButton(qrbtn_jpg);
    qbg_1->setId(qrbtn_crop,1);
    qbg_1->setId(qrbtn_full,2);
    qbg_2->setId(qrbtn_png,1);
    qbg_2->setId(qrbtn_jpg,2);

    //layout
    QHBoxLayout* qh_layout_1 = new QHBoxLayout();
    QHBoxLayout* qh_layout_2 = new QHBoxLayout();
    QHBoxLayout* qh_layout_3 = new QHBoxLayout();
    QHBoxLayout* qh_layout_4 = new QHBoxLayout();
    QHBoxLayout* qh_layout_5 = new QHBoxLayout();
    QHBoxLayout* qh_layout_6 = new QHBoxLayout();
    QVBoxLayout* qv_layout = new QVBoxLayout();
    // set layout
    qh_layout_1->addWidget(qlabel_video);
    qh_layout_1->addWidget(qledit_video);
    qh_layout_1->addWidget(qpbtn_choose_file);
    qh_layout_2->addWidget(qlabel_choice);
    qh_layout_2->addWidget(qrbtn_crop);
    qh_layout_2->addWidget(qrbtn_full);
    qh_layout_3->addWidget(qlabel_img_type);
    qh_layout_3->addWidget(qrbtn_png);
    qh_layout_3->addWidget(qrbtn_jpg);
    qh_layout_4->addWidget(qlabel_img_size);
    qh_layout_4->addWidget(qlabel_img_width);
    qh_layout_4->addWidget(qledit_width);
    qh_layout_4->addWidget(qlabel_img_height);
    qh_layout_4->addWidget(qledit_height);
    qh_layout_5->addWidget(qlabel_sampling_rate);
    qh_layout_5->addWidget(qspinbox_sampling_rate);
    qh_layout_6->addWidget(qpbtn_enter);
    qh_layout_6->addWidget(qpbtn_clear);
    qv_layout->addLayout(qh_layout_1);
    qv_layout->addLayout(qh_layout_2);
    qv_layout->addLayout(qh_layout_3);
    qv_layout->addLayout(qh_layout_4);
    qv_layout->addLayout(qh_layout_5);
    qv_layout->addLayout(qh_layout_6);

    QWidget *widget=new QWidget();
    widget->setLayout(qv_layout);
    widget->setContentsMargins(0,0,0,0);
    this->setCentralWidget(widget);

    converter_ = new Converter();
    connect(qpbtn_choose_file,SIGNAL(clicked()),this,SLOT(on_pushbtn_choose_file_clicked()));
    connect(qpbtn_enter,SIGNAL(clicked()),this,SLOT(on_pushbtn_enter_clicked()));
    connect(qpbtn_clear,SIGNAL(clicked()),this,SLOT(on_pushbtn_clear_clicked()));
}

MainWindow::~MainWindow()
{
}
void MainWindow::on_pushbtn_choose_file_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this,tr("Find files"),QDir::currentPath());
    qledit_video->setText(file_path);
}
void MainWindow::on_pushbtn_clear_clicked()
{
    qledit_video->setText("");
    qledit_width->setText("");
    qledit_height->setText("");
    qspinbox_sampling_rate->setValue(1);
}
void MainWindow::on_pushbtn_enter_clicked()
{
    QString video_path = qledit_video->text();
    quint16 choice_full_crop = qbg_1->checkedId();
    quint16 choice_image_type = qbg_2->checkedId();
    int image_width = qledit_width->text().toInt();
    int image_height = qledit_height->text().toInt();
    quint16 choice_fps_frq = qspinbox_sampling_rate->value();
    std::string s_video_path = video_path.toStdString();
    std::string s_dataset_base_path = "./dataset/";
    // 判空
    if(video_path.isEmpty()){
        QMessageBox::warning(NULL, "warning", QString::fromUtf8("视频文件名不可为空！"), QMessageBox::Yes);
        return;
    }else if(0 == image_width){
        QMessageBox::warning(NULL, "warning", QString::fromUtf8("图片宽度不可为空！"), QMessageBox::Yes);
        return;
    }else if(0 == image_height){
        QMessageBox::warning(NULL, "warning", QString::fromUtf8("图片高度不可为空！"), QMessageBox::Yes);
        return;
    }
    switch (choice_full_crop) {
        // crop
        case 1:
            switch (choice_image_type) {
                case 1:
                    converter_->Video2Png(s_video_path,s_dataset_base_path+"crop",image_width,image_height,choice_fps_frq,true);
                    break;
                case 2:
                    converter_->Video2Jpg(s_video_path,s_dataset_base_path+"crop",image_width,image_height,choice_fps_frq,true);
                    break;
            }
            break;
        // full
        case 2:
            switch (choice_image_type) {
                case 1:
                    converter_->Video2Png(s_video_path,s_dataset_base_path+"full",image_width,image_height,choice_fps_frq,false);
                    break;
                case 2:
                    converter_->Video2Png(s_video_path,s_dataset_base_path+"full",image_width,image_height,choice_fps_frq,false);
                    break;
            }
            break;
        default:
            break;
    }
}


