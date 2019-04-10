//*****************************************************************************
// Ŀ���⼰׷�ٳ���
//
// @file:  recognition_track/recognition_track.cpp
// @author paradoxLOI��paradoxLOI@163.com��
// @date   2019.04.07
//
// ������֡�����˶�Ŀ�겢����������cam_shift�㷨�Լ�⵽��Ŀ�����׷��
//*****************************************************************************
#include "recognition_track.h"

//#define VIDEO_FROM_FILE  // �ӱ����ļ���ȡ��Ƶ

int main() {
  VideoCapture video;

#ifndef VIDEO_FROM_FILE
  video.open(0);  // ������ͷ
#else
  video.open("C:\\Users\\parad\\Desktop\\test3.mp4");  // �򿪱�����Ƶ�ļ�
#endif  // !VIDEO_FROM_FILE

  // �����Ƶ�ļ��������
  int frame_count = video.get(
      CV_CAP_PROP_FRAME_COUNT);  // ��Ƶ�ļ���֡��(���ڴ�������Ƶ�ļ�ʱ��Ч��
  int FPS = video.get(CV_CAP_PROP_FPS);  // ��Ƶ�ļ�֡��

  // ����ͷ��ʧ�ܼ��
  if (!video.isOpened()) {
    std::cout << "error, cam open fail!" << '\n';
    return -1;
  }

  namedWindow("Result", WINDOW_NORMAL);
  //namedWindow("Histogram", WINDOW_NORMAL);
  //namedWindow("Cam Shift", WINDOW_NORMAL);
  //namedWindow("Param Adjust", WINDOW_NORMAL);
  //createTrackbar("Vmin", "Param Adjust", &vmin, 256, 0);      //Ĭ��10
  //createTrackbar("Vmax", "Param Adjust", &vmax, 256, 0);      //Ĭ��256
  //createTrackbar("Smin", "Param Adjust", &smin, 256, 0);      //Ĭ��30
  //createTrackbar("thresh", "Param Adjust", &thresh, 256, 0);  //Ĭ��50
  //createTrackbar("detect size", "Param Adjust", &min_detect_size, 20000, 0);  //Ĭ��10000

  Mat frame, frame_last, frame_before_last, frame_out;
 
  for (int i = 0;; i++) {
    video >> frame;

    // ֡�쳣���
    if (frame.empty()) {
      std::cout << "error, frame is empty!" << '\n';
      break;
    }
    if (!detected_flag) {
      if (0 == i) {
        // ��һ֡���⴦��
        frame_out = MoveDetect(frame, frame, frame);
      } else if (1 == i) {
        // �ڶ�֡���⴦��
        frame_out = MoveDetect(frame_last, frame, frame);
      } else {
        frame_out =
            MoveDetect(frame_before_last, frame_last, frame);
      }

      frame_before_last = frame_last.clone();
      frame_last = frame.clone();
    } else {
      frame_out = TargetTrack(frame, detect_rect);
      track_init_flag = true;
    }

    imshow("Result", frame_out);
    waitKey(1000.0 / FPS);

#ifdef VIDEO_FROM_FILE
    // ��Ƶ������
    if (i == frame_count) {
      break;
    }
#endif  // VIDEO_FROM_FILE
  }
}