//*****************************************************************************
// 目标检测及追踪程序
//
// @file:  recognition_track/recognition_track.cpp
// @author paradoxLOI（paradoxLOI@163.com）
// @date   2019.04.07
//
// 利用三帧差法检测运动目标并标明，利用cam_shift算法对检测到的目标进行追踪
//*****************************************************************************
#include "recognition_track.h"

//#define VIDEO_FROM_FILE  // 从本地文件读取视频

int main() {
  VideoCapture video;

#ifndef VIDEO_FROM_FILE
  video.open(0);  // 打开摄像头
#else
  video.open("C:\\Users\\parad\\Desktop\\test3.mp4");  // 打开本地视频文件
#endif  // !VIDEO_FROM_FILE

  // 针对视频文件处理变量
  int frame_count = video.get(
      CV_CAP_PROP_FRAME_COUNT);  // 视频文件总帧数(仅在处理本地视频文件时有效）
  int FPS = video.get(CV_CAP_PROP_FPS);  // 视频文件帧率

  // 摄像头打开失败检测
  if (!video.isOpened()) {
    std::cout << "error, cam open fail!" << '\n';
    return -1;
  }

  namedWindow("Result", WINDOW_NORMAL);
  //namedWindow("Histogram", WINDOW_NORMAL);
  //namedWindow("Cam Shift", WINDOW_NORMAL);
  //namedWindow("Param Adjust", WINDOW_NORMAL);
  //createTrackbar("Vmin", "Param Adjust", &vmin, 256, 0);      //默认10
  //createTrackbar("Vmax", "Param Adjust", &vmax, 256, 0);      //默认256
  //createTrackbar("Smin", "Param Adjust", &smin, 256, 0);      //默认30
  //createTrackbar("thresh", "Param Adjust", &thresh, 256, 0);  //默认50
  //createTrackbar("detect size", "Param Adjust", &min_detect_size, 20000, 0);  //默认10000

  Mat frame, frame_last, frame_before_last, frame_out;
 
  for (int i = 0;; i++) {
    video >> frame;

    // 帧异常检测
    if (frame.empty()) {
      std::cout << "error, frame is empty!" << '\n';
      break;
    }
    if (!detected_flag) {
      if (0 == i) {
        // 第一帧特殊处理
        frame_out = MoveDetect(frame, frame, frame);
      } else if (1 == i) {
        // 第二帧特殊处理
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
    // 视频流结束
    if (i == frame_count) {
      break;
    }
#endif  // VIDEO_FROM_FILE
  }
}