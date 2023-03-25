//____________________________________________________________________
//
// Single-top Wt-channel (W to electron) analysis task
// 
// Author: Michelangeo Giorgi <mailto: strider@physik.hu-berlin.de>  
// Copyright: 2010 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_Atl_WtChannelTask_e
#define SINGLETOP_Atl_WtChannelTask_e
#ifndef SINGLETOP_Atl_WtChannelTask
#include <Atl_WtChannelTask.C>
#endif


class Atl_WtChannelTask_e : public Atl_WtChannelTask {
    
public:
    Atl_WtChannelTask_e(const char* name, const char* title);
    virtual ~Atl_WtChannelTask_e();
    virtual void InitSelector();
    
    ClassDef(Atl_WtChannelTask_e,0) // Single-top Wt-channel (W to electron) analysis task
};
#endif

#ifndef __CINT__
ClassImp(Atl_WtChannelTask_e);
#endif

//____________________________________________________________________

Atl_WtChannelTask_e::Atl_WtChannelTask_e(const char* name,
				   const char* title) :
    Atl_WtChannelTask(name, title) {
    //
    // Normal constructor
    //
//    SetInputTreeName("t_app");
    SetInputTreeName("physics");
    AddUserEnv("LIBSINGLETOP");
}

//____________________________________________________________________

Atl_WtChannelTask_e::~Atl_WtChannelTask_e() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void Atl_WtChannelTask_e::InitSelector() {
    //
    // Initialise the Wt-channel analysis selector
    //
    // Set event level Etmiss cut
    SetToolCut("AtlSgTop_WtChannelFinder", "fMET_Mag_min","30.");

    Atl_WtChannelTask::InitSelector();

    
    // Set lepton-mode of sgtop Wt-channel analysis
    // Get correct cut-flow for electron case
    SetCut("fMET_Mag_min","30.");
    SetCut("fLeptonMode","Atl_WtChannelAnalysis::kElectronMode");

    SetToolCut("AtlCutFlowTool", "fBinLabels",
 	       "\"No Cuts (D3PD),Signal Lepton,Etmiss > 30 GeV,No. of Jets #geq 1,Mt(W) > 30 GeV,No. of Jets = "+Atl_WtChannelTask::n_jets_excl+",b-Jet,Exactly 1 b-Jet,t#bar{t} Veto,Convergence,P(#chi^{2}) > "+Atl_WtChannelTask::pchi2_min_pc+"%\"");

    SetToolCut("AtlCutFlowTool_AC", "fBinLabels",
 	       "\"No Cuts (D3PD),Signal Lepton,Etmiss > 30 GeV,No. of Jets #geq 1,Mt(W) > 30 GeV,PreTag-No. of Jets = 2,PreTag-No. of Jets = 3,PreTag-No. of Jets = 4,Exactly 1 b-Jet,BTag-No. of Jets = 2,BTag-No. of Jets = 3,BTag-No. of Jets = 4\"");   
}
