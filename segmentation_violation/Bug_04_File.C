#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TRandom.h>
#include <chrono>
#include <iostream>
#include <thread>

void Bug_04_File() {

  // Make a file
  TFile *file = TFile::Open("05_file_test.root", "RECREATE");

  // Make a histogram
  TH1F *hist = new TH1F("hist", "hist", 100, 0, 100);

  // Fill the histogram
  for (int i = 0; i < 100; i++) {
    // gauss
    hist->Fill(gRandom->Gaus(50, 10));
  }

  file->Close();

  hist->Draw();
}

////hist->SetDirectory(0);