//*****************************************************************************
// 目标检测及追踪程序
//
// @file:  recognition_track/recognition_track.h
// @author paradoxLOI（paradoxLOI@163.com）
// @date   2019.04.07
//
// 利用三帧差法检测运动目标并标明，利用cam_shift算法对检测到的目标进行追踪
//*****************************************************************************
#ifndef RECOGNITION_TRACK
#define RECOGNITION_TRACK

#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
using namespace cv;

//#include "frame_difference_detect.h"
//#include "mean_shift_track.h"

extern int vmin, vmax, smin;  // mean_shift算法掩模参数，通过滑动条调整
extern int thresh;  // difference_detect算法阈值参数，通过滑动条调整
extern int min_detect_size; // difference_detect最小检测面积阈值，通过滑动条调整
extern bool detected_flag;
extern bool track_init_flag;
extern Rect detect_rect;

extern Mat MoveDetect(Mat frame_before_last, Mat frame_last, Mat frame);
extern Mat TargetTrack(Mat frame, Rect rect_selection);

#endif  // !RECOGNITION_TRACK