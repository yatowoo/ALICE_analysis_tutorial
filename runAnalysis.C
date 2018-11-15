#if !defined (__CINT__) || defined (__CLING__)
#include "AliAnalysisAlien.h"
#include "AliAnalysisManager.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisTaskMyTask.h"
#endif

void runAnalysis()
{
    // set if you want to run the analysis locally (kTRUE), or on grid (kFALSE)
    Bool_t local = kTRUE;
    // if you run on grid, specify test mode (kTRUE) or full grid model (kFALSE)
    Bool_t gridTest = kFALSE;
    
    // since we will compile a class, tell root where to look for headers  
#if !defined (__CINT__) || defined (__CLING__)
    gInterpreter->ProcessLine(".include $ROOTSYS/include");
    gInterpreter->ProcessLine(".include $ALICE_ROOT/include");
#else
    gROOT->ProcessLine(".include $ROOTSYS/include");
    gROOT->ProcessLine(".include $ALICE_ROOT/include");
#endif

    // create the analysis manager
    AliAnalysisManager *mgr = new AliAnalysisManager("PreAnalysis");
    //AliAODInputHandler *aodH = new AliAODInputHandler();
    //mgr->SetInputEventHandler(aodH);

    // From LEGO train DQ_pp_AOD
    gROOT->LoadMacro(gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/train/AddAODHandler.C"));
    AliVEventHandler *handler = AddAODHandler();

    gROOT->LoadMacro(gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/train/AddAODOutputHandler.C"));
    AliVEventHandler *handler = AddAODOutputHandler();
    AliAnalysisManager::SetGlobalStr("kJetDeltaAODName", "");
    AliAnalysisManager::SetGlobalInt("kFillAODForRun", 0);
    AliAnalysisManager::SetGlobalInt("kFilterAOD", 0);
    ((AliAODHandler *)handler)->SetFillAODforRun(kFALSE);
    ((AliAODHandler *)handler)->SetNeedsHeaderReplication();
//((AliAODHandler*)handler)->SetNeedsTracksBranchReplication();
//((AliAODHandler*)handler)->SetNeedsVerticesBranchReplication();
//((AliAODHandler*)handler)->SetCreateNonStandardAOD();

// compile the class and load the add task macro
// here we have to differentiate between using the just-in-time compiler
// from root6, or the interpreter of root5
#if !defined (__CINT__) || defined (__CLING__)
    gInterpreter->LoadMacro("AliAnalysisTaskMyTask.cxx++g");
    AliAnalysisTaskMyTask *task = reinterpret_cast<AliAnalysisTaskMyTask*>(gInterpreter->ExecuteMacro("AddMyTask.C"));
#else
    gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
    AddTaskPhysicsSelection(kFALSE, kTRUE);

    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
    AddTaskPIDResponse();

    gROOT->LoadMacro("AddTask_cjahnke_JPsi.C");
    AddTask_cjahnke_JPsi("16l", 3, kFALSE, "ConfigJpsi_cj_pp", kFALSE, kFALSE, 0);
#endif


    if(!mgr->InitAnalysis()) return;
    mgr->SetDebugLevel(2);
    mgr->PrintStatus();
    mgr->SetUseProgressBar(1, 25);

    if(local) {
        // if you want to run locally, we need to define some input
        TChain* chain = new TChain("aodTree");
        // add a few files to the chain (change this so that your local files are added)
        chain->Add("AliAODEvent.root");
        // start the analysis locally, reading the events from the tchain
        mgr->StartAnalysis("local", chain);
    } else {
        // if we want to run on grid, we create and configure the plugin
        AliAnalysisAlien *alienHandler = new AliAnalysisAlien();
        // also specify the include (header) paths on grid
        alienHandler->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");
        // make sure your source files get copied to grid
        alienHandler->SetAdditionalLibs("AddTask_cjahnke_JPsi.C ConfigJpsi_cj_pp.C");
        // select the aliphysics version. all other packages
        // are LOADED AUTOMATICALLY!
        alienHandler->SetAliPhysicsVersion("vAN-20180305-1");
        // set the Alien API version
        alienHandler->SetAPIVersion("V1.1x");
        // select the input data
        alienHandler->SetGridDataDir("/alice/data/2016/LHC16l");
        alienHandler->SetDataPattern("*/pass1/AOD/*AOD.root");
        // MC has no prefix, data has prefix 000
        alienHandler->SetRunPrefix("000");
        // runnumber
        alienHandler->AddRunNumber("259888");
        // number of files per subjob
        alienHandler->SetSplitMaxInputFileNumber(20);
        alienHandler->SetAnalysisMacro("cjAnalysis.C");
        alienHandler->SetExecutable("cjTask.sh");
        // specify how many seconds your job may take
        alienHandler->SetTTL(20000);
        alienHandler->SetJDLName("cjTask.jdl");

        alienHandler->SetOutputToRunNo(kTRUE);
        alienHandler->SetKeepLogs(kTRUE);
        // merging: run with kTRUE to merge on grid
        // after re-running the jobs in SetRunMode("terminate") 
        // (see below) mode, set SetMergeViaJDL(kFALSE) 
        // to collect final results
        alienHandler->SetMaxMergeStages(1);
        alienHandler->SetMergeViaJDL(kTRUE);

        // define the output folders
        alienHandler->SetGridWorkingDir("16l_PreAna_CJ_EG1_test");
        alienHandler->SetGridOutputDir("OutputAOD");

        // connect the alien plugin to the manager
        mgr->SetGridHandler(alienHandler);
        if(gridTest) {
            // speficy on how many files you want to run
            alienHandler->SetNtestFiles(1);
            // and launch the analysis
            alienHandler->SetRunMode("test");
            mgr->StartAnalysis("grid");
        } else {
            // else launch the full grid analysis
            alienHandler->SetRunMode("full");
            mgr->StartAnalysis("grid");
        }
    }
}
