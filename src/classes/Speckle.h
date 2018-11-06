#ifndef SPECKLE_H_
  #define SPECKLE_H_

  #include "opencv2/opencv.hpp"

  #include "Base.h"

  class Speckle : public Base {
    public:
      Speckle();
      ~Speckle();

      void SetImage(cv::Mat Image) { m_LastInputImage = m_InputImage; m_InputImage = Image; }

      cv::Mat GetProcessedImage() { return m_ProcessedImage; }

      cv::Mat GetImageDifference();
      void FindIntensityExtremes();

      void CalcActivity();

    private:
      cv::Mat m_InputImage, m_LastInputImage;

      unsigned char m_nImageRows, m_nImageCols;

      double m_MaxIntensity, m_MinIntensity;

      cv::Mat cData, cDataToPlot, cData2, cData2ToPlot;

      double m_nMaxIntensity;
      int m_ImageHeight, m_ImageWidth;
  }

#endif