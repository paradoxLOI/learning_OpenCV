//*****************************************************************************
// ֡��˶�Ŀ�������
//
// @file:  frame_difference_detect/frame_difference_detect.h
// @author paradoxLOI��paradoxLOI@163.com��
// @date   2019.04.06
//
// ������֡�����˶�Ŀ�겢����
//*****************************************************************************
#ifndef FRAME_DIFFERENCE_DETECT
#define FRAME_DIFFERENCE_DETECT

#include <iostream>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

int thresh = 50;
int min_detect_size = 1500;
Rect detect_rect;
int detect_count = 0;
bool detected_flag = false;  // Ŀ�����־����ʼΪδ��⵽


//*****************************************************************************
// �������ƣ�
//   MoveDetect()
//  
// ����������
//   Mat frame_before_last   ֡�k-2֡
//   Mat frame_last          ֡�k-1֡
//   Mat frame               ֡�k֡
//  
// �������أ�
//   �˶�Ŀ������
// 
// ����˵����
//   ������֡����˶�Ŀ����м�⣬���Լ�������и�ʴ�����͵���̬ѧ����
//   �Ժ�ɫ�����ע������Ŀ��
//*****************************************************************************
Mat MoveDetect(Mat frame_before_last, Mat frame_last, Mat frame);

#endif  // !FRAME_DIFFERENCE_DETECT