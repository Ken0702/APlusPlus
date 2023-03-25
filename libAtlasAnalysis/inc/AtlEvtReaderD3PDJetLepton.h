// reader for single-top n-tuples, LHC run 2
#ifndef ATLAS_AtlEvtReaderD3PDJetLepton
#define ATLAS_AtlEvtReaderD3PDJetLepton

#include <vector>
#include <Rtypes.h>
#include "AtlEvtReaderD3PDBase.h"

class AtlSelector;
class TBranch;
class TFile;
class TTree;
class AtlTriggerConf;

class AtlEvtReaderD3PDJetLepton : public AtlEvtReaderD3PDBase {

  public:
    typedef AtlEvtReaderD3PDBase super;

    enum EDataType {
        kUndefinedDataType,
        kRealData,
        kMC,
    };

    static size_t const PeriodLimit = 2;

  protected:
    Float_t v_weight_mc;
    std::vector<float> * v_weights_mc;
    Float_t v_weight_pileup;
    Float_t v_weight_sherpa_22_vjets;
    Float_t v_weight_jvt;
    Float_t v_weight_leptonSF;
    Float_t v_weight_bTagSF_60;
    Float_t v_weight_bTagSF_70;
    Float_t v_weight_bTagSF_77;
    Float_t v_weight_bTagSF_85;
    Float_t v_weight_bTagSF_Continuous;
    std::vector<float> * v_weight_bTagSF_60_vec;
    std::vector<float> * v_weight_bTagSF_70_vec;
    std::vector<float> * v_weight_bTagSF_77_vec;
    std::vector<float> * v_weight_bTagSF_85_vec;
    std::vector<float> * v_weight_bTagSF_Continuous_vec;
    Float_t v_weight_leptonSF_EL_SF_Trigger_UP;
    Float_t v_weight_leptonSF_EL_SF_Trigger_DOWN;
    Float_t v_weight_leptonSF_EL_SF_Reco_UP;
    Float_t v_weight_leptonSF_EL_SF_Reco_DOWN;
    Float_t v_weight_leptonSF_EL_SF_ID_UP;
    Float_t v_weight_leptonSF_EL_SF_ID_DOWN;
    Float_t v_weight_leptonSF_EL_SF_Isol_UP;
    Float_t v_weight_leptonSF_EL_SF_Isol_DOWN;
    Float_t v_weight_leptonSF_MU_SF_Trigger_STAT_UP;
    Float_t v_weight_leptonSF_MU_SF_Trigger_STAT_DOWN;
    Float_t v_weight_leptonSF_MU_SF_Trigger_SYST_UP;
    Float_t v_weight_leptonSF_MU_SF_Trigger_SYST_DOWN;
    Float_t v_weight_leptonSF_MU_SF_ID_STAT_UP;
    Float_t v_weight_leptonSF_MU_SF_ID_STAT_DOWN;
    Float_t v_weight_leptonSF_MU_SF_ID_SYST_UP;
    Float_t v_weight_leptonSF_MU_SF_ID_SYST_DOWN;
    Float_t v_weight_leptonSF_MU_SF_Isol_STAT_UP;
    Float_t v_weight_leptonSF_MU_SF_Isol_STAT_DOWN;
    Float_t v_weight_leptonSF_MU_SF_Isol_SYST_UP;
    Float_t v_weight_leptonSF_MU_SF_Isol_SYST_DOWN;
    ULong64_t v_eventNumber;
    UInt_t v_runNumber;
    UInt_t v_mcChannelNumber;
    Float_t v_mu;
    std::vector<float> * v_el_pt;
    std::vector<float> * v_el_eta;
    std::vector<float> * v_el_phi;
    std::vector<float> * v_el_e;
    std::vector<float> * v_el_charge;
    std::vector<float> * v_el_topoetcone20;
    std::vector<float> * v_el_ptvarcone20;
    std::vector<float> * v_el_d0sig;
    std::vector<int> * v_el_true_type;
    std::vector<int> * v_el_true_origin;
    std::vector<int> * v_el_true_typebkg;
    std::vector<int> * v_el_true_originbkg;

    std::vector<float> * v_mu_pt;
    std::vector<float> * v_mu_eta;
    std::vector<float> * v_mu_phi;
    std::vector<float> * v_mu_e;
    std::vector<float> * v_mu_charge;
    std::vector<float> * v_mu_topoetcone20;
    std::vector<float> * v_mu_ptvarcone30;
    std::vector<float> * v_mu_d0sig;
    std::vector<int> * v_mu_true_type;
    std::vector<int> * v_mu_true_origin;

