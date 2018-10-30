#ifndef GRAPH_CPP_
  #define GRAPH_CPP_

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

  #include "Base.h"

  class Graph : public Base {

    public:
      Graph();
      ~Graph();

      void ZeroDataPoints();

    private:
      const  int m_nNumPlotPoints;
      double m_fXData[g_nNumPlotPoints];
      double m_fYData[g_nNumPlotPoints];
      int    m_nNumDataPoint;
      bool   m_bEraseAllData;

  };

#endif