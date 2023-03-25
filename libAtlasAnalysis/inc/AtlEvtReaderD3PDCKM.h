//
// Author: Christopher Schmitt <mailto: Christopher.Schmitt@cern.ch>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtReaderD3PDCKM
#define ATLAS_AtlEvtReaderD3PDCKM
#ifndef ATLAS_AtlEvtReaderD3PDSgTop
#include <AtlEvtReaderD3PDSgTop.h>
#endif
#include <vector>
#include <map>
#include <TVector2.h>
#include <TBranch.h>

const int pdgIdD = 1;
const int pdgIdS = 3;
const int pdgIdB = 5;

class AtlEventHeader;
class HepVertex;

class AtlEvtReaderD3PDCKM : public AtlEvtReaderD3PDSgTop {

protected:

     TList *fJPM_bQuarkJet;         // List of JPM b-jets
     TList *fJPM_sQuarkJet;         // List of JPM s-jets
     TList *fJPM_bQuark;         // List of JPM b-quarks
     TList *fJPM_sQuark;         // List of JPM s-quarks

     std::vector<int> fJPM_NumbJetsMatching;         // Number of Jets matching top daughter quark

public:

    // =========================
    // Declaration of leaf types
    // =========================

    // v0
    Int_t           v_v0_n;
    std::vector<float>  *v_v0_E;
    std::vector<float>  *v_v0_m;
    std::vector<float>  *v_v0_sumPt;
    std::vector<float>  *v_v0_x;
    std::vector<float>  *v_v0_y;
    std::vector<float>  *v_v0_z;
    std::vector<int>    *v_v0_nTracks;
    std::vector<float>  *v_v0_radius;

    // mc truth
    Int_t           v_mc_n;
    std::vector<float>  *v_mc_E;
    std::vector<float>  *v_mc_pt;
    std::vector<float>  *v_mc_eta;
    std::vector<float>  *v_mc_phi;
    std::vector<int>    *v_mc_status;
    std::vector<int>    *v_mc_index;
    std::vector<int>    *v_mc_pdgId;
    std::vector<std::vector<int> >  *v_mc_parent_index;
    std::vector<std::vector<int> >  *v_mc_child_index;

    // Tracks
    Int_t   	     v_trk_n;
    Int_t   	     v_trk_n_jet;
    std::vector<int>     *v_trk_index;
    std::vector<float>   *v_trk_pt;
    std::vector<float>   *v_trk_eta;
    std::vector<float>   *v_trk_d0_wrtPV;
    std::vector<float>   *v_trk_z0_wrtPV;
    std::vector<float>   *v_trk_phi_wrtPV;
    std::vector<float>   *v_trk_theta_wrtPV;
    std::vector<float>   *v_trk_qoverp_wrtPV;

    std::vector<float>   *v_trk_cov_d0_wrtPV;
    std::vector<float>   *v_trk_cov_z0_wrtPV;
    std::vector<float>   *v_trk_cov_phi_wrtPV;
    std::vector<float>   *v_trk_cov_theta_wrtPV;
    std::vector<float>   *v_trk_cov_qoverp_wrtPV;
    std::vector<float>   *v_trk_chi2;
    std::vector<int>     *v_trk_ndof;

    std::vector<int>     *v_trk_nSCTHits;
    std::vector<int>     *v_trk_nSCTSpoiltHits;

    //Jets
    std::vector<float>   *v_jet_flavor_weight_SV1;
    std::vector<float>   *v_jet_flavor_weight_JetFitterCOMBNNc;
    std::vector<float>   *v_jet_flavor_component_jfitcomb_pu;
    std::vector<float>   *v_jet_flavor_component_jfitcomb_pb;
    std::vector<float>   *v_jet_flavor_component_jfitcomb_pc;
    std::vector<float>   *v_jet_AntiKt4LCTopoJets_flavor_weight_MV3_bVSc;
    std::vector<float>   *v_jet_AntiKt4LCTopoJets_flavor_weight_MV3_bVSu;
    std::vector<float>   *v_jet_AntiKt4LCTopoJets_flavor_weight_MV3_cVSu;

    std::vector<float>   *v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_mass;
    std::vector<float>   *v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_efrc;
    std::vector<float>   *v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_chi2;
    std::vector<float>   *v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_isValid;
    std::vector<float>   *v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_ntrkv;

    std::vector<float>   *v_jet_AntiKt4LCTopoJets_nTrk;

    //vxp
    // Int_t           v_vxp_n;// already in AtlEvtReaderD3PDSgTop
    std::vector<int>     *v_vxp_nTracks;
    std::vector<int>     *v_vxp_type;
    std::vector<float>   *v_vxp_x;
    std::vector<float>   *v_vxp_y;
    // std::vector<float>   *v_vxp_z;// already in AtlEvtReaderD3PDSgTop
    std::vector<std::vector<int> >  *v_vxp_trk_index;

    // ================
    // List of branches
    // ================

