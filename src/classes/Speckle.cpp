#include "Speckle.h"

Speckle::Speckle() {
  m_nMaxIntensity = 0;
  m_MinIntensity  = 1E9;
}

Speckle::~Speckle() {

}

void SetImage(cv::Mat Image) {
  m_LastInputImage = m_InputImage;
  m_InputImage = Image;

  m_nImageRows = Image.rows;
  m_nImageCols = Image.cols;
}

cv::Mat Speckle::GetImageDifference() {
  cv::Mat Difference;

  subtract(m_LastInputImage, m_InputImage, Difference);

  return Difference;
}

void Speckle::FindIntensityExtremes() {
  m_nMaxIntensity = 0;
  m_MinIntensity  = 1E9;

  for (uint16_t nX = 0; nX < m_nImageCols; nX++) {
    for (uint16_t nY = 0; nY < m_nImageRows; nY++) {
      if (m_InputImage.at<ushort>(nY,nX) > m_nMaxIntensity) {
        m_nMaxIntensity = m_InputImage.at<ushort>(nY,nX);
      }
      if (m_InputImage.at<ushort>(nY,nX) < m_nMinIntensity) {
        m_nMinIntensity = m_InputImage.at<ushort>(nY,nX);
      }
      // if (cData2.at<Vec3f>(nY,nX)[1] > fMaxDiffSqr) {
      //   fMaxDiffSqr = cData2.at<Vec3f>(nY,nX)[1];
      // }
    }
  }

}

void Speckle::CalcActivity() {

    double fMaxDiffSqr = 0;



    for (uint16_t nX = 0; nX < m_nImageCols; nX++) {
      for (uint16_t nY = 0; nY < m_nImageRows; nY++) {
        if (cFrame2.at<ushort>(nY,nX) < cData.at<Vec3f>(nY,nX)[0] ) {
          cData.at<Vec3f>(nY,nX)[0] = cFrame2.at<ushort>(nY,nX); // min
        }
        if (cFrame2.at<ushort>(nY,nX) > cData.at<Vec3f>(nY,nX)[1]) {
          cData.at<Vec3f>(nY,nX)[1] = cFrame2.at<ushort>(nY,nX); // max
        }
        cData.at<Vec3f>(nY,nX)[2] = (cData.at<Vec3f>(nY,nX)[1] - cData.at<Vec3f>(nY,nX)[0]); // amplitude

        // Calculate intensity difference between this frama and last frame
        double fDifference = (cFrame2.at<ushort>(nY,nX) - cLastFrame.at<ushort>(nY, nX));

        // Save square of difference
        double fSquareofDifference = pow(fDifference, 2);

        // Average the squares of differences
        cData2.at<Vec3f>(nY,nX)[1] = ((cData2.at<Vec3f>(nY,nX)[0] * (nNumFrame-1)) + fSquareofDifference) / nNumFrame;

        // Save this average
        cData2.at<Vec3f>(nY,nX)[0] = cData2.at<Vec3f>(nY,nX)[1];

        // Save intensity value for next frame
        cData2.at<Vec3f>(nY,nX)[2] = cFrame2.at<ushort>(nY,nX) / g_nMaxIntensity;

        // std::cout << "frame data at (" << nX << ", " << nY << "): " << cFrame2.at<ushort>(nY,nX)
        //         << " min: " << cData.at<Vec3f>(nY,nX)[0]
        //         << " max: " << cData.at<Vec3f>(nY,nX)[1]
        //         << " amplitude: " << cData.at<Vec3f>(nY,nX)[2] << std::endl;
      }
    }

    cLastFrame = cFrame2.clone();

    //ushort intensity = cFrame2.at<ushort>(200, 200);

    // std::cout << "x: "  << g_nMouseX << " y: " << g_nMouseY << " Intensity:" << intensity << std::endl;

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
        g_fYData[nPos] = (unsigned short) cFrame2.at<ushort>(g_nMouseY, g_nMouseX);
      }
      else {
        nPos = g_nNumDataPoint;
        g_fXData[nPos] = g_nNumDataPoint;
        g_fYData[nPos] = (unsigned short) cFrame2.at<ushort>(g_nMouseY, g_nMouseX);
      }

      graph.SetPoint(nPos, g_fXData[nPos], g_fYData[nPos]);
      g_nNumDataPoint++;
    }

    graph.Draw("APL");
    canvas.Update();

    int nRegenerationStep = 1000;

    cData2ToPlot = cData2;

    for (uint16_t nX = 0; nX < m_nImageCols; nX++) {
      for (uint16_t nY = 0; nY < m_nImageRows; nY++) {
        cDataToPlot.at<Vec3f>(nY,nX)[0] = cData2.at<Vec3f>(nY,nX)[2];//cData.at<Vec3f>(nY,nX)[0] / g_nMaxIntensity; // min
        cDataToPlot.at<Vec3f>(nY,nX)[1] = 0;//cData.at<Vec3f>(nY,nX)[1] / g_nMaxIntensity; // max
        cDataToPlot.at<Vec3f>(nY,nX)[2] = cData.at<Vec3f>(nY,nX)[2] / g_nMaxIntensity; // amplitude

        // cData2ToPlot.at<Vec3f>(nY,nX)[0] /= fMaxDiffSqr;
        cData2ToPlot.at<Vec3f>(nY,nX)[1] = 0;
        // cData2ToPlot.at<Vec3f>(nY,nX)[2] /= 5;

        // Bring data down to min
        if (cData.at<Vec3f>(nY,nX)[0] < (65535 - nRegenerationStep)) {
          cData.at<Vec3f>(nY,nX)[0] += nRegenerationStep;
        }

        // Take data up to max
        if (cData.at<Vec3f>(nY,nX)[1] > nRegenerationStep ) {
          cData.at<Vec3f>(nY,nX)[1] -= nRegenerationStep;
        }
        cData.at<Vec3f>(nY,nX)[2] = 65535- (cData.at<Vec3f>(nY,nX)[1] - cData.at<Vec3f>(nY,nX)[0]);
      }
    }


}