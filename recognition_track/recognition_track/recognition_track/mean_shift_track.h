//*****************************************************************************
// Ŀ��׷�ٳ���
//
// @file:  mean_shift_track/mean_shift_track.h
// @author paradoxLOI��paradoxLOI@163.com��
// @date   2019.04.07
//
// ����ɫ�cam_shift�㷨׷��Ŀ��
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
bool track_init_flag = false;  // ׷�ٳ�ʼ����־����ʼΪδ��ʼ��

Mat hsv;                                        // hsvͼ��
Mat mask;                                       // ��ģͼ��
Mat hue;                                        // ɫ��
Mat hist;                                       // ֱ��ͼ
Mat back_project;                               // ����ͶӰ
Mat hist_show = Mat::zeros(200, 320, CV_8UC3);  // ֱ��ͼ��ʾ
Rect trackWindow;

//*****************************************************************************
// �������ƣ�
//   TargetTrack()
//
// ����������
//
//
// �������أ�
//   Ŀ����ٽ��
//
// ����˵����
//   ����cam_shift��ѡȡ��Ŀ����и���
//*****************************************************************************
Mat TargetTrack(Mat frame, Rect rect_selection);

#endif  // !MEAN_SHIFT_TRACK