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

/* AliAnaysisTaskMyTask
 *
 * empty task which can serve as a starting point for building an analysis
 * as an example, one histogram is filled
 */

#include "TChain.h"
#include "TH1F.h"
#include "TList.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisTaskMyTask.h"

class AliAnalysisTaskMyTask;    // your analysis class

using namespace std;            // std namespace: so you can do things like 'cout'

ClassImp(AliAnalysisTaskMyTask) // classimp: necessary for root

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask() : AliAnalysisTaskSE(), 
    fAOD(0), fOutputList(0), fHistPt(0), fHistTriggerOffline(0), fHistTriggerClass(0), fHistTrackEta(0), fHistTpcNClusters(0), fHistTpcChi2PerCluster(0), fHistDcaX(0), fHistDcaY(0), fHistDcaZ(0), fHistDcaXY(0), fHistVertexZ(0), fHistVertexZSpd(0), fHistVertexZTpc(0), fHistMultNtracklets(0), fHistMultESDTracks(0), fHistMultV0A(0), fHistMultV0C(0)
{
    // default constructor, don't allocate memory here!
    // this is used by root for IO purposes, it needs to remain empty
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(const char* name) : AliAnalysisTaskSE(name),
    fAOD(0), fOutputList(0), fHistPt(0), fHistTriggerOffline(0), fHistTriggerClass(0), fHistTrackEta(0), fHistTpcNClusters(0), fHistTpcChi2PerCluster(0), fHistDcaX(0), fHistDcaY(0), fHistDcaZ(0), fHistDcaXY(0), fHistVertexZ(0), fHistVertexZSpd(0), fHistVertexZTpc(0), fHistMultNtracklets(0), fHistMultESDTracks(0), fHistMultV0A(0), fHistMultV0C(0)
{
    // constructor
    DefineInput(0, TChain::Class());    // define the input of the analysis: in this case we take a 'chain' of events
                                        // this chain is created by the analysis manager, so no need to worry about it, 
                                        // it does its work automatically
    DefineOutput(1, TList::Class());    // define the ouptut of the analysis: in this case it's a list of histograms 
                                        // you can add more output objects by calling DefineOutput(2, classname::Class())
                                        // if you add more output objects, make sure to call PostData for all of them, and to
                                        // make changes to your AddTask macro!
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::~AliAnalysisTaskMyTask()
{
    // destructor
    if(fOutputList) {
        delete fOutputList;     // at the end of your task, it is deleted from memory by calling this function
    }
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::UserCreateOutputObjects()
{
    // create output objects
    //
    // this function is called ONCE at the start of your analysis (RUNTIME)
    // here you ceate the histograms that you want to use 
    //
    // the histograms are in this case added to a tlist, this list is in the end saved
    // to an output file
    //
    fOutputList = new TList();          // this is a list which will contain all of your histograms
                                        // at the end of the analysis, the contents of this list are written
                                        // to the output file
    fOutputList->SetOwner(kTRUE);       // memory stuff: the list is owner of all objects it contains and will delete them
                                        // if requested (dont worry about this now)

    // example of a histogram
    fHistPt = new TH1F("fHistPt", "fHistPt", 100, 0, 10);       // create your histogra
    fHistTriggerOffline = new TH1I("hTrig", "BITs in Offline Trigger", 32, 0, 32);
    fHistTriggerClass = new TH1F("hTC","Trigger class codes", 3, 0, 3);
    fHistTrackEta = new TH1F("hEta","Eta of tracks", 200, -10, 10);
    fHistTpcNClusters = new TH1I("hTpcNcls", "Number of TPC clusters",160, 0, 160);
    fHistTpcChi2PerCluster = new TH1F("hTpcChi2", "TPC chi-square per cluster", 1000, 0, 1000);
    fHistDcaX = new TH1F("hDcaX", "Track DCA X position (kIsDCA)", 200, -10, 10);
    fHistDcaY = new TH1F("hDcaY", "Track DCA Y position (kIsDCA)", 200, -10, 10);
    fHistDcaZ = new TH1F("hDcaZ", "Track DCA Z position (kIsDCA)", 200, -10, 10);
    fHistDcaXY = new TH1F("hDcaXY", "Track DCA XY length (kIsDCA)", 200, 0, 20);
    fHistVertexZ = new TH1F("hVtxZ", "Primary vertex Z",200, -100, 100);
    fHistVertexZSpd = new TH1F("hVtxZSpd", "Primary vertex Z (SPD)",200, -100, 100);
    fHistVertexZTpc = new TH1F("hVtxZTpc", "Primary vertex Z (TPC)",200, -100, 100);
    fHistMultNtracklets = new TH1F("hMultNtr", "Multiplicity with number of tracklets", 1000, 0, 1000);
    fHistMultESDTracks = new TH1F("hMultESD", "Multiplicity with number of ESD tracks", 1000, 0, 1000);
    fHistMultV0A = new TH1F("hMultV0A", "Multiplicity with number of V0 amplitude - A side", 1000, 0, 1000);
    fHistMultV0C = new TH1F("hMultV0C", "Multiplicity with number of V0 amplitude - C side", 1000, 0, 1000);
    fOutputList->Add(fHistTrackEta);
    fOutputList->Add(fHistTpcNClusters);
    fOutputList->Add(fHistTpcChi2PerCluster);
    fOutputList->Add(fHistDcaX);
    fOutputList->Add(fHistDcaY);
    fOutputList->Add(fHistDcaZ);
    fOutputList->Add(fHistDcaXY);
    fOutputList->Add(fHistVertexZ);
    fOutputList->Add(fHistVertexZSpd);
    fOutputList->Add(fHistVertexZTpc);
    fOutputList->Add(fHistMultNtracklets);
    fOutputList->Add(fHistMultESDTracks);
    fOutputList->Add(fHistMultV0A);
    fOutputList->Add(fHistMultV0C);
    fOutputList->Add(fHistTriggerClass);
    fOutputList->Add(fHistTriggerOffline);
    fOutputList->Add(fHistPt);          // don't forget to add it to the list! the list will be written to file, so if you want
                                        // your histogram in the output file, add it to the list!
    
    PostData(1, fOutputList);           // postdata will notify the analysis manager of changes / updates to the 
                                        // fOutputList object. the manager will in the end take care of writing your output to file
                                        // so it needs to know what's in the output
}
//_____________________________________________________________________________
#include<iostream>
void AliAnalysisTaskMyTask::UserExec(Option_t *)
{
    // user exec
    // this function is called once for each event
    // the manager will take care of reading the events from file, and with the static function InputEvent() you 
    // have access to the current event. 
    // once you return from the UserExec function, the manager will retrieve the next event from the chain
    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());    // get an event (called fAOD) from the input file
                                                        // there's another event format (ESD) which works in a similar wya
                                                        // but is more cpu/memory unfriendly. for now, we'll stick with aod's
    if(!fAOD) return;                                   // if the pointer to the event is empty (getting it failed) skip this event
        // example part: i'll show how to loop over the tracks in an event 
        // and extract some information from them which we'll store in a histogram
    Int_t iTracks(fAOD->GetNumberOfTracks());           // see how many tracks there are in the event
    for(Int_t i(0); i < iTracks; i++) {                 // loop ove rall these tracks
        AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));         // get a track (type AliAODTrack) from the event
        if(!track) continue;
        if(track->TestFilterBit(AliAODTrack::kTrkTPCOnly)){                 // Pre-defined trackt cut
            fHistPt->Fill(track->Pt());                     // plot the pt value of the track in a histogram
            fHistTrackEta->Fill(track->Eta());
            fHistTpcNClusters->Fill(track->GetTPCNcls());
            if(track->GetTPCNcls() > 0)
                fHistTpcChi2PerCluster->Fill(track->GetTPCchi2()/track->GetTPCNcls());
            else
                fHistTpcChi2PerCluster->Fill(-1); // under flow - Bin(0)
        }// END - TPC only tracks

        // Check DCA to avoid dummy values
        if(track->TestBit(AliAODTrack::kIsDCA)){
            Double_t x = track->XAtDCA();
            Double_t y = track->YAtDCA();
            Double_t z = track->ZAtDCA();
            Double_t xy = TMath::Sqrt(x*x+y*y);
            fHistDcaX->Fill(x);
            fHistDcaY->Fill(y);
            fHistDcaZ->Fill(z);
            fHistDcaXY->Fill(xy);
        }
    }// END - Track
    
    // Primary vertex
    fHistVertexZ->Fill(fAOD->GetPrimaryVertex()->GetZ());
    fHistVertexZSpd->Fill(fAOD->GetPrimaryVertexSPD()->GetZ());
    fHistVertexZTpc->Fill(fAOD->GetPrimaryVertexTPC()->GetZ());

    // Multiplicity
    fHistMultNtracklets->Fill(fAOD->GetMultiplicity()->GetNumberOfTracklets());
    fHistMultESDTracks->Fill(fAOD->GetNumberOfESDTracks());
    fHistMultV0A->Fill(fAOD->GetVZEROData()->GetMTotV0A());
    fHistMultV0C->Fill(fAOD->GetVZEROData()->GetMTotV0C());

    // Fill offline trigger histogram
    ULong64_t  trigOffline = static_cast<AliVAODHeader*>(fAOD->GetHeader())->GetOfflineTrigger();
    if(!trigOffline) fHistTriggerOffline->Fill(0);
    for(int i = 0 ; i < 32 && (trigOffline>>i); i++){
        if((trigOffline >> i) % 2){
            fHistTriggerOffline->Fill(i);
        }
    }// Loop for all bits
    // END - Fill offline trigger histogram

    // Handle string of fired trigger classes
    TString trigClassed = fAOD->GetFiredTriggerClasses();
    TObjArray* arr = trigClassed.Tokenize(" ");
    for(int i = 0; i < arr->GetEntries(); i++){
        TObjString* token = (TObjString*)(arr->At(i));
        fHistTriggerClass->Fill(token->String(), 1);
    }
    arr->Delete();
    delete arr;
    // END - Handle string of fired trigger classes

                                                       // continue until all the tracks are processed
    PostData(1, fOutputList);                           // stream the results the analysis of this event to
                                                        // the output manager which will take care of writing
                                                        // it to a file

}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::Terminate(Option_t *)
{
    // terminate
    // called at the END of the analysis (when all events are processed)
}
//_____________________________________________________________________________
