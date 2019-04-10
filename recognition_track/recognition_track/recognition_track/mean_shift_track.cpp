//*****************************************************************************
// Ŀ��׷�ٳ���
//
// @file:  mean_shift_track/mean_shift_track.cpp
// @author paradoxLOI��paradoxLOI@163.com��
// @date   2019.04.07
//
// ����ɫ�cam_shift�㷨׷��Ŀ��
//*****************************************************************************
#include "mean_shift_track.h"

Mat TargetTrack(Mat frame, Rect rect_selection) { 
  int hsize = 16;
  float h_ranges[] = {0, 180};  // ɫ����Χ
  const float *p_h_ranges = h_ranges;

  cvtColor(frame, hsv, COLOR_BGR2HSV);

  int _vmin = vmin, _vmax = vmax;
  inRange(hsv, Scalar(0, smin, MIN(_vmin, _vmax)),
          Scalar(180, 256, MAX(_vmin, _vmax)),
          mask);  //ͨ��inRange����s��v������Χ���˳�һЩ����
  int ch[] = {0, 0};
  hue.create(hsv.size(), hsv.depth());  // hue��ʼ��Ϊ��hsv��С���һ���ľ���
  mixChannels(&hsv, 1, &hue, 1, ch, 1);  //��hsv��һ��ͨ��(Ҳ����ɫ��)�������Ƶ�hue��

  if (!track_init_flag) {
    // Object has been selected by user, set up CAMShift search properties
    // once
    Mat roi(hue, rect_selection), maskroi(mask, rect_selection);
    calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize,
             &p_h_ranges);  //����ģ��һЩ����inRange�����޳�
    normalize(hist, hist, 0, 255, NORM_MINMAX);  //��hist��һ����0~255

    trackWindow = rect_selection;

    // �������ֱ��ͼ���ƣ�����Ӱ�����ո���Ч��
    hist_show = Scalar::all(0);
    int binW = hist_show.cols / hsize;
    Mat buf(1, hsize, CV_8UC3);
    for (int i = 0; i < hsize; i++)
      buf.at<Vec3b>(i) =
          Vec3b(saturate_cast<uchar>(i * 180. / hsize), 255, 255);
    cvtColor(buf, buf, COLOR_HSV2BGR);

    for (int i = 0; i < hsize; i++) {
      int val = saturate_cast<int>(hist.at<float>(i) * hist_show.rows / 255);
      rectangle(hist_show, Point(i * binW, hist_show.rows),
                Point((i + 1) * binW, hist_show.rows - val),
                Scalar(buf.at<Vec3b>(i)), -1, 8);
    }
  }

  calcBackProject(
      &hue, 1, 0, hist, back_project,
      &p_h_ranges);  //ʹ��ͳ��ѧ�����ԣ�BackProjection�д洢����ֵ�����˲���ͼ���и���������Ƥ������ĸ��ʡ�����������Ƥ������Ŀ����Ը��󣬶������������ʾ���͵Ŀ����ԡ�
  back_project &= mask;
  RotatedRect trackBox =
      CamShift(back_project, trackWindow,
               TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
  if (trackWindow.area() <= 1) {
    int cols = back_project.cols, rows = back_project.rows,
        r = (MIN(cols, rows) + 5) / 6;
    trackWindow = Rect(trackWindow.x - r, trackWindow.y - r, trackWindow.x + r,
                       trackWindow.y + r) &
                  Rect(0, 0, cols, rows);
  }

  ellipse(frame, trackBox, Scalar(0, 0, 255), 3, LINE_AA);

  //imshow("Histogram", hist_show);
  //imshow("backpro", back_project);

  return frame;
}