    std::vector<float> * v_jet_pt;
    std::vector<float> * v_jet_eta;
    std::vector<float> * v_jet_phi;
    std::vector<float> * v_jet_e;
    std::vector<char> * v_jet_isbtagged_60;
    std::vector<char> * v_jet_isbtagged_70;
    std::vector<char> * v_jet_isbtagged_77;
    std::vector<char> * v_jet_isbtagged_85;
    std::vector<int> * v_jet_tagWeightBin_Continuous;
    std::vector<float> * v_jet_mv2c00;
    std::vector<float> * v_jet_mv2c10;
    std::vector<float> * v_jet_mv2c20;
    std::vector<float> * v_jet_ip3dsv1;
    std::vector<float> * v_jet_jvt;
    std::vector<int> * v_jet_truthflav;
    Float_t v_met_met;
    Float_t v_met_phi;
    Int_t v_leptonic[PeriodLimit];
    Int_t v_ejets[PeriodLimit];
    Int_t v_mujets[PeriodLimit];
    Int_t v_ee[PeriodLimit];
    Int_t v_mumu[PeriodLimit];
    Int_t v_emu[PeriodLimit];
    Int_t v_eee[PeriodLimit];
    Int_t v_eemu[PeriodLimit];
    Int_t v_emumu[PeriodLimit];
    Int_t v_mumumu[PeriodLimit];
    Int_t v_efatJet[PeriodLimit];
    Int_t v_mufatJet[PeriodLimit];
    UInt_t v_lbn;
    Float_t v_Vtxz;
    UInt_t v_npVtx;
    UInt_t v_el_n;
    UInt_t v_mu_n;
    UInt_t v_jet_n;
    std::vector<float> * v_jet_m;
    Float_t v_met_px;
    Float_t v_met_py;
    Float_t v_met_sumet;
    // bool v_genfilter_BHadron;
    // bool v_genfilter_CHadronPt4Eta3;
    // bool v_hasMEphoton_DRgt02_nonhad;

