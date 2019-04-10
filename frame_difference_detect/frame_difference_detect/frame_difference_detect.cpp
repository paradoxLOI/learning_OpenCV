//*****************************************************************************
// 帧差法运动目标检测程序
//
// @file:  frame_difference_detect/frame_difference_detect.cpp
// @author paradoxLOI（paradoxLOI@163.com）
// @date   2019.04.06
//
// 利用三帧差法检测运动目标并标明
//*****************************************************************************
#include "frame_difference_detect.h"

#define VIDEO_FROM_FILE  // 从本地文件读取视频

int main() {
  VideoCapture video;

#ifndef VIDEO_FROM_FILE
  video.open(0);  // 打开摄像头
#else
  video.open("C:\\Users\\parad\\Desktop\\test2.avi");  // 打开本地视频文件
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

  Mat frame_raw;                      // 原始图象， 缩放图象
  Mat frame_last, frame_before_last;  // 上一帧，及上上帧
  Mat frame_detect;                   // 检测结果
  int thresh = 50;                    // 帧差法阈值

  // 窗口定义
  // namedWindow("diff_1", WINDOW_NORMAL);
  // namedWindow("diff_2", WINDOW_NORMAL);
  namedWindow("Detect Result", WINDOW_NORMAL);
  createTrackbar("thresh", "Detect Result", &thresh, 255,
                 0);  // 帧差法阈值调整(0-255)，默认为50

  for (int i = 0;; i++) {
    video >> frame_raw;

    // 帧异常检测
    if (frame_raw.empty()) {
      std::cout << "error, frame is empty!" << '\n';
      break;
    }

    if (0 == i) {
      // 第一帧特殊处理
      frame_detect = MoveDetect(frame_raw, frame_raw, frame_raw, thresh);
    } else if (1 == i) {
      // 第二帧特殊处理
      frame_detect = MoveDetect(frame_last, frame_raw, frame_raw, thresh);
    } else {
      frame_detect =
          MoveDetect(frame_before_last, frame_last, frame_raw, thresh);
    }

    imshow("Detect Result", frame_detect);
    waitKey(1000.0 / FPS);

    frame_before_last = frame_last.clone();
    frame_last = frame_raw.clone();

#ifdef VIDEO_FROM_FILE
    // 视频流结束
    if (i == frame_count) {
      break;
    }
#endif  // VIDEO_FROM_FILE
  }

  return 0;
}

Mat MoveDetect(Mat frame_before_last, Mat frame_last, Mat frame, int thresh) {
  Mat frame_detect = frame.clone();

  // 灰度图转换
  Mat gray_before_last, gray_last, gray;
  cvtColor(frame, gray, CV_BGR2GRAY);
  cvtColor(frame_last, gray_last, CV_BGR2GRAY);
  cvtColor(frame_before_last, gray_before_last, CV_BGR2GRAY);

  // 帧差
  Mat diff_1, diff_2;
  absdiff(gray_before_last, gray_last, diff_1);
  absdiff(gray_last, gray, diff_2);

  // 对差值图进行阈值化处理
  Mat diff_thresh_1, diff_thresh_2;
  threshold(diff_1, diff_thresh_1, 0, 255, CV_THRESH_OTSU);
  threshold(diff_2, diff_thresh_2, 0, 255, CV_THRESH_OTSU);
  //threshold(diff_1, diff_thresh_1, 0, 255, CV_THRESH_OTSU);
  //threshold(diff_2, diff_thresh_2, 0, 255, CV_THRESH_OTSU);
  imshow("diff_1", diff_thresh_1);
  imshow("diff_2", diff_thresh_2);

  // 腐蚀
  Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
  erode(diff_thresh_1, diff_thresh_1, kernel_erode);
  erode(diff_thresh_2, diff_thresh_2, kernel_erode);

  // 膨胀
  Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(18, 18));
  dilate(diff_thresh_1, diff_thresh_1, kernel_dilate);
  dilate(diff_thresh_2, diff_thresh_2, kernel_dilate);

  // 差值图象与处理
  Mat diff_thresh;
  //Mat diff_thresh = diff_thresh_2.clone();
  bitwise_and(diff_thresh_1, diff_thresh_2, diff_thresh);
  imshow("diff_thresh", diff_thresh);

  // 绘制轮廓
  std::vector<std::vector<Point> > contours;
  findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
  drawContours(frame_detect, contours, -1, Scalar(0, 0, 255),
               2);  //在frame_detect上绘制轮廓

  // 轮廓外接矩形
  std::vector<Rect> bound_rect(contours.size());
  for (int i = 0; i < contours.size(); i++) {
    bound_rect[i] = boundingRect(contours[i]);
    rectangle(frame_detect, bound_rect[i], Scalar(0, 255, 0),
              2);  //在frame_detect上绘制正外接矩形
  }

  return frame_detect;
}