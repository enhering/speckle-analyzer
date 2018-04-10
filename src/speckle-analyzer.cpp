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

#include "DC1394Wrapper.h"

using namespace cv;

Mat result;
int g_nMouseX, g_nMouseY;

const int g_nNumPlotPoints = 1000;
double g_fXData[g_nNumPlotPoints];
double g_fYData[g_nNumPlotPoints];
int g_nNumDataPoint;
bool g_bEraseAllData;

DC1394Wrapper g_cDC1394Wrapper;

Mat cFrame1, cFrame2;

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
  
  // 

  // CamContext_grab_next_frame_blocking(cc,pixels,0.2); // timeout after 200 msec
  //   //CamContext_grab_next_frame_blocking(cc,pixels,-1.0f); // never timeout
 


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

  g_cDC1394Wrapper.Init();
  
  g_cDC1394Wrapper.Grab();

  unsigned char * pachImage;
  g_cDC1394Wrapper.GetImage(pachImage); // memory is allocated by method
  Mat wrapped(g_cDC1394Wrapper.GetImageHeight(), 
              g_cDC1394Wrapper.GetImageHeight(), 
              IPL_DEPTH_8U, 
              pachImage);


  // cvNamedWindow("Current", CV_WINDOW_AUTOSIZE);
  // cvShowImage("Current", g_pcCVFrame);

  cFrame1 = wrapped.clone();
  free(pachImage);

  //VideoCapture cap(0);// open the default camera
  // VideoCapture cap(cv::CAP_FIREWIRE);// open the default camera
  //if(!cap.isOpened())  // check if we succeeded
  //    return -1;


  namedWindow("result",1);
  namedWindow("Current",1);

  setMouseCallback("Current", onMouse);

  while(1) {
    // cap >> frame1; // get a new frame from camera

    g_cDC1394Wrapper.Grab();

    unsigned char * pachImage;
    g_cDC1394Wrapper.GetImage(pachImage); // memory is allocated by method
    Mat wrapped2(g_cDC1394Wrapper.GetImageHeight(), 
                 g_cDC1394Wrapper.GetImageHeight(), 
                 IPL_DEPTH_8U, 
                 pachImage);

    cFrame2 = wrapped2.clone();
    free(pachImage);
    // cap >> cFrame2;

    subtract(cFrame1, cFrame2, result);

    Scalar intensity = cFrame1.at<uchar>(g_nMouseY, g_nMouseX);

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
    imshow("Current", cFrame1);

    gSystem->ProcessEvents();

    if (waitKey(30) >= 0) break;
  }
  // the camera will be deinitialized automatically in VideoCapture destructor

  g_cDC1394Wrapper.Close();

  return 0;
}
