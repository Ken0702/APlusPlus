//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtReaderD3PDSgTop
#define ATLAS_AtlEvtReaderD3PDSgTop
#ifndef ATLAS_AtlEvtReaderD3PDBase
#include <AtlEvtReaderD3PDBase.h>
#endif
#include <vector>
#include <map>
#include <TVector2.h>

class AtlEventHeader;
class HepVertex;

class AtlEvtReaderD3PDSgTop : public AtlEvtReaderD3PDBase {

  protected:

    // =========================
    // Declaration of leaf types
    // =========================

    // Event header
    Int_t           v_dataPeriod;
    UInt_t          v_RunNumber;
    UInt_t          v_EventNumber;
    Bool_t          v_isMC;
    UInt_t          v_mc_channel_number;
    Int_t           v_top_hfor_type;
    Float_t         v_averageIntPerXing;
    Double_t        v_mcevt_pdf1;
    Double_t        v_mcevt_pdf2;
    Int_t           v_mcevt_pdf_id1;
    Int_t           v_mcevt_pdf_id2;
    Double_t        v_mcevt_pdf_scale;
    Double_t        v_mcevt_pdf_x1;
    Double_t        v_mcevt_pdf_x2;
    // vertices
    Int_t           v_vxp_n;
    std::vector<float>   *v_vxp_z;

    // Event weights
    Float_t        v_GenWeight;
    Float_t        v_PileupWeight;
    Float_t        v_ZVertexWeight;
    Float_t        v_MMWeight_nominal;
    Float_t        v_MMWeight_el_fake_mc_up;
    Float_t        v_MMWeight_el_fake_mc_down;
    Float_t        v_MMWeight_el_fake_alternate;
    Float_t        v_MMWeight_el_real_alternate;
    Float_t        v_MMWeight_mu_fake_mc_up;
    Float_t        v_MMWeight_mu_fake_mc_down;
    Float_t        v_MMWeight_mu_fake_alternate;
    Float_t        v_MMWeight_mu_real_alternate;
    Float_t        v_TotalWeight;

    // Electrons
    Int_t           v_el_n;
    std::vector<float>   *v_el_charge;
    std::vector<bool>    *v_el_isGood;
    std::vector<int>     *v_el_tightPP;
    std::vector<float>   *v_el_Etcone20;
    std::vector<float>   *v_el_ptcone30;
    std::vector<float>   *v_el_E;
    std::vector<float>   *v_el_eta;
    std::vector<float>   *v_el_phi;
    std::vector<float>   *v_el_pt;
    std::vector<float>   *v_el_trackd0;
    std::vector<float>   *v_el_trackqoverp;
    std::vector<float>   *v_el_tracktheta;
    std::vector<float>   *v_el_trackz0;
    // Covariances
    std::vector<float>   *v_el_trackcov_d0;
    std::vector<float>   *v_el_trackcov_d0_phi;
    std::vector<float>   *v_el_trackcov_d0_qoverp;
    std::vector<float>   *v_el_trackcov_d0_theta;
    std::vector<float>   *v_el_trackcov_d0_z0;
    std::vector<float>   *v_el_trackcov_qoverp;
    std::vector<float>   *v_el_trackcov_phi;
    std::vector<float>   *v_el_trackcov_phi_qoverp;
    std::vector<float>   *v_el_trackcov_phi_theta;
    std::vector<float>   *v_el_trackcov_theta;
    std::vector<float>   *v_el_trackcov_theta_qoverp;
    std::vector<float>   *v_el_trackcov_z0;
    std::vector<float>   *v_el_trackcov_z0_phi;
    std::vector<float>   *v_el_trackcov_z0_qoverp;
    std::vector<float>   *v_el_trackcov_z0_theta;
    // ScaleFactors
    std::vector<float>  *v_el_SF;
    std::vector<float>  *v_el_SF_reco_up;
    std::vector<float>  *v_el_SF_reco_down;
    std::vector<float>  *v_el_SF_Id_up;
    std::vector<float>  *v_el_SF_Id_down;
    std::vector<float>  *v_el_trigger_SF;
    std::vector<float>  *v_el_trigger_SF_up;
    std::vector<float>  *v_el_trigger_SF_down;

