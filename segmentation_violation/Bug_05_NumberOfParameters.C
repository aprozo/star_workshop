#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TROOT.h"
#include "TStyle.h"

double my_func(Double_t *x, Double_t *par) {
  double result = par[0] * x[0] * x[0] + par[1] * x[0] + par[2];
  return result;
}

void Bug_05_NumberOfParameters() {
  double par[3] = {1, 0, 1};
  TF1 *f1 = new TF1("f1", my_func, -2, 2, 2);
  f1->SetParameters(par);
  f1->SetParNames("a", "b", "c");
  f1->Draw();
  gPad->SetGrid();
}