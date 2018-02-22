#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat result;
    namedWindow("result",1);

    for(;;) {
      Mat frame1, frame2;
      cap >> frame1; // get a new frame from camera
      cap >> frame2;

      subtract(frame1, frame2, result);

      // cvtColor(frame, result, CV_BGR2GRAY);
      //GaussianBlur(result, result, Size(7,7), 1.5, 1.5);
      // Canny(result, result, 0, 30, 3);
      imshow("result", result);
      if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