    // Muons
    Int_t           v_mu_n;
    std::vector<float>   *v_mu_charge;
    std::vector<bool>    *v_mu_isGood;
    std::vector<float>   *v_mu_etcone20;
    std::vector<float>   *v_mu_ptcone30;
    std::vector<float>   *v_mu_E;
    std::vector<float>   *v_mu_eta;
    std::vector<float>   *v_mu_phi;
    std::vector<float>   *v_mu_pt;
    std::vector<float>   *v_mu_d0_exPV;
    std::vector<float>   *v_mu_phi_exPV;
    std::vector<float>   *v_mu_qoverp_exPV;
    std::vector<float>   *v_mu_theta_exPV;
    std::vector<float>   *v_mu_z0_exPV;
    // Covariances
    std::vector<float>   *v_mu_cov_d0_exPV;
    std::vector<float>   *v_mu_cov_d0_phi_exPV;
    std::vector<float>   *v_mu_cov_d0_qoverp_exPV;
    std::vector<float>   *v_mu_cov_d0_theta_exPV;
    std::vector<float>   *v_mu_cov_d0_z0_exPV;
    std::vector<float>   *v_mu_cov_phi_exPV;
    std::vector<float>   *v_mu_cov_phi_qoverp_exPV;
    std::vector<float>   *v_mu_cov_phi_theta_exPV;
    std::vector<float>   *v_mu_cov_qoverp_exPV;
    std::vector<float>   *v_mu_cov_theta_exPV;
    std::vector<float>   *v_mu_cov_theta_qoverp_exPV;
    std::vector<float>   *v_mu_cov_z0_exPV;
    std::vector<float>   *v_mu_cov_z0_phi_exPV;
    std::vector<float>   *v_mu_cov_z0_qoverp_exPV;
    std::vector<float>   *v_mu_cov_z0_theta_exPV;
    // ScaleFactors
    std::vector<float>  *v_mu_SF;
    std::vector<float>  *v_mu_SF_reco_up;
    std::vector<float>  *v_mu_SF_reco_down;
    std::vector<float>  *v_mu_SF_Id_up;
    std::vector<float>  *v_mu_SF_Id_down;
    std::vector<float>  *v_mu_trigger_SF;
    std::vector<float>  *v_mu_trigger_SF_up;
    std::vector<float>  *v_mu_trigger_SF_down;

    // taus
    Int_t           v_tau_n;
    std::vector<float>   *v_tau_uncorr_pt;
    std::vector<float>   *v_tau_pt;
    std::vector<float>   *v_tau_eta;
    std::vector<float>   *v_tau_phi;
    std::vector<float>   *v_tau_charge;
    std::vector<int>     *v_tau_seedCalo_nWideTrk;
    std::vector<int>     *v_tau_numTrack;
    std::vector<double>  *v_tau_leadTrkPt;
    std::vector<float>   *v_tau_leadTrkEta;
    std::vector<float>   *v_tau_jet_m;
    std::vector<int>     *v_tau_muonVeto;
    std::vector<int>     *v_tau_EleBDTLoose;
    std::vector<int>     *v_tau_EleBDTMedium;
    std::vector<int>     *v_tau_EleBDTTight;
    std::vector<float>   *v_tau_BDTEleScore;
    std::vector<int>     *v_tau_JetBDTSigTight;
    std::vector<int>     *v_tau_JetBDTSigMedium;
    std::vector<int>     *v_tau_JetBDTSigLoose;
    std::vector<float>   *v_tau_BDTJetScore;
    std::vector<int>     *v_tau_truthMatched;
    std::vector<int>     *v_tau_truthMatchedPromptEl;
    std::vector<int>     *v_tau_truthMatchedEl;
    std::vector<int>     *v_tau_truthMatchedMuon;
    std::vector<int>     *v_tau_truthMatchedParton;
    std::vector<int>     *v_tau_pi0_n;
    std::vector<float>   *v_tau_pi0_vistau_m;
    std::vector<float>   *v_tau_massTrkSys;
    std::vector<int>     *v_tau_isTauBadMediumBCH;
    std::vector<int>     *v_tau_isTauBadTightBCH;

