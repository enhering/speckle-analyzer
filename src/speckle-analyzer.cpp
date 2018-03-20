#include <iostream>

#include "opencv2/opencv.hpp"

#include "TCanvas.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TApplication.h"


using namespace cv;

Mat result;
double g_nx, g_ny;

static void onMouse(int event,int x,int y,int,void*) {
  //this function will be called every time you move your mouse over the image
  // the coordinates will be in x and y variables
  g_nx = x;
  g_ny = y;
}

int main(int argc, char* argv[]) {

  const int nNumPlotPoints = 50;
  double fXData[nNumPlotPoints];
  double fYData[nNumPlotPoints];

  int nNumDataPoint = 0;

  for (int nI = 0; nI < nNumPlotPoints; nI++) {
    fXData[nI] = nI;
    fYData[nI] = 0.0;
  }

  TApplication  app("app", &argc, argv);
  TCanvas       canvas("a", "b", 0, 0, 400, 200);
  TGraph        graph(nNumPlotPoints, fXData, fYData);

  graph.SetTitle("Measurement XYZ;lenght [cm];Arb.Units");
  graph.SetMarkerStyle(kOpenCircle);
  graph.SetMarkerColor(kBlue);
  graph.SetLineColor(kBlue);

  graph.DrawClone("APE");

  VideoCapture cap(0);// open the default camera
  if(!cap.isOpened())  // check if we succeeded
      return -1;

  // cap.set(CAP_PROP_POS_MSEC,0); 
  // cap.set(CAP_PROP_POS_FRAMES,0); 
  // cap.set(CAP_PROP_POS_AVI_RATIO,0); 
  // cap.set(CAP_PROP_FRAME_WIDTH,0); 
  // cap.set(CAP_PROP_FRAME_HEIGHT,0); 
  // cap.set(CAP_PROP_FPS,0); 
  // cap.set(CAP_PROP_FOURCC,0); 
  // cap.set(CAP_PROP_FRAME_COUNT,0); 
  // cap.set(CAP_PROP_FORMAT,0); 
//  cap.set(cv::CAP_PROP_MODE,3); 
//  cap.set(CAP_PROP_BRIGHTNESS,0); 
  // cap.set(CAP_PROP_CONTRAST,0); 
//  cap.set(CAP_PROP_SATURATION,0); 
 // cap.set(CAP_PROP_HUE,0); 
 // cap.set(CAP_PROP_GAIN,0); 
 // cap.set(CAP_PROP_EXPOSURE,0); 
  // cap.set(CAP_PROP_CONVERT_RGB,0); 
  // cap.set(CAP_PROP_WHITE_BALANCE_BLUE_U,0); 
  // cap.set(CAP_PROP_RECTIFICATION,0); 
  // cap.set(CAP_PROP_MONOCHROME,0); 
  // cap.set(CAP_PROP_SHARPNESS,0); 
  // cap.set(CAP_PROP_AUTO_EXPOSURE,0); 
  //cap.set(CAP_PROP_GAMMA,0); 
  // cap.set(CAP_PROP_TEMPERATURE,0); 
  // cap.set(CAP_PROP_TRIGGER,0); 
  // cap.set(CAP_PROP_TRIGGER_DELAY,0); 
  // cap.set(CAP_PROP_WHITE_BALANCE_RED_V,0); 
  // cap.set(CAP_PROP_ZOOM,0); 
  // cap.set(CAP_PROP_FOCUS,0); 
  // cap.set(CAP_PROP_GUID,0); 
  // cap.set(CAP_PROP_ISO_SPEED,0); 
  // cap.set(CAP_PROP_BACKLIGHT,0); 
  // cap.set(CAP_PROP_PAN,0); 
  // cap.set(CAP_PROP_TILT,0); 
  // cap.set(CAP_PROP_ROLL,0); 
  // cap.set(CAP_PROP_IRIS,0); 
  // cap.set(CAP_PROP_SETTINGS,0); 
  // cap.set(CAP_PROP_BUFFERSIZE,0); 
  // cap.set(CAP_PROP_AUTOFOCUS,0); 

  Mat frame1, frame2;

  namedWindow("result",1);
  namedWindow("Current",1);

  setMouseCallback("Current", onMouse);

  while(1) {
    cap >> frame1; // get a new frame from camera
    cap >> frame2;

    subtract(frame1, frame2, result);

    Vec3f intensity = frame1.at<Vec3f>(g_ny, g_nx);
    float blue = intensity.val[0];
    float green = intensity.val[1];
    float red = intensity.val[2];

    std::cout << "x: "  << g_nx << " y: " << g_ny << " R:" << red << " G: " << green << " B: " << blue << std::endl;

    int nPos;
    if (nNumDataPoint >= nNumPlotPoints) {
      for(int nI = 0; nI < (nNumPlotPoints - 2); nI++) {
        fXData[nI] = fXData[nI + 1];
        fYData[nI] = fYData[nI + 1];
        graph.SetPoint(nI, fXData[nI], fYData[nI]);
      }
      nPos = nNumPlotPoints - 1;
      fXData[nPos] = nNumDataPoint;
      fYData[nPos] = green;
    }
    else {
      nPos = nNumDataPoint;
      fXData[nPos] = nNumDataPoint;
      fYData[nPos] = green;
    }
    
    graph.SetPoint(nPos, fXData[nPos], fYData[nPos]);
    nNumDataPoint++;
    
    graph.DrawClone("APE");
    canvas.Modified();

    // cvtColor(frame, result, CV_BGR2GRAY);
    //GaussianBlur(result, result, Size(7,7), 1.5, 1.5);
    // Canny(result, result, 0, 30, 3);

    imshow("result", result);
    imshow("Current", frame1);

    gSystem->ProcessEvents();

    if (waitKey(30) >= 0) break;
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
