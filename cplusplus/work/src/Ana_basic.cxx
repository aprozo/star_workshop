// Run by: ./Ana file.list
// e.g. ./Ana /star/u/truhlar/star-upcDst/build/run17.list
// or you can open just root file
// ./Ana /star/data01/pwg_tasks/upc02/UPC_P20ic/18061097.root
// or you can open n-th root file in file.list
// ./Ana file.list index 

#include "Ana_basic.h"

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
    //nev = 10000; // use for debugging and testing
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
    if(!CheckTriggers())
        return;

    hAnalysisFlow->Fill(kCPT);

    // Skip all events with more vertecies than 1
    if( upcEvt->getNumberOfVertices() != 1) return;
    hAnalysisFlow->Fill(kOneVertex);

    vertexZInCm = upcEvt->getVertex(0)->getPosZ();

    // get run number
    runNumber = upcEvt->getRunNumber();

    int totalCharge = 0;
    vector<int> hadronId;

    for(int trackID = 0; trackID < upcEvt->getNumberOfTracks(); ++trackID)
    {
        const StUPCTrack* trk = upcEvt->getTrack(trackID);
        // Skip all tracks that are not primary or they are not matched with TOF
        // In general, you want to skip all bad tracks
        if( !trk->getFlag(StUPCTrack::kPrimary) || !trk->getFlag(StUPCTrack::kTof) || !IsGoodTrack(trk)) 
            continue;

        hadronId.push_back(trackID);
        totalCharge += static_cast<int>( trk->getCharge() );
    } 

    // Skip events with less than 2 TOF tracks
    if(hadronId.size() != nHadrons)
        return;

    TLorentzVector state;
    TLorentzVector hadron;
    for (unsigned int id = 0; id < hadronId.size(); ++id)
    {
        const StUPCTrack* trk = upcEvt->getTrack(hadronId[id]);
        trk->getLorentzVector(hadron, particleMass[kPion]); // looking for K^0_S -> pi+ pi-
        state += hadron;
        SaveTrackInfo(trk, id);
    }

    hAnalysisFlow->Fill(kTPCTOF);

    invMassInGev = state.M();

    if(totalCharge) // Total charge is not zero => background event
    {
        bcgTree->Fill(); // Fill background Tree
    }else{
        hAnalysisFlow->Fill(kTotQ);
        recTree->Fill(); // Fill analysis (reco) Tree
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


bool IsGoodTrack(const StUPCTrack *trk)
{
    if(trk->getDcaXY() < 1.5 && trk->getDcaZ() < 1 && trk->getDcaZ() > -1 
    && trk->getNhitsFit() > 25 && trk->getNhitsDEdx() > 15 && trk->getPt() > 0.2 
    && trk->getTofTime() > 0 && trk->getTofPathLength() > 0)
        return true;
    return false;
}

void SaveTrackInfo(const StUPCTrack *trk, unsigned int iTrack)
{
    dEdxInKevCm[iTrack] = trk->getDEdxSignal()*1000000;
    pTInGev[iTrack] = trk->getPt();
    charge[iTrack] = trk->getCharge();
    tofTimeInNs[iTrack] = trk->getTofTime();
    tofLengthInCm[iTrack] = trk->getTofPathLength();
    dcaXYInCm[iTrack] = trk->getDcaXY();
    dcaZInCm[iTrack] = trk->getDcaZ();
    nHitsFit[iTrack] = trk->getNhitsFit();
    nHitsDEdx[iTrack] = trk->getNhitsDEdx();
    eta[iTrack] = trk->getEta();
    phi[iTrack] = trk->getPhi();
    nSigmaTPC[iTrack][kElectron] = trk->getNSigmasTPC(StUPCTrack::kElectron);
    nSigmaTPC[iTrack][kPion] = trk->getNSigmasTPC(StUPCTrack::kPion);
    nSigmaTPC[iTrack][kKaon] = trk->getNSigmasTPC(StUPCTrack::kKaon);
    nSigmaTPC[iTrack][kProton] = trk->getNSigmasTPC(StUPCTrack::kProton);
}