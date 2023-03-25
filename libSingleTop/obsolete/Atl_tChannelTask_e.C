//____________________________________________________________________
//
// Single-top t-channel (W to electron) analysis task
// 
// Author: Ruth Herrberg <mailto: ruthh@physik.hu-berlin.de>  
// Copyright: 2010 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_Atl_tChannelTask_e
#define SINGLETOP_Atl_tChannelTask_e
#ifndef SINGLETOP_Atl_tChannelTask
#include <Atl_tChannelTask.C>
#endif


class Atl_tChannelTask_e : public Atl_tChannelTask {
    
public:
    Atl_tChannelTask_e(const char* name, const char* title);
    virtual ~Atl_tChannelTask_e();
    virtual void InitSelector();
    
    ClassDef(Atl_tChannelTask_e,0) // Single-top t-channel (W to electron) analysis task
};
#endif

#ifndef __CINT__
ClassImp(Atl_tChannelTask_e);
#endif

//____________________________________________________________________

Atl_tChannelTask_e::Atl_tChannelTask_e(const char* name,
				   const char* title) :
    Atl_tChannelTask(name, title) {
    //
    // Normal constructor
    //
//    SetInputTreeName("t_app");
    SetInputTreeName("physics");
    AddUserEnv("LIBSINGLETOP");
}

//____________________________________________________________________

Atl_tChannelTask_e::~Atl_tChannelTask_e() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void Atl_tChannelTask_e::InitSelector() {
    //
    // Initialise the t-channel analysis selector
    //

    Atl_tChannelTask::InitSelector();

    
    // Set lepton-mode of sgtop t-channel analysis
    SetCut("fLeptonMode","Atl_tChannelAnalysis::kElectronMode");

    // Get correct cut flow for electron case
    
    // for case 1Jet:
    if ( Atl_tChannelTask::n_jets_max == "1" ) {
	SetToolCut("AtlCutFlowTool", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,H_{T} > 160 GeV\"");
	SetToolCut("AtlCutFlowTool2", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,H_{T} > 160 GeV\"");
	// for case 2Jets:
    } else if ( Atl_tChannelTask::n_jets_max == "2" ) {
	SetToolCut("AtlCutFlowTool", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,H_{T} > 160 GeV,Convergence,t#bar{t} Veto,W_{had} Veto,P(#chi^{2}) > "+Atl_tChannelTask::pchi2_min_pc+"%\"");
	SetToolCut("AtlCutFlowTool2", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,H_{T} > 160 GeV,N_Jets = 2,N_BJets = 1\"");
	// for case 3Jets:
    } else if ( Atl_tChannelTask::n_jets_max == "3" ) {
	SetToolCut("AtlCutFlowTool", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,H_{T} > 160 GeV,Convergence,t#bar{t} Veto,W_{had} Veto,P(#chi^{2}) > "+Atl_tChannelTask::pchi2_min_pc+"%\"");
	SetToolCut("AtlCutFlowTool2", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,H_{T} > 160 GeV,N_Jets = 2,N_BJets = 1\"");
	// for case 4Jets:
    } else {
	SetToolCut("AtlCutFlowTool", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,Convergence,t#bar{t} Veto,W_{had} Veto,P(#chi^{2}) > "+Atl_tChannelTask::pchi2_min_pc+"%\"");
	SetToolCut("AtlCutFlowTool2", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,N_Jets = 2,N_BJets = 1\"");
    }

//     // for case 2Jets:
//     if ( Atl_tChannelTask::n_jets_max == "2" ) {
// 	SetToolCut("AtlCutFlowTool", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,H_{T} > 160 GeV,#Delta#eta(BJet-LightJet) > 0.8,Convergence,t#bar{t} Veto,W_{had} Veto,P(#chi^{2}) > "+Atl_tChannelTask::pchi2_min_pc+"%,#eta_{Top} < 3.2,#eta_{W} < 3,p_{T}^{#nu} > 26 GeV,|p_{T}^{top}+p_{T}^{LeadFwdJet}| < 50 GeV,#Delta R(lepton-bjet) < 3.3\"");
// 	SetToolCut("AtlCutFlowTool2", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,H_{T} > 160 GeV,#Delta#eta(BJet-LightJet) > 0.8,N_Jets = 2,N_BJets = 1\"");
// 	// for case 3Jets:
//     } else if ( Atl_tChannelTask::n_jets_max == "3" ) {
// 	SetToolCut("AtlCutFlowTool", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,H_{T} > 160 GeV,Convergence,t#bar{t} Veto,W_{had} Veto,P(#chi^{2}) > "+Atl_tChannelTask::pchi2_min_pc+"%,#eta_{Top} < 3.2,#eta_{W} < 3,p_{T}^{#nu} > 26 GeV,|p_{T}^{top}+p_{T}^{LeadFwdJet}| < 50 GeV,#Delta R(lepton-bjet) < 3.3\"");
// 	SetToolCut("AtlCutFlowTool2", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,H_{T} > 160 GeV,N_Jets = 2,N_BJets = 1\"");
// 	// for case 4Jets:
//     } else {
// 	SetToolCut("AtlCutFlowTool", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,Convergence,t#bar{t} Veto,W_{had} Veto,P(#chi^{2}) > "+Atl_tChannelTask::pchi2_min_pc+"%,#eta_{Top} < 3.2,#eta_{W} < 2,p_{T}^{#nu} > 26 GeV,|p_{T}^{top}+p_{T}^{LeadFwdJet}| < 110 GeV,#Delta R(lepton-bjet) < 3.3,M_{eff} < 500\"");
// 	SetToolCut("AtlCutFlowTool2", "fBinLabels", "\"No Cuts (D3PD),Etmiss > "+Atl_tChannelTask::etmiss_min+" GeV,Signal Lepton,No. of Jets #geq "+Atl_tChannelTask::n_jets_min+",No. of Jets #leq "+Atl_tChannelTask::n_jets_max+",Mt(W) > "+Atl_tChannelTask::m_W_T_min+" GeV,b-Jet,1 b-Tag,N_Jets = 2,N_BJets = 1\"");
//     }

}