    TBranch * b_weight_mc; //!
    TBranch * b_weights_mc; //!
    TBranch * b_weight_pileup; //!
    TBranch * b_weight_sherpa_22_vjets; //!
    TBranch * b_weight_jvt; //!
    TBranch * b_weight_leptonSF; //!
    TBranch * b_weight_bTagSF_60; //!
    TBranch * b_weight_bTagSF_70; //!
    TBranch * b_weight_bTagSF_77; //!
    TBranch * b_weight_bTagSF_85; //!
    TBranch * b_weight_bTagSF_Continuous; //!
    TBranch * b_eventNumber; //!
    TBranch * b_runNumber; //!
    TBranch * b_mcChannelNumber; //!
    TBranch * b_mu; //!
    TBranch * b_el_pt; //!
    TBranch * b_el_eta; //!
    TBranch * b_el_phi; //!
    TBranch * b_el_e; //!
    TBranch * b_el_charge; //!
    TBranch * b_el_topoetcone20; //!
    TBranch * b_el_ptvarcone20; //!
    TBranch * b_mu_pt; //!
    TBranch * b_mu_eta; //!
    TBranch * b_mu_phi; //!
    TBranch * b_mu_e; //!
    TBranch * b_mu_charge; //!
    TBranch * b_mu_topoetcone20; //!
    TBranch * b_mu_ptvarcone30; //!
    TBranch * b_jet_pt; //!
    TBranch * b_jet_eta; //!
    TBranch * b_jet_phi; //!
    TBranch * b_jet_e; //!
    TBranch * b_jet_isbtagged_60; //!
    TBranch * b_jet_isbtagged_70; //!
    TBranch * b_jet_isbtagged_77; //!
    TBranch * b_jet_isbtagged_85; //!
    TBranch * b_jet_tagWeightBin_Continuous; //!
    TBranch * b_jet_mv2c00; //!
    TBranch * b_jet_mv2c10; //!
    TBranch * b_jet_mv2c20; //!
    TBranch * b_jet_ip3dsv1; //!
    TBranch * b_jet_jvt; //!
    TBranch * b_jet_truthflav; //!
    TBranch * b_met_met; //!
    TBranch * b_met_phi; //!
    TBranch * b_leptonic; //!
    TBranch * b_ejets; //!
    TBranch * b_mujets; //!
    TBranch * b_ee; //!
    TBranch * b_mumu; //!
    TBranch * b_emu; //!
    TBranch * b_eee; //!
    TBranch * b_eemu; //!
    TBranch * b_emumu; //!
    TBranch * b_mumumu; //!
    TBranch * b_efatJet; //!
    TBranch * b_mufatJet; //!
    TBranch * b_leptonic_2015; //!
    TBranch * b_ejets_2015; //!
    TBranch * b_mujets_2015; //!
    TBranch * b_ee_2015; //!
    TBranch * b_mumu_2015; //!
    TBranch * b_emu_2015; //!
    TBranch * b_eee_2015; //!
    TBranch * b_eemu_2015; //!
    TBranch * b_emumu_2015; //!
    TBranch * b_mumumu_2015; //!
    TBranch * b_efatJet_2015; //!
    TBranch * b_mufatJet_2015; //!
    TBranch * b_leptonic_2016; //!
    TBranch * b_ejets_2016; //!
    TBranch * b_mujets_2016; //!
    TBranch * b_ee_2016; //!
    TBranch * b_mumu_2016; //!
    TBranch * b_emu_2016; //!
    TBranch * b_eee_2016; //!
    TBranch * b_eemu_2016; //!
    TBranch * b_emumu_2016; //!
    TBranch * b_mumumu_2016; //!
    TBranch * b_efatJet_2016; //!
    TBranch * b_mufatJet_2016; //!
    TBranch * b_HLT_e24_lhmedium_L1EM20VH;
    TBranch * b_HLT_e24_lhmedium_iloose_L1EM20VH; //!
    TBranch * b_HLT_e24_lhmedium_L1EM18VH; //!
    TBranch * b_HLT_e24_lhmedium_nod0_L1EM18VH; //!
    TBranch * b_HLT_e26_lhtight_nod0_ivarloose;
    TBranch * b_HLT_e60_lhmedium_nod0;
    TBranch * b_HLT_e60_lhmedium;
    TBranch * b_HLT_e120_lhloose;
    TBranch * b_HLT_e140_lhloose_nod0;
    TBranch * b_HLT_mu20_iloose_L1MU15;
    TBranch * b_HLT_mu20_iloose_mu8noL1; //!
    TBranch * b_HLT_mu24;
    TBranch * b_HLT_mu24_ivarmedium;
    TBranch * b_HLT_mu26_ivarmedium;
    TBranch * b_HLT_mu50;
    TBranch * b_el_trigMatch; //!
    TBranch * b_mu_trigMatch; //!
    TBranch * b_lbn; //!
    TBranch * b_Vtxz; //!
    TBranch * b_npVtx; //!
    TBranch * b_el_n; //!
    TBranch * b_el_d0; //!
    TBranch * b_el_z0; //!
    TBranch * b_el_d0sig; //!
    TBranch * b_el_z0sig; //!
    TBranch * b_el_topoetcone30; //!
    TBranch * b_el_topoetcone40; //!
    TBranch * b_el_ptcone30; //!
    TBranch * b_el_ptcone40; //!
    TBranch * b_el_tight; //!
    TBranch * b_el_isoFixedCutTight;
    TBranch * b_el_isoFixedCutTightTrackOnly;
    TBranch * b_el_MT; //!
    TBranch * b_el_nu_pT; //!
    TBranch * b_el_nu_eta; //!
    TBranch * b_el_nu_phi; //!
    TBranch * b_el_W_pT; //!
    TBranch * b_el_W_eta; //!
    TBranch * b_el_W_phi; //!
    TBranch * b_el_true_type; //!
    TBranch * b_el_true_origin; //!
    TBranch * b_el_true_typebkg; //!
    TBranch * b_el_true_originbkg; //!
    TBranch * b_el_true_pdg; //!
    TBranch * b_el_true_pt; //!
    TBranch * b_el_true_eta; //!
    TBranch * b_mu_n; //!
    TBranch * b_mu_d0; //!
    TBranch * b_mu_z0; //!
    TBranch * b_mu_d0sig; //!
    TBranch * b_mu_z0sig; //!
    TBranch * b_mu_topoetcone30; //!
    TBranch * b_mu_topoetcone40; //!
    TBranch * b_mu_ptcone30; //!
    TBranch * b_mu_ptcone40; //!
    TBranch * b_mu_MT; //!
    TBranch * b_mu_nu_pT; //!
    TBranch * b_mu_nu_eta; //!
    TBranch * b_mu_nu_phi; //!
    TBranch * b_mu_W_pT; //!
    TBranch * b_mu_W_eta; //!
    TBranch * b_mu_W_phi; //!
    TBranch * b_mu_tight; //!
    TBranch * b_mu_isoFixedCutTightTrackOnly;
    TBranch * b_mu_true_type; //!
    TBranch * b_mu_true_origin; //!
    TBranch * b_mu_true_typebkg; //!
    TBranch * b_mu_true_originbkg; //!
    TBranch * b_mu_true_pdg; //!
    TBranch * b_mu_true_pt; //!
    TBranch * b_mu_true_eta; //!
    TBranch * b_jet_n; //!
    TBranch * b_jet_m; //!
    TBranch * b_met_px; //!
    TBranch * b_met_py; //!
    TBranch * b_met_sumet; //!
    TBranch * b_genfilter_BHadron; //!
    TBranch * b_genfilter_CHadronPt4Eta3; //!
    TBranch * b_hasMEphoton_DRgt02_nonhad; //!

