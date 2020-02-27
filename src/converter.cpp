#include <iostream>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <QMessageBox>
#include <QProgressDialog>
#include "converter.h"

using namespace cv;
Converter::Converter()
{

}
const string Converter::CurrentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
bool Converter::Mkdir(const string& target_path)
{
    string command;
    string cur_time = CurrentDateTime();

    command = "mkdir -p " + target_path ;
    if(-1 == system(command.c_str()))
    {
        return false;
    }
    return true;
}
bool Converter::Video2Imgs(const string& _src_path,const string& _dst_path \
                           ,const string& _img_type,const int _img_width   \
                           ,const int _img_height,const int _fps_frq=10,bool _time_txt=false)
{
    VideoCapture vc = VideoCapture(_src_path);
    long max_frame_num = 10000;
    if(!vc.isOpened()){
        QMessageBox::warning(NULL, "warning", QString::fromUtf8("视频文件不存在！"), QMessageBox::Yes);
        return false;
    }
    // 确定文件夹路径
    string cur_time = CurrentDateTime();
    string target_path_1 = _dst_path + "/" + cur_time;
    string target_path_2 = _dst_path + "/" + cur_time + "/image_0";
    Mkdir(target_path_1);
    Mkdir(target_path_2);

    // 记录系统时间
    auto start = chrono::system_clock::now();
    int img_sq = 0;
    target_path_1 += "/";
    target_path_2 += "/";

    // 写入txt文件
    ofstream txt_srm;
    if(_time_txt)
        txt_srm.open(target_path_1 + "times.txt");

    // 帧转换为图片
    Mat frame,frame_resized;
    int current_frame = 0;
    do{
        vc.read(frame);
        stringstream str_srm;
        if(current_frame % _fps_frq == 0){
            cout << "正在处理第" << current_frame << "帧" << endl;
            str_srm << setw(6) << setfill('0') << img_sq++ << _img_type;
            cv::resize(frame, frame_resized, cv::Size(_img_width,_img_height));
            imwrite(target_path_2 + str_srm.str(),frame_resized);
            if(_time_txt && txt_srm.is_open()){
                auto now = chrono::system_clock::now();
                auto timestamp = chrono::duration_cast<chrono::milliseconds>(now - start);
                txt_srm << scientific << float(timestamp.count()) << endl;
            }
        }
        current_frame++;
    }while(current_frame < max_frame_num && !frame.empty());
    QMessageBox::about(NULL, "Success", QString::fromUtf8("转换成功！"));
    return true;
}

bool Converter::Video2Jpg(const string& _src_path,const string& _dst_path \
                          ,const int _img_width,const int _img_height,const int _fps_frq=10,bool _time_txt=false)
{
    return Video2Imgs(_src_path,_dst_path,".jpg",_img_width,_img_height,_fps_frq,_time_txt);
}
bool Converter::Video2Png(const string& _src_path,const string& _dst_path \
                          ,const int _img_width,const int _img_height,const int _fps_frq=10,bool _time_txt=false)
{
    return Video2Imgs(_src_path,_dst_path,".png",_img_width,_img_height,_fps_frq,_time_txt);
}
