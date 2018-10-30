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
}

Graph::~Graph() {

}

void Graph::ZeroDataPoints() {
  for (int nI = 0; nI < g_nNumPlotPoints; nI++) {
    g_fXData[nI] = nI;
    g_fYData[nI] = 0.0;
  }
}