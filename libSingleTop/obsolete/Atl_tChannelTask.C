//____________________________________________________________________
//
// Single-top t-channel analysis task
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_Atl_tChannelTask
#define SINGLETOP_Atl_tChannelTask
#ifndef ATLAS_AtlAppAnalysisTask
#include <AtlAppAnalysisTask.h>
#endif
#ifndef ATLAS_AtlTopPairFinder
#include <AtlTopPairFinder.h>
#endif
#ifndef ATLAS_AtlSgTop_tChannelFinder
#include <AtlSgTop_tChannelFinder.h>
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
#include <Atl_tChannelAnalysis.h>
#include <AtlSelector.h>

class Atl_tChannelTask : public AtlAppAnalysisTask {
    
public:
    Atl_tChannelTask(const char* name, const char* title);
    virtual ~Atl_tChannelTask();
    virtual void InitSelector();
    TString n_jets_min;
    TString n_jets_max;
    TString pchi2_min_pc;
    TString m_W_T_min;
    TString etmiss_min;
    
    ClassDef(Atl_tChannelTask,0) // Single-top t-channel analysis task
};
#endif

#ifndef __CINT__
ClassImp(Atl_tChannelTask);
#endif

//____________________________________________________________________

Atl_tChannelTask::Atl_tChannelTask(const char* name,
				   const char* title) :
    AtlAppAnalysisTask(name, title) {
    //
    // Normal constructor
    //
    SetInputTreeName("physics");
    AddUserEnv("LIBSINGLETOP");
}

