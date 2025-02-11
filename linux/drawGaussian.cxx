#include "TCanvas.h"
#include "TF1.h"
#include <iostream>
#include <sstream>
using namespace std;

void drawGaussian()
{
    // Create a canvas to draw on
    TCanvas *canvas = new TCanvas("canvas", "Gaussian Distribution", 800, 600);

    // Define a Gaussian function with mean=0 and sigma=1
    TF1 *gaussian = new TF1("gaussian", "gaus(0)", -2, 2); // Gaussian function: mean=0, sigma=0.7, range [-2, 2]
    gaussian->SetParameters(1, 0, 0.5);                      // Set parameters: amplitude, mean, sigma
    gaussian->SetTitle("");

    // Draw the Gaussian function
    gaussian->Draw();

    // Save the canvas as a PNG file
    canvas->SaveAs("gaussian.png");

    // Cleanup
    delete canvas;
    delete gaussian;
}
