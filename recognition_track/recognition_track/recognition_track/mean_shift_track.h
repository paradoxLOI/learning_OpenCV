//*****************************************************************************
// 目标追踪程序
//
// @file:  mean_shift_track/mean_shift_track.h
// @author paradoxLOI（paradoxLOI@163.com）
// @date   2019.04.07
//
// 利用色差及cam_shift算法追踪目标
//*****************************************************************************
#ifndef MEAN_SHIFT_TRACK
#define MEAN_SHIFT_TRACK

#include <iostream>

#include <opencv2/core/utility.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/videoio.hpp"

using namespace cv;

int vmin = 10, vmax = 256, smin = 30;
bool track_init_flag = false;  // 追踪初始化标志，初始为未初始化

Mat hsv;                                        // hsv图象
Mat mask;                                       // 掩模图象
Mat hue;                                        // 色调
Mat hist;                                       // 直方图
Mat back_project;                               // 反向投影
Mat hist_show = Mat::zeros(200, 320, CV_8UC3);  // 直方图显示
Rect trackWindow;

//*****************************************************************************
// 函数名称：
//   TargetTrack()
//
// 函数参数：
//
//
// 函数返回：
//   目标跟踪结果
//
// 函数说明：
//   利用cam_shift对选取的目标进行跟踪
//*****************************************************************************
Mat TargetTrack(Mat frame, Rect rect_selection);

#endif  // !MEAN_SHIFT_TRACK