    // Jets
    Int_t           v_jet_n;
    std::vector<float>   *v_jet_E;
    std::vector<float>   *v_jet_eta;
    std::vector<int>     *v_jet_flavor_truth_label;
    std::vector<float>   *v_jet_flavor_weight_IP3D;
    std::vector<float>   *v_jet_flavor_weight_MV1;
    std::vector<float>   *v_jet_flavor_weight_MV1c;
    std::vector<float>   *v_jet_flavor_weight_JetFitterCOMBNN;
    std::vector<float>   *v_jet_flavor_weight_SV0;
    std::vector<float>   *v_jet_phi;
    std::vector<float>   *v_jet_pt;
    // Hadron Level Jets
    Int_t           v_jet_AntiKt4Truth_n;
    std::vector<float>   *v_jet_AntiKt4Truth_pt;
    std::vector<float>   *v_jet_AntiKt4Truth_eta;
    std::vector<float>   *v_jet_AntiKt4Truth_phi;
    std::vector<float>   *v_jet_AntiKt4Truth_E;

    // ScaleFactors

    std::vector<float>  *v_jet_MV1_60_SF;
    std::vector<float>  *v_jet_MV1_60_SF_btag_up;
    std::vector<float>  *v_jet_MV1_60_SF_btag_down;
    std::vector<float>  *v_jet_MV1_60_SF_ctautag_up;
    std::vector<float>  *v_jet_MV1_60_SF_ctautag_down;
    std::vector<float>  *v_jet_MV1_60_SF_mistag_up;
    std::vector<float>  *v_jet_MV1_60_SF_mistag_down;
    std::vector<float>  *v_jet_MV1_70_SF;
    std::vector<float>  *v_jet_MV1_70_SF_btag_up;
    std::vector<float>  *v_jet_MV1_70_SF_btag_down;
    std::vector<float>  *v_jet_MV1_70_SF_ctautag_up;
    std::vector<float>  *v_jet_MV1_70_SF_ctautag_down;
    std::vector<float>  *v_jet_MV1_70_SF_mistag_up;
    std::vector<float>  *v_jet_MV1_70_SF_mistag_down;
    std::vector<float>  *v_jet_MV1_80_SF;
    std::vector<float>  *v_jet_MV1_80_SF_btag_up;
    std::vector<float>  *v_jet_MV1_80_SF_btag_down;
    std::vector<float>  *v_jet_MV1_80_SF_ctautag_up;
    std::vector<float>  *v_jet_MV1_80_SF_ctautag_down;
    std::vector<float>  *v_jet_MV1_80_SF_mistag_up;
    std::vector<float>  *v_jet_MV1_80_SF_mistag_down;
    std::vector<float>  *v_jet_MV1c_50_SF;
    std::vector<float>  *v_jet_MV1c_50_SF_btag_up;
    std::vector<float>  *v_jet_MV1c_50_SF_btag_down;
    std::vector<float>  *v_jet_MV1c_50_SF_ctautag_up;
    std::vector<float>  *v_jet_MV1c_50_SF_ctautag_down;
    std::vector<float>  *v_jet_MV1c_50_SF_mistag_up;
    std::vector<float>  *v_jet_MV1c_50_SF_mistag_down;

    // MC Truth
    Float_t         v_mc_Wm_child1_final_pt;
    Float_t         v_mc_Wm_child1_final_eta;
    Float_t         v_mc_Wm_child1_final_phi;
    Float_t         v_mc_Wm_child1_final_E;
    Int_t           v_mc_Wm_child1_final_pdgId;

