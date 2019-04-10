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

//*****************************************************************************
// �������ƣ�
//   MoveDetect()
//  
// ����������
//   Mat frame_before_last   ֡�k-2֡
//   Mat frame_last          ֡�k-1֡
//   Mat frame               ֡�k֡
//   int thresh              ֡���ֵ
//  
// �������أ�
//   �˶�Ŀ������
// 
// ����˵����
//   ������֡����˶�Ŀ����м�⣬���Լ�������и�ʴ�����͵���̬ѧ����
//   �Ժ�ɫ�����ע������Ŀ��
//*****************************************************************************
Mat MoveDetect(Mat frame_before_last, Mat frame_last, Mat frame, int thresh);

#endif  // !FRAME_DIFFERENCE_DETECT