#ifndef Ana_h
#define Ana_h

// c++ headers
#include <iostream>
#include <string>    
#include <utility>
#include <sstream> 
#include <algorithm> 
#include <stdio.h> 
#include <stdlib.h> 
#include <vector> 
#include <fstream> 
#include <cmath> 
#include <cstdlib>

// ROOT headers
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include <TH2.h> 
#include <TF1.h> 
#include <TF2.h> 
#include <THStack.h> 
#include <TStyle.h> 
#include <TGraph.h> 
#include <TGraph2D.h> 
#include <TGraphErrors.h> 
#include <TCanvas.h> 
#include <TLegend.h> 
#include <TGaxis.h> 
#include <TString.h> 
#include <TColor.h> 
#include <TLine.h> 
#include <TExec.h> 
#include <TFitResultPtr.h> 
#include <TFitResult.h> 
#include <TLatex.h> 
#include <TMath.h>
#include <TLorentzVector.h>

// picoDst headers
#include "StRPEvent.h"
#include "StUPCRpsTrack.h"
#include "StUPCRpsTrackPoint.h"
#include "StUPCEvent.h"
#include "StUPCTrack.h"
#include "StUPCBemcCluster.h"
#include "StUPCVertex.h"
#include "StUPCTofHit.h"



using namespace std;

// enums are very usefull 
enum { kAll = 1, kCPT, kOneVertex, kTPCTOF, kTotQ, kMax};
enum { kElectron = 0, kPion, kKaon, kProton, nParticles};
enum { Plus, Minus, nHadrons};

TString summaryLabels[] = { TString("All"), TString("CPT"), TString("1 vertex"), TString("2 TOF"), 
		TString("TotQ=0")};
TString particleLable[nParticles] = { TString("Electron"), TString("Pion"), TString("Kaon"), TString("Proton")};

const double particleMass[nParticles] = { 0.000510998, 0.13957, 0.49367, 0.93827}; // in GeV /c^2 
const int nTriggers = 17;
const int triggerID[] = { 570209, 570219, 570229, 570701, 570702, 570703, 570704, 570705, 
                  570709, 570711, 570712, 570719, 590701, 590703, 590705, 590708, 590709};
// 570702 RP_UPC // 570712 RP_UPC // 570703 RP_SDT // 570709 RP_ET // 570719 RP_ET // 570701 RP_CPT2 // 570711 RP_CPT2 // 570705 RP_CPT2noBBCL // 570704 RP_Zerobias // 590703 RP_SDT // 590709 RP_ET // 590701 RP_CPT2 // 590705 RP_CPT2noBBCL // 590708 RP_CPTnoBBCL // 570209 JPsi*HTTP // 570219 JPsi*HTTP // 570229 JPsi*HTTP
const int CEPtriggers[] = { 570701, 570705, 570711, 590701, 590705, 590708};

TFile *infile, *outfile;
TChain *upcChain, *rpChain;
StUPCEvent *upcEvt;
TTree *upcTree, *recTree, *bcgTree;

// control plots
TH1I *hTriggerBits, *hAnalysisFlow; 

// event info
UInt_t runNumber;

// Vertex info
Double_t invMassInGev;
 
// Reco state info
Double_t vertexZInCm;
 
// Central hadrons info
Double_t dEdxInKevCm[2], pTInGev[2], tofTimeInNs[2], tofLengthInCm[2], charge[2], nSigmaTPC[2][nParticles]; 
Double_t dcaXYInCm[2], dcaZInCm[2], nHitsFit[2], nHitsDEdx[2], eta[2], phi[2]; 

void Make();
void Init();
bool ConnectInput(int argc, char** argv);
TFile *CreateOutputTree(const string& out);
bool CheckTriggers();
bool IsGoodTrack(const StUPCTrack *trk);
void SaveTrackInfo(const StUPCTrack *trk, unsigned int iTrack);

