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

Mat image;

bool backprojMode = false;
bool selectObject = false;
int trackObject = 0;
bool showHist = true;
Point origin;
Rect selection;
int vmin = 10, vmax = 256, smin = 30;

// User draws box around object to track. This triggers CAMShift to start
// tracking
static void onMouse(int event, int x, int y, int, void*) {
  if (selectObject) {
    selection.x = MIN(x, origin.x);
    selection.y = MIN(y, origin.y);
    selection.width = std::abs(x - origin.x);
    selection.height = std::abs(y - origin.y);

    selection &= Rect(0, 0, image.cols, image.rows);
  }

  switch (event) {
    case EVENT_LBUTTONDOWN:
      origin = Point(x, y);
      selection = Rect(x, y, 0, 0);
      selectObject = true;
      break;
    case EVENT_LBUTTONUP:
      selectObject = false;
      if (selection.width > 0 && selection.height > 0)
        trackObject = -1;  // Set up CAMShift properties in main() loop
      break;
  }
}

string hot_keys =
    "\n\nHot keys: \n"
    "\tESC - quit the program\n"
    "\tc - stop the tracking\n"
    "\tb - switch to/from backprojection view\n"
    "\th - show/hide object histogram\n"
    "\tp - pause video\n"
    "To initialize tracking, select the object with mouse\n";

static void help() {
  cout << "\nThis is a demo that shows mean-shift based tracking\n"
          "You select a color objects such as your face and it tracks it.\n"
          "This reads from video camera (0 by default, or the camera number "
          "the user enters\n"
          "Usage: \n"
          "   ./camshiftdemo [camera number]\n";
  cout << hot_keys;
}

const char* keys = {
    "{help h | | show help message}{@camera_number| 0 | camera number}"};

int main(int argc, const char** argv) {
  VideoCapture cap;
  Rect trackWindow;
  int hsize = 16;
  float hranges[] = {0, 180};
  const float* phranges = hranges;
  CommandLineParser parser(argc, argv, keys);
  if (parser.has("help")) {
    help();
    return 0;
  }
  int camNum = parser.get<int>(0);
  cap.open(camNum);

  if (!cap.isOpened()) {
    help();
    cout << "***Could not initialize capturing...***\n";
    cout << "Current parameter's value: \n";
    parser.printMessage();
    return -1;
  }
  cout << hot_keys;
  namedWindow("Histogram", 0);
  namedWindow("CamShift Demo", 0);
  setMouseCallback("CamShift Demo", onMouse, 0);
  //createTrackbar("Vmin", "CamShift Demo", &vmin, 256, 0);
  //createTrackbar("Vmax", "CamShift Demo", &vmax, 256, 0);
  //createTrackbar("Smin", "CamShift Demo", &smin, 256, 0);

  Mat frame, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3),
                                   backproj;
  bool paused = false;

  for (;;) {
    if (!paused) {
      cap >> frame;
      if (frame.empty()) break;
    }

    frame.copyTo(image);

    if (!paused) {
      cvtColor(image, hsv, COLOR_BGR2HSV);

      if (trackObject) {
        int _vmin = vmin, _vmax = vmax;

        inRange(hsv, Scalar(0, smin, MIN(_vmin, _vmax)),
                Scalar(180, 256, MAX(_vmin, _vmax)), mask); //通过inRange限制s和v分量范围，滤除一些干扰
        int ch[] = {0, 0};
        hue.create(hsv.size(), hsv.depth());  //hue初始化为与hsv大小深度一样的矩阵
        mixChannels(&hsv, 1, &hue, 1, ch, 1); //将hsv第一个通道(也就是色调)的数复制到hue中

        if (trackObject < 0) {  // 起始帧初始化，得到颜色直方图分布
          // Object has been selected by user, set up CAMShift search properties
          // once
          Mat roi(hue, selection), maskroi(mask, selection);
          calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);  //用掩模把一些不在inRange的量剔除
          normalize(hist, hist, 0, 255, NORM_MINMAX); //将hist归一化到0~255

          trackWindow = selection;
          trackObject = 1;  // Don't set up again, unless user selects new ROI

          // 下面的是直方图绘制，并不影响最终跟踪效果
          histimg = Scalar::all(0);
          int binW = histimg.cols / hsize;
          Mat buf(1, hsize, CV_8UC3);
          for (int i = 0; i < hsize; i++)
            buf.at<Vec3b>(i) =
                Vec3b(saturate_cast<uchar>(i * 180. / hsize), 255, 255);
          cvtColor(buf, buf, COLOR_HSV2BGR);

          for (int i = 0; i < hsize; i++) {
            int val =
                saturate_cast<int>(hist.at<float>(i) * histimg.rows / 255);
            rectangle(histimg, Point(i * binW, histimg.rows),
                      Point((i + 1) * binW, histimg.rows - val),
                      Scalar(buf.at<Vec3b>(i)), -1, 8);
          }
        }

        // Perform CAMShift
        calcBackProject(&hue, 1, 0, hist, backproj, &phranges); //使用统计学的语言，BackProjection中存储的数值代表了测试图像中该像素属于皮肤区域的概率。亮的区域是皮肤区域的可能性更大，而暗的区域则表示更低的可能性。
        backproj &= mask;
        RotatedRect trackBox = CamShift(
            backproj, trackWindow,
            TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
        if (trackWindow.area() <= 1) {
          int cols = backproj.cols, rows = backproj.rows,
              r = (MIN(cols, rows) + 5) / 6;
          trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
                             trackWindow.x + r, trackWindow.y + r) &
                        Rect(0, 0, cols, rows);
        }

        if (backprojMode) cvtColor(backproj, image, COLOR_GRAY2BGR);
        ellipse(image, trackBox, Scalar(0, 0, 255), 3, LINE_AA);
      }
    } else if (trackObject < 0)
      paused = false;

    if (selectObject && selection.width > 0 && selection.height > 0) {
      Mat roi(image, selection);
      bitwise_not(roi, roi);
    }

    imshow("CamShift Demo", image);
    imshow("Histogram", histimg);

    char c = (char)waitKey(10);
    if (c == 27) break;
    switch (c) {
      case 'b':
        backprojMode = !backprojMode;
        break;
      case 'c':
        trackObject = 0;
        histimg = Scalar::all(0);
        break;
      case 'h':
        showHist = !showHist;
        if (!showHist)
          destroyWindow("Histogram");
        else
          namedWindow("Histogram", 1);
        break;
      case 'p':
        paused = !paused;
        break;
      default:;
    }
  }

  return 0;
}