    Float_t         v_mc_Wm_child2_final_pt;
    Float_t         v_mc_Wm_child2_final_eta;
    Float_t         v_mc_Wm_child2_final_phi;
    Float_t         v_mc_Wm_child2_final_E;
    Int_t           v_mc_Wm_child2_final_pdgId;

    Float_t         v_mc_Wp_child1_final_pt;
    Float_t         v_mc_Wp_child1_final_eta;
    Float_t         v_mc_Wp_child1_final_phi;
    Float_t         v_mc_Wp_child1_final_E;
    Int_t           v_mc_Wp_child1_final_pdgId;

    Float_t         v_mc_Wp_child2_final_pt;
    Float_t         v_mc_Wp_child2_final_eta;
    Float_t         v_mc_Wp_child2_final_phi;
    Float_t         v_mc_Wp_child2_final_E;
    Int_t           v_mc_Wp_child2_final_pdgId;

    Float_t         v_mc_b_final_pt;
    Float_t         v_mc_b_final_eta;
    Float_t         v_mc_b_final_phi;
    Float_t         v_mc_b_final_E;
    Int_t           v_mc_b_final_pdgId;

    Float_t         v_mc_bbar_final_pt;
    Float_t         v_mc_bbar_final_eta;
    Float_t         v_mc_bbar_final_phi;
    Float_t         v_mc_bbar_final_E;
    Int_t           v_mc_bbar_final_pdgId;

    // Missing energy
    Float_t         v_MET_etx;
    Float_t         v_MET_ety;
    Float_t         v_MET_sumet;

    // ================
    // List of branches
    // ================

    // Event header
    TBranch        *b_dataPeriod;   //!
    TBranch        *b_RunNumber;   //!
    TBranch        *b_EventNumber;   //!
    TBranch        *b_isMC;   //!
    TBranch        *b_mc_channel_number;//!
    TBranch        *b_top_hfor_type;//!
    TBranch        *b_averageIntPerXing;   //!
    TBranch        *b_vxp_n;   //!
    TBranch        *b_vxp_z;   //!
    TBranch        *b_mcevt_pdf1;   //!
    TBranch        *b_mcevt_pdf2;   //!
    TBranch        *b_mcevt_pdf_id1;   //!
    TBranch        *b_mcevt_pdf_id2;   //!
    TBranch        *b_mcevt_pdf_scale;   //!
    TBranch        *b_mcevt_pdf_x1;   //!
    TBranch        *b_mcevt_pdf_x2;   //!

    // Event weights
    TBranch        *b_GenWeight; //!
    TBranch        *b_PileupWeight;   //!
    TBranch        *b_ZVertexWeight;   //!

    TBranch        *b_MMWeight_nominal; //!
    TBranch        *b_MMWeight_el_fake_mc_up; //!
    TBranch        *b_MMWeight_el_fake_mc_down; //!
    TBranch        *b_MMWeight_el_fake_alternate; //!
    TBranch        *b_MMWeight_el_real_alternate; //!
    TBranch        *b_MMWeight_mu_fake_mc_up; //!
    TBranch        *b_MMWeight_mu_fake_mc_down; //!
    TBranch        *b_MMWeight_mu_fake_alternate; //!
    TBranch        *b_MMWeight_mu_real_alternate; //!
    TBranch        *b_TotalWeight;     //!

