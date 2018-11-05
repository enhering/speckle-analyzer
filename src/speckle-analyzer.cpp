#include <iostream>

#include "opencv2/opencv.hpp"

#include "DC1394Wrapper.h"
#include "Speckle.h"

int g_nMouseX, g_nMouseY;

// int g_nNumBytesPerPixel = 2;

static void onMouse(int event,int x,int y,int,void*) {
  g_nMouseX = x;
  g_nMouseY = y;

  g_bEraseAllData = true;
}

int main(int argc, char* argv[]) {

  DC1394Wrapper * g_pDC1394Wrapper = new DC1394Wrapper();
  Speckle * g_pSpeckle = new Speckle();

  TApplication  app("app", &argc, argv);


  g_pDC1394Wrapper->Init();

  cv::Mat CapturedFrame = g_pDC1394Wrapper->CaptureImage().clone();
  g_pDC1394Wrapper->ReleaseFrame();

  g_pSpeckle->SetImage(CapturedFrame);

  namedWindow("Current",1);
  setMouseCallback("Current", onMouse);

  long nNumFrame = 1;
  bool bRepeat = true;
  char nImageSelect = 0;

  while(bRepeat) {

    cv::Mat CapturedFrame = g_pDC1394Wrapper->CaptureImage().clone();
    g_pDC1394Wrapper->ReleaseFrame();

    g_pSpeckle->SetImage(CapturedFrame);

    // add(cDataToPlot, cFrame2, cDataToPlot);

    // std::string strFileName = std::to_string(nNumFrame) + ".png";

    // imwrite("data/" + strSampleName + "/" + strFileName, cFrame2);
    // imwrite("data/activity/" + strSampleName + "/" + strFileName, cData2ToPlot);
    // // imwrite(strFileName+"dif.tiff", result);
    // imwrite("data/amplitudes/"+ strSampleName + "/" + strFileName, cDataToPlot);

    gSystem->ProcessEvents();

    char PressedKey = waitkey(30);

    switch (PressedKey) {
      case -1: break; // Nothing pressed
      case 67: nImageSelect = 0; break;
      default: bRepeat = false;
    }

    switch(nImageSelect) {
      case 0:
      default: imshow("Current", CapturedFrame);
    }

    nNumFrame++;

    // if (nNumFrame > 400) {
    //   break;
    // }
  }

  g_pDC1394Wrapper->Close();

  return 0;
}
