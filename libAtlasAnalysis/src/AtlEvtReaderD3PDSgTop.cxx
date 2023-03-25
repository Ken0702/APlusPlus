//____________________________________________________________________
//
// Event reader for slimmed Top D3PDs used by AtlSelector
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtReaderD3PDSgTop
#include <AtlEvtReaderD3PDSgTop.h>
#endif
#include <AtlEventHeader.h>
#include <HepVertex.h>
#include <iostream>
#include <vector>
#include <TTree.h>


using namespace std;

#ifndef __CINT__
ClassImp(AtlEvtReaderD3PDSgTop);
#endif

//____________________________________________________________________

AtlEvtReaderD3PDSgTop::AtlEvtReaderD3PDSgTop(AtlSelector *parent) :
    AtlEvtReaderD3PDBase(parent) {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlEvtReaderD3PDSgTop::~AtlEvtReaderD3PDSgTop() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTop::InitObjPointers() {
    //
    // Init object pointers
    //
    v_vxp_n = 0;
    v_vxp_z = 0;

    v_el_charge = 0;
    v_el_isGood = 0;
    v_el_tightPP = 0;
    v_el_Etcone20 = 0;
    v_el_ptcone30 = 0;
    v_el_E = 0;
    v_el_eta = 0;
    v_el_phi = 0;
    v_el_pt = 0;
    v_el_trackd0 = 0;
    v_el_trackqoverp = 0;
    v_el_tracktheta = 0;
    v_el_trackz0 = 0;
    v_el_trackcov_d0 = 0; 
    v_el_trackcov_d0_z0 = 0;
    v_el_trackcov_d0_phi = 0;
    v_el_trackcov_d0_theta = 0;
    v_el_trackcov_d0_qoverp = 0;
    v_el_trackcov_phi = 0;
    v_el_trackcov_phi_theta = 0;
    v_el_trackcov_phi_qoverp = 0;
    v_el_trackcov_qoverp = 0;
    v_el_trackcov_theta = 0;
    v_el_trackcov_theta_qoverp = 0;
    v_el_trackcov_z0 = 0;
    v_el_trackcov_z0_phi = 0;
    v_el_trackcov_z0_theta = 0;
    v_el_trackcov_z0_qoverp = 0;
    v_el_SF = 0;
    v_el_SF_reco_up = 0;
    v_el_SF_reco_down = 0;
    v_el_SF_Id_up = 0;
    v_el_SF_Id_down = 0;
    v_el_trigger_SF = 0;
    v_el_trigger_SF_up = 0;
    v_el_trigger_SF_down = 0;

    v_jet_E = 0;
    v_jet_eta = 0;
    v_jet_flavor_truth_label = 0;
    v_jet_flavor_weight_IP3D = 0;
    v_jet_flavor_weight_MV1 = 0;
    v_jet_flavor_weight_MV1c = 0;
    v_jet_flavor_weight_JetFitterCOMBNN = 0;    
    v_jet_flavor_weight_SV0 = 0;
    v_jet_phi = 0;
    v_jet_pt = 0;

    v_jet_MV1_60_SF = 0;
    v_jet_MV1_60_SF_btag_up = 0;
    v_jet_MV1_60_SF_btag_down = 0;
    v_jet_MV1_60_SF_ctautag_up = 0;
    v_jet_MV1_60_SF_ctautag_down = 0;
    v_jet_MV1_60_SF_mistag_up = 0;
    v_jet_MV1_60_SF_mistag_down = 0;
    v_jet_MV1_70_SF = 0;
    v_jet_MV1_70_SF_btag_up = 0;
    v_jet_MV1_70_SF_btag_down = 0;
    v_jet_MV1_70_SF_ctautag_up = 0;
    v_jet_MV1_70_SF_ctautag_down = 0;
    v_jet_MV1_70_SF_mistag_up = 0;
    v_jet_MV1_70_SF_mistag_down = 0;
    v_jet_MV1_80_SF = 0;
    v_jet_MV1_80_SF_btag_up = 0;
    v_jet_MV1_80_SF_btag_down = 0;
    v_jet_MV1_80_SF_ctautag_up = 0;
    v_jet_MV1_80_SF_ctautag_down = 0;
    v_jet_MV1_80_SF_mistag_up = 0;
    v_jet_MV1_80_SF_mistag_down = 0;
    v_jet_MV1c_50_SF = 0;
    v_jet_MV1c_50_SF_btag_up = 0;
    v_jet_MV1c_50_SF_btag_down = 0;
    v_jet_MV1c_50_SF_ctautag_up = 0;
    v_jet_MV1c_50_SF_ctautag_down = 0;
    v_jet_MV1c_50_SF_mistag_up = 0;
    v_jet_MV1c_50_SF_mistag_down = 0;
    v_jet_AntiKt4Truth_pt = 0; 
    v_jet_AntiKt4Truth_eta = 0;
    v_jet_AntiKt4Truth_phi = 0;
    v_jet_AntiKt4Truth_E = 0;
    
    v_mu_E = 0;
    v_mu_charge = 0;
    v_mu_isGood = 0;
    v_mu_cov_d0_exPV = 0;            
    v_mu_cov_d0_phi_exPV = 0;
    v_mu_cov_d0_qoverp_exPV = 0;
    v_mu_cov_d0_theta_exPV = 0;
    v_mu_cov_d0_z0_exPV = 0;
    v_mu_cov_phi_exPV = 0;
    v_mu_cov_phi_qoverp_exPV = 0;
    v_mu_cov_phi_theta_exPV = 0;
    v_mu_cov_qoverp_exPV = 0;
    v_mu_cov_theta_exPV = 0;
    v_mu_cov_theta_qoverp_exPV = 0;
    v_mu_cov_z0_exPV = 0;
    v_mu_cov_z0_phi_exPV = 0;
    v_mu_cov_z0_qoverp_exPV = 0;
    v_mu_cov_z0_theta_exPV = 0;
    v_mu_d0_exPV = 0;
    v_mu_eta = 0;
    v_mu_phi = 0;
    v_mu_phi_exPV = 0;
    v_mu_pt = 0;
    v_mu_qoverp_exPV = 0;
    v_mu_theta_exPV = 0;
    v_mu_z0_exPV = 0;
    v_mu_SF = 0;
    v_mu_SF_reco_up = 0;
    v_mu_SF_reco_down = 0;
    v_mu_SF_Id_up = 0;
    v_mu_SF_Id_down = 0;
    v_mu_trigger_SF = 0;
    v_mu_trigger_SF_up = 0;
    v_mu_trigger_SF_down = 0;

    v_tau_uncorr_pt = 0;
    v_tau_pt = 0;
    v_tau_eta = 0;
    v_tau_phi = 0;
    v_tau_charge = 0;
    v_tau_seedCalo_nWideTrk = 0;
    v_tau_numTrack = 0;
    v_tau_leadTrkPt = 0;
    v_tau_leadTrkEta = 0;
    v_tau_jet_m = 0;
    v_tau_muonVeto = 0;
    v_tau_EleBDTLoose = 0;
    v_tau_EleBDTMedium = 0;
    v_tau_EleBDTTight = 0;
    v_tau_BDTEleScore = 0;
    v_tau_JetBDTSigTight = 0;
    v_tau_JetBDTSigMedium = 0;
    v_tau_JetBDTSigLoose = 0;
    v_tau_BDTJetScore = 0;
    v_tau_truthMatched = 0;
    v_tau_truthMatchedPromptEl = 0;
    v_tau_truthMatchedEl = 0;
    v_tau_truthMatchedMuon = 0;
    v_tau_truthMatchedParton = 0;
    v_tau_pi0_n = 0;
    v_tau_pi0_vistau_m = 0;
    v_tau_massTrkSys = 0;
    v_tau_isTauBadMediumBCH = 0;
    v_tau_isTauBadTightBCH = 0;
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTop::SetBranches(TTree *t) {
    //
    // Set branch assignments of the input D3PD tree.
    //
    // Note that at this stage methods like TTree::FindBranch()
    // etc. do not work because the tree is not yet loaded from the
    // input file(s) and thus its structure not really known. For this
    // the Notify() function can be used.
    //
    InitBranches(t);

    // ====================
    // Set branch addresses
    // ====================

    // Event header
    SetupBranch(t, "dataPeriod", &v_dataPeriod, &b_dataPeriod );
    SetupBranch(t, "RunNumber", &v_RunNumber, &b_RunNumber);
    SetupBranch(t, "EventNumber", &v_EventNumber, &b_EventNumber);
    SetupBranch(t, "isMC", &v_isMC, &b_isMC);
    SetupBranch(t, "mc_channel_number", &v_mc_channel_number, &b_mc_channel_number);
    SetupBranch(t, "top_hfor_type", &v_top_hfor_type, &b_top_hfor_type);
    SetupBranch(t, "averageIntPerXing", &v_averageIntPerXing, &b_averageIntPerXing);
    // SetupBranch(t, "vxp_n", &v_vxp_n, &b_vxp_n);
    SetupBranch(t, "vxp_z", &v_vxp_z, &b_vxp_z);
    SetupBranch(t, "mcevt_pdf1", &v_mcevt_pdf1, &b_mcevt_pdf1);
    SetupBranch(t, "mcevt_pdf2", &v_mcevt_pdf2, &b_mcevt_pdf2);
    SetupBranch(t, "mcevt_pdf_id1", &v_mcevt_pdf_id1, &b_mcevt_pdf_id1);
    SetupBranch(t, "mcevt_pdf_id2", &v_mcevt_pdf_id2, &b_mcevt_pdf_id2);
    SetupBranch(t, "mcevt_pdf_scale", &v_mcevt_pdf_scale, &b_mcevt_pdf_scale);
    SetupBranch(t, "mcevt_pdf_x1", &v_mcevt_pdf_x1, &b_mcevt_pdf_x1);
    SetupBranch(t, "mcevt_pdf_x2", &v_mcevt_pdf_x2, &b_mcevt_pdf_x2);
    SetupBranch(t, "GenWeight", &v_GenWeight, &b_GenWeight);
    SetupBranch(t, "PileupWeight", &v_PileupWeight, &b_PileupWeight);
    SetupBranch(t, "ZVertexWeight", &v_ZVertexWeight, &b_ZVertexWeight);
    SetupBranch(t, "MMWeight_nominal", &v_MMWeight_nominal, &b_MMWeight_nominal);
    SetupBranch(t, "MMWeight_el_fake_mc_up", &v_MMWeight_el_fake_mc_up, &b_MMWeight_el_fake_mc_up);
    SetupBranch(t, "MMWeight_el_fake_mc_down", &v_MMWeight_el_fake_mc_down, &b_MMWeight_el_fake_mc_down);
    SetupBranch(t, "MMWeight_el_fake_alternate", &v_MMWeight_el_fake_alternate, &b_MMWeight_el_fake_alternate);
    SetupBranch(t, "MMWeight_el_real_alternate", &v_MMWeight_el_real_alternate, &b_MMWeight_el_real_alternate);
    SetupBranch(t, "MMWeight_mu_fake_mc_up", &v_MMWeight_mu_fake_mc_up, &b_MMWeight_mu_fake_mc_up);
    SetupBranch(t, "MMWeight_mu_fake_mc_down", &v_MMWeight_mu_fake_mc_down, &b_MMWeight_mu_fake_mc_down);
    SetupBranch(t, "MMWeight_mu_fake_alternate", &v_MMWeight_mu_fake_alternate, &b_MMWeight_mu_fake_alternate);
    SetupBranch(t, "MMWeight_mu_real_alternate", &v_MMWeight_mu_real_alternate, &b_MMWeight_mu_real_alternate);
 
    // MET
    SetupBranch(t, "MET_etx", &v_MET_etx, &b_MET_etx);
    SetupBranch(t, "MET_ety", &v_MET_ety, &b_MET_ety);
    SetupBranch(t, "MET_sumet", &v_MET_sumet, &b_MET_sumet);

    // Electrons
    SetupBranch(t, "el_n", &v_el_n, &b_el_n);
    SetupBranch(t, "el_charge", &v_el_charge, &b_el_charge);
    SetupBranch(t, "el_isGood", &v_el_isGood, &b_el_isGood);
    SetupBranch(t, "el_tightPP", &v_el_tightPP, &b_el_tightPP);
    SetupBranch(t, "el_Etcone20", &v_el_Etcone20, &b_el_Etcone20);
    SetupBranch(t, "el_ptcone30", &v_el_ptcone30, &b_el_ptcone30);
    SetupBranch(t, "el_E", &v_el_E, &b_el_E);
    SetupBranch(t, "el_eta", &v_el_eta, &b_el_eta);
    SetupBranch(t, "el_phi", &v_el_phi, &b_el_phi);
    SetupBranch(t, "el_pt", &v_el_pt, &b_el_pt);
    SetupBranch(t, "el_trackd0", &v_el_trackd0, &b_el_trackd0);
    SetupBranch(t, "el_trackqoverp", &v_el_trackqoverp, &b_el_trackqoverp);
    SetupBranch(t, "el_tracktheta", &v_el_tracktheta, &b_el_tracktheta);
    SetupBranch(t, "el_trackz0", &v_el_trackz0, &b_el_trackz0);

    // Electron scale factors
    SetupBranch(t, "el_SF", &v_el_SF, &b_el_SF);
    SetupBranch(t, "el_SF_reco_up", &v_el_SF_reco_up, &b_el_SF_reco_up);
    SetupBranch(t, "el_SF_reco_down", &v_el_SF_reco_down, &b_el_SF_reco_down);
    SetupBranch(t, "el_SF_Id_up", &v_el_SF_Id_up, &b_el_SF_Id_up);
    SetupBranch(t, "el_SF_Id_down", &v_el_SF_Id_down, &b_el_SF_Id_down);
    SetupBranch(t, "el_trigger_SF", &v_el_trigger_SF, &b_el_trigger_SF);
    SetupBranch(t, "el_trigger_SF_up", &v_el_trigger_SF_up, &b_el_trigger_SF_up);
    SetupBranch(t, "el_trigger_SF_down", &v_el_trigger_SF_down, &b_el_trigger_SF_down);

    // Electron covariance matrix
    SetupBranch(t, "el_trackcov_d0", &v_el_trackcov_d0, &b_el_trackcov_d0);
    SetupBranch(t, "el_trackcov_d0_phi", &v_el_trackcov_d0_phi, &b_el_trackcov_d0_phi);
    SetupBranch(t, "el_trackcov_d0_qoverp", &v_el_trackcov_d0_qoverp, &b_el_trackcov_d0_qoverp);
    SetupBranch(t, "el_trackcov_d0_theta", &v_el_trackcov_d0_theta, &b_el_trackcov_d0_theta);
    SetupBranch(t, "el_trackcov_d0_z0", &v_el_trackcov_d0_z0, &b_el_trackcov_d0_z0);
    SetupBranch(t, "el_trackcov_phi", &v_el_trackcov_phi, &b_el_trackcov_phi);
    SetupBranch(t, "el_trackcov_phi_qoverp", &v_el_trackcov_phi_qoverp, &b_el_trackcov_phi_qoverp);
    SetupBranch(t, "el_trackcov_phi_theta", &v_el_trackcov_phi_theta, &b_el_trackcov_phi_theta);
    SetupBranch(t, "el_trackcov_qoverp", &v_el_trackcov_qoverp, &b_el_trackcov_qoverp);
    SetupBranch(t, "el_trackcov_theta", &v_el_trackcov_theta, &b_el_trackcov_theta);
    SetupBranch(t, "el_trackcov_theta_qoverp", &v_el_trackcov_theta_qoverp, &b_el_trackcov_theta_qoverp);
    SetupBranch(t, "el_trackcov_z0", &v_el_trackcov_z0, &b_el_trackcov_z0);
    SetupBranch(t, "el_trackcov_z0_phi", &v_el_trackcov_z0_phi, &b_el_trackcov_z0_phi);
    SetupBranch(t, "el_trackcov_z0_qoverp", &v_el_trackcov_z0_qoverp, &b_el_trackcov_z0_qoverp);
    SetupBranch(t, "el_trackcov_z0_theta", &v_el_trackcov_z0_theta, &b_el_trackcov_z0_theta);
    
    // Muons
    SetupBranch(t, "mu_n", &v_mu_n, &b_mu_n);
    SetupBranch(t, "mu_charge", &v_mu_charge, &b_mu_charge);
    SetupBranch(t, "mu_isGood", &v_mu_isGood, &b_mu_isGood);
    SetupBranch(t, "mu_E", &v_mu_E, &b_mu_E);
    SetupBranch(t, "mu_eta", &v_mu_eta, &b_mu_eta);
    SetupBranch(t, "mu_phi", &v_mu_phi, &b_mu_phi);
    SetupBranch(t, "mu_pt", &v_mu_pt, &b_mu_pt);
    SetupBranch(t, "mu_d0_exPV", &v_mu_d0_exPV, &b_mu_d0_exPV);
    SetupBranch(t, "mu_phi_exPV", &v_mu_phi_exPV, &b_mu_phi_exPV);
    SetupBranch(t, "mu_qoverp_exPV", &v_mu_qoverp_exPV, &b_mu_qoverp_exPV);
    SetupBranch(t, "mu_theta_exPV", &v_mu_theta_exPV, &b_mu_theta_exPV);
    SetupBranch(t, "mu_z0_exPV", &v_mu_z0_exPV, &b_mu_z0_exPV);

    // Muon scale factors
    SetupBranch(t, "mu_SF", &v_mu_SF, &b_mu_SF);
    SetupBranch(t, "mu_SF_reco_up", &v_mu_SF_reco_up, &b_mu_SF_reco_up);
    SetupBranch(t, "mu_SF_reco_down", &v_mu_SF_reco_down, &b_mu_SF_reco_down);
    SetupBranch(t, "mu_SF_Id_up", &v_mu_SF_Id_up, &b_mu_SF_Id_up);
    SetupBranch(t, "mu_SF_Id_down", &v_mu_SF_Id_down, &b_mu_SF_Id_down);
    SetupBranch(t, "mu_trigger_SF", &v_mu_trigger_SF, &b_mu_trigger_SF);
    SetupBranch(t, "mu_trigger_SF_up", &v_mu_trigger_SF_up, &b_mu_trigger_SF_up);
    SetupBranch(t, "mu_trigger_SF_down", &v_mu_trigger_SF_down, &b_mu_trigger_SF_down);

    // Muon covariance matrix
    SetupBranch(t, "mu_cov_d0_exPV", &v_mu_cov_d0_exPV, &b_mu_cov_d0_exPV);
    SetupBranch(t, "mu_cov_d0_phi_exPV", &v_mu_cov_d0_phi_exPV, &b_mu_cov_d0_phi_exPV);
    SetupBranch(t, "mu_cov_d0_qoverp_exPV", &v_mu_cov_d0_qoverp_exPV, &b_mu_cov_d0_qoverp_exPV);
    SetupBranch(t, "mu_cov_d0_theta_exPV", &v_mu_cov_d0_theta_exPV, &b_mu_cov_d0_theta_exPV);
    SetupBranch(t, "mu_cov_d0_z0_exPV", &v_mu_cov_d0_z0_exPV, &b_mu_cov_d0_z0_exPV);
    SetupBranch(t, "mu_cov_phi_exPV", &v_mu_cov_phi_exPV, &b_mu_cov_phi_exPV);
    SetupBranch(t, "mu_cov_phi_qoverp_exPV", &v_mu_cov_phi_qoverp_exPV, &b_mu_cov_phi_qoverp_exPV);
    SetupBranch(t, "mu_cov_phi_theta_exPV", &v_mu_cov_phi_theta_exPV, &b_mu_cov_phi_theta_exPV);
    SetupBranch(t, "mu_cov_qoverp_exPV", &v_mu_cov_qoverp_exPV, &b_mu_cov_qoverp_exPV);
    SetupBranch(t, "mu_cov_theta_exPV", &v_mu_cov_theta_exPV, &b_mu_cov_theta_exPV);
    SetupBranch(t, "mu_cov_theta_qoverp_exPV", &v_mu_cov_theta_qoverp_exPV, &b_mu_cov_theta_qoverp_exPV);
    SetupBranch(t, "mu_cov_z0_exPV", &v_mu_cov_z0_exPV, &b_mu_cov_z0_exPV);
    SetupBranch(t, "mu_cov_z0_phi_exPV", &v_mu_cov_z0_phi_exPV, &b_mu_cov_z0_phi_exPV);
    SetupBranch(t, "mu_cov_z0_qoverp_exPV", &v_mu_cov_z0_qoverp_exPV, &b_mu_cov_z0_qoverp_exPV);
    SetupBranch(t, "mu_cov_z0_theta_exPV", &v_mu_cov_z0_theta_exPV, &b_mu_cov_z0_theta_exPV);

    // Taus
    SetupBranch(t, "tau_n", &v_tau_n, &b_tau_n);
    SetupBranch(t, "tau_uncorr_pt", &v_tau_uncorr_pt, &b_tau_uncorr_pt);
    SetupBranch(t, "tau_pt", &v_tau_pt, &b_tau_pt);
    SetupBranch(t, "tau_eta", &v_tau_eta, &b_tau_eta);
    SetupBranch(t, "tau_phi", &v_tau_phi, &b_tau_phi);
    SetupBranch(t, "tau_charge", &v_tau_charge, &b_tau_charge);
    SetupBranch(t, "tau_seedCalo_nWideTrk", &v_tau_seedCalo_nWideTrk, &b_tau_seedCalo_nWideTrk);
    SetupBranch(t, "tau_numTrack", &v_tau_numTrack, &b_tau_numTrack);
    SetupBranch(t, "tau_leadTrkPt", &v_tau_leadTrkPt, &b_tau_leadTrkPt);
    SetupBranch(t, "tau_leadTrkEta", &v_tau_leadTrkEta, &b_tau_leadTrkEta);
    SetupBranch(t, "tau_jet_m", &v_tau_jet_m, &b_tau_jet_m);
    SetupBranch(t, "tau_muonVeto", &v_tau_muonVeto, &b_tau_muonVeto);
    SetupBranch(t, "tau_EleBDTLoose", &v_tau_EleBDTLoose, &b_tau_EleBDTLoose);
    SetupBranch(t, "tau_EleBDTMedium", &v_tau_EleBDTMedium, &b_tau_EleBDTMedium);
    SetupBranch(t, "tau_EleBDTTight", &v_tau_EleBDTTight, &b_tau_EleBDTTight);
    SetupBranch(t, "tau_BDTEleScore", &v_tau_BDTEleScore, &b_tau_BDTEleScore);
    SetupBranch(t, "tau_JetBDTSigTight", &v_tau_JetBDTSigTight, &b_tau_JetBDTSigTight);
    SetupBranch(t, "tau_JetBDTSigMedium", &v_tau_JetBDTSigMedium, &b_tau_JetBDTSigMedium);
    SetupBranch(t, "tau_JetBDTSigLoose", &v_tau_JetBDTSigLoose, &b_tau_JetBDTSigLoose);
    SetupBranch(t, "tau_BDTJetScore", &v_tau_BDTJetScore, &b_tau_BDTJetScore);
    SetupBranch(t, "tau_truthMatched", &v_tau_truthMatched, &b_tau_truthMatched);
    SetupBranch(t, "tau_truthMatchedPromptEl", &v_tau_truthMatchedPromptEl, &b_tau_truthMatchedPromptEl);
    SetupBranch(t, "tau_truthMatchedEl", &v_tau_truthMatchedEl, &b_tau_truthMatchedEl);
    SetupBranch(t, "tau_truthMatchedMuon", &v_tau_truthMatchedMuon, &b_tau_truthMatchedMuon);
    SetupBranch(t, "tau_truthMatchedParton", &v_tau_truthMatchedParton, &b_tau_truthMatchedParton);
    SetupBranch(t, "tau_pi0_n", &v_tau_pi0_n, &b_tau_pi0_n);
    SetupBranch(t, "tau_pi0_vistau_m", &v_tau_pi0_vistau_m, &b_tau_pi0_vistau_m);
    SetupBranch(t, "tau_massTrkSys", &v_tau_massTrkSys, &b_tau_massTrkSys);
    SetupBranch(t, "tau_isTauBadMediumBCH", &v_tau_isTauBadMediumBCH, &b_tau_isTauBadMediumBCH);
    SetupBranch(t, "tau_isTauBadTightBCH", &v_tau_isTauBadTightBCH, &b_tau_isTauBadTightBCH);

    // Jets
    SetupBranch(t, "jet_n", &v_jet_n, &b_jet_n);
    SetupBranch(t, "jet_E", &v_jet_E, &b_jet_E);
    SetupBranch(t, "jet_pt", &v_jet_pt, &b_jet_pt);
    SetupBranch(t, "jet_eta", &v_jet_eta, &b_jet_eta);
    SetupBranch(t, "jet_phi", &v_jet_phi, &b_jet_phi);

    SetupBranch(t, "jet_flavor_truth_label", &v_jet_flavor_truth_label, &b_jet_flavor_truth_label);
    SetupBranch(t, "jet_flavor_weight_IP3D", &v_jet_flavor_weight_IP3D, &b_jet_flavor_weight_IP3D);
    SetupBranch(t, "jet_flavor_weight_JetFitterCOMBNN", &v_jet_flavor_weight_JetFitterCOMBNN,
    			&b_jet_flavor_weight_JetFitterCOMBNN);
    SetupBranch(t, "jet_flavor_weight_SV0", &v_jet_flavor_weight_SV0, &b_jet_flavor_weight_SV0);
    SetupBranch(t, "jet_flavor_weight_MV1", &v_jet_flavor_weight_MV1, &b_jet_flavor_weight_MV1);
    SetupBranch(t, "jet_flavor_weight_MV1c", &v_jet_flavor_weight_MV1c, &b_jet_flavor_weight_MV1c);

    // Hadron Level Jets
    SetupBranch(t, "jet_AntiKt4Truth_n", &v_jet_AntiKt4Truth_n, &b_jet_AntiKt4Truth_n);
    SetupBranch(t, "jet_AntiKt4Truth_E", &v_jet_AntiKt4Truth_E, &b_jet_AntiKt4Truth_E);
    SetupBranch(t, "jet_AntiKt4Truth_pt", &v_jet_AntiKt4Truth_pt, &b_jet_AntiKt4Truth_pt);
    SetupBranch(t, "jet_AntiKt4Truth_eta", &v_jet_AntiKt4Truth_eta, &b_jet_AntiKt4Truth_eta);
    SetupBranch(t, "jet_AntiKt4Truth_phi", &v_jet_AntiKt4Truth_phi, &b_jet_AntiKt4Truth_phi);

    // Jet scale factors
    SetupBranch(t, "jet_MV1_60_SF", &v_jet_MV1_60_SF, &b_jet_MV1_60_SF);
    SetupBranch(t, "jet_MV1_60_SF_btag_up", &v_jet_MV1_60_SF_btag_up, &b_jet_MV1_60_SF_btag_up);
    SetupBranch(t, "jet_MV1_60_SF_btag_down", &v_jet_MV1_60_SF_btag_down, &b_jet_MV1_60_SF_btag_down);
    SetupBranch(t, "jet_MV1_60_SF_ctautag_up", &v_jet_MV1_60_SF_ctautag_up, &b_jet_MV1_60_SF_ctautag_up);
    SetupBranch(t, "jet_MV1_60_SF_ctautag_down", &v_jet_MV1_60_SF_ctautag_down, &b_jet_MV1_60_SF_ctautag_down);
    SetupBranch(t, "jet_MV1_60_SF_mistag_up", &v_jet_MV1_60_SF_mistag_up, &b_jet_MV1_60_SF_mistag_up);
    SetupBranch(t, "jet_MV1_60_SF_mistag_down", &v_jet_MV1_60_SF_mistag_down, &b_jet_MV1_60_SF_mistag_down);
    SetupBranch(t, "jet_MV1_70_SF", &v_jet_MV1_70_SF, &b_jet_MV1_70_SF);
    SetupBranch(t, "jet_MV1_70_SF_btag_up", &v_jet_MV1_70_SF_btag_up, &b_jet_MV1_70_SF_btag_up);
    SetupBranch(t, "jet_MV1_70_SF_btag_down", &v_jet_MV1_70_SF_btag_down, &b_jet_MV1_70_SF_btag_down);
    SetupBranch(t, "jet_MV1_70_SF_ctautag_up", &v_jet_MV1_70_SF_ctautag_up, &b_jet_MV1_70_SF_ctautag_up);
    SetupBranch(t, "jet_MV1_70_SF_ctautag_down", &v_jet_MV1_70_SF_ctautag_down, &b_jet_MV1_70_SF_ctautag_down);
    SetupBranch(t, "jet_MV1_70_SF_mistag_up", &v_jet_MV1_70_SF_mistag_up, &b_jet_MV1_70_SF_mistag_up);
    SetupBranch(t, "jet_MV1_70_SF_mistag_down", &v_jet_MV1_70_SF_mistag_down, &b_jet_MV1_70_SF_mistag_down);
    SetupBranch(t, "jet_MV1_80_SF", &v_jet_MV1_80_SF, &b_jet_MV1_80_SF);
    SetupBranch(t, "jet_MV1_80_SF_btag_up", &v_jet_MV1_80_SF_btag_up, &b_jet_MV1_80_SF_btag_up);
    SetupBranch(t, "jet_MV1_80_SF_btag_down", &v_jet_MV1_80_SF_btag_down, &b_jet_MV1_80_SF_btag_down);
    SetupBranch(t, "jet_MV1_80_SF_ctautag_up", &v_jet_MV1_80_SF_ctautag_up, &b_jet_MV1_80_SF_ctautag_up);
    SetupBranch(t, "jet_MV1_80_SF_ctautag_down", &v_jet_MV1_80_SF_ctautag_down, &b_jet_MV1_80_SF_ctautag_down);
    SetupBranch(t, "jet_MV1_80_SF_mistag_up", &v_jet_MV1_80_SF_mistag_up, &b_jet_MV1_80_SF_mistag_up);
    SetupBranch(t, "jet_MV1_80_SF_mistag_down", &v_jet_MV1_80_SF_mistag_down, &b_jet_MV1_80_SF_mistag_down);
    SetupBranch(t, "jet_MV1c_50_SF", &v_jet_MV1c_50_SF, &b_jet_MV1c_50_SF);
    SetupBranch(t, "jet_MV1c_50_SF_btag_up", &v_jet_MV1c_50_SF_btag_up, &b_jet_MV1c_50_SF_btag_up);
    SetupBranch(t, "jet_MV1c_50_SF_btag_down", &v_jet_MV1c_50_SF_btag_down, &b_jet_MV1c_50_SF_btag_down);
    SetupBranch(t, "jet_MV1c_50_SF_ctautag_up", &v_jet_MV1c_50_SF_ctautag_up, &b_jet_MV1c_50_SF_ctautag_up);
    SetupBranch(t, "jet_MV1c_50_SF_ctautag_down", &v_jet_MV1c_50_SF_ctautag_down, &b_jet_MV1c_50_SF_ctautag_down);
    SetupBranch(t, "jet_MV1c_50_SF_mistag_up", &v_jet_MV1c_50_SF_mistag_up, &b_jet_MV1c_50_SF_mistag_up);
    SetupBranch(t, "jet_MV1c_50_SF_mistag_down", &v_jet_MV1c_50_SF_mistag_down, &b_jet_MV1c_50_SF_mistag_down);

    // MC Neutrino
    fGenParticles = kFALSE;
    if (fGenParticles)	{
    	Info("SetBranches", "Setting up mc truth branches");
	
    	SetupBranch(t, "mc_Wm_child1_final_pt", &v_mc_Wm_child1_final_pt, &b_mc_Wm_child1_final_pt);
    	SetupBranch(t, "mc_Wm_child1_final_eta", &v_mc_Wm_child1_final_eta, &b_mc_Wm_child1_final_eta);
    	SetupBranch(t, "mc_Wm_child1_final_phi", &v_mc_Wm_child1_final_phi, &b_mc_Wm_child1_final_phi);
    	SetupBranch(t, "mc_Wm_child1_final_E", &v_mc_Wm_child1_final_E, &b_mc_Wm_child1_final_E);
    	SetupBranch(t, "mc_Wm_child1_final_pdgId", &v_mc_Wm_child1_final_pdgId, &b_mc_Wm_child1_final_pdgId);

    	SetupBranch(t, "mc_Wm_child2_final_pt", &v_mc_Wm_child2_final_pt, &b_mc_Wm_child2_final_pt);
    	SetupBranch(t, "mc_Wm_child2_final_eta", &v_mc_Wm_child2_final_eta, &b_mc_Wm_child2_final_eta);
    	SetupBranch(t, "mc_Wm_child2_final_phi", &v_mc_Wm_child2_final_phi, &b_mc_Wm_child2_final_phi);
    	SetupBranch(t, "mc_Wm_child2_final_E", &v_mc_Wm_child2_final_E, &b_mc_Wm_child2_final_E);
    	SetupBranch(t, "mc_Wm_child2_final_pdgId", &v_mc_Wm_child2_final_pdgId, &b_mc_Wm_child2_final_pdgId);

    	SetupBranch(t, "mc_Wp_child1_final_pt", &v_mc_Wp_child1_final_pt, &b_mc_Wp_child1_final_pt);
    	SetupBranch(t, "mc_Wp_child1_final_eta", &v_mc_Wp_child1_final_eta, &b_mc_Wp_child1_final_eta);
    	SetupBranch(t, "mc_Wp_child1_final_phi", &v_mc_Wp_child1_final_phi, &b_mc_Wp_child1_final_phi);
    	SetupBranch(t, "mc_Wp_child1_final_E", &v_mc_Wp_child1_final_E, &b_mc_Wp_child1_final_E);
    	SetupBranch(t, "mc_Wp_child1_final_pdgId", &v_mc_Wp_child1_final_pdgId, &b_mc_Wp_child1_final_pdgId);

    	SetupBranch(t, "mc_Wp_child2_final_pt", &v_mc_Wp_child2_final_pt, &b_mc_Wp_child2_final_pt);
    	SetupBranch(t, "mc_Wp_child2_final_eta", &v_mc_Wp_child2_final_eta, &b_mc_Wp_child2_final_eta);
    	SetupBranch(t, "mc_Wp_child2_final_phi", &v_mc_Wp_child2_final_phi, &b_mc_Wp_child2_final_phi);
    	SetupBranch(t, "mc_Wp_child2_final_E", &v_mc_Wp_child2_final_E, &b_mc_Wp_child2_final_E);
    	SetupBranch(t, "mc_Wp_child2_final_pdgId", &v_mc_Wp_child2_final_pdgId, &b_mc_Wp_child2_final_pdgId);

    	SetupBranch(t, "mc_b_final_pt", &v_mc_b_final_pt, &b_mc_b_final_pt);
    	SetupBranch(t, "mc_b_final_eta", &v_mc_b_final_eta, &b_mc_b_final_eta);
    	SetupBranch(t, "mc_b_final_phi", &v_mc_b_final_phi, &b_mc_b_final_phi);
    	SetupBranch(t, "mc_b_final_E", &v_mc_b_final_E, &b_mc_b_final_E);
    	SetupBranch(t, "mc_b_final_pdgId", &v_mc_b_final_pdgId, &b_mc_b_final_pdgId);

    	SetupBranch(t, "mc_bbar_final_pt", &v_mc_bbar_final_pt, &b_mc_bbar_final_pt);
    	SetupBranch(t, "mc_bbar_final_eta", &v_mc_bbar_final_eta, &b_mc_bbar_final_eta);
    	SetupBranch(t, "mc_bbar_final_phi", &v_mc_bbar_final_phi, &b_mc_bbar_final_phi);
    	SetupBranch(t, "mc_bbar_final_E", &v_mc_bbar_final_E, &b_mc_bbar_final_E);
    	SetupBranch(t, "mc_bbar_final_pdgId", &v_mc_bbar_final_pdgId, &b_mc_bbar_final_pdgId);
    }
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTop::BuildEvent() {
    //
    // Build A++ event from D3PD content
    //
    fIsMC = v_isMC;
    
    // Build header
    BuildEventHeader();

    // Build energy sum
    BuildEnergySum();

    // Build electrons
    BuildElectrons();

    // Build muons
    BuildMuons();

    // Build taus
    BuildTaus();

    // Build jets
    BuildJets();

    // Build MC neutrinos
    if ( fGenParticles )  BuildMCParticles();
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTop::BuildEventHeader() {
    //
    // Build A++ event header
    //
    
    Float_t MMWeight_nominal        = 0;
    Float_t MMWeight_fake_mc_up     = 0;
    Float_t MMWeight_fake_mc_down   = 0;
    Float_t MMWeight_fake_alternate = 0;
    Float_t MMWeight_real_alternate = 0;
    if ( v_el_n > 0 && v_mu_n == 0 ) {
	MMWeight_nominal        = v_MMWeight_nominal;
	MMWeight_fake_mc_up     = v_MMWeight_el_fake_mc_up;
	MMWeight_fake_mc_down   = v_MMWeight_el_fake_mc_down;
	MMWeight_fake_alternate = v_MMWeight_el_fake_alternate;
	MMWeight_real_alternate = v_MMWeight_el_real_alternate;
    } else if (v_mu_n > 0 && v_el_n == 0) {
	MMWeight_nominal        = v_MMWeight_nominal;
	MMWeight_fake_mc_up     = v_MMWeight_mu_fake_mc_up;
	MMWeight_fake_mc_down   = v_MMWeight_mu_fake_mc_down;
	MMWeight_fake_alternate = v_MMWeight_mu_fake_alternate;
	MMWeight_real_alternate = v_MMWeight_mu_real_alternate;
    } else {
	// Warning("BuildEventHeader", "No MM weights available!");
	MMWeight_nominal        = 0;
	MMWeight_fake_mc_up     = 0;
	MMWeight_fake_mc_down   = 0;
	MMWeight_fake_alternate = 0;
	MMWeight_real_alternate = 0;
    }	
    
    fEvent->SetEventHeader(v_RunNumber, v_EventNumber, 0,//lumiblocknumber not longer saved
			   4000.,
			   fIsMC, // IsMC?
			   AtlEventHeader::kPhysics,
			   (fIsMC ) ? v_GenWeight : 1.,
			   (fIsMC ) ? v_PileupWeight : 1.,
			   (fIsMC ) ? v_ZVertexWeight : 1.,
			   (fIsMC ) ? 1. : MMWeight_nominal,
			   (fIsMC ) ? 1. : MMWeight_fake_mc_up,
			   (fIsMC ) ? 1. : MMWeight_fake_mc_down,
			   (fIsMC ) ? 1. : MMWeight_fake_alternate,
			   (fIsMC ) ? 1. : MMWeight_real_alternate,
                           1. );
    AtlEventHeader* eventheader = const_cast<AtlEventHeader*>(fEvent->GetEventHeader());
    
    // MC channel nummer
    eventheader->SetChannelNumber(v_mc_channel_number);

    // HFOR type
    eventheader->SetHFORType(v_top_hfor_type);

    // Pile-up variables
    eventheader->SetAverageIntPerXing(v_averageIntPerXing);

    // Number of primary vertices
    // v_vxp_n = 1;
    // fEvent->SetN_PrimaryVertices(v_vxp_n);

    // Vertices
    
    // Loop over all vertices
    // for ( Int_t i = 0; i < v_vxp_n; i++ ) {
    //   	// ================
    // 	// Build A++ vertex
    // 	// ================
    // 	fEvent->AddVertex(0.,0., //not stored
    // 			  v_vxp_z->at(i)/10., 
    // 			  0., // Chi2 n/a
    // 			  0, // NDoF n/a
    // 			  0.,0.,0.,0,0); //not stored
    // }


    // PDF information for systematics
    eventheader->SetPDFInfo(v_mcevt_pdf1,
			    v_mcevt_pdf2,
			    v_mcevt_pdf_id1,
			    v_mcevt_pdf_id2,
			    v_mcevt_pdf_scale,
			    v_mcevt_pdf_x1,
			    v_mcevt_pdf_x2);
}

//___________________________________________________________

void AtlEvtReaderD3PDSgTop::BuildEnergySum() {
    //
    // Build A++ energy sum
    //
    AtlEnergySum* energysum = const_cast<AtlEnergySum*>(fEvent->GetEnergySum());
    energysum->SetMissingEt(v_MET_etx/1000., v_MET_ety/1000.);
    energysum->SetSumEt(v_MET_sumet/1000.);
}

//___________________________________________________________

void AtlEvtReaderD3PDSgTop::BuildElectrons() {
    //
    // Build list of A++ electrons
    //
    
    // Loop over all electrons
    for ( Int_t i = 0; i < v_el_n; i++ ) {
	// ==================
	// Build A++ electron
	// ==================
	
	// Get reconstruction algorithm (author)
	AtlEMShower::EAuthor author = AtlEMShower::EAuthor(0);
	author = AtlEMShower::EAuthor(author | AtlEMShower::kHighPtElectron);

	// IsEM selection results
	AtlEMShower::EIsEM isem_sel = AtlEMShower::EIsEM(0);
	//if ( v_el_mediumPP->at(i) == 1 ) isem_sel = AtlEMShower::EIsEM(isem_sel | AtlEMShower::kElectronMediumPP); // only for loose samples
//  	if ( v_el_tight->at(i) == 1 ) isem_sel = AtlEMShower::EIsEM(isem_sel | AtlEMShower::kElectronTight); // still present?
 	if ( v_el_tightPP != 0 ) {
	    if ( v_el_tightPP->at(i) == 1 ) isem_sel = AtlEMShower::EIsEM(isem_sel | AtlEMShower::kElectronTightPP);
	} else {
	    isem_sel = AtlEMShower::EIsEM(isem_sel | AtlEMShower::kElectronTightPP);
	}

	if ( v_el_isGood != 0 ) {
	    // Use Electron tight flag for the tight/loose definition
	    // although this is not compatible with the ATLAS conventions
	    // However, this enable us to use Lepton::IsTight/IsLoose
	    // in the object selection later...
	    if ( v_el_isGood->at(i) ) {
		isem_sel = AtlEMShower::EIsEM(isem_sel | AtlEMShower::kElectronTight);
	    } else {
		// >>>>>> This flag is used for veto electrons in the 8TeV single-lepton analyses <<<<<<
		isem_sel = AtlEMShower::EIsEM(isem_sel | AtlEMShower::kElectronLoose);
	    }
	} else {
	    // for backward compatibility
	    // only tight electrons are included in SgTopD3PDs
	    isem_sel = AtlEMShower::EIsEM(isem_sel | AtlEMShower::kElectronTight);
	}
	
	// Set cluster 4-momentum
	TLorentzVector p_cl(0., 0., 1., 1.);
	if ( v_el_eta != 0 ) {
	    p_cl.SetPtEtaPhiE(0, 0, 0, 0);
	}

	// Add electron to list
	AtlElectron *el = fEvent->AddElectron(0., 0., 1., 1.,
					      ( v_el_charge->at(i) > 0. ) ? kTRUE : kFALSE,
					      -99999., -99999., // bkg weights
					      0, // c.f. AtlElectron::IsGoodOQ()
					      0, // el_isEMTight is not longer saved
					      isem_sel,
					      author,
					      p_cl);
	  

	// Set to recommendations of Egamma group:
	//
	// cluster energy over CosH(IDTrack eta) as pt,
	// angles from track (eta,phi),
	// energy from cluster
	//
	// Warning: does not work for di-leptons
	el->SetPtEtaPhiE(
	    v_el_pt->at(i)/1000.,
	    v_el_eta->at(i),
	    v_el_phi->at(i),
	    v_el_E->at(i)/1000.);

	if ( v_el_Etcone20 != 0 ) {
	    // Set PtCone variables
	    el->SetPtConeVars(0.,//ptcone20 not stored
			      v_el_ptcone30->at(i)/1000.,
			      0.);
	    
	    // Set EtCone variables
	    el->SetEtConeVars( v_el_Etcone20->at(i)/1000., // these might also be corrected
			       0., // el_Etcone20 not stored
			       0., 0.,        
			       0.,
			       0.,
			       0.,
			       -999., -999., -999,
			       0.,
			       0.,
			       0.,
			       -999., -999., -999,
			       0.,
			       0.,
			       0.,
			       -999., -999., -999);
	}
	
	//Set trigger match
		
	// =============================
	// Create corresponding ID track
	// =============================
	if ( v_el_trackd0 != 0 ) {
	    AtlIDTrack *trk = fEvent->AddIDTrack(-999., -999,
						 0., 0., 0., // No ref values (might be set to prim vtx)
						 0., //not stored in ntuple
						 v_el_trackqoverp->at(i)*1000.,
						 v_el_trackd0->at(i)/10.,
						 v_el_trackz0->at(i)/10.,
						 v_el_tracktheta->at(i));
	    el->SetIDTrack(trk);

	    
	    // Set global momentum at the DCA (in units of GeV)
	    
	    // Cov. matrix at DCA
	    if ( v_el_trackcov_d0 != 0 ) {
		Float_t CovMat[15];
		CovMat[0]  = v_el_trackcov_d0->at(i)/100.;
		CovMat[1]  = v_el_trackcov_d0_z0->at(i)/100.;         
		CovMat[2]  = v_el_trackcov_d0_phi->at(i)/10.;   
		CovMat[3]  = v_el_trackcov_d0_theta->at(i)/10.;   
		CovMat[4]  = v_el_trackcov_d0_qoverp->at(i)*100.;
		CovMat[5]  = v_el_trackcov_z0->at(i)/100.;
		CovMat[6]  = v_el_trackcov_z0_phi->at(i)/10.;
		CovMat[7]  = v_el_trackcov_z0_theta->at(i)/10.;      
		CovMat[8]  = v_el_trackcov_z0_qoverp->at(i)*100.; 
		CovMat[9]  = v_el_trackcov_phi->at(i);
		CovMat[10] = v_el_trackcov_phi_theta->at(i);     
		CovMat[11] = v_el_trackcov_phi_qoverp->at(i)*1000.;    
		CovMat[12] = v_el_trackcov_theta->at(i);
		CovMat[13] = v_el_trackcov_theta_qoverp->at(i)*1000.;  
		CovMat[14] = v_el_trackcov_qoverp->at(i)*1000000.;
		trk->SetCovMatrix(CovMat);
	    }
	}

	// Scale factors (only on MC)
	if ( fIsMC ) el->SetElectronScaleFactors(v_el_SF->at(i),
						 v_el_SF_reco_up->at(i),
						 v_el_SF_reco_down->at(i),
   						 v_el_SF_Id_up->at(i),
						 v_el_SF_Id_down->at(i),
   						 v_el_trigger_SF->at(i),
   						 v_el_trigger_SF_up->at(i),
						 v_el_trigger_SF_down->at(i));
    }
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTop::BuildJets() {
    //
    // Build A++ jets
    //

    // ==================
    // Loop over all jets
    // ==================
    for ( Int_t i = 0; i < v_jet_n; ++i ) {
	
	// Set quality
	AtlJet::EJetQuality quality = AtlJet::kIsGood;
		
	// EMScale 4-vector is not filled
	TLorentzVector p_emscale(0., 0., 1., 1.);
	p_emscale.SetPtEtaPhiE(0., 0., 0., 0.);
	
	// JEScorrScale is not filled
	TLorentzVector p_jescorr(0., 0., 0., 0.);

	// Build A++ jet
	AtlJet *jet = fEvent->AddJet(AtlJet::kAntiKt4LCTopo,
				     1., 0., 0., 1.,
				     quality, p_emscale, p_jescorr,
				     999., 999., 999., 0.);
	jet->SetPtEtaPhiE(v_jet_pt->at(i)/1000.,
			  v_jet_eta->at(i), 
			  v_jet_phi->at(i),
			  v_jet_E->at(i)/1000.);

	// Set truth information from hadron level jets
	AtlJet::EFlavour jet_flavour;
	switch (v_jet_flavor_truth_label->at(i)) {
	    case 0:
		jet_flavour=AtlJet::kLightFlavoured;
		break;
	    case 4:
		jet_flavour=AtlJet::kCharm;
		break;
	    case 5:
		jet_flavour=AtlJet::kBeauty;
		break;
	    case 16:
		jet_flavour=AtlJet::kTau;
		break;
	    default:
		jet_flavour=AtlJet::kUnknownFlavour;
		break;
	}
	jet->SetTruthFlavour(jet_flavour);
	
	// -------------
	// Add b-taggers
	// -------------
	jet->AddTag(AtlBTag::kIP3d, v_jet_flavor_weight_IP3D->at(i), kTRUE);
	jet->AddTag(AtlBTag::kSV0,  v_jet_flavor_weight_SV0->at(i),  kTRUE);
	if ( v_jet_flavor_weight_MV1 != 0 ) jet->AddTag(AtlBTag::kMV1,
							v_jet_flavor_weight_MV1->at(i),
							kTRUE);
	if ( v_jet_flavor_weight_MV1c != 0 ) jet->AddTag(AtlBTag::kMV1c,
							 v_jet_flavor_weight_MV1c->at(i),
							 kTRUE);
	
	// JetFitter taggers
	jet->AddTag(AtlBTag::kJetFitterCOMBNN, v_jet_flavor_weight_JetFitterCOMBNN->at(i),
		    kTRUE);

	// Scale factors (only on MC)
	if ( fIsMC ) {
	    // Since v8: SF_ctag is called SF_ctautag
	    //           keep "ctag" in AtlJet
	    
	    // MV1 WP 
	    jet->SetJetScaleFactors_MV1_60(
		v_jet_MV1_60_SF->at(i),
		v_jet_MV1_60_SF_btag_up->at(i),
		v_jet_MV1_60_SF_btag_down->at(i),
		v_jet_MV1_60_SF_ctautag_up->at(i),
		v_jet_MV1_60_SF_ctautag_down->at(i),
		v_jet_MV1_60_SF_mistag_up->at(i),
		v_jet_MV1_60_SF_mistag_down->at(i) );
	    
	    // MV1 WP 
	    jet->SetJetScaleFactors_MV1_70(
		v_jet_MV1_70_SF->at(i),
		v_jet_MV1_70_SF_btag_up->at(i),
		v_jet_MV1_70_SF_btag_down->at(i),
		v_jet_MV1_70_SF_ctautag_up->at(i),
		v_jet_MV1_70_SF_ctautag_down->at(i),
		v_jet_MV1_70_SF_mistag_up->at(i),
		v_jet_MV1_70_SF_mistag_down->at(i) );
	    
	    // MV1 WP
	    jet->SetJetScaleFactors_MV1_80(
		v_jet_MV1_80_SF->at(i),
		v_jet_MV1_80_SF_btag_up->at(i),
		v_jet_MV1_80_SF_btag_down->at(i),
		v_jet_MV1_80_SF_ctautag_up->at(i),
		v_jet_MV1_80_SF_ctautag_down->at(i),
		v_jet_MV1_80_SF_mistag_up->at(i),
		v_jet_MV1_80_SF_mistag_down->at(i) );
	    
	    // MV1 WP
	    jet->SetJetScaleFactors_MV1c_50(
		v_jet_MV1c_50_SF->at(i),
		v_jet_MV1c_50_SF_btag_up->at(i),
		v_jet_MV1c_50_SF_btag_down->at(i),
		v_jet_MV1c_50_SF_ctautag_up->at(i),
		v_jet_MV1c_50_SF_ctautag_down->at(i),
		v_jet_MV1c_50_SF_mistag_up->at(i),
		v_jet_MV1c_50_SF_mistag_down->at(i) );
	}
    }
    
    // ===============================
    // Loop over all hadron level jets
    // ===============================
    for ( Int_t i = 0; i < v_jet_AntiKt4Truth_n; ++i ) {
	
	// Set quality
	AtlJet::EJetQuality quality = AtlJet::kIsGood;
		
	// EMScale 4-vector doesn't apply
	TLorentzVector p_emscale(0., 0., 1., 1.);
		
	// JEScorrScale doesn't apply
	TLorentzVector p_jescorr(0., 0., 0., 0.);
	
	// Build A++ hadron level jet
	AtlJet *mc_jet = fEvent->AddJet(AtlJet::kMCAntiKt4Hadron,
					1., 0., 0., 1.,
					quality, p_emscale, p_jescorr,
					999.,
					999., 999.,
					999.);
	mc_jet->SetPtEtaPhiE(v_jet_AntiKt4Truth_pt->at(i)/1000.,
			     v_jet_AntiKt4Truth_eta->at(i), 
			     v_jet_AntiKt4Truth_phi->at(i),
			     v_jet_AntiKt4Truth_E->at(i)/1000.);
    }
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTop::BuildMuons() {
    //
    // Build list of A++ muons
    //
    
    // Loop over all muons
    for ( Int_t i = 0; i < v_mu_n; ++i ) {
	// ==============
	// Build A++ muon
	// ==============

	// Get reconstruction algorithm (author)
	AtlMuon::EAuthor author = AtlMuon::EAuthor(0);
	author = AtlMuon::EAuthor(author | AtlMuon::kMuidCo);

	// Muon quality
	AtlMuon::EQuality quality = AtlMuon::EQuality(0);
	quality = AtlMuon::EQuality(quality | AtlMuon::kTight);

	// hack for dilepton
	if ( v_mu_isGood != 0 ) {
	    // overwrite quality decision
	    // do not use OR (|) here!	    
	    if ( v_mu_isGood->at(i) ) {
		quality = AtlMuon::kTight;
	    } else {
		quality = AtlMuon::kLoose;
	    }
	}
	// if above does not overwrite the quality
	// muons are always tight, thus it is
	// backward compatible
	
	// Set muon momentum 
	TLorentzVector p_mu(0., 0., 1., 1.);
	p_mu.SetPtEtaPhiE(v_mu_pt->at(i)/1000., v_mu_eta->at(i), v_mu_phi->at(i),v_mu_E->at(i)/1000.);
	// Set dummy non-physical muon spectrometer kinematics
	TLorentzVector p_ms_mu(-999., -999., -999., -999.); 
	
	AtlMuon *mu = fEvent->AddMuon((float)p_mu.Px(),
				      (float)p_mu.Py(),
				      (float)p_mu.Pz(),
				      (float)p_mu.E(),
				      ( v_mu_charge->at(i) > 0. ) ? kTRUE : kFALSE,
				      -999,
				      -999., //etcone20 not stored
				      -999., //etcone30 not stored
				      -999,
				      -999, -999, -999, -999, 
				      -999,
				      -999.,//ptcone20 not stored
				      -999.,//ptcone30 not stored
				      -999,
				      author, quality, -999, -999, kTRUE,
				      // not stored: v_mu_matchchi2->at(i), v_mu_matchndof->at(i), bool(v_mu_isCombinedMuon->at(i)),
				      p_ms_mu, 0);

	// Create corresponding ID track
	// (last argument is not explicitly given here: const Float_t CovMat[15] is filled below)
	if ( v_mu_d0_exPV != 0 ) {
	    AtlIDTrack *trk = fEvent->AddIDTrack(-999.,-999,
						 0., 0., 0.,
						 v_mu_phi_exPV->at(i),
						 v_mu_qoverp_exPV->at(i)*1000.,
						 v_mu_d0_exPV->at(i)/10.,
						 v_mu_z0_exPV->at(i)/10.,
						 v_mu_theta_exPV->at(i));
	    mu->SetIDTrack(trk);

	
	    // Cov. matrix at DCA
	    if ( v_mu_cov_d0_exPV != 0 ) {
		Float_t CovMat[15];
		
		// Set vtx-fitted covariance parameters
		CovMat[0]  = v_mu_cov_d0_exPV->at(i)/100.;            
		CovMat[1]  = v_mu_cov_d0_z0_exPV->at(i)/100.;         
		CovMat[2]  = v_mu_cov_d0_phi_exPV->at(i)/10.;   
		CovMat[3]  = v_mu_cov_d0_theta_exPV->at(i)/10.;   
		CovMat[4]  = v_mu_cov_d0_qoverp_exPV->at(i)*100.;  
		CovMat[5]  = v_mu_cov_z0_exPV->at(i)/100.;
		CovMat[6]  = v_mu_cov_z0_phi_exPV->at(i)/10.;      
		CovMat[7]  = v_mu_cov_z0_theta_exPV->at(i)/10.;      
		CovMat[8]  = v_mu_cov_z0_qoverp_exPV->at(i)*100.; 
		CovMat[9]  = v_mu_cov_phi_exPV->at(i);
		CovMat[10] = v_mu_cov_phi_theta_exPV->at(i);     
		CovMat[11] = v_mu_cov_phi_qoverp_exPV->at(i)*1000.;    
		CovMat[12] = v_mu_cov_theta_exPV->at(i);
		CovMat[13] = v_mu_cov_theta_qoverp_exPV->at(i)*1000.;  
		CovMat[14] = v_mu_cov_qoverp_exPV->at(i)*1000000.;
		
		trk->SetVtxParameters(-999., -999,
				      v_mu_phi_exPV->at(i),
				      v_mu_qoverp_exPV->at(i)*1000.,
				      v_mu_d0_exPV->at(i)/10.,
				      v_mu_z0_exPV->at(i)/10.,
				      v_mu_theta_exPV->at(i),
				      CovMat);
		
		// Necessary in order to be able to convert (d0, phi, qoverp, theta, z0) to (pt, eta, phi),
		// since functionality is only provided in base class HepTrackHelix:
		trk->SetCovMatrix(CovMat);
	    }
	}

	// Scale factors (only on MC)
	if ( fIsMC ) mu->SetMuonScaleFactors(v_mu_SF->at(i),
					     v_mu_SF_reco_up->at(i),
					     v_mu_SF_reco_down->at(i),
					     v_mu_SF_Id_up->at(i),
					     v_mu_SF_Id_down->at(i),
					     v_mu_trigger_SF->at(i),
					     v_mu_trigger_SF_up->at(i),
					     v_mu_trigger_SF_down->at(i));
    }
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTop::BuildTaus() {
    //
    // Build list of A++ taus
    //
    
    // Loop over all muons
    for ( Int_t i = 0; i < v_tau_n; ++i ) {
	// =============
	// Build A++ tau
	// =============

	// Set Tau flag
	AtlTau::ETauFlag tauflag = AtlTau::ETauFlag(0);
	if ( v_tau_muonVeto ) tauflag = AtlTau::ETauFlag(tauflag | AtlTau::kMuonVeto);
	if ( v_tau_EleBDTLoose ) tauflag = AtlTau::ETauFlag(tauflag | AtlTau::kElectronVetoLoose);
	if ( v_tau_EleBDTMedium ) tauflag = AtlTau::ETauFlag(tauflag | AtlTau::kElectronVetoMedium);
	if ( v_tau_EleBDTTight ) tauflag = AtlTau::ETauFlag(tauflag | AtlTau::kElectronVetoTight);
	if ( v_tau_JetBDTSigLoose ) tauflag = AtlTau::ETauFlag(tauflag | AtlTau::kJetVetoLoose);
	if ( v_tau_JetBDTSigMedium ) tauflag = AtlTau::ETauFlag(tauflag | AtlTau::kJetVetoMedium);
	if ( v_tau_JetBDTSigTight ) tauflag = AtlTau::ETauFlag(tauflag | AtlTau::kJetVetoTight);
	
	// Set tau momentum 
	TLorentzVector p_tau(0., 0., 1., 1.);
	
	p_tau.SetPtEtaPhiM(v_tau_pt->at(i)/1000., v_tau_eta->at(i), v_tau_phi->at(i),
			   0.); // this might be wrong...?
	
	fEvent->AddTau((float)p_tau.Px(),
		       (float)p_tau.Py(),
		       (float)p_tau.Pz(),
		       (float)p_tau.E(),
		       ( v_tau_charge->at(i) > 0. ) ? kTRUE : kFALSE,
		       AtlTau::kInvalidAuthor,
		       tauflag);
    }
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTop::BuildMCParticles() {
    //
    // Build list of MC neutrinos
    //

    // Optional Parameters of AddMCParticles
    // Int_t MCStatus, Bool_t IsGenerator,
    // Bool_t IsGenNonInteracting, Bool_t IsGenStable, Bool_t IsGenSimulStable,
    // Bool_t IsGenInteracting, Bool_t IsConversion, Bool_t IsBremsstrahlung);

    HepMCParticle *mcprt = 0;
    // ttbar event:
    // ------------

    //
    // - Add b-Quarks
    //
    mcprt = fEvent->AddMCParticle(v_mc_b_final_pdgId,
				  0., 0., 0., // Px,Py,Pz - dummy values
				  v_mc_b_final_E,
				  0, kTRUE,
				  kFALSE, kFALSE, kFALSE,
				  kFALSE, kFALSE, kFALSE);
    mcprt->SetPtEtaPhiE(v_mc_b_final_pt/1000.,
			v_mc_b_final_eta,
			v_mc_b_final_phi,
			v_mc_b_final_E/1000.);

    mcprt = fEvent->AddMCParticle(v_mc_bbar_final_pdgId,
				  0., 0., 0., // Px,Py,Pz - dummy values
				  v_mc_bbar_final_E,
				  0, kTRUE,
				  kFALSE, kFALSE, kFALSE,
				  kFALSE, kFALSE, kFALSE);
    mcprt->SetPtEtaPhiE(v_mc_bbar_final_pt/1000.,
			v_mc_bbar_final_eta,
			v_mc_bbar_final_phi,
			v_mc_bbar_final_E/1000.);

    //
    // - W^{-} decay productions
    //
    mcprt = fEvent->AddMCParticle(v_mc_Wm_child1_final_pdgId,
				  0., 0., 0., // Px,Py,Pz - dummy values
				  v_mc_Wm_child1_final_E,
				  0, kTRUE,
				  kFALSE, kFALSE, kFALSE,
				  kFALSE, kFALSE, kFALSE);
    mcprt->SetPtEtaPhiE(v_mc_Wm_child1_final_pt/1000.,
			v_mc_Wm_child1_final_eta,
			v_mc_Wm_child1_final_phi,
			v_mc_Wm_child1_final_E/1000.);

    mcprt = fEvent->AddMCParticle(v_mc_Wm_child2_final_pdgId,
				  0., 0., 0., // Px,Py,Pz - dummy values
				  v_mc_Wm_child2_final_E,
				  0, kTRUE,
				  kFALSE, kFALSE, kFALSE,
				  kFALSE, kFALSE, kFALSE);
    mcprt->SetPtEtaPhiE(v_mc_Wm_child2_final_pt/1000.,
			v_mc_Wm_child2_final_eta,
			v_mc_Wm_child2_final_phi,
			v_mc_Wm_child2_final_E/1000.);

    //
    // - W^{-} decay productions
    //
    mcprt = fEvent->AddMCParticle(v_mc_Wp_child1_final_pdgId,
				  0., 0., 0., // Px,Py,Pz - dummy values
				  v_mc_Wp_child1_final_E,
				  0, kTRUE,
				  kFALSE, kFALSE, kFALSE,
				  kFALSE, kFALSE, kFALSE);
    mcprt->SetPtEtaPhiE(v_mc_Wp_child1_final_pt/1000.,
			v_mc_Wp_child1_final_eta,
			v_mc_Wp_child1_final_phi,
			v_mc_Wp_child1_final_E/1000.);

    mcprt = fEvent->AddMCParticle(v_mc_Wp_child2_final_pdgId,
				  0., 0., 0., // Px,Py,Pz - dummy values
				  v_mc_Wp_child2_final_E,
				  0, kTRUE,
				  kFALSE, kFALSE, kFALSE,
				  kFALSE, kFALSE, kFALSE);
    mcprt->SetPtEtaPhiE(v_mc_Wp_child2_final_pt/1000.,
			v_mc_Wp_child2_final_eta,
			v_mc_Wp_child2_final_phi,
			v_mc_Wp_child2_final_E/1000.);

}


//____________________________________________________________________

Bool_t AtlEvtReaderD3PDSgTop::Notify() {
    //
    // This method is called whenever a new input file is opened
    //

    TTree *t = fParent->GetCurrentTree();

    if ( t->FindBranch("mc_b_final_pt") != 0 ) {
	fGenParticles=kTRUE;
	Info("Notify", "Enable truth branches");
    } else {
	fGenParticles=kFALSE;
	Info("Notify", "No truth branches found");
    }
    return kTRUE;
}