      // v0
    TBranch   *b_v0_n;
    TBranch   *b_v0_E;
    TBranch   *b_v0_m;
    TBranch   *b_v0_sumPt;
    TBranch   *b_v0_x;
    TBranch   *b_v0_y;
    TBranch   *b_v0_z;
    TBranch   *b_v0_nTracks;
    TBranch   *b_v0_radius;

    // mc truth
    TBranch   *b_mc_n;
    TBranch   *b_mc_E;
    TBranch   *b_mc_pt;
    TBranch   *b_mc_eta;
    TBranch   *b_mc_phi;
    TBranch   *b_mc_status;
    TBranch   *b_mc_index;
    TBranch   *b_mc_pdgId;
    TBranch   *b_mc_parent_index;
    TBranch   *b_mc_child_index;

    // Tracks
    TBranch        *b_trk_n;
    TBranch        *b_trk_n_jet;
    TBranch        *b_trk_index;
    TBranch        *b_trk_pt;
    TBranch        *b_trk_eta;
    TBranch        *b_trk_d0_wrtPV;
    TBranch        *b_trk_z0_wrtPV;
    TBranch        *b_trk_phi_wrtPV;
    TBranch        *b_trk_theta_wrtPV;
    TBranch        *b_trk_qoverp_wrtPV;

    TBranch        *b_trk_cov_d0_wrtPV;
    TBranch        *b_trk_cov_z0_wrtPV;
    TBranch        *b_trk_cov_phi_wrtPV;
    TBranch        *b_trk_cov_theta_wrtPV;
    TBranch        *b_trk_cov_qoverp_wrtPV;
    TBranch        *b_trk_chi2;
    TBranch        *b_trk_ndof;

    TBranch        *b_trk_nSCTHits;
    TBranch        *b_trk_nSCTSpoiltHits;

    //Jets

    TBranch        *b_jet_flavor_weight_SV1;
    TBranch        *b_jet_flavor_weight_JetFitterCOMBNNc;
    TBranch        *b_jet_flavor_component_jfitcomb_pu;
    TBranch        *b_jet_flavor_component_jfitcomb_pb;
    TBranch        *b_jet_flavor_component_jfitcomb_pc;
    TBranch        *b_jet_AntiKt4LCTopoJets_flavor_weight_MV3_bVSc;
    TBranch        *b_jet_AntiKt4LCTopoJets_flavor_weight_MV3_bVSu;
    TBranch        *b_jet_AntiKt4LCTopoJets_flavor_weight_MV3_cVSu;

    TBranch        *b_jet_AntiKt4LCTopoJets_flavor_component_sv0p_mass;
    TBranch        *b_jet_AntiKt4LCTopoJets_flavor_component_sv0p_efrc;
    TBranch        *b_jet_AntiKt4LCTopoJets_flavor_component_sv0p_chi2;
    TBranch        *b_jet_AntiKt4LCTopoJets_flavor_component_sv0p_isValid;
    TBranch        *b_jet_AntiKt4LCTopoJets_flavor_component_sv0p_ntrkv;

    TBranch        *b_jet_AntiKt4LCTopoJets_nTrk;

    //vxp

    // TBranch           *b_vxp_n;// already in AtlEvtReaderD3PDSgTop
    TBranch        *b_vxp_nTracks;
    TBranch        *b_vxp_type;
    TBranch        *b_vxp_x;
    TBranch        *b_vxp_y;
    // TBranch   *b_vxp_z;// already in AtlEvtReaderD3PDSgTop
    TBranch        *b_vxp_trk_index;

    AtlEvtReaderD3PDCKM(AtlSelector *parent);
    virtual ~AtlEvtReaderD3PDCKM();
    virtual void SetBranches(TTree *t);
//     virtual void   Clear();

    virtual TList* GetJPM_bQuarkJet() { return fJPM_bQuarkJet; }
    virtual TList* GetJPM_sQuarkJet() { return fJPM_sQuarkJet; }
    virtual TList* GetJPM_bQuark() { return fJPM_bQuark; }
    virtual TList* GetJPM_sQuark() { return fJPM_sQuark; }

    std::vector<int> &GetJPM_NumbJetsMatching() { return fJPM_NumbJetsMatching; }

private:

      // Jet Parton Matching
    HepMCParticle *top_quark;
    HepMCParticle *top_daughter;

    AtlJet *bsdJet;

    int bsd_quark;
    int bsdJet_index;
    int numJetsMatching;

    Double_t dR_JetQuark;
    Double_t dRTemp_JetQuark;
    Double_t RatioPt_JetQuark;

  protected:
    virtual void InitObjPointers();
    virtual void BuildEvent();
    virtual void BuildJets();
    virtual void BuildMCParticles();
    virtual void FillJPM_sbQuarkJet();

    ClassDef(AtlEvtReaderD3PDCKM,0) // Event reader for slimmed Top D3PDs
};
#endif

// copy-paste by accident??:
// class AtlEvtReaderD3PDSgTop;