bool ConnectInput(int argc, char** argv) 
{
    int fileId = -1;
    upcTree = 0x0;

    if( argc == 2 || argc == 3)
    {
        const string& input = argv[1];
        if(input.find(".root") != string::npos && argc == 2)
        {
            cout << "Input from root file: "<< input << endl;
            infile = TFile::Open(input.c_str(), "read");
            if(!infile)
            {
                cout<< "Couldn't open input root file..."<<endl;
                return false;
            } 
            upcTree = dynamic_cast<TTree*>( infile->Get("mUPCTree") );
        }else if(input.find(".list") != string::npos )
        {
            cout << "Input from chain" << endl;
            upcChain = new TChain("mUPCTree");
            ifstream instr(input.c_str());
            if (!instr.is_open())
            {
                cout<< "Couldn't open: "<<input.c_str()<<endl;
                return false;
            }

            string line;
            int lineId=0;
            if(argc == 3)
                fileId = atoi(argv[2]);

            while(getline(instr, line)) 
            {
                if(fileId==lineId || fileId== -1)
                {
                    upcChain->AddFile(line.c_str());
                    infile = TFile::Open(line.c_str(), "read");
                    if(!infile)
                    {
                        cout<< "Couldn't open: "<<line.c_str()<<endl;
                        return false;
                    } 
                }
                lineId++;
            }
            instr.close();
            upcTree = dynamic_cast<TTree*>( upcChain );
        }
    }
    
    if(!upcTree) 
        return false;

    upcTree->SetBranchAddress("mUPCEvent", &upcEvt);

    return true;
}//ConnectInput


//_____________________________________________________________________________
TFile *CreateOutputTree(const string& out) {

    TFile *outputFile = TFile::Open(out.c_str(), "recreate");
    if(!outputFile) 
        return 0x0;

    //standard reconstructed tree
    recTree = new TTree("recTree", "recTree");

    // event info
    recTree->Branch("runNumber", &runNumber, "runNumber/i");

    // Vertex info
    recTree->Branch("vertexZInCm", &vertexZInCm, "vertexZInCm/D");

    // Reco state info
    recTree->Branch("invMassInGev", &invMassInGev, "invMassInGev/D");

    // Central hadrons info
    for (int i = 0; i < nHadrons; ++i)
    {
        recTree->Branch(Form("dEdxInKevCm%i",i), &dEdxInKevCm[i], Form("dEdxInKevCm%i/D",i));
        recTree->Branch(Form("pTInGev%i",i), &pTInGev[i], Form("pTInGev%i/D",i));
        recTree->Branch(Form("charge%i",i), &charge[i], Form("charge%i/D",i));
        recTree->Branch(Form("tofTimeInNsInNs%i",i), &tofTimeInNs[i], Form("tofTimeInNs%i/D",i));
        recTree->Branch(Form("tofLengthInCm%i",i), &tofLengthInCm[i], Form("tofLengthInCm%i/D",i));
        recTree->Branch(Form("dcaXYInCm%i",i), &dcaXYInCm[i], Form("dcaXYInCm%i/D",i));
        recTree->Branch(Form("dcaZInCm%i",i), &dcaZInCm[i], Form("dcaZInCm%i/D",i));
        recTree->Branch(Form("nHitsFit%i",i), &nHitsFit[i], Form("nHitsFit%i/D",i));
        recTree->Branch(Form("nHitsDEdx%i",i), &nHitsDEdx[i], Form("nHitsDEdx%i/D",i));
        recTree->Branch(Form("eta%i",i), &eta[i], Form("eta%i/D",i));
        recTree->Branch(Form("phi%i",i), &phi[i], Form("phi%i/D",i));
        for (int iPart = 0; iPart < nParticles; ++iPart)
            recTree->Branch(Form("nSigmaTPC%i_",i) + particleLable[iPart], &nSigmaTPC[i][iPart], Form("nSigmaTPC%i_",i) + particleLable[iPart] + "/D");
    }


    // Setting background Tree
    bcgTree = recTree->CloneTree(0);
    bcgTree->SetName("Background");

    return outputFile;

}//CreateOutputTree

bool CheckTriggers()
{
	bool trigger = false;
	for(int var = 0; var < nTriggers; ++var)
	{
		if(upcEvt->isTrigger(triggerID[var]))
		{
			hTriggerBits->Fill(var);
			//Checked if it is CPT trigger
			for (int i = 0; i < *(&CEPtriggers + 1) - CEPtriggers; ++i)
				if(triggerID[var] == CEPtriggers[i])
					trigger=true;
		}
	}

	return trigger;

}

#endif