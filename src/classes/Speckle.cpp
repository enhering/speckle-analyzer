#include "Speckle.h"

Speckle::Speckle() {
  m_nMaxIntensity = 0;
  m_MinIntensity  = 1E9;

  m_nNumFrame = 0;
}

Speckle::~Speckle() {

}

void Speckle::SetImage(cv::Mat Image) {
  m_LastInputImage = m_InputImage;
  m_InputImage = Image;

  m_nImageRows = Image.rows;
  m_nImageCols = Image.cols;

  m_nNumFrame++;
}

cv::Mat Speckle::GetImageDifference() {
  cv::Mat Difference;

  subtract(m_LastInputImage, m_InputImage, Difference);

  return Difference;
}

void Speckle::InitDataMatrix() {
  std::cout << "Initializing data matrix...";

  cData = cv::Mat::zeros(m_ImageHeight, m_ImageWidth, CV_32FC3);
  cDataToPlot = cv::Mat::zeros(m_ImageHeight, m_ImageWidth, CV_32FC3);
  cData2 = cv::Mat::zeros(m_ImageHeight, m_ImageWidth, CV_32FC3);
  cData2ToPlot = cv::Mat::zeros(m_ImageHeight, m_ImageWidth, CV_32FC3);

  for (uint16_t nX = 0; nX < m_ImageWidth; nX++) {
    for (uint16_t nY = 0; nY < m_ImageHeight; nY++) {

      cData.at<cv::Vec3f>(nY,nX)[0] = 65000; // min
      cData.at<cv::Vec3f>(nY,nX)[1] = 0; // max
      cData.at<cv::Vec3f>(nY,nX)[2] = 0; // amplitude
      // std::cout << "(" << nX << "," << nY << "): " << cData.at<cv::Vec3f>(nY,nX)[0] << std::endl;
    }
  }
  std::cout << "Done." << std::endl;
}


void Speckle::FindIntensityExtremes() {
  m_nMaxIntensity = 0;
  m_MinIntensity  = 1E9;

  for (uint16_t nX = 0; nX < m_nImageCols; nX++) {
    for (uint16_t nY = 0; nY < m_nImageRows; nY++) {
      if (m_InputImage.at<ushort>(nY,nX) > m_nMaxIntensity) {
        m_MaxIntensity = m_InputImage.at<ushort>(nY,nX);
      }
      if (m_InputImage.at<ushort>(nY,nX) < m_MinIntensity) {
        m_MinIntensity = m_InputImage.at<ushort>(nY,nX);
      }
      // if (cData2.at<cv::Vec3f>(nY,nX)[1] > fMaxDiffSqr) {
      //   fMaxDiffSqr = cData2.at<cv::Vec3f>(nY,nX)[1];
      // }
    }
  }
}

void Speckle::CalcActivity() {

    double fMaxDiffSqr = 0;

    for (uint16_t nX = 0; nX < m_nImageCols; nX++) {
      for (uint16_t nY = 0; nY < m_nImageRows; nY++) {
        if (m_InputImage.at<ushort>(nY,nX) < cData.at<cv::Vec3f>(nY,nX)[0] ) {
          cData.at<cv::Vec3f>(nY,nX)[0] = m_InputImage.at<ushort>(nY,nX); // min
        }
        if (m_InputImage.at<ushort>(nY,nX) > cData.at<cv::Vec3f>(nY,nX)[1]) {
          cData.at<cv::Vec3f>(nY,nX)[1] = m_InputImage.at<ushort>(nY,nX); // max
        }
        cData.at<cv::Vec3f>(nY,nX)[2] = (cData.at<cv::Vec3f>(nY,nX)[1] - cData.at<cv::Vec3f>(nY,nX)[0]); // amplitude

        // Calculate intensity difference between this frama and last frame
        double fDifference = (m_InputImage.at<ushort>(nY,nX) - m_LastInputImage.at<ushort>(nY, nX));

        // Save square of difference
        double fSquareofDifference = pow(fDifference, 2);

        // Average the squares of differences
        cData2.at<cv::Vec3f>(nY,nX)[1] = ((cData2.at<cv::Vec3f>(nY,nX)[0] * (m_nNumFrame-1)) + fSquareofDifference) / m_nNumFrame;

        // Save this average
        cData2.at<cv::Vec3f>(nY,nX)[0] = cData2.at<cv::Vec3f>(nY,nX)[1];

        // Save intensity value for next frame
        cData2.at<cv::Vec3f>(nY,nX)[2] = m_InputImage.at<ushort>(nY,nX) / m_nMaxIntensity;

        // std::cout << "frame data at (" << nX << ", " << nY << "): " << m_InputImage.at<ushort>(nY,nX)
        //         << " min: " << cData.at<cv::Vec3f>(nY,nX)[0]
        //         << " max: " << cData.at<cv::Vec3f>(nY,nX)[1]
        //         << " amplitude: " << cData.at<cv::Vec3f>(nY,nX)[2] << std::endl;
      }
    }

    m_LastInputImage = m_InputImage.clone();

    //ushort intensity = m_InputImage.at<ushort>(200, 200);

    // std::cout << "x: "  << g_nMouseX << " y: " << g_nMouseY << " Intensity:" << intensity << std::endl;

    int nRegenerationStep = 1000;

    cData2ToPlot = cData2;

    for (uint16_t nX = 0; nX < m_nImageCols; nX++) {
      for (uint16_t nY = 0; nY < m_nImageRows; nY++) {
        cDataToPlot.at<cv::Vec3f>(nY,nX)[0] = cData2.at<cv::Vec3f>(nY,nX)[2];//cData.at<cv::Vec3f>(nY,nX)[0] / m_nMaxIntensity; // min
        cDataToPlot.at<cv::Vec3f>(nY,nX)[1] = 0;//cData.at<cv::Vec3f>(nY,nX)[1] / m_nMaxIntensity; // max
        cDataToPlot.at<cv::Vec3f>(nY,nX)[2] = cData.at<cv::Vec3f>(nY,nX)[2] / m_nMaxIntensity; // amplitude

        // cData2ToPlot.at<cv::Vec3f>(nY,nX)[0] /= fMaxDiffSqr;
        cData2ToPlot.at<cv::Vec3f>(nY,nX)[1] = 0;
        // cData2ToPlot.at<cv::Vec3f>(nY,nX)[2] /= 5;

        // Bring data down to min
        if (cData.at<cv::Vec3f>(nY,nX)[0] < (65535 - nRegenerationStep)) {
          cData.at<cv::Vec3f>(nY,nX)[0] += nRegenerationStep;
        }

        // Take data up to max
        if (cData.at<cv::Vec3f>(nY,nX)[1] > nRegenerationStep ) {
          cData.at<cv::Vec3f>(nY,nX)[1] -= nRegenerationStep;
        }
        cData.at<cv::Vec3f>(nY,nX)[2] = 65535- (cData.at<cv::Vec3f>(nY,nX)[1] - cData.at<cv::Vec3f>(nY,nX)[0]);
      }
    }


}