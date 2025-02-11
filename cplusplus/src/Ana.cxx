// Run by: ./Ana file.list
// e.g. ./Ana /star/u/truhlar/star-upcDst/build/run17.list
// or you can open just root file
// ./Ana /star/data01/pwg_tasks/upc02/UPC_P20ic/18061097.root
// or you can open n-th root file in file.list
// ./Ana file.list index 


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

//_____________________________________________________________________________
int main(int argc, char** argv) 
{
	//connect input file(s)
	if(!ConnectInput(argc, argv))
	{
		cout << "Wrong input parameters..." << endl; 
		return 1;
	}
	//open output file
	outfile = CreateOutputTree("AnalysisOutput.root"); 
	if(!outfile) 
	{
		cout << "Can not open output file." << endl; 
		return -1;
	}

	// Initiate histograms
	Init();

	//ask for number of events
	Long64_t nev = upcTree->GetEntries();
	nev = 10000; // use for debugging and testing
	cout<<"Proccesing "<<nev<<" events"<<endl;

	//event loop
	for(Long64_t iev=0; iev<nev; ++iev) 
	{ //get the event
		upcTree->GetEntry(iev); 
		Make();
	} 

	//close the outputs
	outfile->Write();
	outfile->Close();
	cout<<"Ending Analysis... GOOD BYE!"<<endl;
	return 0;
}//main

void Make()
{
	hAnalysisFlow->Fill(kAll);
	// Require CPT trigger
	if(CheckTriggers()){
		if( upcEvt->getNumberOfVertices() == 1){
			hAnalysisFlow->Fill(kCPT);

			// Require events with 1 vertex
			hAnalysisFlow->Fill(kOneVertex);
		
			vertexZInCm = upcEvt->getVertex(0)->getPosZ();

			// get run number
			runNumber = upcEvt->getRunNumber();
			int totalCharge = 0;
			int nTOFtracks = 0;
			vector<int> hadronId;

			for(int trackID = 0; trackID < upcEvt->getNumberOfTracks(); ++trackID){
				const StUPCTrack* trk = upcEvt->getTrack(trackID);
				// Require all tracks to be primary and have matched with TOF
				// In general, you want to skip all bad tracks
				if( trk->getFlag(StUPCTrack::kPrimary) && trk->getFlag(StUPCTrack::kTof)) 
				{
					nTOFtracks++;

					hadronId.push_back(trackID);
					totalCharge += static_cast<int>( trk->getCharge() );
				}
			} 
			// Require events with at least 2 TOF tracks (looking for K^0_S -> pi+ pi-)
			// We are going to modify it to exactly 2 TOF tracks
			if(nTOFtracks >= nHadrons){
				hAnalysisFlow->Fill(kTPCTOF);
				TLorentzVector state;
				TLorentzVector hadron;
				for (unsigned int id = 0; id < hadronId.size(); ++id)
				{
					const StUPCTrack* trk = upcEvt->getTrack(hadronId[id]);
					trk->getLorentzVector(hadron, particleMass[kPion]); // looking for K^0_S -> pi+ pi-
					state += hadron;

					dEdxInKevCm[id] = trk->getDEdxSignal()*1000000; // convert to KeV/cm
					pTInGev[id] = trk->getPt();
					charge[id] = trk->getCharge();
					tofTimeInNs[id] = trk->getTofTime();
					tofLengthInCm[id] = trk->getTofPathLength();
					dcaXYInCm[id] = trk->getDcaXY();
					dcaZInCm[id] = trk->getDcaZ();
					nHitsFit[id] = trk->getNhitsFit();
					nHitsDEdx[id] = trk->getNhitsDEdx();
					eta[id] = trk->getEta();
					phi[id] = trk->getPhi();
					nSigmaTPC[id][kElectron] = trk->getNSigmasTPC(StUPCTrack::kElectron);
					nSigmaTPC[id][kPion] = trk->getNSigmasTPC(StUPCTrack::kPion);
					nSigmaTPC[id][kKaon] = trk->getNSigmasTPC(StUPCTrack::kKaon);
					nSigmaTPC[id][kProton] = trk->getNSigmasTPC(StUPCTrack::kProton);
				}


				invMassInGev = state.M();

				if(totalCharge) // Total charge is not zero => background event
				{
					bcgTree->Fill(); // Fill background Tree
				}else{
					hAnalysisFlow->Fill(kTotQ);
					recTree->Fill(); // Fill analysis (reco) Tree
				}
			}
		}
	}
}

void Init()
{

	hAnalysisFlow = new TH1I("AnalysisFlow", "CutsFlow", kMax-1, 1, kMax);

	for(int tb=1; tb<kMax; ++tb) 
		hAnalysisFlow->GetXaxis()->SetBinLabel(tb, summaryLabels[tb-1]);

	hTriggerBits = new TH1I("TriggerBits", "TriggerBits", nTriggers, -0.5, 16.5);
	for(int tb=0; tb<nTriggers; ++tb){
		TString label; label.Form("%d",triggerID[tb]);
		hTriggerBits->GetXaxis()->SetBinLabel(tb+1, label);
	}

	outfile->cd();
}

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
