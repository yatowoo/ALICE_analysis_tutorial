/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

#ifndef AliAnalysisTaskMyTask_H
#define AliAnalysisTaskMyTask_H

#include "AliAnalysisTaskSE.h"

#include<map>

class AliAnalysisTaskMyTask : public AliAnalysisTaskSE  
{
    public:
                                AliAnalysisTaskMyTask();
                                AliAnalysisTaskMyTask(const char *name);
        virtual                 ~AliAnalysisTaskMyTask();

        virtual void            UserCreateOutputObjects();
        virtual void            UserExec(Option_t* option);
        virtual void            Terminate(Option_t* option);

    private:
        AliAODEvent*            fAOD;           //! input event
        TList*                  fOutputList;    //! output list
        TH1F*                   fHistPt;        //! dummy histogram
        TH1I*                   fHistTriggerOffline; // BITs in offline trigger
        TH1F*                   fHistTriggerClass; // Trigger class codes
        TH1F*                   fHistTrackEta;  // Eta of tracks
        TH1I*                   fHistTpcNClusters; // TPC Cluster numbers for each track
        TH1F*                   fHistTpcChi2PerCluster; // TPC Chi-square per cluster
        TH1F*                   fHistDcaX;      // DCA X of track kIsDCA
        TH1F*                   fHistDcaY;      // DCA X of track kIsDCA
        TH1F*                   fHistDcaZ;      // DCA X of track kIsDCA
        TH1F*                   fHistDcaXY;      // DCA \sqrt(X^2+Y^2) of track kIsDCA
        TH1F*                   fHistVertexZ;   // Primary Vertex Z
        TH1F*                   fHistVertexZSpd;   // TPC Primary Vertex Z
        TH1F*                   fHistVertexZTpc;   // SPD Primary Vertex Z
        TH1F*                   fHistMultNtracklets;   // Number of tracklets
        TH1F*                   fHistMultESDTracks;   // Number of ESD tracks
        TH1F*                   fHistMultV0A;   // Number of tracklets
        TH1F*                   fHistMultV0C;   // Number of tracklets
        TH2F*                   fH2MultVtxZ;   // Multiplicity distribution as function of vertex Z
        TH1F*                   fHistEmcalMB; // EMCal energy in MB
        TH1F*                   fHistEmcalEG1; // EMCal energy in EG1
        TH1F*                   fHistEmcalEG2; // EMCal energy in EG2
        AliAnalysisTaskMyTask(const AliAnalysisTaskMyTask&); // not implemented
        AliAnalysisTaskMyTask& operator=(const AliAnalysisTaskMyTask&); // not implemented

        ClassDef(AliAnalysisTaskMyTask, 1);
};

#endif
