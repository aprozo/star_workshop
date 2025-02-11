#include "TCanvas.h"
#include "TF1.h"
#include <iostream>
#include <sstream>
using namespace std;

void drawGaussianFromFile(TString fileName)
{
    TFile *file = new TFile(fileName, "READ");
    // Create a canvas to draw on
    TCanvas *canvas = new TCanvas("canvas", "Gaussian Distribution", 800, 600);

    // Define a Gaussian function with mean=0 and sigma=1
    TF1 *gaussian = (TF1 *)file->Get("gaussian");
    gaussian->SetTitle("");

    // Draw the Gaussian function
    gaussian->Draw();

    // Save the canvas as a PNG file
    canvas->SaveAs("gaussianFromFile.png");

    // Cleanup
    delete canvas;
    delete gaussian;
}
