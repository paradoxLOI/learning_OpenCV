//*****************************************************************************
// Ŀ���⼰׷�ٳ���
//
// @file:  recognition_track/recognition_track.h
// @author paradoxLOI��paradoxLOI@163.com��
// @date   2019.04.07
//
// ������֡�����˶�Ŀ�겢����������cam_shift�㷨�Լ�⵽��Ŀ�����׷��
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

extern int vmin, vmax, smin;  // mean_shift�㷨��ģ������ͨ������������
extern int thresh;  // difference_detect�㷨��ֵ������ͨ������������
extern int min_detect_size; // difference_detect��С��������ֵ��ͨ������������
extern bool detected_flag;
extern bool track_init_flag;
extern Rect detect_rect;

extern Mat MoveDetect(Mat frame_before_last, Mat frame_last, Mat frame);
extern Mat TargetTrack(Mat frame, Rect rect_selection);

#endif  // !RECOGNITION_TRACK