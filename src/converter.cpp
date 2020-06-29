#include <iostream>
#include <iomanip>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <QMessageBox>
#include <QProgressDialog>
#include <QDateTime>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/superres/superres.hpp>
#include <opencv2/superres/optical_flow.hpp>
#include <opencv2/contrib/contrib.hpp>
#include "converter.h"

using namespace cv;

#define MEASURE_TIME(op) \
{ \
    TickMeter tm; \
    tm.start(); \
    op; \
    tm.stop(); \
    cout << tm.getTimeSec() << " sec" << endl; \
}

Converter::Converter()
{

}
bool Converter::Mkdir(const string& target_path)
{
    string command;

    command = "mkdir -p " + target_path ;
    if(-1 == system(command.c_str()))
    {
        return false;
    }
    return true;
}
bool Converter::Video2Imgs(const string& _src_path,const string& _dst_path \
                           ,const string& _img_type,const int _img_width   \
                           ,const int _img_height,const int _fps_frq,bool _crop_flag)
{
    VideoCapture vc = VideoCapture(_src_path);
    if(!vc.isOpened()){
        QMessageBox::warning(NULL, "warning", QString::fromUtf8("视频文件不存在！"), QMessageBox::Yes);
        return false;
    }
    const string optFlow = "farneback";  //使用的算法：farneback, tvl1, brox, pyrlk
    const bool gpu = false;
    Ptr<cv::superres::SuperResolution> super_res;
    if ( gpu )
        super_res = cv::superres::createSuperResolution_BTVL1_GPU();
    else
        super_res = cv::superres::createSuperResolution_BTVL1();

    Ptr<cv::superres::FrameSource> frame_source;
    //设置要使用的超分辨算法
    if( gpu )
    {
        //如果要使用gpu的话，要将视频进行gpu编码
        try
        {
            frame_source = cv::superres::createFrameSource_Video_GPU(_src_path);
            Mat frame;
            frame_source->nextFrame(frame);
        }
        catch (const cv::Exception&)
        {
            frame_source.release();
        }
    }
    if (!frame_source)
    {
        frame_source = cv::superres::createFrameSource_Video(_src_path);
    }
    // 跳过第一帧，因为第一帧通常是坏的
    Mat frame;
    frame_source->nextFrame(frame);

    super_res->setInput(frame_source);

    // 确定文件夹路径
    QDateTime tmp_cur_time = QDateTime::currentDateTime();
    QString qcur_time = tmp_cur_time.toString("yyyy-MM-dd_hh:mm:ss.zzz");
    String cur_time = qcur_time.toStdString();
    string target_path_1 = _dst_path + "/" + cur_time;
    string target_path_2 = _dst_path + "/" + cur_time + "/image";
    Mkdir(target_path_1);
    Mkdir(target_path_2);

    // 记录系统时间
    auto start = chrono::system_clock::now();
    target_path_1 += "/";
    target_path_2 += "/";

    // 写入txt文件
    ofstream txt_srm;
    txt_srm.open(target_path_1 + "times.txt");

    // 开始超分辨率重建图像
    int img_sq = 0;     // 用于时间序列
    for (int i = 0;; ++i)
    {
        // 采样
        if(i % _fps_frq != 0)
            continue;
        Mat src_frame;
        frame_source->nextFrame( src_frame );
        if(src_frame.empty())
            break;
        /*超分辨率重建图像--begin*/
        //cout << flush; 无条件的将缓冲区中的输出信息送至显示器
        cout << '[' << setw(3) << i << "] : 超分辨率重建,用时 " << flush;
        Mat result;
        //nextFrame(result)的作用是处理下一帧，同时利用result返回当前帧的处理结果
        MEASURE_TIME(super_res->nextFrame(result));

        if (result.empty())
            break;
        /*超分辨率重建图像--end*/
        /*图像裁剪及时间序列生成--begin*/
        Mat frame_resized;
        stringstream str_srm;
        str_srm << setw(6) << setfill('0') << img_sq++ << _img_type;
        cv::resize(result, frame_resized, cv::Size(_img_width,_img_height));
        // 裁剪图像
        Rect area(0,frame_resized.rows/8.0,frame_resized.cols/9.0*8,frame_resized.rows/14.0*11);
        Mat corp_frame_resized = frame_resized(area);
        imwrite(target_path_2 + str_srm.str(),corp_frame_resized);
        if(txt_srm.is_open()){
            auto now = chrono::system_clock::now();
            auto timestamp = chrono::duration_cast<chrono::milliseconds>(now - start);
            txt_srm << scientific << float(timestamp.count()) << endl;
        }
        /*图像裁剪及时间序列生成--end*/
    }
    QMessageBox::about(NULL, "Success", QString::fromUtf8("转换成功！"));
    return true;
}

bool Converter::Video2Jpg(const string& _src_path,const string& _dst_path \
                          ,const int _img_width,const int _img_height,const int _fps_frq=10,bool _crop_flag=true)
{
    return Video2Imgs(_src_path,_dst_path,".jpg",_img_width,_img_height,_fps_frq,_crop_flag);
}
bool Converter::Video2Png(const string& _src_path,const string& _dst_path \
                          ,const int _img_width,const int _img_height,const int _fps_frq=10,bool _crop_flag=true)
{
    return Video2Imgs(_src_path,_dst_path,".png",_img_width,_img_height,_fps_frq,_crop_flag);
}
