/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

//-----------------------------------------------------------------------
// This class compares the global reconstruction with the TPConly 
// reconstruction
// Momentum resolution is stored as function of track cuts and pt.
// Output: Histograms for different set of cuts
//-----------------------------------------------------------------------
// Author : Marta Verweij - UU
//-----------------------------------------------------------------------

#ifndef ALIPWG4HIGHPTQATPCONLY_CXX
#define ALIPWG4HIGHPTQATPCONLY_CXX

#include "AliPWG4HighPtQATPConly.h"

#include "TVector3.h"
#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TList.h"
#include "TChain.h"
#include "TH3F.h"
#include <Bytes.h>
#include <TTree.h>

#include "AliAnalysisManager.h"
#include "AliESDInputHandler.h"
#include "AliESDtrack.h"
#include "AliESDfriend.h"
#include "AliESDfriendTrack.h"
#include "AliESDtrackCuts.h"
#include "AliExternalTrackParam.h"
#include "AliLog.h"
//#include "AliAnalysisHelperJetTasks.h"

#include "AliStack.h"
#include "TParticle.h"
#include "TH1I.h"
#include "AliMCEvent.h"
#include "AliMCEventHandler.h"

using namespace std; //required for resolving the 'cout' symbol

ClassImp(AliPWG4HighPtQATPConly)