    // Electrons
    TBranch        *b_el_n;   //!
    TBranch        *b_el_charge;   //!
    TBranch        *b_el_isGood;   //!
    TBranch        *b_el_tightPP;   //!
    TBranch        *b_el_Etcone20;   //!
    TBranch        *b_el_ptcone30;   //!
    TBranch        *b_el_E;   //!
    TBranch        *b_el_eta;   //!
    TBranch        *b_el_phi;   //!
    TBranch        *b_el_pt;
    TBranch        *b_el_trackd0;   //!
    TBranch        *b_el_trackqoverp;   //!
    TBranch        *b_el_tracktheta;   //!
    TBranch        *b_el_trackz0;   //!
    // Covariances
    TBranch        *b_el_trackcov_d0;
    TBranch        *b_el_trackcov_d0_phi;   //!
    TBranch        *b_el_trackcov_d0_qoverp;   //!
    TBranch        *b_el_trackcov_d0_theta;   //!
    TBranch        *b_el_trackcov_d0_z0;   //!
    TBranch        *b_el_trackcov_phi;
    TBranch        *b_el_trackcov_phi_qoverp;   //!
    TBranch        *b_el_trackcov_phi_theta;   //!
    TBranch        *b_el_trackcov_qoverp;
    TBranch        *b_el_trackcov_theta;
    TBranch        *b_el_trackcov_theta_qoverp;   //!
    TBranch        *b_el_trackcov_z0;
    TBranch        *b_el_trackcov_z0_phi;   //!
    TBranch        *b_el_trackcov_z0_qoverp;   //!
    TBranch        *b_el_trackcov_z0_theta;   //!
    // ScaleFactors
    TBranch        *b_el_SF;   //!
    TBranch        *b_el_SF_reco_up;   //!
    TBranch        *b_el_SF_reco_down;   //!
    TBranch        *b_el_SF_Id_up;   //!
    TBranch        *b_el_SF_Id_down;   //!
    TBranch        *b_el_trigger_SF;   //!
    TBranch        *b_el_trigger_SF_up;   //!
    TBranch        *b_el_trigger_SF_down;   //!

    // Muons
    TBranch        *b_mu_n;   //!
    TBranch        *b_mu_charge;   //!
    TBranch        *b_mu_isGood;   //!
    TBranch        *b_mu_etcone20;   //!
    TBranch        *b_mu_ptcone30;   //!
    TBranch        *b_mu_E;   //!
    TBranch        *b_mu_eta;   //!
    TBranch        *b_mu_phi;   //!
    TBranch        *b_mu_pt;   //!
    TBranch        *b_mu_d0_exPV;
    TBranch        *b_mu_phi_exPV;
    TBranch        *b_mu_qoverp_exPV;
    TBranch        *b_mu_theta_exPV;
    TBranch        *b_mu_z0_exPV;
    // Covariances
    TBranch   *b_mu_cov_d0_exPV;
    TBranch   *b_mu_cov_d0_phi_exPV;
    TBranch   *b_mu_cov_d0_qoverp_exPV;
    TBranch   *b_mu_cov_d0_theta_exPV;
    TBranch   *b_mu_cov_d0_z0_exPV;
    TBranch   *b_mu_cov_phi_exPV;
    TBranch   *b_mu_cov_phi_qoverp_exPV;
    TBranch   *b_mu_cov_phi_theta_exPV;
    TBranch   *b_mu_cov_qoverp_exPV;
    TBranch   *b_mu_cov_theta_exPV;
    TBranch   *b_mu_cov_theta_qoverp_exPV;
    TBranch   *b_mu_cov_z0_exPV;
    TBranch   *b_mu_cov_z0_phi_exPV;
    TBranch   *b_mu_cov_z0_qoverp_exPV;
    TBranch   *b_mu_cov_z0_theta_exPV;
    // ScaleFactors
    TBranch   *b_mu_SF;   //!
    TBranch   *b_mu_SF_reco_up;   //!
    TBranch   *b_mu_SF_reco_down;   //!
    TBranch   *b_mu_SF_Id_up;   //!
    TBranch   *b_mu_SF_Id_down;   //!
    TBranch   *b_mu_trigger_SF;   //!
    TBranch   *b_mu_trigger_SF_up;   //!
    TBranch   *b_mu_trigger_SF_down;   //!

