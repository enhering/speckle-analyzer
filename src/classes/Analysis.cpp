#include "Analysis.h"

Analysis::Analysis() {

}

Analysis::~Analysis() {

}

void Analysis::InitDataMatrix() {
  std::cout << "Initializing data matrix...";

  cData = cv::Mat::zeros(g_ImageHeight, g_ImageWidth, CV_32FC3);
  cDataToPlot = cv::Mat::zeros(g_ImageHeight, g_ImageWidth, CV_32FC3);
  cData2 = cv::Mat::zeros(g_ImageHeight, g_ImageWidth, CV_32FC3);
  cData2ToPlot = cv::Mat::zeros(g_ImageHeight, g_ImageWidth, CV_32FC3);

  for (uint16_t nX = 0; nX < g_ImageWidth; nX++) {
    for (uint16_t nY = 0; nY < g_ImageHeight; nY++) {

      cData.at<Vec3f>(nY,nX)[0] = 65000; // min
      cData.at<Vec3f>(nY,nX)[1] = 0; // max
      cData.at<Vec3f>(nY,nX)[2] = 0; // amplitude
      // std::cout << "(" << nX << "," << nY << "): " << cData.at<Vec3f>(nY,nX)[0] << std::endl;
    }
  }
  std::cout << "Done." << std::endl;
}