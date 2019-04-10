//*****************************************************************************
// 帧差法运动目标检测程序
//
// @file:  frame_difference_detect/frame_difference_detect.h
// @author paradoxLOI（paradoxLOI@163.com）
// @date   2019.04.06
//
// 利用三帧差法检测运动目标并标明
//*****************************************************************************
#ifndef FRAME_DIFFERENCE_DETECT
#define FRAME_DIFFERENCE_DETECT

#include <iostream>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

//*****************************************************************************
// 函数名称：
//   MoveDetect()
//  
// 函数参数：
//   Mat frame_before_last   帧差法k-2帧
//   Mat frame_last          帧差法k-1帧
//   Mat frame               帧差法k帧
//   int thresh              帧差法阈值
//  
// 函数返回：
//   运动目标检测结果
// 
// 函数说明：
//   利用三帧差法对运动目标进行检测，并对检测结果进行腐蚀、膨胀等形态学处理，
//   以红色方框标注检测出的目标
//*****************************************************************************
Mat MoveDetect(Mat frame_before_last, Mat frame_last, Mat frame, int thresh);

#endif  // !FRAME_DIFFERENCE_DETECT