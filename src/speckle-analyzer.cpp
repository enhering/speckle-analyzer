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
#include "TAxis.h"

using namespace cv;

Mat result;
int g_nMouseX, g_nMouseY;

const int g_nNumPlotPoints = 1000;
double g_fXData[g_nNumPlotPoints];
double g_fYData[g_nNumPlotPoints];
int g_nNumDataPoint;
bool g_bEraseAllData;

void ZeroDataPoints() {
  for (int nI = 0; nI < g_nNumPlotPoints; nI++) {
    g_fXData[nI] = nI;
    g_fYData[nI] = 0.0;
  }
}

static void onMouse(int event,int x,int y,int,void*) {
  //this function will be called every time you move your mouse over the image
  // the coordinates will be in x and y variables

  g_nMouseX = x;
  g_nMouseY = y;

  g_bEraseAllData = true;
}

int main(int argc, char* argv[]) {

  g_bEraseAllData = false;
  g_nNumDataPoint = 0;

  TApplication  app("app", &argc, argv);
  TCanvas       canvas("a", "b", 500, 700, 400, 200);
  TGraph        graph(g_nNumPlotPoints, g_fXData, g_fYData);

  graph.SetTitle("Speckle intensity; pixel intensity [0..255]; num data point");
  
  graph.SetMarkerStyle(2);
  graph.SetMarkerColor(4);
  graph.SetMarkerSize(0.3);

  graph.SetLineColor(4);
  graph.SetLineWidth(1);

  graph.GetXaxis()->SetNdivisions(5, kTRUE);

  graph.Draw("APL");

  VideoCapture cap(cv::CAP_FIREWIRE);// open the default camera
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
  //cap.set(cv::CAP_PROP_MODE,3); 
//  cap.set(CAP_PROP_BRIGHTNESS,0); 
  // cap.set(CAP_PROP_CONTRAST,0); 
//  cap.set(CAP_PROP_SATURATION,0); 
  //cap.set(CAP_PROP_HUE,0); 
  //cap.set(CAP_PROP_GAIN,0); 
 // cap.set(CAP_PROP_EXPOSURE,0); 
  // cap.set(CAP_PROP_CONVERT_RGB,0); 
  // cap.set(CAP_PROP_WHITE_BALANCE_BLUE_U,0); 
  // cap.set(CAP_PROP_RECTIFICATION,0); 
  // cap.set(CAP_PROP_MONOCHROME,0); 
  // cap.set(CAP_PROP_SHARPNESS,0); 
  // cap.set(CAP_PROP_AUTO_EXPOSURE,0); 
  //cap.set(CAP_PROP_GAMMA,1); 
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

    Scalar intensity = frame1.at<uchar>(g_nMouseY, g_nMouseX);

    std::cout << "x: "  << g_nMouseX << " y: " << g_nMouseY << " Intensity:" << intensity << std::endl;

    int nPos;

    if (g_bEraseAllData) {
      for(int nI = 0; nI <= g_nNumPlotPoints - 1; nI++) {
        graph.SetPoint(nI, nI, 0);
      }
      g_nNumDataPoint = 0;
      g_bEraseAllData = false;
    }
    else {
      if (g_nNumDataPoint >= g_nNumPlotPoints) {
        for(int nI = 0; nI <= (g_nNumPlotPoints - 2); nI++) {
          g_fXData[nI] = g_fXData[nI + 1];
          g_fYData[nI] = g_fYData[nI + 1];
          graph.SetPoint(nI, g_fXData[nI], g_fYData[nI]);
        }
        nPos = g_nNumPlotPoints - 1;
        g_fXData[nPos] = g_nNumDataPoint;
        g_fYData[nPos] = (int) intensity.val[0];
      }
      else {
        nPos = g_nNumDataPoint;
        g_fXData[nPos] = g_nNumDataPoint;
        g_fYData[nPos] = (int) intensity.val[0];
      }
      
      graph.SetPoint(nPos, g_fXData[nPos], g_fYData[nPos]);
      g_nNumDataPoint++;
    }

    graph.Draw("APL");
    canvas.Update();

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
