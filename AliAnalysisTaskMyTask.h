/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

#ifndef AliAnalysisTaskMyTask_H
#define AliAnalysisTaskMyTask_H

#include "AliAnalysisTaskSE.h"
#include "AliPIDResponse.h"

class AliPIDResponse;

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
        TH1F*                   fHistVertexZ;   // Step 3
        TH1F*                   fHistCentral;   // Step 3
        TH2F*                   fH2TPCsignal;   // Step 5 - before PID
        TH2F*                   fH2PionTPC;     // Step 5 - PID
        TH1F*                   fHistPionPt;    // Step 5 - PID
        TH1F*                   fHistPionEta;    // Step 5 - PID
        TH1F*                   fHistPionPhi;    // Step 5 - PID
        AliPIDResponse*         fPIDResponse;   // Step 5 - PID

        AliAnalysisTaskMyTask(const AliAnalysisTaskMyTask&); // not implemented
        AliAnalysisTaskMyTask& operator=(const AliAnalysisTaskMyTask&); // not implemented

        ClassDef(AliAnalysisTaskMyTask, 1);
};

#endif