    // Truth tree
    ULong64_t v_tt_eventNumber;
    UInt_t    v_tt_runNumber;
    Float_t   v_tt_MC_b_from_t_pt;
    Float_t   v_tt_MC_b_from_t_eta;
    Float_t   v_tt_MC_b_from_t_phi;
    Float_t   v_tt_MC_b_from_t_m;
    Float_t   v_tt_MC_b_from_tbar_pt;
    Float_t   v_tt_MC_b_from_tbar_eta;
    Float_t   v_tt_MC_b_from_tbar_phi;
    Float_t   v_tt_MC_b_from_tbar_m;
    TBranch  *b_tt_eventNumber; //!
    TBranch  *b_tt_runNumber; //!
    TBranch  *b_tt_MC_b_from_t_pt; //!
    TBranch  *b_tt_MC_b_from_t_eta; //!
    TBranch  *b_tt_MC_b_from_t_phi; //!
    TBranch  *b_tt_MC_b_from_t_m; //!
    TBranch  *b_tt_MC_b_from_tbar_pt; //!
    TBranch  *b_tt_MC_b_from_tbar_eta; //!
    TBranch  *b_tt_MC_b_from_tbar_phi; //!
    TBranch  *b_tt_MC_b_from_tbar_m; //!

    EDataType fDataType;
    Int_t     fBtagSFVariationComponent;
    Int_t     fMcWeightNumber;
    Int_t     fIsSherpa22;

    Int_t     fD3PDversion;
    Int_t     fPeriodCount;
    Bool_t    fForceContinBTag;        // use contin btag when fixed and contin are available (v25)
    TString   fBtagSFVariationName;
    TString   fLeptonSFVariationName;
    TString   fPileupSFVariationName;
    TString   fJvtSFVariationName;
    TString   fTreeName;

    TTree    *fTruthTree;   // MC truth tree

  public:
    AtlEvtReaderD3PDJetLepton(AtlSelector * parent, EDataType dataType,
			    const char* D3PDversion, char const * systematicName,
			    char const * treeNameSuffix, Int_t mcWeightNumber = -1);
    AtlEvtReaderD3PDJetLepton(AtlSelector * parent, EDataType dataType,
			    const char* D3PDversion, char const * treeName,
                            Int_t mcWeightNumber = -1);
    ~AtlEvtReaderD3PDJetLepton() override;

    void SetBranches(TTree * tree) override;
    InitialSumOfWeights_t GetInitialSumOfWeights(TFile * inputFile) const override;
    virtual void Print(Option_t *option = "") const override;
    char const * GetTreeName() const { return fTreeName.Data(); }
    inline void SetTreeName(char const * treename) { fTreeName = TString(treename); }
    virtual Bool_t Notify() override;

  protected:
    virtual void BuildEvent() override;
    virtual void BuildEventHeader() override;
    virtual void BuildEnergySum();
    virtual void BuildElectrons();
    virtual void BuildMuons();
    virtual void BuildJets();
    virtual void BuildFromTruthTree();
    Int_t GetEntry(TTree * t, Long64_t entry) override;
    virtual void ClearBranches();
    void LoadTruthTree();
    inline void InitObjPointers() override { ClearBranches(); }

    inline Bool_t IsMC() const {
	//
	// Is DATA or MC ?
	//
	return (fDataType == kMC);
    }

    ClassDefOverride(AtlEvtReaderD3PDJetLepton, 0)
};

#endif
