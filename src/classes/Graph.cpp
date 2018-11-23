#include "Graph.h"

Graph::Graph() {
  m_nNumPlotPoints = 1000;

  TCanvas       canvas("a", "b", 800, 700, 400, 200);
  TGraph        graph(g_nNumPlotPoints, g_fXData, g_fYData);

  graph.SetTitle("Speckle intensity; pixel intensity [0..255]; num data point");

  graph.SetMarkerStyle(2);
  graph.SetMarkerColor(4);
  graph.SetMarkerSize(0.3);

  graph.SetLineColor(4);
  graph.SetLineWidth(1);

  graph.GetXaxis()->SetNdivisions(5, kTRUE);

  graph.Draw("APL");

  // const int g_nNumPlotPoints = 1000;
// double g_fXData[g_nNumPlotPoints];
// double g_fYData[g_nNumPlotPoints];
// int g_nNumDataPoint;
// bool g_bEraseAllData;

}

Graph::~Graph() {

}

void Graph::ZeroDataPoints() {
  for (int nI = 0; nI < g_nNumPlotPoints; nI++) {
    g_fXData[nI] = nI;
    g_fYData[nI] = 0.0;
  }
}

void Graph::AddDatapoint(double fX, double fY) {
      // g_nNumDataPoint++;
    //if (g_nNumDataPoint > 100) break;
}


    // graph.Draw("APL");
    // canvas.Update();


// int nPos;

//     if (g_bEraseAllData) {
//       for(int nI = 0; nI <= g_nNumPlotPoints - 1; nI++) {
//         graph.SetPoint(nI, nI, 0);
//       }
//       g_nNumDataPoint = 0;
//       g_bEraseAllData = false;
//     }
//     else {
//       if (g_nNumDataPoint >= g_nNumPlotPoints) {
//         for(int nI = 0; nI <= (g_nNumPlotPoints - 2); nI++) {
//           g_fXData[nI] = g_fXData[nI + 1];
//           g_fYData[nI] = g_fYData[nI + 1];
//           graph.SetPoint(nI, g_fXData[nI], g_fYData[nI]);
//         }
//         nPos = g_nNumPlotPoints - 1;
//         g_fXData[nPos] = g_nNumDataPoint;
//         g_fYData[nPos] = (unsigned short) m_InputImage.at<ushort>(g_nMouseY, g_nMouseX);
//       }
//       else {
//         nPos = g_nNumDataPoint;
//         g_fXData[nPos] = g_nNumDataPoint;
//         g_fYData[nPos] = (unsigned short) m_InputImage.at<ushort>(g_nMouseY, g_nMouseX);
//       }

//       graph.SetPoint(nPos, g_fXData[nPos], g_fYData[nPos]);
//       g_nNumDataPoint++;
//     }