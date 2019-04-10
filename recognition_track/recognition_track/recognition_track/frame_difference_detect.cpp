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

Mat MoveDetect(Mat frame_before_last, Mat frame_last, Mat frame) {
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
  threshold(diff_1, diff_thresh_1, thresh, 255, CV_THRESH_BINARY);
  threshold(diff_2, diff_thresh_2, thresh, 255, CV_THRESH_BINARY);
  // threshold(diff_1, diff_thresh_1, 0, 255, CV_THRESH_OTSU);
  // threshold(diff_2, diff_thresh_2, 0, 255, CV_THRESH_OTSU);
  //imshow("diff_1", diff_thresh_1);
  //imshow("diff_2", diff_thresh_2);

  // ��ʴ
  Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
  erode(diff_thresh_1, diff_thresh_1, kernel_erode);
  erode(diff_thresh_2, diff_thresh_2, kernel_erode);

  // ����
  Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(27, 27));
  dilate(diff_thresh_1, diff_thresh_1, kernel_dilate);
  dilate(diff_thresh_2, diff_thresh_2, kernel_dilate);

  // ��ֵͼ���봦��
  Mat diff_thresh;
  // Mat diff_thresh = diff_thresh_2.clone();
  bitwise_and(diff_thresh_1, diff_thresh_2, diff_thresh);
  //imshow("diff_thresh", diff_thresh);

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

  // ������������
  Rect max_rect(0, 0, 0, 0);
  for (int i = 0; i < bound_rect.size(); i++) {
    if (max_rect.area() < bound_rect[i].area()) {
      max_rect = bound_rect[i];
    }
  }
  std::cout << max_rect.area() << '\n';
  if (max_rect.area() > min_detect_size) {
    detect_count++;
    if (detect_count > 15) {
      detected_flag = true;
      detect_rect = max_rect;
    }
  } else {
    detect_count = 0;
  }
  //imshow("Detect Result", frame_detect);

  return frame_detect;
}