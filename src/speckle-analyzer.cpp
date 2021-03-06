#include "speckle-analyzer.h"

static void onMouse(int event,int x,int y,int,void*) {
  g_nMouseX = x;
  g_nMouseY = y;
}

int main(int argc, char* argv[]) {

  g_pDC1394Wrapper = new DC1394Wrapper();
  g_pSpeckle = new Speckle();

  g_pSpeckle->InitDataMatrix();

  TApplication app("app", &argc, argv);

  cv::Mat CapturedFrame, ProcessedFrame, ImageDifference;

  g_pDC1394Wrapper->Init();

  CapturedFrame = g_pDC1394Wrapper->CaptureImage().clone();
  g_pDC1394Wrapper->ReleaseFrame();

  g_pSpeckle->SetImage(CapturedFrame);

  cv::namedWindow("Current",1);
  cv::setMouseCallback("Current", onMouse);

  char nImageSelect = 0;

  bool bRepeat = true;
  long nNumFrame = 1;

  while(bRepeat) {
    CapturedFrame = g_pDC1394Wrapper->CaptureImage().clone();
    g_pDC1394Wrapper->ReleaseFrame();

    g_pSpeckle->SetImage(CapturedFrame);
    g_pSpeckle->FindIntensityExtremes();
    g_pSpeckle->CalcActivity();

    ImageDifference = g_pSpeckle->GetImageDifference();
    ProcessedFrame = g_pSpeckle->GetProcessedImage();

    gSystem->ProcessEvents();

    char PressedKey = cv::waitKey(30);

    switch (PressedKey) {
      case -1: break; // Nothing pressed
      case 99: nImageSelect = 0; break;  //c
      case 100: nImageSelect = 1; break; //d
      case 101: nImageSelect = 2; break; //e
      default: bRepeat = false;
    }

    switch(nImageSelect) {
      case 2:  imshow("Current", ImageDifference);
      case 1:  imshow("Current", ProcessedFrame);
      case 0:
      default: imshow("Current", CapturedFrame);
    }

    nNumFrame++;
  }

  g_pDC1394Wrapper->Close();

  return 0;
}


    // add(cDataToPlot, cFrame2, cDataToPlot);

    // std::string strFileName = std::to_string(nNumFrame) + ".png";

    // imwrite("data/" + strSampleName + "/" + strFileName, cFrame2);
    // imwrite("data/activity/" + strSampleName + "/" + strFileName, cData2ToPlot);
    // // imwrite(strFileName+"dif.tiff", result);
    // imwrite("data/amplitudes/"+ strSampleName + "/" + strFileName, cDataToPlot);

    // if (nNumFrame > 400) {
    //   break;
    // }
