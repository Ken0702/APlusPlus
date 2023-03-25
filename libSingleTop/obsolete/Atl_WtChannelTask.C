//____________________________________________________________________
//
// Single-top W-channel analysis task
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_Atl_WtChannelTask
#define SINGLETOP_Atl_WtChannelTask
#ifndef ATLAS_AtlAppAnalysisTask
#include <AtlAppAnalysisTask.h>
#endif
#ifndef ATLAS_AtlTopPairFinder
#include <AtlTopPairFinder.h>
#endif
#ifndef ATLAS_AtlSgTop_WtChannelFinder
#include <AtlSgTop_WtChannelFinder.h>
#endif
#ifndef ATLAS_AtlCutFlowTool
#include <AtlCutFlowTool.h>
#endif
#ifndef ATLAS_AtlPdfReweightingTool
#include <AtlPdfReweightingTool.h>
#endif
#include <TObjArray.h>
#include <TObjString.h>
#include <TString.h>
#include <Atl_WtChannelAnalysis.h>
#include <AtlSelector.h>

class Atl_WtChannelTask : public AtlAppAnalysisTask {
    
public:
    Atl_WtChannelTask(const char* name, const char* title);
    virtual ~Atl_WtChannelTask();
    virtual void InitSelector();
    TString n_jets_excl;
    TString n_jets_min;
    TString n_jets_max;
    TString pchi2_min_pc;

    ClassDef(Atl_WtChannelTask,0) // Single-top t-channel analysis task
};
#endif

#ifndef __CINT__
ClassImp(Atl_WtChannelTask);
#endif

//____________________________________________________________________

Atl_WtChannelTask::Atl_WtChannelTask(const char* name,
				   const char* title) :
    AtlAppAnalysisTask(name, title) {
    //
    // Normal constructor
    //
    SetInputTreeName("physics");
    AddUserEnv("LIBSINGLETOP");
}

//____________________________________________________________________

