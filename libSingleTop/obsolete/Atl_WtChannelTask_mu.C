//____________________________________________________________________
//
// Single-top Wt-channel (W to muon) analysis task
//
//  
// Author: Michelangelo Giorgi <mailto: strider@physik.hu-berlin.de>
// Copyright: 2010 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_Atl_WtChannelTask_mu
#define SINGLETOP_Atl_WtChannelTask_mu
#ifndef SINGLETOP_Atl_WtChannelTask
#include <Atl_WtChannelTask.C>
#endif

class Atl_WtChannelTask_mu : public Atl_WtChannelTask {
    
public:
    Atl_WtChannelTask_mu(const char* name, const char* title);
    virtual ~Atl_WtChannelTask_mu();
    virtual void InitSelector();
    
    ClassDef(Atl_WtChannelTask_mu,0) // Single-top Wt-channel (W to muon) analysis task
};
#endif

#ifndef __CINT__
ClassImp(Atl_WtChannelTask_mu);
#endif

//____________________________________________________________________

Atl_WtChannelTask_mu::Atl_WtChannelTask_mu(const char* name,
				   const char* title) :
    Atl_WtChannelTask(name, title) {
    //
    // Normal constructor
    //
    SetInputTreeName("physics");
    AddUserEnv("LIBSINGLETOP");
}

//____________________________________________________________________

Atl_WtChannelTask_mu::~Atl_WtChannelTask_mu() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void Atl_WtChannelTask_mu::InitSelector() {
    //
    // Initialise the t-channel analysis selector
    //
    SetToolCut("AtlSgTop_WtChannelFinder", "fMET_Mag_min","25.");
    Atl_WtChannelTask::InitSelector();

    // Set lepton-mode of sgtop t-channel analysis
    SetCut("fMET_Mag_min","25.");
    SetCut("fLeptonMode","Atl_WtChannelAnalysis::kMuonMode");
    SetToolCut("AtlCutFlowTool", "fBinLabels",
 	       "\"No Cuts (D3PD),Etmiss > 25 GeV,Signal Lepton,No. of Jets #geq 1,Mt(W) + Etmiss > 60 GeV,No. of Jets = "+Atl_WtChannelTask::n_jets_excl+",b-Jet,Exactly 1 b-Jet,t#bar{t} Veto,Convergence,P(#chi^{2}) > "+Atl_WtChannelTask::pchi2_min_pc+"%\"");
    SetToolCut("AtlCutFlowTool_AC", "fBinLabels",
 	       "\"No Cuts (D3PD),Signal Lepton,Etmiss > 25 GeV,No. of Jets #geq 1,Mt(W) + Etmiss > 60 GeV,PreTag-No. of Jets = 2,PreTag-No. of Jets = 3,PreTag-No. of Jets = 4,Exactly 1 b-Jet,BTag-No. of Jets = 2,BTag-No. of Jets = 3,BTag-No. of Jets = 4\"");
}
