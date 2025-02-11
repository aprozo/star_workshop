#include <TFile.h>
#include <TH1F.h>
#include <iostream>


void DoRooFit(TH1 *hist);

double polynom(double x){
    return -2*x + 0.3*x*x;
}

void segmentation_valoaition(){

    // Open a ROOT file for writing
    TFile* fOut = new TFile("output.root", "RECREATE"); // back-up root file

    TCanvas *canvas = new TCanvas("canvas0", "Histogram with Error Bars", 800, 600);
    // Create a histogram

    //#define LOCAL

    #if defined(LOCAL)
        TH1F hist("hist", "Histogram with Statistical Error Bars;X-axis;Y-axis", 50, 0, 50);
    #else
        TH1F *hist = new TH1F("hist", "Histogram with Statistical Error Bars;X-axis;Y-axis", 50, 0, 50);
    #endif

    // Fill the histogram with random data
    TRandom randGen;
    for (int i = 0; i < 100000; ++i) {

        double signal = randGen.Gaus(20, 3);
        double bcg = polynom(randGen.Uniform(0,50));
        #if defined(LOCAL)
            hist.Fill(signal);
            hist.Fill(bcg);
        #else
            hist->Fill(signal);
            hist->Fill(bcg);
        #endif
    }
    #if defined(LOCAL)
        hist.Draw("E");
    #else
        hist->Draw("E");
    #endif

    //DoRooFit(hist);
/*
    canvas->Write("Histogram");
    canvas->Close();

    TCanvas *c = (TCanvas*) fOut->Get("Histogram");

    if (!c) {
        std::cerr << "Error: Could not find canvas in file!" << std::endl;
        return;
    }

    c->Draw();
    c->Close();

    // Close the file
*/
    //delete hist; // here it will delete hist from the root file 

    canvas->Write("Histogram");
}



void DoRooFit(TH1 *hist)
{
    // Declare observable x
    RooRealVar x("x","var []",0,50) ;
    x.setRange("fitRange", 0,50);
    // Turn off RooFit info messages
    RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);

    // Create a binned dataset that imports contents of TH1 and associates its contents to observable 'x'
    RooDataHist dh("dh","dh",x,RooFit::Import(*hist));
    RooPlot *frame = x.frame(RooFit::Title("RooFit")) ;
    

    RooRealVar a1("a0","a0",-2,-10.0,0.0);
    RooRealVar a2("a1","a1", 0.3,0.0,0.5);
    RooPolynomial bkg("bkg","bkg",x,RooArgList(a1,a2));


    // define function to fit peak in data|
    RooRealVar sigmean("s1","s1",20,0,50);
    RooRealVar sigwidth("s2","s2",3.0,0.0,10.0);
    RooGaussian sig{"sig", "sig PDF", x, sigmean, sigwidth};

    //combine function into model
    RooRealVar bkgfrac("bkgfrac","bkgfrac",0.5,0.,1.);
    RooRealVar nsig("nsig","signal events",100000,0,1000000);
    RooRealVar nbkg("nbkg","background events",100000,0,1000000);

    RooAddPdf model("model","model",RooArgList(sig,bkg),RooArgList(nsig,nbkg));
    

    //fit data with model
    model.fitTo(dh, RooFit::Save(true), RooFit::Range("fitRange"), RooFit::PrintLevel(-1));

    //plotting
    dh.plotOn(frame,RooFit::DataError(RooAbsData::SumW2),RooFit::Name("data") ); 
    //model.plotOn(frame, RooFit::Name("model") );

    //model.plotOn(frame, RooFit::Components(bkg),RooFit::LineStyle(kDashed), RooFit::LineColor(kRed), RooFit::Name("background") ); 
    
    frame->Draw("same hist E");
}