#include <TApplication.h>
#include "TCanvas.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include "TArrow.h"
#include "TLatex.h"

int main(int argc, char* argv[]) {

  TApplication app ("app", &argc, argv);

  auto mycanvas = new TCanvas();

  const int n_points=10;
  double x_vals[n_points]= {1,2,3,4,5,6,7,8,9,10};
  double y_vals[n_points]= {6,12,14,20,22,24,35,45,44,53};

  // Instance of the graph
  TGraph graph(n_points,x_vals,y_vals);

  graph.SetTitle("Measurement XYZ;lenght [cm];Arb.Units");

  // Make the plot estetically better
  graph.SetMarkerStyle(kOpenCircle);
  graph.SetMarkerColor(kBlue);
  graph.SetLineColor(kBlue);

  // The canvas on which we'll draw the graph
  
  // Draw the graph !
  graph.DrawClone("APE");

  mycanvas->Update();
}

