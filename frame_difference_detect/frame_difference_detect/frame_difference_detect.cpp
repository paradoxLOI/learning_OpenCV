//*****************************************************************************
// ֡��˶�Ŀ�������
//
// @file:  frame_difference_detect/frame_difference_detect.cpp
// @author paradoxLOI��paradoxLOI@163.com��
// @date   2019.04.06
//
// ������֡�����˶�Ŀ�겢����
//*****************************************************************************
#include "frame_difference_detect.h"

#define VIDEO_FROM_FILE  // �ӱ����ļ���ȡ��Ƶ

int main() {
  VideoCapture video;

#ifndef VIDEO_FROM_FILE
  video.open(0);  // ������ͷ
#else
  video.open("C:\\Users\\parad\\Desktop\\test2.avi");  // �򿪱�����Ƶ�ļ�
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

  Mat frame_raw;                      // ԭʼͼ�� ����ͼ��
  Mat frame_last, frame_before_last;  // ��һ֡��������֡
  Mat frame_detect;                   // �����
  int thresh = 50;                    // ֡���ֵ

  // ���ڶ���
  // namedWindow("diff_1", WINDOW_NORMAL);
  // namedWindow("diff_2", WINDOW_NORMAL);
  namedWindow("Detect Result", WINDOW_NORMAL);
  createTrackbar("thresh", "Detect Result", &thresh, 255,
                 0);  // ֡���ֵ����(0-255)��Ĭ��Ϊ50

  for (int i = 0;; i++) {
    video >> frame_raw;

    // ֡�쳣���
    if (frame_raw.empty()) {
      std::cout << "error, frame is empty!" << '\n';
      break;
    }

    if (0 == i) {
      // ��һ֡���⴦��
      frame_detect = MoveDetect(frame_raw, frame_raw, frame_raw, thresh);
    } else if (1 == i) {
      // �ڶ�֡���⴦��
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
    // ��Ƶ������
    if (i == frame_count) {
      break;
    }
#endif  // VIDEO_FROM_FILE
  }

  return 0;
}

Mat MoveDetect(Mat frame_before_last, Mat frame_last, Mat frame, int thresh) {
  Mat frame_detect = frame.clone();

  // �Ҷ�ͼת��
  Mat gray_before_last, gray_last, gray;
  cvtColor(frame, gray, CV_BGR2GRAY);
  cvtColor(frame_last, gray_last, CV_BGR2GRAY);
  cvtColor(frame_before_last, gray_before_last, CV_BGR2GRAY);

  // ֡��
  Mat diff_1, diff_2;
  absdiff(gray_before_last, gray_last, diff_1);
  absdiff(gray_last, gray, diff_2);

  // �Բ�ֵͼ������ֵ������
  Mat diff_thresh_1, diff_thresh_2;
  threshold(diff_1, diff_thresh_1, 0, 255, CV_THRESH_OTSU);
  threshold(diff_2, diff_thresh_2, 0, 255, CV_THRESH_OTSU);
  //threshold(diff_1, diff_thresh_1, 0, 255, CV_THRESH_OTSU);
  //threshold(diff_2, diff_thresh_2, 0, 255, CV_THRESH_OTSU);
  imshow("diff_1", diff_thresh_1);
  imshow("diff_2", diff_thresh_2);

  // ��ʴ
  Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
  erode(diff_thresh_1, diff_thresh_1, kernel_erode);
  erode(diff_thresh_2, diff_thresh_2, kernel_erode);

  // ����
  Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(18, 18));
  dilate(diff_thresh_1, diff_thresh_1, kernel_dilate);
  dilate(diff_thresh_2, diff_thresh_2, kernel_dilate);

  // ��ֵͼ���봦��
  Mat diff_thresh;
  //Mat diff_thresh = diff_thresh_2.clone();
  bitwise_and(diff_thresh_1, diff_thresh_2, diff_thresh);
  imshow("diff_thresh", diff_thresh);

  // ��������
  std::vector<std::vector<Point> > contours;
  findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
  drawContours(frame_detect, contours, -1, Scalar(0, 0, 255),
               2);  //��frame_detect�ϻ�������

  // ������Ӿ���
  std::vector<Rect> bound_rect(contours.size());
  for (int i = 0; i < contours.size(); i++) {
    bound_rect[i] = boundingRect(contours[i]);
    rectangle(frame_detect, bound_rect[i], Scalar(0, 255, 0),
              2);  //��frame_detect�ϻ�������Ӿ���
  }

  return frame_detect;
}