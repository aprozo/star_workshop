#include <TFile.h>
#include <TCanvas.h>
#include <TDirectory.h>

void saveCanvasObjects(TDirectory *dir, const TString& figsDir) {
    // Loop over all keys in the current directory
    TIter next(dir->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)next())) {
        TObject *obj = key->ReadObj();
        if (!obj) continue;

        // Check if the object is a directory
        if (obj->InheritsFrom("TDirectory")) {
            // Recursively call this function for subdirectories
            TDirectory *subDir = (TDirectory*)obj;
            saveCanvasObjects(subDir, figsDir);
        }

        // Check if the object is a canvas
        if (obj->InheritsFrom("TCanvas")) {
            TCanvas *canvas = (TCanvas*)obj;

            // Get the directory path relative to the top-level directory
            TString dirPath = dir->GetPath();
            //dirPath.ReplaceAll(":/", ""); // Remove the initial ":/" from the path
            Int_t pos = dirPath.Index(":/");
            if (pos >= 0) {
                dirPath = dirPath(pos + 2, dirPath.Length() - pos - 2);
            }

            // Create the corresponding directory in Figs if it doesn't exist
            TString figsSubDir = figsDir + "/" + dirPath;
            gSystem->mkdir(figsSubDir, kTRUE);

            // Extract the canvas name
            TString canvasName = canvas->GetName();
            // Remove special characters from the canvas name to form a valid file name
            canvasName.ReplaceAll(" ", "_");
            canvasName.ReplaceAll("/", "_");
            canvasName.ReplaceAll(":", "_");

            // Save the canvas as a PDF file in the Figs directory
            TString filePath = figsSubDir + "/" + canvasName + ".pdf";
            canvas->SaveAs(filePath);
            //std::cout << "Saved canvas '" << canvasName << "' to " << filePath << std::endl;
        }
    }
}

// FinalPlots
//void savePlots(const char* rootFileName = "vertex1810.root") {
//void savePlots(const char* rootFileName = "Resolution.root") {
void savePlots(const char* rootFileName = "FinalPlots.root") {
    gStyle->SetLineScalePS(1);
    // Open the ROOT file
    TFile *file = TFile::Open(rootFileName);
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Unable to open ROOT file " << rootFileName << std::endl;
        return;
    }

    // Create a directory to save figures if it doesn't exist
    //TString figsDir = "Resolution";
    TString figsDir = "Figs";
    gSystem->mkdir(figsDir, kTRUE);

    // Start from the top-level directory
    saveCanvasObjects(file, figsDir);

    // Close the ROOT file
    file->Close();
}