AliPWG4HighPtQATPConly::AliPWG4HighPtQATPConly(): AliAnalysisTask("AliPWG4HighPtQATPConly", ""), 
  fESD(0), 
  fESDfriend(0), 
  fMC(0),
  fTrackCuts(0), 
  fTrackCutsITS(0),
  fTrigger(0),
  fNEventAll(0),
  fNEventSel(0),
  fPtAll(0),
  fPtSel(0),
  fPtAllminPtTPCvsPtAll(0),
  fPtAllminPtTPCvsPtAllNPointTPC(0),
  fPtAllminPtTPCvsPtAllDCAR(0),
  fPtAllminPtTPCvsPtAllDCAZ(0),
  fPtAllminPtTPCvsPtAllPhi(0),
  fPtAllminPtTPCvsPtAllNPointITS(0),
  fPtAllminPtTPCvsPtAllNSigmaToVertex(0),
  fPtAllminPtTPCvsPtAllChi2C(0),
  fPtAllminPtTPCvsPtAllRel1PtUncertainty(0),
  fPtAllminPtTPCvsPtAllChi2PerNClusTPC(0),
  fPtAllminPtTPCvsPtAllChi2PerNClusITS(0),
  fEtaPhiOutliers(0),
  fPtSelITSouter(0),
  fPtITSouterminPtTPCvsPtAll(0),
  fPtITSouterminPtTPCvsPtAllNPointTPC(0),
  fPtITSouterminPtTPCvsPtAllDCAR(0),
  fPtITSouterminPtTPCvsPtAllDCAZ(0),
  fPtITSouterminPtTPCvsPtAllPhi(0),
  fPtITSouterminPtTPCvsPtAllNPointITS(0),
  fPtITSouterminPtTPCvsPtAllNSigmaToVertex(0),
  fPtITSouterminPtTPCvsPtAllChi2C(0),
  fPtITSouterminPtTPCvsPtAllRel1PtUncertainty(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusTPC(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS(0),
  fPtITSouterminPtTPCvsPtAll_ITSLayer0(0),
  fPtITSouterminPtTPCvsPtAll_ITSLayer1(0),
  fPtITSouterminPtTPCvsPtAll_ITSLayer2(0),
  fPtITSouterminPtTPCvsPtAll_ITSLayer3(0),
  fPtITSouterminPtTPCvsPtAll_ITSLayer4(0),
  fPtITSouterminPtTPCvsPtAll_ITSLayer5(0),
  fPtITSouterminPtTPCvsPtAll_NoSPD(0),
  fPtITSouterminPtTPCvsPtAll_NoSDD(0),
  fPtITSouterminPtTPCvsPtAll_NoSSD(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer0(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer1(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer2(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer3(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer4(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer5(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSPD(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSDD(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSSD(0),
  fHistList(0),
  fPtAllTPC(0),
  fPtSelTPC(0),
  fPtSelTPCITS(0),
  fHistListTPC(0),
  fPtSelITS(0),
  fPtITSminPtTPCvsPtITS(0),
  fPtITSminPtTPCvsPtITSNPointTPC(0),
  fPtITSminPtTPCvsPtITSDCAR(0),
  fPtITSminPtTPCvsPtITSDCAZ(0),
  fPtITSminPtTPCvsPtITSPhi(0),
  fPtITSminPtTPCvsPtITSNPointITS(0),
  fPtITSminPtTPCvsPtITSNSigmaToVertex(0),
  fPtITSminPtTPCvsPtITSChi2C(0),
  fPtITSminPtTPCvsPtITSRel1PtUncertainty(0),
  fPtITSminPtTPCvsPtITSChi2PerNClusTPC(0),
  fPtITSminPtTPCvsPtITSChi2PerNClusITS(0),
  fPtOuterPtInner(0),
  fPtRel1PtUncertaintyChi2PerClusTPC(0),
  fHistListITS(0)
{

}
//________________________________________________________________________
AliPWG4HighPtQATPConly::AliPWG4HighPtQATPConly(const char *name): 
  AliAnalysisTask(name, ""), 
  fESD(0),
  fESDfriend(0), 
  fMC(0),
  fTrackCuts(),
  fTrackCutsITS(),
  fTrigger(0),
  fNEventAll(0),
  fNEventSel(0),
  fPtAll(0),
  fPtSel(0),
  fPtAllminPtTPCvsPtAll(0),
  fPtAllminPtTPCvsPtAllNPointTPC(0),
  fPtAllminPtTPCvsPtAllDCAR(0),
  fPtAllminPtTPCvsPtAllDCAZ(0),
  fPtAllminPtTPCvsPtAllPhi(0),
  fPtAllminPtTPCvsPtAllNPointITS(0),
  fPtAllminPtTPCvsPtAllNSigmaToVertex(0),
  fPtAllminPtTPCvsPtAllChi2C(0),
  fPtAllminPtTPCvsPtAllRel1PtUncertainty(0),
  fPtAllminPtTPCvsPtAllChi2PerNClusTPC(0),
  fPtAllminPtTPCvsPtAllChi2PerNClusITS(0),
  fEtaPhiOutliers(0),
  fPtSelITSouter(0),
  fPtITSouterminPtTPCvsPtAll(0),
  fPtITSouterminPtTPCvsPtAllNPointTPC(0),
  fPtITSouterminPtTPCvsPtAllDCAR(0),
  fPtITSouterminPtTPCvsPtAllDCAZ(0),
  fPtITSouterminPtTPCvsPtAllPhi(0),
  fPtITSouterminPtTPCvsPtAllNPointITS(0),
  fPtITSouterminPtTPCvsPtAllNSigmaToVertex(0),
  fPtITSouterminPtTPCvsPtAllChi2C(0),
  fPtITSouterminPtTPCvsPtAllRel1PtUncertainty(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusTPC(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS(0),
  fPtITSouterminPtTPCvsPtAll_ITSLayer0(0),
  fPtITSouterminPtTPCvsPtAll_ITSLayer1(0),
  fPtITSouterminPtTPCvsPtAll_ITSLayer2(0),
  fPtITSouterminPtTPCvsPtAll_ITSLayer3(0),
  fPtITSouterminPtTPCvsPtAll_ITSLayer4(0),
  fPtITSouterminPtTPCvsPtAll_ITSLayer5(0),
  fPtITSouterminPtTPCvsPtAll_NoSPD(0),
  fPtITSouterminPtTPCvsPtAll_NoSDD(0),
  fPtITSouterminPtTPCvsPtAll_NoSSD(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer0(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer1(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer2(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer3(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer4(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer5(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSPD(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSDD(0),
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSSD(0),
  fHistList(0),
  fPtAllTPC(0),
  fPtSelTPC(0),
  fPtSelTPCITS(0),
  fHistListTPC(0),
  fPtSelITS(0),
  fPtITSminPtTPCvsPtITS(0),
  fPtITSminPtTPCvsPtITSNPointTPC(0),
  fPtITSminPtTPCvsPtITSDCAR(0),
  fPtITSminPtTPCvsPtITSDCAZ(0),
  fPtITSminPtTPCvsPtITSPhi(0),
  fPtITSminPtTPCvsPtITSNPointITS(0),
  fPtITSminPtTPCvsPtITSNSigmaToVertex(0),
  fPtITSminPtTPCvsPtITSChi2C(0),
  fPtITSminPtTPCvsPtITSRel1PtUncertainty(0),
  fPtITSminPtTPCvsPtITSChi2PerNClusTPC(0),
  fPtITSminPtTPCvsPtITSChi2PerNClusITS(0),
  fPtOuterPtInner(0),
  fPtRel1PtUncertaintyChi2PerClusTPC(0),
  fHistListITS(0)
{
  //
  // Constructor. Initialization of Inputs and Outputs
  //
  Info("AliPWG4HighPtQATPConly","Calling Constructor");
  // Input slot #0 works with a TChain ESD
  DefineInput(0, TChain::Class());
  // Output slot #0 writes into a TList
  DefineOutput(0, TList::Class());
  // Output slot #1 writes into a TList
  DefineOutput(1, TList::Class());
  // Output slot #2 writes into a TList
  DefineOutput(2, TList::Class());
}

//________________________________________________________________________
void AliPWG4HighPtQATPConly::ConnectInputData(Option_t *) 
{
  // Connect ESD here
  // Called once
  AliDebug(2,Form(">> AliPWG4HighPtSpectra::ConnectInputData \n"));
  TTree* tree = dynamic_cast<TTree*> (GetInputData(0));
  if (!tree) {
    AliDebug(2,Form( "ERROR: Could not read chain from input slot 0 \n"));
    return;
  } 
  
  AliESDInputHandler *esdH = dynamic_cast<AliESDInputHandler*> (AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());
  
  if (!esdH) {
    AliDebug(2,Form("ERROR: Could not get ESDInputHandler")); 
    return;
  } else
    fESD = esdH->GetEvent();
  
 AliMCEventHandler *eventHandler = dynamic_cast<AliMCEventHandler*> (AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler());
 // AliMCEventHandler* mcH = dynamic_cast<AliMCEventHandler*>
 //                        (AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler());
  if (!eventHandler) {
    AliDebug(2,Form( "ERROR: Could not retrieve MC event handler \n"));
  }
  else
    fMC = eventHandler->MCEvent();

  //attach the ESD friend
   //  tree->SetBranchStatus("*", kTRUE);
//   tree->SetBranchStatus("Tracks*", kTRUE);
//   tree->SetBranchStatus("ESDfriend*", kTRUE);
  //  fESD->ReadFromTree(tree);

  fESDfriend = (AliESDfriend*)fESD->FindListObject("AliESDfriend");
  if (!fESDfriend)
    {
      // works for both, we just want to avoid setting the branch adress twice
      // in case of the new ESD
      tree->SetBranchAddress("ESDfriend.",&fESDfriend);
    }
  
}

//________________________________________________________________________
void AliPWG4HighPtQATPConly::CreateOutputObjects() {
  //Create output objects
  AliDebug(2,Form(">> AliPWG4HighPtQATPConly::CreateOutputObjects \n")); 

  Bool_t oldStatus = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE); 

  OpenFile(0);
  fHistList = new TList();
  OpenFile(1);
  fHistListTPC = new TList();
  OpenFile(2);
  fHistListITS = new TList();

  Int_t fgkNPhiBins=18;
  Float_t kMinPhi = 0.;
  Float_t kMaxPhi = 2.*TMath::Pi();
  
  Float_t fgkPtMin=0.;
  Float_t fgkPtMax=100.;
  Int_t fgkNPtBins=(int)(fgkPtMax-fgkPtMin);

  Float_t fgkChi2PerClusMin = 0.;
  Float_t fgkChi2PerClusMax = 3.5;
  Int_t fgkChi2PerClusBins = (int)(fgkChi2PerClusMax*10.);


  Int_t fgkResPtBins=80;

  fNEventAll = new TH1F("fNEventAll","NEventAll",1,-0.5,0.5);
  fHistList->Add(fNEventAll);
  fNEventSel = new TH1F("fNEventSel","NEvent Selected for analysis",1,-0.5,0.5);
  fHistList->Add(fNEventSel);
  fPtAll = new TH1F("fPtAll","PtAll",fgkNPtBins, fgkPtMin, fgkPtMax);
  fHistList->Add(fPtAll);
  fPtSel = new TH1F("fPtSel","PtSel",fgkNPtBins, fgkPtMin, fgkPtMax);
  fHistList->Add(fPtSel);
  
  fPtAllminPtTPCvsPtAll = new TH2F("fPtAllminPtTPCvsPtAll","PtAllminPtTPCvsPtAll",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.);
  fPtAllminPtTPCvsPtAll->SetXTitle("p_{t}^{Global}");
  fPtAllminPtTPCvsPtAll->SetYTitle("(1/p_{t}^{Global}-1/p_{t}^{TPC})/(1/p_{t}^{Global})");
  fHistList->Add(fPtAllminPtTPCvsPtAll);
  
  fPtAllminPtTPCvsPtAllNPointTPC = new TH3F("fPtAllminPtTPCvsPtAllNPointTPC","PtAllminPtTPCvsPtAllNPointTPC",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,160,0.5,160.5);
  fPtAllminPtTPCvsPtAllNPointTPC->SetXTitle("p_{t}^{Global}");
  fPtAllminPtTPCvsPtAllNPointTPC->SetYTitle("(1/p_{t}^{Global}-1/p_{t}^{TPC})/(1/p_{t}^{Global})");
  fPtAllminPtTPCvsPtAllNPointTPC->SetZTitle("N_{point,TPC}");
  fHistList->Add(fPtAllminPtTPCvsPtAllNPointTPC);

  fPtAllminPtTPCvsPtAllDCAR = new TH3F("fPtAllminPtTPCvsPtAllDCAR","PtAllminPtTPCvsPtAllDCAR",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,80,-0.2,0.2);
  fPtAllminPtTPCvsPtAllDCAR->SetXTitle("p_{t}^{Global}");
  fPtAllminPtTPCvsPtAllDCAR->SetYTitle("(1/p_{t}^{Global}-1/p_{t}^{TPC})/(1/p_{t}^{Global})");
  fPtAllminPtTPCvsPtAllDCAR->SetZTitle("DCA_{R}");
  fHistList->Add(fPtAllminPtTPCvsPtAllDCAR);

  fPtAllminPtTPCvsPtAllDCAZ = new TH3F("fPtAllminPtTPCvsPtAllDCAZ","PtAllminPtTPCvsPtAllDCAZ",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,80,-2.,2.);
  fPtAllminPtTPCvsPtAllDCAZ->SetXTitle("p_{t}^{Global}");
  fPtAllminPtTPCvsPtAllDCAZ->SetYTitle("(1/p_{t}^{Global}-1/p_{t}^{TPC})/(1/p_{t}^{Global})");
  fPtAllminPtTPCvsPtAllDCAZ->SetZTitle("DCA_{Z}");
  fHistList->Add(fPtAllminPtTPCvsPtAllDCAZ);

  fPtAllminPtTPCvsPtAllPhi = new TH3F("fPtAllminPtTPCvsPtAllPhi","PtAllminPtTPCvsPtAllPhi",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkNPhiBins,kMinPhi,kMaxPhi);
  fPtAllminPtTPCvsPtAllPhi->SetXTitle("p_{t}^{Global}");
  fPtAllminPtTPCvsPtAllPhi->SetYTitle("(1/p_{t}^{Global}-1/p_{t}^{TPC})/(1/p_{t}^{Global})");
  fPtAllminPtTPCvsPtAllPhi->SetZTitle("#phi");
  fHistList->Add(fPtAllminPtTPCvsPtAllPhi);

  fPtAllminPtTPCvsPtAllNPointITS = new TH3F("fPtAllminPtTPCvsPtAllNPointITS","PtAllminPtTPCvsPtAllNPointITS",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,9,-0.5,8.5);
  fPtAllminPtTPCvsPtAllNPointITS->SetXTitle("p_{t}^{Global}");
  fPtAllminPtTPCvsPtAllNPointITS->SetYTitle("(1/p_{t}^{Global}-1/p_{t}^{TPC})/(1/p_{t}^{Global})");
  fPtAllminPtTPCvsPtAllNPointITS->SetZTitle("N_{point,ITS}");
  fHistList->Add(fPtAllminPtTPCvsPtAllNPointITS);
  
  fPtAllminPtTPCvsPtAllNSigmaToVertex = new TH3F("fPtAllminPtTPCvsPtAllNSigmaToVertex","PtAllminPtTPCvsPtAllNSigmaToVertex",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,40,0.,8.);
  fPtAllminPtTPCvsPtAllNSigmaToVertex->SetXTitle("p_{t}^{Global}");
  fPtAllminPtTPCvsPtAllNSigmaToVertex->SetYTitle("(1/p_{t}^{Global}-1/p_{t}^{TPC})/(1/p_{t}^{Global})");
  fPtAllminPtTPCvsPtAllNSigmaToVertex->SetZTitle("N#sigma to vertex");
  fHistList->Add(fPtAllminPtTPCvsPtAllNSigmaToVertex);

  fPtAllminPtTPCvsPtAllChi2C = new TH3F("fPtAllminPtTPCvsPtAllChi2C","PtAllminPtTPCvsPtAllChi2C",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,20,0.,10.);
  fPtAllminPtTPCvsPtAllChi2C->SetXTitle("p_{t}^{Global}");
  fPtAllminPtTPCvsPtAllChi2C->SetYTitle("(1/p_{t}^{Global}-1/p_{t}^{TPC})/(1/p_{t}^{Global})");
  fPtAllminPtTPCvsPtAllChi2C->SetZTitle("Constrained #chi^{2}");
  fHistList->Add(fPtAllminPtTPCvsPtAllChi2C);

  fPtAllminPtTPCvsPtAllRel1PtUncertainty = new TH3F("fPtAllminPtTPCvsPtAllRel1PtUncertainty","PtAllminPtTPCvsPtAllRel1PtUncertainty",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,30,0.,0.3);
  fPtAllminPtTPCvsPtAllRel1PtUncertainty->SetXTitle("p_{t}^{Global}");
  fPtAllminPtTPCvsPtAllRel1PtUncertainty->SetYTitle("(1/p_{t}^{Global}-1/p_{t}^{TPC})/(1/p_{t}^{Global})");
  fPtAllminPtTPCvsPtAllRel1PtUncertainty->SetZTitle("Rel1PtUncertainty");
  fHistList->Add(fPtAllminPtTPCvsPtAllRel1PtUncertainty);

  fPtAllminPtTPCvsPtAllChi2PerNClusTPC = new TH3F("fPtAllminPtTPCvsPtAllChi2PerNClusTPC","PtAllminPtTPCvsPtAllChi2PerNClusTPC",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkChi2PerClusBins,fgkChi2PerClusMin,fgkChi2PerClusMax);
  fPtAllminPtTPCvsPtAllChi2PerNClusTPC->SetXTitle("p_{t}^{Global}");
  fPtAllminPtTPCvsPtAllChi2PerNClusTPC->SetYTitle("(1/p_{t}^{Global}-1/p_{t}^{TPC})/(1/p_{t}^{Global})");
  fPtAllminPtTPCvsPtAllChi2PerNClusTPC->SetZTitle("#chi^{2}/NClusTPC");
  fHistList->Add(fPtAllminPtTPCvsPtAllChi2PerNClusTPC);

  fPtAllminPtTPCvsPtAllChi2PerNClusITS = new TH3F("fPtAllminPtTPCvsPtAllChi2PerNClusITS","PtAllminPtTPCvsPtAllChi2PerNClusITS",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkChi2PerClusBins,fgkChi2PerClusMin,fgkChi2PerClusMax);
  fPtAllminPtTPCvsPtAllChi2PerNClusITS->SetXTitle("p_{t}^{Global}");
  fPtAllminPtTPCvsPtAllChi2PerNClusITS->SetYTitle("(1/p_{t}^{Global}-1/p_{t}^{TPC})/(1/p_{t}^{Global})");
  fPtAllminPtTPCvsPtAllChi2PerNClusITS->SetZTitle("#chi^{2}/NClusITS");
  fHistList->Add(fPtAllminPtTPCvsPtAllChi2PerNClusITS);

  fEtaPhiOutliers = new TH2F("fEtaPhiOutliers","PtAllminPtTPCvsPtAll",20, -1.,1.,fgkNPhiBins,kMinPhi,kMaxPhi);
  fEtaPhiOutliers->SetXTitle("#eta");
  fEtaPhiOutliers->SetYTitle("#phi");
  fHistList->Add(fEtaPhiOutliers);

  //Global vs ITSouter-TPCinner
  fPtSelITSouter = new TH1F("fPtSelITSouter","PtSelITSouter",fgkNPtBins,fgkPtMin,fgkPtMax);
  fHistList->Add(fPtSelITSouter);
  
  fPtITSouterminPtTPCvsPtAll = new TH2F("fPtITSouterminPtTPCvsPtAll","PtAllminPtTPCvsPtAll",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.);
  fPtITSouterminPtTPCvsPtAll->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAll->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fHistList->Add(fPtITSouterminPtTPCvsPtAll);
  
  fPtITSouterminPtTPCvsPtAllNPointTPC = new TH3F("fPtITSouterminPtTPCvsPtAllNPointTPC","PtAllminPtTPCvsPtAllNPointTPC",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,160,0.5,160.5);
  fPtITSouterminPtTPCvsPtAllNPointTPC->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllNPointTPC->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllNPointTPC->SetZTitle("N_{point,TPC}");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllNPointTPC);

  fPtITSouterminPtTPCvsPtAllDCAR = new TH3F("fPtITSouterminPtTPCvsPtAllDCAR","PtAllminPtTPCvsPtAllDCAR",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,80,-0.2,0.2);
  fPtITSouterminPtTPCvsPtAllDCAR->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllDCAR->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllDCAR->SetZTitle("DCA_{R}");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllDCAR);

  fPtITSouterminPtTPCvsPtAllDCAZ = new TH3F("fPtITSouterminPtTPCvsPtAllDCAZ","PtAllminPtTPCvsPtAllDCAZ",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,80,-2.,2.);
  fPtITSouterminPtTPCvsPtAllDCAZ->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllDCAZ->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllDCAZ->SetZTitle("DCA_{Z}");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllDCAZ);

  fPtITSouterminPtTPCvsPtAllPhi = new TH3F("fPtITSouterminPtTPCvsPtAllPhi","PtAllminPtTPCvsPtAllPhi",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkNPhiBins,kMinPhi,kMaxPhi);
  fPtITSouterminPtTPCvsPtAllPhi->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllPhi->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllPhi->SetZTitle("#phi");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllPhi);

  fPtITSouterminPtTPCvsPtAllNPointITS = new TH3F("fPtITSouterminPtTPCvsPtAllNPointITS","PtAllminPtTPCvsPtAllNPointITS",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,9,-0.5,8.5);
  fPtITSouterminPtTPCvsPtAllNPointITS->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllNPointITS->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllNPointITS->SetZTitle("N_{point,ITS}");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllNPointITS);
  
  fPtITSouterminPtTPCvsPtAllNSigmaToVertex = new TH3F("fPtITSouterminPtTPCvsPtAllNSigmaToVertex","PtAllminPtTPCvsPtAllNSigmaToVertex",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,40,0.,8.);
  fPtITSouterminPtTPCvsPtAllNSigmaToVertex->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllNSigmaToVertex->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllNSigmaToVertex->SetZTitle("N#sigma to vertex");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllNSigmaToVertex);

  fPtITSouterminPtTPCvsPtAllChi2C = new TH3F("fPtITSouterminPtTPCvsPtAllChi2C","PtAllminPtTPCvsPtAllChi2C",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,20,0.,10.);
  fPtITSouterminPtTPCvsPtAllChi2C->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllChi2C->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllChi2C->SetZTitle("Constrained #chi^{2}");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllChi2C);

  fPtITSouterminPtTPCvsPtAllRel1PtUncertainty = new TH3F("fPtITSouterminPtTPCvsPtAllRel1PtUncertainty","PtAllminPtTPCvsPtAllRel1PtUncertainty",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,30,0.,0.3);
  fPtITSouterminPtTPCvsPtAllRel1PtUncertainty->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllRel1PtUncertainty->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllRel1PtUncertainty->SetZTitle("Rel1PtUncertainty");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllRel1PtUncertainty);

  fPtITSouterminPtTPCvsPtAllChi2PerNClusTPC = new TH3F("fPtITSouterminPtTPCvsPtAllChi2PerNClusTPC","PtAllminPtTPCvsPtAllChi2PerNClusTPC",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,35,0.,3.5);
  fPtITSouterminPtTPCvsPtAllChi2PerNClusTPC->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusTPC->SetYTitle("(1/p_{t}^{Global}-1/p_{t}^{TPC})/(1/p_{t}^{Global})");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusTPC->SetZTitle("#chi^{2}/NClusTPC");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllChi2PerNClusTPC);

  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS = new TH3F("fPtITSouterminPtTPCvsPtAllChi2PerNClusITS","PtAllminPtTPCvsPtAllChi2PerNClusITS",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,35,0.,3.5);
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS->SetYTitle("(1/p_{t}^{Global}-1/p_{t}^{TPC})/(1/p_{t}^{Global})");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS->SetZTitle("#chi^{2}/NClusITS");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllChi2PerNClusITS);

  //As function of ITS layers
  fPtITSouterminPtTPCvsPtAll_ITSLayer0 = new TH2F("fPtITSouterminPtTPCvsPtAll_ITSLayer0","PtAllminPtTPCvsPtAll_ITSLayer0",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.);
  fPtITSouterminPtTPCvsPtAll_ITSLayer0->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAll_ITSLayer0->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fHistList->Add(fPtITSouterminPtTPCvsPtAll_ITSLayer0);

  fPtITSouterminPtTPCvsPtAll_ITSLayer1 = new TH2F("fPtITSouterminPtTPCvsPtAll_ITSLayer1","PtAllminPtTPCvsPtAll_ITSLayer1",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.);
  fPtITSouterminPtTPCvsPtAll_ITSLayer1->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAll_ITSLayer1->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fHistList->Add(fPtITSouterminPtTPCvsPtAll_ITSLayer1);

  fPtITSouterminPtTPCvsPtAll_ITSLayer2 = new TH2F("fPtITSouterminPtTPCvsPtAll_ITSLayer2","PtAllminPtTPCvsPtAll_ITSLayer2",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.);
  fPtITSouterminPtTPCvsPtAll_ITSLayer2->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAll_ITSLayer2->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fHistList->Add(fPtITSouterminPtTPCvsPtAll_ITSLayer2);

  fPtITSouterminPtTPCvsPtAll_ITSLayer3 = new TH2F("fPtITSouterminPtTPCvsPtAll_ITSLayer3","PtAllminPtTPCvsPtAll_ITSLayer3",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.);
  fPtITSouterminPtTPCvsPtAll_ITSLayer3->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAll_ITSLayer3->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fHistList->Add(fPtITSouterminPtTPCvsPtAll_ITSLayer3);

  fPtITSouterminPtTPCvsPtAll_ITSLayer4 = new TH2F("fPtITSouterminPtTPCvsPtAll_ITSLayer4","PtAllminPtTPCvsPtAll_ITSLayer4",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.);
  fPtITSouterminPtTPCvsPtAll_ITSLayer4->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAll_ITSLayer4->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fHistList->Add(fPtITSouterminPtTPCvsPtAll_ITSLayer4);

  fPtITSouterminPtTPCvsPtAll_ITSLayer5 = new TH2F("fPtITSouterminPtTPCvsPtAll_ITSLayer5","PtAllminPtTPCvsPtAll_ITSLayer5",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.);
  fPtITSouterminPtTPCvsPtAll_ITSLayer5->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAll_ITSLayer5->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fHistList->Add(fPtITSouterminPtTPCvsPtAll_ITSLayer5);

  fPtITSouterminPtTPCvsPtAll_NoSPD = new TH2F("fPtITSouterminPtTPCvsPtAll_NoSPD","PtAllminPtTPCvsPtAll_NoSPD",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.);
  fPtITSouterminPtTPCvsPtAll_NoSPD->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAll_NoSPD->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fHistList->Add(fPtITSouterminPtTPCvsPtAll_NoSPD);

  fPtITSouterminPtTPCvsPtAll_NoSDD = new TH2F("fPtITSouterminPtTPCvsPtAll_NoSDD","PtAllminPtTPCvsPtAll_NoSDD",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.);
  fPtITSouterminPtTPCvsPtAll_NoSDD->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAll_NoSDD->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fHistList->Add(fPtITSouterminPtTPCvsPtAll_NoSDD);

  fPtITSouterminPtTPCvsPtAll_NoSSD = new TH2F("fPtITSouterminPtTPCvsPtAll_NoSSD","PtAllminPtTPCvsPtAll_NoSSD",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.);
  fPtITSouterminPtTPCvsPtAll_NoSSD->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAll_NoSSD->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fHistList->Add(fPtITSouterminPtTPCvsPtAll_NoSSD);

  //
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer0 = new TH3F("fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer0","PtAllminPtTPCvsPtAllChi2PerNClusITS_ITSLayer0",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkChi2PerClusBins,fgkChi2PerClusMin,fgkChi2PerClusMax);
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer0->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer0->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer0->SetZTitle("#chi^{2}/NPointITS");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer0);

  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer1 = new TH3F("fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer1","PtAllminPtTPCvsPtAllChi2PerNClusITS_ITSLayer1",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkChi2PerClusBins,fgkChi2PerClusMin,fgkChi2PerClusMax);
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer1->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer1->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer1->SetZTitle("#chi^{2}/NPointITS");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer1);

  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer2 = new TH3F("fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer2","PtAllminPtTPCvsPtAllChi2PerNClusITS_ITSLayer2",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkChi2PerClusBins,fgkChi2PerClusMin,fgkChi2PerClusMax);
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer2->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer2->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer2->SetZTitle("#chi^{2}/NPointITS");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer2);

  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer3 = new TH3F("fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer3","PtAllminPtTPCvsPtAllChi2PerNClusITS_ITSLayer3",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkChi2PerClusBins,fgkChi2PerClusMin,fgkChi2PerClusMax);
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer3->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer3->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer3->SetZTitle("#chi^{2}/NPointITS");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer3);

  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer4 = new TH3F("fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer4","PtAllminPtTPCvsPtAllChi2PerNClusITS_ITSLayer4",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkChi2PerClusBins,fgkChi2PerClusMin,fgkChi2PerClusMax);
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer4->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer4->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer4->SetZTitle("#chi^{2}/NPointITS");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer4);

  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer5 = new TH3F("fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer5","PtAllminPtTPCvsPtAllChi2PerNClusITS_ITSLayer5",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkChi2PerClusBins,fgkChi2PerClusMin,fgkChi2PerClusMax);
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer5->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer5->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer5->SetZTitle("#chi^{2}/NPointITS");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer5);

  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSPD = new TH3F("fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSPD","PtAllminPtTPCvsPtAllChi2PerNClusITS_NoSPD",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkChi2PerClusBins,fgkChi2PerClusMin,fgkChi2PerClusMax);
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSPD->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSPD->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSPD->SetZTitle("#chi^{2}/NPointITS");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSPD);

  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSDD = new TH3F("fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSDD","PtAllminPtTPCvsPtAllChi2PerNClusITS_NoSDD",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkChi2PerClusBins,fgkChi2PerClusMin,fgkChi2PerClusMax);
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSDD->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSDD->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSDD->SetZTitle("#chi^{2}/NPointITS");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSDD);

  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSSD = new TH3F("fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSSD","PtAllminPtTPCvsPtAllChi2PerNClusITS_NoSSD",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkChi2PerClusBins,fgkChi2PerClusMin,fgkChi2PerClusMax);
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSSD->SetXTitle("p_{t}^{Global}");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSSD->SetYTitle("(1/p_{t}^{ITSouter}-1/p_{t}^{TPCinner})/(1/p_{t}^{ITSouter})");
  fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSSD->SetZTitle("#chi^{2}/NPointITS");
  fHistList->Add(fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSSD);


  //ITSrefit
  fPtSelITS = new TH1F("fPtSelITSrefit","PtSel",fgkNPtBins, fgkPtMin, fgkPtMax);
  fHistListITS->Add(fPtSelITS);
  
  fPtITSminPtTPCvsPtITS = new TH2F("fPtITSminPtTPCvsPtITS","PtITSminPtTPCvsPtITS",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.);
  fPtITSminPtTPCvsPtITS->SetXTitle("p_{t}^{ITS}");
  fPtITSminPtTPCvsPtITS->SetYTitle("(1/p_{t}^{ITS}-1/p_{t}^{TPC})/(1/p_{t}^{ITS})");
  fHistListITS->Add(fPtITSminPtTPCvsPtITS);
  
  fPtITSminPtTPCvsPtITSNPointTPC = new TH3F("fPtITSminPtTPCvsPtITSNPointTPC","PtITSminPtTPCvsPtITSNPointTPC",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,160,0.5,160.5);
  fPtITSminPtTPCvsPtITSNPointTPC->SetXTitle("p_{t}^{ITSrefit}");
  fPtITSminPtTPCvsPtITSNPointTPC->SetYTitle("(1/p_{t}^{ITSrefit}-1/p_{t}^{TPC})/(1/p_{t}^{ITSrefit})");
  fPtITSminPtTPCvsPtITSNPointTPC->SetZTitle("N_{point,TPC}");
  fHistListITS->Add(fPtITSminPtTPCvsPtITSNPointTPC);
    
  fPtITSminPtTPCvsPtITSDCAR = new TH3F("fPtITSminPtTPCvsPtITSDCAR","PtITSminPtTPCvsPtITSDCAR",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,80,-0.2,0.2);
  fPtITSminPtTPCvsPtITSDCAR->SetXTitle("p_{t}^{ITSrefit}");
  fPtITSminPtTPCvsPtITSDCAR->SetYTitle("(1/p_{t}^{ITSrefit}-1/p_{t}^{TPC})/(1/p_{t}^{ITSrefit})");
  fPtITSminPtTPCvsPtITSDCAR->SetZTitle("DCA_{R}");
  fHistListITS->Add(fPtITSminPtTPCvsPtITSDCAR);
  
  fPtITSminPtTPCvsPtITSDCAZ = new TH3F("fPtITSminPtTPCvsPtITSDCAZ","PtITSminPtTPCvsPtITSDCAZ",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,80,-2.,2.);
  fPtITSminPtTPCvsPtITSDCAZ->SetXTitle("p_{t}^{ITSrefit}");
  fPtITSminPtTPCvsPtITSDCAZ->SetYTitle("(1/p_{t}^{ITSrefit}-1/p_{t}^{TPC})/(1/p_{t}^{ITSrefit})");
  fPtITSminPtTPCvsPtITSDCAZ->SetZTitle("DCA_{Z}");
  fHistListITS->Add(fPtITSminPtTPCvsPtITSDCAZ);
  
  fPtITSminPtTPCvsPtITSPhi = new TH3F("fPtITSminPtTPCvsPtITSPhi","PtITSminPtTPCvsPtITSPhi",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,fgkNPhiBins,kMinPhi,kMaxPhi);
  fPtITSminPtTPCvsPtITSPhi->SetXTitle("p_{t}^{ITSrefit}");
  fPtITSminPtTPCvsPtITSPhi->SetYTitle("(1/p_{t}^{ITSrefit}-1/p_{t}^{TPC})/(1/p_{t}^{ITSrefit})");
  fPtITSminPtTPCvsPtITSPhi->SetZTitle("#phi");
  fHistListITS->Add(fPtITSminPtTPCvsPtITSPhi);
  
  fPtITSminPtTPCvsPtITSNPointITS = new TH3F("fPtITSminPtTPCvsPtITSNPointITS","PtITSminPtTPCvsPtITSNPointITS",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,9,-0.5,8.5);
  fPtITSminPtTPCvsPtITSNPointITS->SetXTitle("p_{t}^{ITSrefit}");
  fPtITSminPtTPCvsPtITSNPointITS->SetYTitle("(1/p_{t}^{ITSrefit}-1/p_{t}^{TPC})/(1/p_{t}^{ITSrefit})");
  fPtITSminPtTPCvsPtITSNPointITS->SetZTitle("N_{point,ITS}");
  fHistListITS->Add(fPtITSminPtTPCvsPtITSNPointITS); 
  
  fPtITSminPtTPCvsPtITSNSigmaToVertex = new TH3F("fPtITSminPtTPCvsPtITSNSigmaToVertex","PtITSminPtTPCvsPtITSNSigmaToVertex",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,40,0.,8.);
  fPtITSminPtTPCvsPtITSNSigmaToVertex->SetXTitle("p_{t}^{ITS}");
  fPtITSminPtTPCvsPtITSNSigmaToVertex->SetYTitle("(1/p_{t}^{ITS}-1/p_{t}^{TPC})/(1/p_{t}^{ITS})");
  fPtITSminPtTPCvsPtITSNSigmaToVertex->SetZTitle("N#sigma to vertex");
  fHistListITS->Add(fPtITSminPtTPCvsPtITSNSigmaToVertex);

  fPtITSminPtTPCvsPtITSChi2C = new TH3F("fPtITSminPtTPCvsPtITSChi2C","PtITSminPtTPCvsPtITSChi2C",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,20,0.,10.);
  fPtITSminPtTPCvsPtITSChi2C->SetXTitle("p_{t}^{ITS}");
  fPtITSminPtTPCvsPtITSChi2C->SetYTitle("(1/p_{t}^{ITS}-1/p_{t}^{TPC})/(1/p_{t}^{ITS})");
  fPtITSminPtTPCvsPtITSChi2C->SetZTitle("Constrained #chi^{2}");
  fHistListITS->Add(fPtITSminPtTPCvsPtITSChi2C);

  fPtITSminPtTPCvsPtITSRel1PtUncertainty = new TH3F("fPtITSminPtTPCvsPtITSRel1PtUncertainty","PtITSminPtTPCvsPtITSRel1PtUncertainty",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,30,0.,0.3);
  fPtITSminPtTPCvsPtITSRel1PtUncertainty->SetXTitle("p_{t}^{ITS}");
  fPtITSminPtTPCvsPtITSRel1PtUncertainty->SetYTitle("(1/p_{t}^{ITS}-1/p_{t}^{TPC})/(1/p_{t}^{ITS})");
  fPtITSminPtTPCvsPtITSRel1PtUncertainty->SetZTitle("Rel1PtUncertainty");
  fHistListITS->Add(fPtITSminPtTPCvsPtITSRel1PtUncertainty);

  fPtITSminPtTPCvsPtITSChi2PerNClusTPC = new TH3F("fPtITSminPtTPCvsPtITSChi2PerNClusTPC","PtITSminPtTPCvsPtITSChi2PerNClusTPC",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,35,0.,3.5);
  fPtITSminPtTPCvsPtITSChi2PerNClusTPC->SetXTitle("p_{t}^{ITS}");
  fPtITSminPtTPCvsPtITSChi2PerNClusTPC->SetYTitle("(1/p_{t}^{ITS}-1/p_{t}^{TPC})/(1/p_{t}^{ITS})");
  fPtITSminPtTPCvsPtITSChi2PerNClusTPC->SetZTitle("#chi^{2}/NClusTPC");
  fHistListITS->Add(fPtITSminPtTPCvsPtITSChi2PerNClusTPC);

  fPtITSminPtTPCvsPtITSChi2PerNClusITS = new TH3F("fPtITSminPtTPCvsPtITSChi2PerNClusITS","PtITSminPtTPCvsPtITSChi2PerNClusITS",fgkNPtBins, fgkPtMin,fgkPtMax,fgkResPtBins,-1.,1.,35,0.,3.5);
  fPtITSminPtTPCvsPtITSChi2PerNClusITS->SetXTitle("p_{t}^{ITS}");
  fPtITSminPtTPCvsPtITSChi2PerNClusITS->SetYTitle("(1/p_{t}^{ITS}-1/p_{t}^{TPC})/(1/p_{t}^{ITS})");
  fPtITSminPtTPCvsPtITSChi2PerNClusITS->SetZTitle("#chi^{2}/NClusITS");
  fHistListITS->Add(fPtITSminPtTPCvsPtITSChi2PerNClusITS);

  fPtOuterPtInner = new TH2F("fPtOuterPtInner","fPtOuterPtInner",fgkNPtBins, fgkPtMin,fgkPtMax,fgkNPtBins, fgkPtMin,fgkPtMax);
  fPtOuterPtInner->SetXTitle("p_{t}^{inner}");
  fPtOuterPtInner->SetYTitle("p_{t}^{outer}");
  fHistListITS->Add(fPtOuterPtInner);

  fPtRel1PtUncertaintyChi2PerClusTPC = new TH3F("fPtRel1PtUncertaintyChi2PerClusTPC","PtITSminPtTPCvsPtITSRel1PtUncertainty",fgkNPtBins, fgkPtMin,fgkPtMax,30,0.,0.3,35,0.,3.5);
  fPtRel1PtUncertaintyChi2PerClusTPC->SetXTitle("p_{t}^{global}");
  fPtRel1PtUncertaintyChi2PerClusTPC->SetYTitle("Rel1PtUncertainty");
  fPtRel1PtUncertaintyChi2PerClusTPC->SetZTitle("#chi^{2}/N_{clusters}^{TPC}");
  fHistListITS->Add(fPtRel1PtUncertaintyChi2PerClusTPC);

  fPtAllTPC = new TH1F("fPtAllTPC","PtAll",fgkNPtBins, fgkPtMin, fgkPtMax);
  fHistListTPC->Add(fPtAllTPC);
  fPtSelTPC = new TH1F("fPtSelTPC","PtSel",fgkNPtBins, fgkPtMin, fgkPtMax);
  fHistListTPC->Add(fPtSelTPC);
  fPtSelTPCITS = new TH1F("fPtSelTPCITS","PtSel",fgkNPtBins, fgkPtMin, fgkPtMax);
  fHistListTPC->Add(fPtSelTPCITS);

  TH1::AddDirectory(oldStatus);   

}
//________________________________________________________________________
void AliPWG4HighPtQATPConly::Exec(Option_t *) {  
  // Main loop
  // Called for each event
  AliDebug(2,Form(">> AliPWG4HighPtQATPConly::Exec \n"));  

  // All events without selection
  fNEventAll->Fill(0.);

  if (!fESD) {
    AliDebug(2,Form("ERROR: fESD not available"));
    // Post output data
     PostData(0, fHistList);
     PostData(1, fHistListTPC);
     PostData(2, fHistListITS);
    return;
  }

  fESD->SetESDfriend(fESDfriend); //Attach the friend to the ESD
  if (!fESDfriend) {
    AliDebug(2,Form("ERROR: fESDfriend not available"));
    // Post output data
     PostData(0, fHistList);
     PostData(1, fHistListTPC);
     PostData(2, fHistListITS);
    return;
  }

  Bool_t isSelected = ((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected();
  if(!isSelected) { //Select collison candidates
    AliDebug(2,Form(" Trigger Selection: event REJECTED ... "));
    // Post output data
     PostData(0, fHistList);
     PostData(1, fHistListTPC);
     PostData(2, fHistListITS);
    return;
  }

  //  AliMCEventHandler *eventHandler = dynamic_cast<AliMCEventHandler*> (AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler());
//   //  AliMCEventHandler* eventHandler = (AliMCEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler());
  
  AliStack* stack = 0x0;
  AliMCEvent* mcEvent = 0x0;
  
  if(fMC) {
    mcEvent = fMC;
    if (!mcEvent) {
      AliDebug(2,Form("ERROR: Could not retrieve MC event"));
      PostData(0, fHistList);
      PostData(1, fHistListTPC);
      PostData(2, fHistListITS);
      return;
    }
    
    AliDebug(2,Form("MC particles: %d", mcEvent->GetNumberOfTracks()));
    
    stack = mcEvent->Stack();                //Particles Stack
    
    AliDebug(2,Form("MC particles stack: %d", stack->GetNtrack()));
  }
  

  const AliESDVertex *vtx = fESD->GetPrimaryVertexTracks();
  // Need vertex cut
  if (vtx->GetNContributors() < 2) {
    // Post output data
    PostData(0, fHistList);
    PostData(1, fHistListTPC);
    PostData(2, fHistListITS);
    return;
  }

  AliDebug(2,Form("Vertex title %s, status %d, nCont %d\n",vtx->GetTitle(), vtx->GetStatus(), vtx->GetNContributors()));
  double primVtx[3];
  vtx->GetXYZ(primVtx);
  //  printf("primVtx: %g  %g  %g \n",primVtx[0],primVtx[1],primVtx[2]);
  if(TMath::Sqrt(primVtx[0]*primVtx[0] + primVtx[1]*primVtx[1])>1. || TMath::Abs(primVtx[2]>10.)){
    // Post output data
    PostData(0, fHistList);
    PostData(1, fHistListTPC);
    PostData(2, fHistListITS);
    return;
  }
  if(!fESD->GetNumberOfTracks() || fESD->GetNumberOfTracks()<2){ 
    // Post output data
    PostData(0, fHistList);
    PostData(1, fHistListTPC);
    PostData(2, fHistListITS);
    return;
  }
  Int_t nTracks = fESD->GetNumberOfTracks();
  AliDebug(2,Form("nTracks %d\n", nTracks));

  if(!fTrackCuts) return;

  // Selected events for analysis
  fNEventSel->Fill(0.);

  for (Int_t iTrack = 0; iTrack < nTracks; iTrack++) {
    
    AliESDtrack *track = fESD->GetTrack(iTrack);
    AliExternalTrackParam *trackTPC = (AliExternalTrackParam *)track->GetTPCInnerParam();
    if(!track || !trackTPC) continue;
    const AliESDfriendTrack* constfriendtrack = track->GetFriendTrack();
    if (!constfriendtrack) { continue;}
    AliESDfriendTrack friendtrack(*constfriendtrack);
 
    Float_t pt = track->Pt();
    Float_t ptTPC = trackTPC->Pt();
    Float_t phi = track->Phi();
    Float_t dca2D, dcaZ;
    track->GetImpactParameters(dca2D,dcaZ);
    // Float_t dca2DTPC, dcaZTPC;
    //track->GetImpactParametersTPC(dca2DTPC,dcaZTPC);
    UChar_t itsMap = track->GetITSClusterMap();
    Int_t nPointITS = 0;
    for (Int_t i=0; i < 6; i++) {
      if (itsMap & (1 << i))
	nPointITS ++;
    }
    double mom[3];
    track->GetPxPyPz(mom);
    double momTPC[3];
    trackTPC->GetPxPyPz(momTPC);
    Float_t nSigmaToVertex = fTrackCuts->GetSigmaToVertex(track);// Calculates the number of sigma to the vertex for a track.
    Float_t chi2C = track->GetConstrainedChi2();
    Float_t relUncertainty1Pt = TMath::Sqrt(TMath::Abs(track->GetSigma1Pt2()))*pt;
    Float_t chi2PerClusterTPC = -1.;
    Float_t nClustersTPC = track->GetTPCNcls();
    if(nClustersTPC!=0.) chi2PerClusterTPC = track->GetTPCchi2()/nClustersTPC;

    fPtAll->Fill(pt);
    fPtAllTPC->Fill(ptTPC);
    
    if (fTrackCuts->AcceptTrack(track)) {

      fPtSel->Fill(pt);
      fPtSelTPC->Fill(ptTPC);
      if(ptTPC==0. || pt==0.) continue;
      fPtAllminPtTPCvsPtAll->Fill(pt,(1./pt-1./ptTPC)/(1./pt) );
      fPtAllminPtTPCvsPtAllNPointTPC->Fill(pt,(1./pt-1./ptTPC)/(1./pt),track->GetTPCNcls());
      fPtAllminPtTPCvsPtAllDCAR->Fill(pt,(1./pt-1./ptTPC)/(1./pt),dca2D);
      fPtAllminPtTPCvsPtAllDCAZ->Fill(pt,(1./pt-1./ptTPC)/(1./pt),dcaZ);
      fPtAllminPtTPCvsPtAllPhi->Fill(pt,(1./pt-1./ptTPC)/(1./pt),phi);
      fPtAllminPtTPCvsPtAllNPointITS->Fill(pt,(1./pt-1./ptTPC)/(1./pt),nPointITS);
      fPtAllminPtTPCvsPtAllNSigmaToVertex->Fill(pt,(1./pt-1./ptTPC)/(1./pt),nSigmaToVertex);
      fPtAllminPtTPCvsPtAllChi2C->Fill(pt,(1./pt-1./ptTPC)/(1./pt),chi2C);
      fPtAllminPtTPCvsPtAllRel1PtUncertainty->Fill(pt,(1./pt-1./ptTPC)/(1./pt),relUncertainty1Pt);
      fPtAllminPtTPCvsPtAllChi2PerNClusTPC->Fill(pt,(1./pt-1./ptTPC)/(1./pt),chi2PerClusterTPC);
      if(nPointITS>0) fPtAllminPtTPCvsPtAllChi2PerNClusITS->Fill(pt,(1./pt-1./ptTPC)/(1./pt),track->GetITSchi2()/(float)nPointITS);
      if(TMath::Abs((1./pt-1./ptTPC)/(1./pt))>0.8) fEtaPhiOutliers->Fill(track->Eta(),phi);
      if (friendtrack.GetITSOut()) {
	AliExternalTrackParam trackITSouter(*(friendtrack.GetITSOut())); 
	Float_t ptITSouter = trackITSouter.Pt();
	if(ptITSouter==0.) continue;
	fPtSelITSouter->Fill(ptITSouter);
	fPtITSouterminPtTPCvsPtAll->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter) );
	fPtITSouterminPtTPCvsPtAllNPointTPC->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),track->GetTPCNcls());
	fPtITSouterminPtTPCvsPtAllDCAR->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),dca2D);
	fPtITSouterminPtTPCvsPtAllDCAZ->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),dcaZ);
	fPtITSouterminPtTPCvsPtAllPhi->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),phi);
	fPtITSouterminPtTPCvsPtAllNPointITS->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),nPointITS);
	fPtITSouterminPtTPCvsPtAllNSigmaToVertex->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),nSigmaToVertex);
	fPtITSouterminPtTPCvsPtAllChi2C->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),chi2C);
	fPtITSouterminPtTPCvsPtAllRel1PtUncertainty->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),relUncertainty1Pt);
	fPtITSouterminPtTPCvsPtAllChi2PerNClusTPC->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),chi2PerClusterTPC);
	if(nPointITS>0) fPtITSouterminPtTPCvsPtAllChi2PerNClusITS->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),track->GetITSchi2()/nPointITS);
	if(track->HasPointOnITSLayer(0)) {
	  fPtITSouterminPtTPCvsPtAll_ITSLayer0->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter));
	  if(nPointITS>0) fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer0->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),track->GetITSchi2()/nPointITS);
	}
	if(!track->HasPointOnITSLayer(0) && track->HasPointOnITSLayer(1)) {
	  fPtITSouterminPtTPCvsPtAll_ITSLayer1->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter));
	  if(nPointITS>0) fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer1->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),track->GetITSchi2()/nPointITS);
	}
	if(!track->HasPointOnITSLayer(0) && !track->HasPointOnITSLayer(1) && track->HasPointOnITSLayer(2)) {
	  fPtITSouterminPtTPCvsPtAll_ITSLayer2->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter));
	  if(nPointITS>0) fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer2->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),track->GetITSchi2()/nPointITS);
	}
	if(!track->HasPointOnITSLayer(0) && !track->HasPointOnITSLayer(1) && !track->HasPointOnITSLayer(2) && track->HasPointOnITSLayer(3)) {
	  fPtITSouterminPtTPCvsPtAll_ITSLayer3->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter));
	  if(nPointITS>0) fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer3->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),track->GetITSchi2()/nPointITS);
	}
	if(!track->HasPointOnITSLayer(0) && !track->HasPointOnITSLayer(1) && !track->HasPointOnITSLayer(2) && !track->HasPointOnITSLayer(3) && track->HasPointOnITSLayer(4)) {
	  fPtITSouterminPtTPCvsPtAll_ITSLayer4->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter));
	  if(nPointITS>0) fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer4->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),track->GetITSchi2()/nPointITS);
	}
	if(!track->HasPointOnITSLayer(0) && !track->HasPointOnITSLayer(1) && !track->HasPointOnITSLayer(2) && !track->HasPointOnITSLayer(3) && !track->HasPointOnITSLayer(4) && track->HasPointOnITSLayer(5)) {
	  fPtITSouterminPtTPCvsPtAll_ITSLayer5->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter));
	  if(nPointITS>0) fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_ITSLayer5->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),track->GetITSchi2()/nPointITS);
	}

	if(!track->HasPointOnITSLayer(0) && !track->HasPointOnITSLayer(1)) {
	  fPtITSouterminPtTPCvsPtAll_NoSPD->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter));
	  if(nPointITS>0) fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSPD->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),track->GetITSchi2()/nPointITS);
	}
	if(!track->HasPointOnITSLayer(2) && !track->HasPointOnITSLayer(3)) {
	  fPtITSouterminPtTPCvsPtAll_NoSDD->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter));
	  if(nPointITS>0) fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSDD->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),track->GetITSchi2()/nPointITS);
	}
	if(!track->HasPointOnITSLayer(4) && !track->HasPointOnITSLayer(5)) {
	  fPtITSouterminPtTPCvsPtAll_NoSSD->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter));
	  if(nPointITS>0) fPtITSouterminPtTPCvsPtAllChi2PerNClusITS_NoSSD->Fill(pt,(1./ptITSouter-1./ptTPC)/(1./ptITSouter),track->GetITSchi2()/nPointITS);
	}
      }
    }//fTrackCuts selection
    
    
    //ITSrefit selection
    if (fTrackCutsITS->AcceptTrack(track)) {
      
      fPtSelITS->Fill(pt);
      fPtSelTPCITS->Fill(ptTPC);
      fPtITSminPtTPCvsPtITS->Fill(pt,(1./pt-1./ptTPC)/(1./pt) );
      fPtITSminPtTPCvsPtITSNPointTPC->Fill(pt,(1./pt-1./ptTPC)/(1./pt),track->GetTPCNcls());
      fPtITSminPtTPCvsPtITSDCAR->Fill(pt,(1./pt-1./ptTPC)/(1./pt),dca2D);
      fPtITSminPtTPCvsPtITSDCAZ->Fill(pt,(1./pt-1./ptTPC)/(1./pt),dcaZ);
      fPtITSminPtTPCvsPtITSPhi->Fill(pt,(pt-ptTPC)/(pt),phi);
      fPtITSminPtTPCvsPtITSNPointITS->Fill(pt,(pt-ptTPC)/(pt),nPointITS);
      fPtITSminPtTPCvsPtITSNSigmaToVertex->Fill(pt,(1./pt-1./ptTPC)/(1./pt),nSigmaToVertex);
      fPtITSminPtTPCvsPtITSChi2C->Fill(pt,(1./pt-1./ptTPC)/(1./pt),chi2C);
      fPtITSminPtTPCvsPtITSRel1PtUncertainty->Fill(pt,(1./pt-1./ptTPC)/(1./pt),relUncertainty1Pt);
      //      fPtOuterPtInner->Fill(trackInner->Pt(),trackOuter->Pt());
      fPtRel1PtUncertaintyChi2PerClusTPC->Fill(pt,relUncertainty1Pt,chi2PerClusterTPC);
      fPtITSminPtTPCvsPtITSChi2PerNClusTPC->Fill(pt,(1./pt-1./ptTPC)/(1./pt),chi2PerClusterTPC);
      if(nPointITS>0) fPtITSminPtTPCvsPtITSChi2PerNClusITS->Fill(pt,(1./pt-1./ptTPC)/(1./pt),track->GetITSchi2()/nPointITS);
    }//fTrackCutsITS loop
      
  }//ESD track loop
   
  // Post output data
  PostData(0, fHistList);
  PostData(1, fHistListTPC);
  PostData(2, fHistListITS);

}
//________________________________________________________________________
void AliPWG4HighPtQATPConly::Terminate(Option_t *)
{

}

#endif