    // Taus
    TBranch        *b_tau_n;   //!
    TBranch        *b_tau_uncorr_pt;   //!
    TBranch        *b_tau_pt;   //!
    TBranch        *b_tau_eta;   //!
    TBranch        *b_tau_phi;   //!
    TBranch        *b_tau_charge;   //!
    TBranch        *b_tau_seedCalo_nWideTrk;   //!
    TBranch        *b_tau_numTrack;   //!
    TBranch        *b_tau_leadTrkPt;   //!
    TBranch        *b_tau_leadTrkEta;   //!
    TBranch        *b_tau_jet_m;   //!
    TBranch        *b_tau_muonVeto;   //!
    TBranch        *b_tau_EleBDTLoose;   //!
    TBranch        *b_tau_EleBDTMedium;   //!
    TBranch        *b_tau_EleBDTTight;   //!
    TBranch        *b_tau_BDTEleScore;   //!
    TBranch        *b_tau_JetBDTSigTight;   //!
    TBranch        *b_tau_JetBDTSigMedium;   //!
    TBranch        *b_tau_JetBDTSigLoose;   //!
    TBranch        *b_tau_BDTJetScore;   //!
    TBranch        *b_tau_truthMatched;   //!
    TBranch        *b_tau_truthMatchedPromptEl;   //!
    TBranch        *b_tau_truthMatchedEl;   //!
    TBranch        *b_tau_truthMatchedMuon;   //!
    TBranch        *b_tau_truthMatchedParton;   //!
    TBranch        *b_tau_pi0_n;   //!
    TBranch        *b_tau_pi0_vistau_m;   //!
    TBranch        *b_tau_massTrkSys;   //!
    TBranch        *b_tau_isTauBadMediumBCH;   //!
    TBranch        *b_tau_isTauBadTightBCH;   //!

    // Jets
    TBranch   *b_jet_n;   //!
    TBranch   *b_jet_E;
    TBranch   *b_jet_eta;
    TBranch   *b_jet_flavor_truth_label;
    TBranch   *b_jet_flavor_weight_IP3D;
    TBranch   *b_jet_flavor_weight_MV1;
    TBranch   *b_jet_flavor_weight_MV1c;
    TBranch   *b_jet_flavor_weight_JetFitterCOMBNN;
    TBranch   *b_jet_flavor_weight_SV0;
    TBranch   *b_jet_phi;
    TBranch   *b_jet_pt;
    // Hadron Level Jets
    TBranch   *b_jet_AntiKt4Truth_n;   //!
    TBranch   *b_jet_AntiKt4Truth_pt;
    TBranch   *b_jet_AntiKt4Truth_eta;
    TBranch   *b_jet_AntiKt4Truth_phi;
    TBranch   *b_jet_AntiKt4Truth_E;
    // ScaleFactors
    TBranch  *b_jet_MV1_60_SF;   //!
    TBranch  *b_jet_MV1_60_SF_btag_up;   //!
    TBranch  *b_jet_MV1_60_SF_btag_down;   //!
    TBranch  *b_jet_MV1_60_SF_ctautag_up;   //!
    TBranch  *b_jet_MV1_60_SF_ctautag_down;   //!
    TBranch  *b_jet_MV1_60_SF_mistag_up;   //!
    TBranch  *b_jet_MV1_60_SF_mistag_down;   //!
    TBranch  *b_jet_MV1_70_SF;   //!
    TBranch  *b_jet_MV1_70_SF_btag_up;   //!
    TBranch  *b_jet_MV1_70_SF_btag_down;   //!
    TBranch  *b_jet_MV1_70_SF_ctautag_up;   //!
    TBranch  *b_jet_MV1_70_SF_ctautag_down;   //!
    TBranch  *b_jet_MV1_70_SF_mistag_up;   //!
    TBranch  *b_jet_MV1_70_SF_mistag_down;   //!
    TBranch  *b_jet_MV1_80_SF;   //!
    TBranch  *b_jet_MV1_80_SF_btag_up;   //!
    TBranch  *b_jet_MV1_80_SF_btag_down;   //!
    TBranch  *b_jet_MV1_80_SF_ctautag_up;   //!
    TBranch  *b_jet_MV1_80_SF_ctautag_down;   //!
    TBranch  *b_jet_MV1_80_SF_mistag_up;   //!
    TBranch  *b_jet_MV1_80_SF_mistag_down;   //!
    TBranch  *b_jet_MV1c_50_SF;   //!
    TBranch  *b_jet_MV1c_50_SF_btag_up;   //!
    TBranch  *b_jet_MV1c_50_SF_btag_down;   //!
    TBranch  *b_jet_MV1c_50_SF_ctautag_up;   //!
    TBranch  *b_jet_MV1c_50_SF_ctautag_down;   //!
    TBranch  *b_jet_MV1c_50_SF_mistag_up;   //!
    TBranch  *b_jet_MV1c_50_SF_mistag_down;   //!

