//*****************************************************************************
// 目标追踪程序
//
// @file:  mean_shift_track/mean_shift_track.cpp
// @author paradoxLOI（paradoxLOI@163.com）
// @date   2019.04.07
//
// 利用色差及cam_shift算法追踪目标
//*****************************************************************************
#include "mean_shift_track.h"

Mat TargetTrack(Mat frame, Rect rect_selection) { 
  int hsize = 16;
  float h_ranges[] = {0, 180};  // 色调范围
  const float *p_h_ranges = h_ranges;

  cvtColor(frame, hsv, COLOR_BGR2HSV);

  int _vmin = vmin, _vmax = vmax;
  inRange(hsv, Scalar(0, smin, MIN(_vmin, _vmax)),
          Scalar(180, 256, MAX(_vmin, _vmax)),
          mask);  //通过inRange限制s和v分量范围，滤除一些干扰
  int ch[] = {0, 0};
  hue.create(hsv.size(), hsv.depth());  // hue初始化为与hsv大小深度一样的矩阵
  mixChannels(&hsv, 1, &hue, 1, ch, 1);  //将hsv第一个通道(也就是色调)的数复制到hue中

  if (!track_init_flag) {
    // Object has been selected by user, set up CAMShift search properties
    // once
    Mat roi(hue, rect_selection), maskroi(mask, rect_selection);
    calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize,
             &p_h_ranges);  //用掩模把一些不在inRange的量剔除
    normalize(hist, hist, 0, 255, NORM_MINMAX);  //将hist归一化到0~255

    trackWindow = rect_selection;

    // 下面的是直方图绘制，并不影响最终跟踪效果
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
      &p_h_ranges);  //使用统计学的语言，BackProjection中存储的数值代表了测试图像中该像素属于皮肤区域的概率。亮的区域是皮肤区域的可能性更大，而暗的区域则表示更低的可能性。
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