//____________________________________________________________________
Atl_tChannelTask::~Atl_tChannelTask() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void Atl_tChannelTask::InitSelector() {
    //
    // Initialise the t-channel analysis selector
    //
    if ( fGridJob ) {
	fSelector = new Atl_tChannelAnalysis(fGridAnalysisName->Data());
    } else {
	fSelector = new Atl_tChannelAnalysis(fOutputFileName->Data());
    }

    //
    // Choose jet bin of analysis    
    n_jets_min = "2"; // 1,2,3,4 (1 only for W macros)
    n_jets_max = "2"; // 1,2,3,4 (1 only for W macros)

    
    // P(Chi2) cut in t-channel finder
    TString pchi2_min    = "0.05";// 0.1, 0.2
    pchi2_min_pc = "5";


    // Minimum W transverse mass
    m_W_T_min = "30";
    
    // Minimum Etmiss
    etmiss_min = "30";

    
    // Jet settings
    SetCut("fJetsAuthor","AtlJet::kAntiKt4TopoEM");
    
    // Set event level Etmiss cut
    SetCut("fMET_Mag_min",etmiss_min);

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
    SetToolCut("AtlCutFlowTool", "fLabelsDeflate", "kFALSE");

    if ( n_jets_max == "1" )
	SetToolCut("AtlCutFlowTool", "fNBins", "10");
    else if ( n_jets_max == "2" ) 
	SetToolCut("AtlCutFlowTool", "fNBins", "14");
    else if ( n_jets_max == "3" )
	SetToolCut("AtlCutFlowTool", "fNBins", "14");
    else
	SetToolCut("AtlCutFlowTool", "fNBins", "13");
//     if ( n_jets_max == "2" ) 
// 	SetToolCut("AtlCutFlowTool", "fNBins", "20");
//     else if ( n_jets_max == "3" )
// 	SetToolCut("AtlCutFlowTool", "fNBins", "19");
//     else
// 	SetToolCut("AtlCutFlowTool", "fNBins", "19");

    fSelector->AddTool(CF_tool);
    
    // For cut-based analysis
    AtlCutFlowTool *CF_tool_2 = new AtlCutFlowTool("AtlCutFlowTool2","Cut-flow Tool 2");
    SetToolCut("AtlCutFlowTool2", "fLabelsDeflate", "kFALSE");
    if ( n_jets_max == "1" )
	SetToolCut("AtlCutFlowTool", "fNBins", "10");
    else if ( n_jets_max == "2" ) 
	SetToolCut("AtlCutFlowTool2", "fNBins", "13");
    else if ( n_jets_max == "3" )
	SetToolCut("AtlCutFlowTool2", "fNBins", "12");
    else
	SetToolCut("AtlCutFlowTool2", "fNBins", "11");
//     if ( n_jets_max == "2" ) 
// 	SetToolCut("AtlCutFlowTool2", "fNBins", "13");
//     else if ( n_jets_max == "3" )
// 	SetToolCut("AtlCutFlowTool2", "fNBins", "12");
//     else
// 	SetToolCut("AtlCutFlowTool2", "fNBins", "11");

    fSelector->AddTool(CF_tool_2);


    
    // ======================
    // Add TopPairFinder tool
    // ======================    
    AtlTopPairFinder *TopPairKinFit = new AtlTopPairFinder("AtlTopPairFinder",
							   "Top Pair Reconstruction by KinFitter");
    
    SetToolCut("AtlTopPairFinder", "fTmassconstraint", "AtlTopPairFinder::kSameTmass");
    SetToolCut("AtlTopPairFinder", "fApplyBTagInReco", "kTRUE");
    SetToolCut("AtlTopPairFinder", "fBJets_N_Min",     "1");
    SetToolCut("AtlTopPairFinder", "fBJetTagger",      "AtlBTag::kMV1");
    // SetToolCut("AtlTopPairFinder", "fBJetWeightMin",   "0.905363");
    SetToolCut("AtlTopPairFinder", "fMin_KFJet_Pt",    "30."); // was 25.
    SetToolCut("AtlTopPairFinder", "fMax_KFJet_Eta",   "2.5");
    SetToolCut("AtlTopPairFinder", "fIterMax",         "100");
    SetToolCut("AtlTopPairFinder", "fAlwaysFillHistos","kTRUE");
    SetToolCut("AtlTopPairFinder", "fMinChi2Prob",     "0.001");
    // NOT 1000., but in sync with tChannelFinder instead:
    SetToolCut("AtlTopPairFinder", "fJets_N_Max",      n_jets_max); 
    fSelector->AddTool(TopPairKinFit);

        

    // ============================================
    // Add KinFitter-mode of sgtop t-channel finder
    // ============================================
    AtlSgTop_tChannelFinder *finder = new AtlSgTop_tChannelFinder("AtlSgTop_tChannelFinder","Single-Top t-Channel Reconstruction by Finder Tool");

    // Running over signal MC?
    SetToolCut("AtlSgTop_tChannelFinder", "fSignalSampleMode", "kTRUE");

    // Doing acceptance challenge?
    SetToolCut("AtlSgTop_tChannelFinder", "fIsAcceptanceChallenge", "kFALSE");
    
    // Set finder's verbosity level
    //    SetToolCut("AtlSgTop_tChannelFinder", "fVerbosityLevel", "0");
    
    //    SetToolCut("AtlSgTop_tChannelFinder", "fMode",     "AtlKinFitterTool::kAll");
    //    SetToolCut("AtlSgTop_tChannelFinder", "fModeMass", "AtlKinFitterTool::kGauss");// kGauss,kBW
    SetToolCut("AtlSgTop_tChannelFinder", "fMaxNbIter", "100");
    

    // Switch on needed vetos
    SetToolCut("AtlSgTop_tChannelFinder", "fDoWhadVeto","kTRUE");
    SetToolCut("AtlSgTop_tChannelFinder", "fDoTtbarVeto","kTRUE");
    
    // Set P(chi2) cuts for top fit and Whad veto
    SetToolCut("AtlSgTop_tChannelFinder", "fChi2ProbMin",pchi2_min);
    SetToolCut("AtlSgTop_tChannelFinder", "fVetoWhadProbMin", "0.05");


    // Set further options
    SetToolCut("AtlSgTop_tChannelFinder", "fGuessNeutrinoEtaE", "kFALSE");
    SetToolCut("AtlSgTop_tChannelFinder", "fTruthMatch", "kFALSE");

    // Minimum W transverse mass
    SetToolCut("AtlSgTop_tChannelFinder", "fWmassT_min",m_W_T_min);


    // cut-based analysis cuts
//     SetToolCut("AtlSgTop_tChannelFinder", "fLeadNonBJetEtaMin","2.");
    SetToolCut("AtlSgTop_tChannelFinder", "fH_TMin","160.");
    SetToolCut("AtlSgTop_tChannelFinder", "fDeltaEtaBJetNonBJetMin","0.8");
    SetToolCut("AtlSgTop_tChannelFinder", "fMassAllJetsMin","450.");
    
    // ====
    // Jets
    // ====

    // Jet bins
    SetToolCut("AtlSgTop_tChannelFinder","fNJetsMin",n_jets_min);
    SetToolCut("AtlSgTop_tChannelFinder","fNJetsMax",n_jets_max);
    
    
    // B-jet cuts (in finder)
    SetToolCut("AtlSgTop_tChannelFinder","fBJetTagger","AtlBTag::kMV1");
    // SetToolCut("AtlSgTop_tChannelFinder","fBJetWeightMin","0.905363");
    SetToolCut("AtlSgTop_tChannelFinder","fBJets_Et_min","30.0");
    SetToolCut("AtlSgTop_tChannelFinder","fBJets_Eta_min","-2.5");
    SetToolCut("AtlSgTop_tChannelFinder","fBJets_Eta_max","2.5");

    // Whad jets (in finder)
    SetToolCut("AtlSgTop_tChannelFinder","fWhadJets_Et_min", "30.0"); // JES only up to 25 GeV...
    SetToolCut("AtlSgTop_tChannelFinder","fWhadJets_Eta_min","-2.0"); // was 2.5 (both)
    SetToolCut("AtlSgTop_tChannelFinder","fWhadJets_Eta_max","2.0");  // here, one could try to lower further (|eta| < 1.5 etc. --  needs some study)
    
    // Fwd jets (in finder)
    SetToolCut("AtlSgTop_tChannelFinder","fFwdJets_Et_min","30.0");
    SetToolCut("AtlSgTop_tChannelFinder","fFwdJets_Eta_min","2.5");
    SetToolCut("AtlSgTop_tChannelFinder","fFwdJets_Eta_max","5.");
    SetToolCut("AtlSgTop_tChannelFinder","fFwdJetTagger","AtlBTag::kSV0");
    SetToolCut("AtlSgTop_tChannelFinder","fFwdBJetWeightMax","5.85");
    
    
    fSelector->AddTool(finder);

}
