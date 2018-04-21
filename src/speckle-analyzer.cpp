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

int g_nMouseX, g_nMouseY;

int g_nNumBytesPerPixel = 2;

const int g_nNumPlotPoints = 1000;
double g_fXData[g_nNumPlotPoints];
double g_fYData[g_nNumPlotPoints];
int g_nNumDataPoint;
bool g_bEraseAllData;

int g_ImageHeight, g_ImageWidth;

DC1394Wrapper g_cDC1394Wrapper;

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

cv::Mat CaptureImage() {
  g_cDC1394Wrapper.Grab();

  g_ImageHeight = g_cDC1394Wrapper.GetImageHeight();
  g_ImageWidth  = g_cDC1394Wrapper.GetImageWidth();

  // std::cout << "Frame1  data: width=" 
  //           << g_ImageWidth 
  //           << " height=" 
  //           << g_ImageHeight 
  //           << " size " 
  //           << g_cDC1394Wrapper.GetImageSize()
  //           << "bytes." << std::endl;

  uint8_t * pFrameAddress = g_cDC1394Wrapper.GetRawImage();
  Mat cFrame;

  if (pFrameAddress == NULL) {
    cFrame = cv::Mat::zeros(g_ImageHeight, g_ImageWidth, CV_16UC1);  
    return cFrame;
  }
  else {
    cFrame.create(Size(g_ImageWidth, 
                       g_ImageHeight), 
                       CV_16UC1);
    cFrame.data = pFrameAddress;
  }
  return cFrame;
}

int main(int argc, char* argv[]) {

  g_bEraseAllData = false;
  g_nNumDataPoint = 0;
  Mat cFrame1, cFrame2, result;
  Mat cData;

  bool bFirstRun = true;
  
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
  
  cFrame1 = CaptureImage().clone();
  g_cDC1394Wrapper.ReleaseFrame();

  cData = cv::Mat::zeros(g_ImageHeight, g_ImageWidth, CV_16UC3);  

  namedWindow("result",1);
  namedWindow("Current",1);
  namedWindow("Data",1);

  setMouseCallback("Current", onMouse);

  std::vector<int> qualityType;
  qualityType.push_back(CV_IMWRITE_JPEG_QUALITY);
  qualityType.push_back(90);

  // cv::imwrite("Frame1.jpg", cFrame1, qualityType);

  while(1) {
    // cap >> frame1; // get a new frame from camera

    cFrame2 = CaptureImage().clone();
    g_cDC1394Wrapper.ReleaseFrame();

    // cv::imwrite("Frame2.jpg", cFrame1, qualityType);

    subtract(cFrame1, cFrame2, result);

    for (uint16_t nX = 0; nX < g_ImageWidth; nX++) {
      for (uint16_t nY = 0; nY < g_ImageHeight; nY++) {
        Scalar intensity = cFrame2.at<uchar>(nY, nX);

        if (bFirstRun) {
          cData.at<Vec3s>(nX,nY)[0] = cFrame1.at<ushort>(nX,nY); // min
          cData.at<Vec3s>(nX,nY)[1] = cFrame1.at<ushort>(nX,nY); // max
          cData.at<Vec3s>(nX,nY)[2] = 0; // amplitude
          bFirstRun = false;
        }
        else {
          if (cFrame1.at<ushort>(nX,nY) < cData.at<Vec3s>(nX,nY)[0] ) {
            cData.at<Vec3s>(nX,nY)[0] = cFrame1.at<ushort>(nX,nY); // min
          }
          if (cFrame1.at<ushort>(nX,nY) > cData.at<Vec3s>(nX,nY)[1]) {
            cData.at<Vec3s>(nX,nY)[1] = cFrame1.at<ushort>(nX,nY); // max
          }
          cData.at<Vec3s>(nX,nY)[2] = cData.at<Vec3s>(nX,nY)[1] - cData.at<Vec3s>(nX,nY)[0]; // amplitude
        }
      }
    }

    Scalar intensity = cFrame2.at<ushort>(g_nMouseY, g_nMouseX);

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

    // imshow("result", result);
    // imshow("Current", cFrame2);
    // imshow("Data", cData);

    gSystem->ProcessEvents();

    if (waitKey(30) >= 0) break;
  }

  cv::imwrite("Data.jpg", cData, qualityType);
  // the camera will be deinitialized automatically in VideoCapture destructor

  g_cDC1394Wrapper.Close();

  return 0;
}