    // MC Neutrino
    TBranch        *b_mc_Wm_child1_final_pt;   //!
    TBranch        *b_mc_Wm_child1_final_eta;   //!
    TBranch        *b_mc_Wm_child1_final_phi;   //!
    TBranch        *b_mc_Wm_child1_final_E;   //!
    TBranch        *b_mc_Wm_child1_final_pdgId;   //!
    TBranch        *b_mc_Wm_child2_final_pt;   //!
    TBranch        *b_mc_Wm_child2_final_eta;   //!
    TBranch        *b_mc_Wm_child2_final_phi;   //!
    TBranch        *b_mc_Wm_child2_final_E;   //!
    TBranch        *b_mc_Wm_child2_final_pdgId;   //!

    TBranch        *b_mc_Wp_child1_final_pt;   //!
    TBranch        *b_mc_Wp_child1_final_eta;   //!
    TBranch        *b_mc_Wp_child1_final_phi;   //!
    TBranch        *b_mc_Wp_child1_final_E;   //!
    TBranch        *b_mc_Wp_child1_final_pdgId;   //!
    TBranch        *b_mc_Wp_child2_final_pt;   //!
    TBranch        *b_mc_Wp_child2_final_eta;   //!
    TBranch        *b_mc_Wp_child2_final_phi;   //!
    TBranch        *b_mc_Wp_child2_final_E;   //!
    TBranch        *b_mc_Wp_child2_final_pdgId;   //!

    TBranch        *b_mc_b_final_pt;   //!
    TBranch        *b_mc_b_final_eta;   //!
    TBranch        *b_mc_b_final_phi;   //!
    TBranch        *b_mc_b_final_E;   //!
    TBranch        *b_mc_b_final_pdgId;   //!
    TBranch        *b_mc_bbar_final_pt;   //!
    TBranch        *b_mc_bbar_final_eta;   //!
    TBranch        *b_mc_bbar_final_phi;   //!
    TBranch        *b_mc_bbar_final_E;   //!
    TBranch        *b_mc_bbar_final_pdgId;   //!

    // Missing energy
    TBranch  *b_MET_etx;
    TBranch  *b_MET_ety;
    TBranch  *b_MET_sumet;

    Bool_t fIsMC;            // Is sample file MC or Data?
    Bool_t fGenParticles;    // Generator Particles in Tree?

  public:
    AtlEvtReaderD3PDSgTop(AtlSelector *parent);
    virtual ~AtlEvtReaderD3PDSgTop();
    virtual void SetBranches(TTree *t);
    virtual Bool_t Notify();

  protected:
    virtual void InitObjPointers();
    virtual void BuildEvent();
    virtual void BuildEventHeader();
    virtual void BuildEnergySum();
    virtual void BuildElectrons();
    virtual void BuildMuons();
    virtual void BuildTaus();
    virtual void BuildJets();
    virtual void BuildMCParticles();

    ClassDef(AtlEvtReaderD3PDSgTop,0) // Event reader for slimmed Top D3PDs
};
#endif
