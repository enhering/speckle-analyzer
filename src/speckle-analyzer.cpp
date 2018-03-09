#include "opencv2/opencv.hpp"
#include <iostream>


using namespace cv;

Mat result;

int g_nx, g_ny;

static void onMouse(int event,int x,int y,int,void*) {
  //this function will be called every time you move your mouse over the image
  // the coordinates will be in x and y variables
  g_nx = x;
  g_ny = y;
}

int main(int, char**) {
  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
      return -1;

  Mat frame1, frame2;

  namedWindow("result",1);
  namedWindow("Current",1);

  setMouseCallback("result", onMouse);

  for(;;) {
    cap >> frame1; // get a new frame from camera
    cap >> frame2;

    subtract(frame1, frame2, result);

    Vec3f intensity = result.at<Vec3f>(g_ny, g_nx);
    float blue = intensity.val[0];
    float green = intensity.val[1];
    float red = intensity.val[2];

    std::cout << "x: "  << g_nx << " y: " << g_ny << " R:" << red << " G: " << green << " B: " << blue << std::endl;

    // cvtColor(frame, result, CV_BGR2GRAY);
    //GaussianBlur(result, result, Size(7,7), 1.5, 1.5);
    // Canny(result, result, 0, 30, 3);
    imshow("result", result);
    imshow("Current", frame1);
    if(waitKey(30) >= 0) break;
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