Atl_WtChannelTask::~Atl_WtChannelTask() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void Atl_WtChannelTask::InitSelector() {
    //
    // Initialise the t-channel analysis selector
    //
    if ( fGridJob ) {
	fSelector = new Atl_WtChannelAnalysis(fGridAnalysisName->Data());
    } else {
	fSelector = new Atl_WtChannelAnalysis(fOutputFileName->Data());
    }

    // Jet settings
    SetCut("fJetsAuthor","AtlJet::kAntiKt4TopoEM");
    // Set event level Etmiss cut
    SetCut("fMET_Mag_min","25.");

    //
    // Choose jet bin of analysis
    //
    n_jets_excl = "3"; // 2,3,4
    n_jets_min  = "3"; // 2,3,4
    n_jets_max  = "3"; // 2,3,4

    // P(Chi2) cut in t-channel finder
    TString pchi2_min    = "0.1";// 0.1, 0.2
    pchi2_min_pc = "10";
 
    // =========================
    // Append PDFReweightingTool
    // =========================

    // For PDF reweighting systematic
    AtlPdfReweightingTool *pdf_tool = new AtlPdfReweightingTool("AtlPdfReweightingTool",
								"Pdf Reweighting Tool for Pdf Systematics");
    fSelector->AddTool(pdf_tool);

    // ==================
    // Append CutFlowTool
    // ==================

    // For kinfitter analysis
    AtlCutFlowTool *CF_tool = new AtlCutFlowTool("AtlCutFlowTool","Cut-flow Tool");
    SetToolCut("AtlCutFlowTool", "fNBins", "11");    
    SetToolCut("AtlCutFlowTool", "fLabelsDeflate", "kFALSE");
    AtlCutFlowTool *CF_tool_AC = new AtlCutFlowTool("AtlCutFlowTool_AC","Cut-flow Tool");
    SetToolCut("AtlCutFlowTool_AC", "fNBins", "12");    
    SetToolCut("AtlCutFlowTool_AC", "fLabelsDeflate", "kFALSE");

    fSelector->AddTool(CF_tool);
    fSelector->AddTool(CF_tool_AC);
    
    // ======================
    // Add TopPairFinder tool
    // ======================    
    AtlTopPairFinder *TopPairKinFit = new AtlTopPairFinder("AtlTopPairFinder",
							   "Top Pair Reconstruction by KinFitter");
    
    SetToolCut("AtlTopPairFinder", "fTmassconstraint", "AtlTopPairFinder::kSameTmass");
    SetToolCut("AtlTopPairFinder", "fApplyBTagInReco", "kTRUE");
    SetToolCut("AtlTopPairFinder", "fBJets_N_Min",     "1");
    SetToolCut("AtlTopPairFinder", "fBJetTagger",      "AtlBTag::kMV1");
//    SetToolCut("AtlTopPairFinder", "fBJetWeightMin",   "0.602");
    SetToolCut("AtlTopPairFinder", "fMin_KFJet_Pt",    "25."); // was 25., 20.
    SetToolCut("AtlTopPairFinder", "fMax_KFJet_Eta",   "2.5");
    SetToolCut("AtlTopPairFinder", "fJets_N_Max",      n_jets_excl);
    SetToolCut("AtlTopPairFinder", "fIterMax",         "100");
    SetToolCut("AtlTopPairFinder", "fAlwaysFillHistos","kTRUE"); 
    fSelector->AddTool(TopPairKinFit);

        

    // ============================================
    // Add KinFitter-mode of sgtop t-channel finder
    // ============================================
    AtlSgTop_WtChannelFinder *finder = new AtlSgTop_WtChannelFinder("AtlSgTop_WtChannelFinder","Single-Top Wt-Channel Reconstruction by Finder Tool");

    // Running over signal MC?
    SetToolCut("AtlSgTop_WtChannelFinder", "fSignalSampleMode", "kTRUE");

    // Doing acceptance challenge?
    SetToolCut("AtlSgTop_WtChannelFinder", "fIsAcceptanceChallenge", "kFALSE");
    
//     // Set finder's verbosity level
//     SetToolCut("AtlSgTop_WtChannelFinder", "fVerbosityLevel", "0");
    
//     SetToolCut("AtlSgTop_WtChannelFinder", "fMode",     "AtlKinFitterTool::kKinFit");
//     SetToolCut("AtlSgTop_WtChannelFinder", "fModeMass", "AtlKinFitterTool::kGauss");// kGauss,kBW
//     SetToolCut("AtlSgTop_WtChannelFinder", "fMaxNbIter", "100");
   
    /////////////////////////////////////////////////////////////////
    // Switch on needed vetos
    // !!!!!! doesnt exist yet
    //SetToolCut("AtlSgTop_WtChannelFinder", "fDoTtbarVeto","kTRUE");
    /////////////////////////////////////////////////////////////////
    
    // Set P(chi2) cuts for top fit and Whad veto
    SetToolCut("AtlSgTop_WtChannelFinder", "fChi2ProbMin",pchi2_min);

// MOVED TO electron/muon task since became 30 GeV (el) and 25 GeV (mu)    
//    // Set event level Etmiss cut
//    SetToolCut("AtlSgTop_WtChannelFinder", "fMET_Mag_min","25.");

    // Set further options
    SetToolCut("AtlSgTop_WtChannelFinder", "fGuessNeutrinoEtaE", "kFALSE");
    SetToolCut("AtlSgTop_WtChannelFinder", "fTruthMatch", "kFALSE");

    
    // ====
    // Jets
    // ====

    // Jet bins
    SetToolCut("AtlSgTop_WtChannelFinder","fNJetsMin", n_jets_excl);
    SetToolCut("AtlSgTop_WtChannelFinder","fNJetsMax", n_jets_excl);
    SetToolCut("AtlSgTop_WtChannelFinder","fNJetsExcl",n_jets_excl);
    
    
    // B-jet cuts (in finder)
    SetToolCut("AtlSgTop_WtChannelFinder","fBJetTagger","AtlBTag::kMV1");
//    SetToolCut("AtlSgTop_WtChannelFinder","fBJetWeightMin","0.602");// 2.2 - 57% w.p // 0.35 - 70% w.p.
    SetToolCut("AtlSgTop_WtChannelFinder","fBJets_Et_min","25.0");
    SetToolCut("AtlSgTop_WtChannelFinder","fBJets_Eta_min","-2.5");
    SetToolCut("AtlSgTop_WtChannelFinder","fBJets_Eta_max","2.5");

    // Whad jets (in finder)
    SetToolCut("AtlSgTop_WtChannelFinder","fWhadJets_Et_min", "25.0"); // JES only up to 25 GeV... (was 25., 20.)
    SetToolCut("AtlSgTop_WtChannelFinder","fWhadJets_Eta_min","-2.5"); // was 2.5 (both)
    SetToolCut("AtlSgTop_WtChannelFinder","fWhadJets_Eta_max","2.5");  // here, one could try to lower further (|eta| < 1.5 etc. --  needs some study)
     
    fSelector->AddTool(finder);
    
}
