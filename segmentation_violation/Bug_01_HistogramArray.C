#include "TH1F.h"
#include <iostream>
#include <string>

void Bug_01_HistogramArray() {

  // Create an array of histograms
  TH1F *hist[2];

  // Create new histograms
  hist[0] = new TH1F("hist1", "hist1", 100, 0, 100);
  hist[1] = new TH1F("hist2", "hist2", 100, 0, 100);

  // Try to access the third histogram
  hist[2]->Draw();
}