//____________________________________________________________________
//
// ATLAS Event Reader for the SgTop-DPDs at Run-II
//
// See BuildTriggerConfig() for details about the available trigger
// decision. To access the trigger information use the function
// AtlEvent::HasPassedHLT() on the current event.
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2015 (C) Oliver Maria Kind
//
#include "AtlEvtReaderD3PDSgTopR2.h"

/* standard includes */
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
//#include <strstream>

/* ROOT includes */
#include <TBranch.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TKey.h>
#include <TLorentzVector.h>
#include <TTree.h>

/* A++ includes */
#include "AtlBTag.h"
#include "AtlElectron.h"
#include "AtlEvent.h"
#include "AtlEventHeader.h"
#include "AtlIDTrack.h"
#include "AtlJet.h"
#include "AtlMuon.h"
#include "AtlScaleFactor.h"
#include <AtlTriggerConf.h>

using namespace std;

namespace {

/* units expressed in A++ units */
    static double const MeV = 1.e-3; /* in units of GeV */
    static double const mm = 1.e-1; /* in units of cm */

/* conversion functions */

/** compute a boolean from a non-zero charge */
    Bool_t IsPositive(int charge) {
        if ( !charge ) {
            gSystem->Error(__FUNCTION__, "Sign of lepton charge not defined!");
        }
        return (charge > 0 ? kTRUE : kFALSE);
    }

/** build a bitarray */
    template<typename _T>
    class BitsBuilder {
    public:
        BitsBuilder() : m_bits(static_cast<_T>(0)) { }
        void add(_T bit) {
            m_bits = static_cast<_T>(m_bits | bit);
        }
        void addIf(_T bit, int flag) {
            if (flag > 0)
                add(bit);
        }
        void addIfAny(_T bit, int const * flags, int count) {
            for ( int i = 0; i < count; ++i ) {
                if ( flags[i] > 0 ) {
                    add(bit);
                    break;
                }
            }
        }
        _T get() const {
            return m_bits;
        }
    private:
        _T m_bits;
    };

/** compute a TLorentzVector from Pt, Eta, Phi and Energy */
    TLorentzVector PtEtaPhiE(double pt, double eta, double phi, double e) {

        if(pt < 0.) pt = 0.; // for the time beginning, just to be able to run over AntiMuon15-16
        // assert(pt > 0.);
        return TLorentzVector(
            pt * cos(phi),
            pt * sin(phi),
            pt * sinh(eta),
            e
            );
    }


    long asLong(char const * str) {
        char * tailptr = 0;
        long result = strtol(str, &tailptr, 10);
        return ( *tailptr ? -1 : result );
    }


/** clear functions */
    template<typename _T> void clear(std::vector<_T> * & v) {
        if ( v ) {
            v->clear();
        }
    }

/** type-safe version of SetupBranch */
    template<typename _T>
    void SetupBranch(TTree * tree, char const * name, _T * address, TBranch * & branch, bool use = true) {
        if ( use ) {
            tree->SetBranchStatus(name, kTRUE);
            tree->SetBranchAddress(name, address, &branch);
        }
    }

}


/* !!! automatically generated content follows !!! */

//____________________________________________________________________

AtlEvtReaderD3PDSgTopR2::AtlEvtReaderD3PDSgTopR2(AtlSelector * parent,
						 EDataType dataType,
						 const char * D3PDversion,
						 char const * systematicName,
						 char const * treeNameSuffix,
						 Int_t mcWeightNumber)
    : super(parent)
    , v_weights_mc(0)
    , v_weight_bTagSF_60(NAN)
    , v_weight_bTagSF_70(NAN)
    , v_weight_bTagSF_77(NAN)
    , v_weight_bTagSF_85(NAN)
    , v_weight_bTagSF_Continuous(NAN)
    , v_weight_bTagSF_60_vec(0)
    , v_weight_bTagSF_70_vec(0)
    , v_weight_bTagSF_77_vec(0)
    , v_weight_bTagSF_85_vec(0)
    , v_weight_bTagSF_Continuous_vec(nullptr)
    , v_el_pt(0)
    , v_el_eta(0)
    , v_el_phi(0)
    , v_el_e(0)
    , v_el_charge(0)
    , v_mu_pt(0)
    , v_mu_eta(0)
    , v_mu_phi(0)
    , v_mu_e(0)
    , v_mu_charge(0)
    , v_mu_ptvarcone30(0)
    , v_jet_pt(0)
    , v_jet_eta(0)
    , v_jet_phi(0)
    , v_jet_e(0)
    , v_jet_isbtagged_60(0)
    , v_jet_isbtagged_70(0)
    , v_jet_isbtagged_77(0)
    , v_jet_isbtagged_85(0)
    , v_jet_tagWeightBin_Continuous(nullptr)
    , v_jet_mv2c00(0)
    , v_jet_mv2c10(0)
    , v_jet_mv2c20(0)
    , v_jet_ip3dsv1(0)
    , v_jet_jvt(0)
    , v_jet_truthflav(0)
    , v_el_trigMatch(0)
    , v_mu_trigMatch(0)
    , v_el_isTight(0)
    , v_el_d0(0)
    , v_el_z0(0)
    , v_el_d0sig(0)
    , v_el_z0sig(0)
    , v_el_topoetcone30(0)
    , v_el_topoetcone40(0)
    , v_el_ptcone30(0)
    , v_el_ptcone40(0)
    , v_el_tight(0)
    , v_el_isoFixedCutTight(0)
    , v_el_isoFixedCutTightTrackOnly(0)
    , v_el_tight_nobool(nullptr)
    , v_el_MT(0)
    , v_el_nu_pT(0)
    , v_el_nu_eta(0)
    , v_el_nu_phi(0)
    , v_el_W_pT(0)
    , v_el_W_eta(0)
    , v_el_W_phi(0)
    , v_el_true_type(0)
    , v_el_true_origin(0)
    , v_el_true_typebkg(0)
    , v_el_true_originbkg(0)
    , v_el_true_pdg(0)
    , v_el_true_pt(0)
    , v_el_true_eta(0)
    , v_mu_isTight(0)
    , v_mu_d0(0)
    , v_mu_z0(0)
    , v_mu_d0sig(0)
    , v_mu_z0sig(0)
    , v_mu_topoetcone30(0)
    , v_mu_topoetcone40(0)
    , v_mu_ptcone30(0)
    , v_mu_ptcone40(0)
    , v_mu_MT(0)
    , v_mu_nu_pT(0)
    , v_mu_nu_eta(0)
    , v_mu_nu_phi(0)
    , v_mu_W_pT(0)
    , v_mu_W_eta(0)
    , v_mu_W_phi(0)
    , v_mu_tight(0)
    , v_mu_tight_nobool(0)
    , v_mu_true_type(0)
    , v_mu_true_origin(0)
    , v_mu_true_typebkg(0)
    , v_mu_true_originbkg(0)
    , v_mu_true_pdg(0)
    , v_mu_true_pt(0)
    , v_mu_true_eta(0)
    , v_jet_m(0)
    , v_bdt_response(0)
    , b_weight_mc(0)
    , b_weight_pileup(0)
    , b_weight_sherpa_22_vjets(0)
    , b_weight_leptonSF(0)
    , b_weight_forwardjvt(0)
	, b_weight_globalLeptonTriggerSF(0)
    , b_weight_leptonSF_loose(0)
    , b_weight_bTagSF_60(0)
    , b_weight_bTagSF_70(0)
    , b_weight_bTagSF_77(0)
    , b_weight_bTagSF_85(0)
    , b_weight_bTagSF_Continuous(nullptr)
    , b_eventNumber(0)
    , b_runNumber(0)
    , b_mcChannelNumber(0)
    , b_mu(0)
    , b_el_pt(0)
    , b_el_eta(0)
    , b_el_phi(0)
    , b_el_e(0)
    , b_el_charge(0)
    , b_mu_pt(0)
    , b_mu_eta(0)
    , b_mu_phi(0)
    , b_mu_e(0)
    , b_mu_charge(0)
    , b_mu_ptvarcone30(0)
    , b_jet_pt(0)
    , b_jet_eta(0)
    , b_jet_phi(0)
    , b_jet_e(0)
    , b_jet_isbtagged_60(0)
    , b_jet_isbtagged_70(0)
    , b_jet_isbtagged_77(0)
    , b_jet_isbtagged_85(0)
    , b_jet_tagWeightBin_Continuous(nullptr)
    , b_jet_mv2c00(0)
    , b_jet_mv2c10(0)
    , b_jet_mv2c20(0)
    , b_jet_ip3dsv1(0)
    , b_jet_jvt(0)
    , b_met_met(0)
    , b_met_phi(0)
    , b_leptonic(0)
    , b_ejets(0)
    , b_mujets(0)
    , b_ee(0)
    , b_mumu(0)
    , b_emu(0)
    , b_eee(0)
    , b_eemu(0)
    , b_emumu(0)
    , b_mumumu(0)
    , b_efatJet(0)
    , b_mufatJet(0)
    , b_leptonic_2015(0)
    , b_ejets_2015(0)
    , b_mujets_2015(0)
    , b_ee_2015(0)
    , b_mumu_2015(0)
    , b_emu_2015(0)
    , b_eee_2015(0)
    , b_eemu_2015(0)
    , b_emumu_2015(0)
    , b_mumumu_2015(0)
    , b_efatJet_2015(0)
    , b_mufatJet_2015(0)
    , b_leptonic_2016(0)
    , b_ejets_2016(0)
    , b_mujets_2016(0)
    , b_ee_2016(0)
    , b_mumu_2016(0)
    , b_emu_2016(0)
    , b_eee_2016(0)
    , b_eemu_2016(0)
    , b_emumu_2016(0)
    , b_mumumu_2016(0)
    , b_efatJet_2016(0)
    , b_mufatJet_2016(0)
	, b_leptonic_2017(0)
    , b_ejets_2017(0)
    , b_mujets_2017(0)
    , b_ee_2017(0)
    , b_mumu_2017(0)
    , b_emu_2017(0)
    , b_eee_2017(0)
    , b_eemu_2017(0)
    , b_emumu_2017(0)
    , b_mumumu_2017(0)

		, b_leptonic_2018(0)
	    , b_ejets_2018(0)
	    , b_mujets_2018(0)
	    , b_ee_2018(0)
	    , b_mumu_2018(0)
	    , b_emu_2018(0)
	    , b_eee_2018(0)
	    , b_eemu_2018(0)
	    , b_emumu_2018(0)
	    , b_mumumu_2018(0)


    , b_HLT_e24_lhmedium_L1EM20VH(0)
    , b_HLT_e24_lhmedium_iloose_L1EM20VH(0)
    , b_HLT_e24_lhmedium_L1EM18VH(0)
    , b_HLT_e24_lhmedium_nod0_L1EM18VH(0)
    , b_HLT_e26_lhtight_nod0_ivarloose(0)
    , b_HLT_e60_lhmedium_nod0(0)
    , b_HLT_e60_lhmedium(0)
    , b_HLT_e120_lhloose(0)
    , b_HLT_e140_lhloose_nod0(0)
    , b_HLT_mu20_iloose_L1MU15(0)
    , b_HLT_mu20_iloose_mu8noL1(0)
    , b_HLT_mu24(0)
    , b_HLT_mu24_ivarmedium(0)
    , b_HLT_mu26_ivarmedium(0)
    , b_HLT_mu50(0)
    , b_el_trigMatch(0)
    , b_mu_trigMatch(0)
    , b_lbn(0)
    , b_Vtxz(0)
    , b_npVtx(0)
    , b_el_n(0)
    , b_el_isTight(0)
    , b_el_d0(0)
    , b_el_z0(0)
    , b_el_d0sig(0)
    , b_el_z0sig(0)
    , b_el_topoetcone30(0)
    , b_el_topoetcone40(0)
    , b_el_ptcone30(0)
    , b_el_ptcone40(0)
    , b_el_tight(0)
    , b_el_isoFixedCutTight(0)
    , b_el_isoFixedCutTightTrackOnly(0)
    , b_el_MT(0)
    , b_el_nu_pT(0)
    , b_el_nu_eta(0)
    , b_el_nu_phi(0)
    , b_el_W_pT(0)
    , b_el_W_eta(0)
    , b_el_W_phi(0)
    , b_el_true_type(0)
    , b_el_true_origin(0)
    , b_el_true_typebkg(0)
    , b_el_true_originbkg(0)
    , b_el_true_pdg(0)
    , b_el_true_pt(0)
    , b_el_true_eta(0)
    , b_mu_n(0)
    , b_mu_isTight(0)
    , b_mu_d0(0)
    , b_mu_z0(0)
    , b_mu_d0sig(0)
    , b_mu_z0sig(0)
    , b_mu_topoetcone30(0)
    , b_mu_topoetcone40(0)
    , b_mu_ptcone30(0)
    , b_mu_ptcone40(0)
    , b_mu_MT(0)
    , b_mu_nu_pT(0)
    , b_mu_nu_eta(0)
    , b_mu_nu_phi(0)
    , b_mu_W_pT(0)
    , b_mu_W_eta(0)
    , b_mu_W_phi(0)
    , b_mu_tight(0)
    , b_mu_true_type(0)
    , b_mu_true_origin(0)
    , b_mu_true_typebkg(0)
    , b_mu_true_originbkg(0)
    , b_mu_true_pdg(0)
    , b_mu_true_pt(0)
    , b_mu_true_eta(0)
    , b_jet_n(0)
    , b_jet_m(0)
    , b_met_px(0)
    , b_met_py(0)
    , b_met_sumet(0)
    , b_MT2(0)
    , b_pTsys(0)
    , b_sigma_pTsys(0)
    , b_Ht(0)
    , b_genfilter_BHadron(0)
    , b_genfilter_CHadronPt4Eta3(0)
    , b_hasMEphoton_DRgt02_nonhad(0)
    , b_bdt_response(0)
    , fDataType(dataType)
    , fBtagSFVariationComponent(-1)
    , fMcWeightNumber(mcWeightNumber)
    , fIsSherpa22(-1)
    , fMCWeightPosition(-1)
    , fMCWeightTotalEvents(-1.){
    //
    // Default constructor
    //
    TString vers(D3PDversion);
    vers.Remove(0, 1); // Remove "v" at the beginning
    fD3PDversion = vers.Atoi();
    TString systName(systematicName);
    fPeriodCount = -1;
    fForceContinBTag = kTRUE; // hardcode switch here for now
    fMCWeightPositionString = "";
    fMCWeightTotalEventsString = "";
    fMCWeightsProcessed = kFALSE;


    bool useNominalTree;
    if ( systName.IsNull() ||
         systName == "nominal" ||
         systName.BeginsWith("Njet_") ||
         systName == "kLEP_FAKE_BDECAYGEN_SHAPE" ||
         systName.BeginsWith("MCWEIGHTS_") ||
         systName.BeginsWith("MCWEIGHT_") ) {
        useNominalTree = true;
    }
    else {
        useNominalTree = false;

        char const * suffix = 0, * suffixUpper = 0;
        if ( systName.EndsWith("__1up") ) {
            suffix = "up"; suffixUpper = "UP";
            systName.Remove(systName.Length() - 5);
        }
        else if ( systName.EndsWith("__1down") ) {
            suffix = "down"; suffixUpper = "DOWN";
            systName.Remove(systName.Length() - 7);
        }

        int component = -1;
        char const * flavour = 0;
        if ( systName.BeginsWith("bTagSF_B_") ) {
            flavour = "B";
            component = asLong(systName.Data() + 9);
        }
        else if ( systName.BeginsWith("bTagSF_C_") ) {
            flavour = "C";
            component = asLong(systName.Data() + 9);
        }
        else if ( systName.BeginsWith("bTagSF_Light_") ) {
            flavour = "Light";
            component = asLong(systName.Data() + 13);
        }
        else if ( systName.BeginsWith("bTagSF_extrapolation") ) {
            useNominalTree = true;
            systName.Remove(0, 7);
            fBtagSFVariationName = Form("_%s_%s", systName.Data(), suffix);
        }
        else {
            assert( !systName.BeginsWith("bTagSF_") );
        }
        if ( flavour ) {
            useNominalTree = true;
            fBtagSFVariationName = Form("_eigenvars_%s_%s", flavour, suffix);
            fBtagSFVariationComponent = component;
        }

        if ( systName.BeginsWith("EL_SF_") || systName.BeginsWith("MU_SF_") ) {
            useNominalTree = true;
            fLeptonSFVariationName = Form("_%s_%s", systName.Data(), suffixUpper);
        }

        if ( systName.BeginsWith("EL_Trigger") || systName.BeginsWith("MU_Trigger") ) { //this is not correct yet
                  useNominalTree = true;
                  //fGlobalLeptonTriggerSFVariationName = "_EL_Trigger_DOWN";
                  fGlobalLeptonTriggerSFVariationName = Form("_%s_%s", systName.Data(), suffixUpper);
        // Not needed: See Comment at SetupBranch
        }


        if ( systName == "Pileup_SF" ) {
            useNominalTree = true;
            fPileupSFVariationName = Form("_%s", suffixUpper);
        }

        if ( systName == "JVT_SF" ) {
            useNominalTree = true;
            fJvtSFVariationName = Form("_%s", suffixUpper);
        }
        if ( systName == "ForwardJVT_SF" ) {
            useNominalTree = true;
            fForwardJVTSFVariationName = Form("_%s", suffixUpper);
        }



    }
    fTreeName = Form("%s%s", ( useNominalTree ? "nominal" : systematicName ), treeNameSuffix);
    fTruthTree = 0;

    // Init trigger config
    fTriggerConfDbase = AtlTriggerConf::Instance();
    AtlTrigger::SetConfig(fTriggerConfDbase);

    // Create trigger tree to store the configurations
    fTriggerConfTree = fTriggerConfDbase->CreateTree();
}

//____________________________________________________________________

AtlEvtReaderD3PDSgTopR2::AtlEvtReaderD3PDSgTopR2(AtlSelector * parent,
						 EDataType dataType,
						 const char * D3PDversion,
                                                 char const * treeName,
						 Int_t mcWeightNumber)
    : AtlEvtReaderD3PDSgTopR2(parent, dataType, D3PDversion, "nominal",
                              "", mcWeightNumber) {
    //
    // Same function as first constructor, but for setting the
    // tree name directly in the constructor
    //
    SetTreeName(treeName);
}

//____________________________________________________________________

AtlEvtReaderD3PDSgTopR2::~AtlEvtReaderD3PDSgTopR2() {
    //
    // Default destructor
    //
    delete fTriggerConfDbase;
}

//____________________________________________________________________

Int_t AtlEvtReaderD3PDSgTopR2::GetEntry(TTree *t, Long64_t entry) {
    //
    // Clear all branches and get tree entry
    //
    ClearBranches();
    return super::GetEntry(t, entry);
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTopR2::ClearBranches() {
    //
    // Clear all tree branches
    //
    v_weight_mc = NAN;
    clear(v_weights_mc);
    v_weight_pileup = NAN;
    v_weight_sherpa_22_vjets = NAN;
    v_weight_jvt = NAN;
    v_weight_forwardjvt = NAN;
    v_weight_globalLeptonTriggerSF = NAN;
    v_weight_leptonSF = NAN;
    v_weight_leptonSF_loose = NAN;
    v_weight_bTagSF_60 = NAN;
    v_weight_bTagSF_70 = NAN;
    v_weight_bTagSF_77 = NAN;
    v_weight_bTagSF_85 = NAN;
    v_weight_bTagSF_Continuous = NAN;
    clear(v_weight_bTagSF_60_vec);
    clear(v_weight_bTagSF_70_vec);
    clear(v_weight_bTagSF_77_vec);
    clear(v_weight_bTagSF_85_vec);
    clear(v_weight_bTagSF_Continuous_vec);
    v_eventNumber = std::numeric_limits<UInt_t>::max();
    v_runNumber = std::numeric_limits<UInt_t>::max();
    v_mcChannelNumber = std::numeric_limits<UInt_t>::max();
    v_mu = NAN;
    clear(v_el_pt);
    clear(v_el_eta);
    clear(v_el_phi);
    clear(v_el_e);
    clear(v_el_charge);
    clear(v_mu_pt);
    clear(v_mu_eta);
    clear(v_mu_phi);
    clear(v_mu_e);
    clear(v_mu_charge);
    clear(v_mu_ptvarcone30);
    clear(v_jet_pt);
    clear(v_jet_eta);
    clear(v_jet_phi);
    clear(v_jet_e);
    clear(v_jet_isbtagged_60);
    clear(v_jet_isbtagged_70);
    clear(v_jet_isbtagged_77);
    clear(v_jet_isbtagged_85);
    clear(v_jet_tagWeightBin_Continuous);
    clear(v_jet_mv2c00);
    clear(v_jet_mv2c10);
    clear(v_jet_mv2c20);
    clear(v_jet_ip3dsv1);
    clear(v_jet_jvt);
    clear(v_jet_truthflav);
    v_met_met = NAN;
    v_met_phi = NAN;
    v_HLT_e24_lhmedium_L1EM20VH = std::numeric_limits<Char_t>::min();
    v_HLT_e24_lhmedium_iloose_L1EM20VH = std::numeric_limits<Char_t>::min();
    v_HLT_e24_lhmedium_L1EM18VH = std::numeric_limits<Char_t>::min();
    v_HLT_e24_lhmedium_nod0_L1EM18VH = std::numeric_limits<Char_t>::min();
    v_HLT_e26_lhtight_nod0_ivarloose = std::numeric_limits<Char_t>::min();
    v_HLT_e60_lhmedium_nod0 = std::numeric_limits<Char_t>::min();
    v_HLT_e60_lhmedium = std::numeric_limits<Char_t>::min();
    v_HLT_e120_lhloose = std::numeric_limits<Char_t>::min();
    v_HLT_e140_lhloose_nod0 = std::numeric_limits<Char_t>::min();
    v_HLT_mu20_iloose_L1MU15 = std::numeric_limits<Char_t>::min();
    v_HLT_mu20_iloose_mu8noL1 = std::numeric_limits<Char_t>::min();
    v_HLT_mu24 = std::numeric_limits<Char_t>::min();
    v_HLT_mu24_ivarmedium = std::numeric_limits<Char_t>::min();
    v_HLT_mu26_ivarmedium = std::numeric_limits<Char_t>::min();
    v_HLT_mu50 = std::numeric_limits<Char_t>::min();
    clear(v_el_trigMatch);
    clear(v_mu_trigMatch);
    v_lbn = std::numeric_limits<UInt_t>::max();
    v_Vtxz = NAN;
    v_npVtx = std::numeric_limits<UInt_t>::max();
    v_el_n = std::numeric_limits<UInt_t>::max();
    clear(v_el_d0);
    clear(v_el_z0);
    clear(v_el_d0sig);
    clear(v_el_z0sig);
    clear(v_el_topoetcone30);
    clear(v_el_topoetcone40);
    clear(v_el_ptcone30);
    clear(v_el_ptcone40);
    clear(v_el_tight);
    clear(v_el_isoFixedCutTight);
    clear(v_el_isoFixedCutTightTrackOnly);
    clear(v_el_tight_nobool);
    clear(v_el_MT);
    clear(v_el_nu_pT);
    clear(v_el_nu_eta);
    clear(v_el_nu_phi);
    clear(v_el_W_pT);
    clear(v_el_W_eta);
    clear(v_el_W_phi);
    clear(v_el_true_type);
    clear(v_el_true_origin);
    clear(v_el_true_typebkg);
    clear(v_el_true_originbkg);
    clear(v_el_true_pdg);
    clear(v_el_true_pt);
    clear(v_el_true_eta);
    v_mu_n = std::numeric_limits<UInt_t>::max();
    clear(v_mu_d0);
    clear(v_mu_z0);
    clear(v_mu_d0sig);
    clear(v_mu_z0sig);
    clear(v_mu_topoetcone30);
    clear(v_mu_topoetcone40);
    clear(v_mu_ptcone30);
    clear(v_mu_ptcone40);
    clear(v_mu_MT);
    clear(v_mu_nu_pT);
    clear(v_mu_nu_eta);
    clear(v_mu_nu_phi);
    clear(v_mu_W_pT);
    clear(v_mu_W_eta);
    clear(v_mu_W_phi);
    clear(v_mu_tight);
    clear(v_mu_tight_nobool);
    clear(v_mu_true_type);
    clear(v_mu_true_origin);
    clear(v_mu_true_typebkg);
    clear(v_mu_true_originbkg);
    clear(v_mu_true_pdg);
    clear(v_mu_true_pt);
    clear(v_mu_true_eta);
    v_jet_n = std::numeric_limits<UInt_t>::max();
    clear(v_jet_m);
    v_met_px = NAN;
    v_met_py = NAN;
    v_met_sumet = NAN;
    v_MT2 = NAN;
    v_pTsys = NAN;
    v_sigma_pTsys = NAN;
    v_Ht = NAN;
    v_genfilter_BHadron = false;
    v_genfilter_CHadronPt4Eta3 = false;
    v_hasMEphoton_DRgt02_nonhad = false;
    v_bdt_response = NAN;
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTopR2::SetBranches(TTree *t) {
    //
    // Set branch addresses
    //

    bool isMC = IsMC();
    InitBranches(t);
    //Info(__FUNCTION__, Form("HELLO %d", fMCWeightPosition));
    if ( fMcWeightNumber > 0 || fMCWeightPosition < -1 ) { //kkreul : was >= 0
        if ( fD3PDversion >= 23 ) {
            ::SetupBranch(t, "mc_generator_weights", &v_weights_mc, b_weights_mc, isMC);
        } else {
            ::SetupBranch(t, "weights_mc", &v_weights_mc, b_weights_mc, isMC);
        }
    }
    else {

        ::SetupBranch(t, "weight_mc", &v_weight_mc, b_weight_mc, isMC);
    }
    ::SetupBranch(t, Form("weight_pileup%s", fPileupSFVariationName.Data()), &v_weight_pileup, b_weight_pileup, isMC);
    if ( fD3PDversion >= 12 && fD3PDversion < 20) {
        ::SetupBranch(t, "weight_sherpa_22_vjets", &v_weight_sherpa_22_vjets, b_weight_sherpa_22_vjets, isMC);
    }
    ::SetupBranch(t, Form("weight_jvt%s", fJvtSFVariationName.Data()), &v_weight_jvt, b_weight_jvt, isMC);
    ::SetupBranch(t, Form("weight_forwardjvt%s", fForwardJVTSFVariationName.Data()), &v_weight_forwardjvt, b_weight_forwardjvt, isMC);

    ::SetupBranch(t, Form("weight_globalLeptonTriggerSF%s", fGlobalLeptonTriggerSFVariationName.Data()), &v_weight_globalLeptonTriggerSF, b_weight_globalLeptonTriggerSF, isMC);
    //GlobalLeptonTrigger is redundant bc its already in LeptonSF!! I still read it here and fill it in EventHeader but I
    //don't multiply it in AtlObjRecoScaleFactorTool


    if ( fD3PDversion == 25 ) {
        ::SetupBranch(t, Form("weight_leptonSF_tight%s", fLeptonSFVariationName.Data()), &v_weight_leptonSF, b_weight_leptonSF, isMC);
        ::SetupBranch(t, Form("weight_leptonSF_loose%s", fLeptonSFVariationName.Data()), &v_weight_leptonSF_loose, b_weight_leptonSF_loose, isMC);
    } else {
        ::SetupBranch(t, Form("weight_leptonSF%s", fLeptonSFVariationName.Data()), &v_weight_leptonSF, b_weight_leptonSF, isMC);
    }
    //SETUP DL1r HERE !
    if ( fD3PDversion >= 31 ) {
    	Info(__FUNCTION__, Form("weight_bTagSF_DL1r_Continuous%s", fBtagSFVariationName.Data()));
    	if ( fBtagSFVariationComponent >= 0 ) {
    	            assert(!fBtagSFVariationName.IsNull());
    	            ::SetupBranch(t, Form("weight_bTagSF_DL1r_Continuous%s", fBtagSFVariationName.Data()),
    	                          &v_weight_bTagSF_Continuous_vec, b_weight_bTagSF_Continuous, isMC);
    	        } else {
    	            ::SetupBranch(t, Form("weight_bTagSF_DL1r_Continuous%s", fBtagSFVariationName.Data()),
    	                          &v_weight_bTagSF_Continuous, b_weight_bTagSF_Continuous, isMC);
    	        }
    }

    else if ( fD3PDversion >= 25 && fD3PDversion < 31 ) {
        if ( fBtagSFVariationComponent >= 0 ) {
            assert(!fBtagSFVariationName.IsNull());
            ::SetupBranch(t, Form("weight_bTagSF_MV2c10_Continuous%s", fBtagSFVariationName.Data()),
                          &v_weight_bTagSF_Continuous_vec, b_weight_bTagSF_Continuous, isMC);
        } else {
            ::SetupBranch(t, Form("weight_bTagSF_MV2c10_Continuous%s", fBtagSFVariationName.Data()),
                          &v_weight_bTagSF_Continuous, b_weight_bTagSF_Continuous, isMC);
        }
    }
    if ( fD3PDversion >= 29 ) {
        // no fixed WP b-tagging
    } else if ( fD3PDversion >= 20 ) {
        if ( fBtagSFVariationComponent >= 0 ) {
            assert(!fBtagSFVariationName.IsNull());
            ::SetupBranch(t, Form("weight_bTagSF_MV2c10_60%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_60_vec, b_weight_bTagSF_60, isMC);
            ::SetupBranch(t, Form("weight_bTagSF_MV2c10_70%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_70_vec, b_weight_bTagSF_70, isMC);
            ::SetupBranch(t, Form("weight_bTagSF_MV2c10_77%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_77_vec, b_weight_bTagSF_77, isMC);
            ::SetupBranch(t, Form("weight_bTagSF_MV2c10_85%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_85_vec, b_weight_bTagSF_85, isMC);
        }
        else {
            ::SetupBranch(t, Form("weight_bTagSF_MV2c10_60%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_60, b_weight_bTagSF_60, isMC);
            ::SetupBranch(t, Form("weight_bTagSF_MV2c10_70%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_70, b_weight_bTagSF_70, isMC);
            ::SetupBranch(t, Form("weight_bTagSF_MV2c10_77%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_77, b_weight_bTagSF_77, isMC);
            ::SetupBranch(t, Form("weight_bTagSF_MV2c10_85%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_85, b_weight_bTagSF_85, isMC);
        }
    } else {
        if ( fBtagSFVariationComponent >= 0 ) {
            assert(!fBtagSFVariationName.IsNull());
            ::SetupBranch(t, Form("weight_bTagSF_70%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_70_vec, b_weight_bTagSF_70, isMC);
            ::SetupBranch(t, Form("weight_bTagSF_77%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_77_vec, b_weight_bTagSF_77, isMC);
            ::SetupBranch(t, Form("weight_bTagSF_85%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_85_vec, b_weight_bTagSF_85, isMC);
        }
        else {
            ::SetupBranch(t, Form("weight_bTagSF_70%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_70, b_weight_bTagSF_70, isMC);
            ::SetupBranch(t, Form("weight_bTagSF_77%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_77, b_weight_bTagSF_77, isMC);
            ::SetupBranch(t, Form("weight_bTagSF_85%s", fBtagSFVariationName.Data()), &v_weight_bTagSF_85, b_weight_bTagSF_85, isMC);
        }
    }
    ::SetupBranch(t, "eventNumber", &v_eventNumber, b_eventNumber);
    ::SetupBranch(t, "runNumber", &v_runNumber, b_runNumber); //Default
    //::SetupBranch(t, "randomRunNumber", &v_runNumber, b_runNumber);
    ::SetupBranch(t, "mcChannelNumber", &v_mcChannelNumber, b_mcChannelNumber);
    ::SetupBranch(t, "mu", &v_mu, b_mu);
    ::SetupBranch(t, "el_pt", &v_el_pt, b_el_pt);
    ::SetupBranch(t, "el_eta", &v_el_eta, b_el_eta);
    ::SetupBranch(t, "el_phi", &v_el_phi, b_el_phi);
    ::SetupBranch(t, "el_e", &v_el_e, b_el_e);
    ::SetupBranch(t, "el_charge", &v_el_charge, b_el_charge);
    ::SetupBranch(t, "mu_pt", &v_mu_pt, b_mu_pt);
    ::SetupBranch(t, "mu_eta", &v_mu_eta, b_mu_eta);
    ::SetupBranch(t, "mu_phi", &v_mu_phi, b_mu_phi);
    ::SetupBranch(t, "mu_e", &v_mu_e, b_mu_e);
    ::SetupBranch(t, "mu_charge", &v_mu_charge, b_mu_charge);
    if(fD3PDversion < 31)  ::SetupBranch(t, "mu_ptvarcone30", &v_mu_ptvarcone30, b_mu_ptvarcone30);
    ::SetupBranch(t, "jet_pt", &v_jet_pt, b_jet_pt);
    ::SetupBranch(t, "jet_eta", &v_jet_eta, b_jet_eta);
    ::SetupBranch(t, "jet_phi", &v_jet_phi, b_jet_phi);
    ::SetupBranch(t, "jet_e", &v_jet_e, b_jet_e);
    if ( fD3PDversion >= 25 && fD3PDversion < 31) {
        ::SetupBranch(t, "jet_tagWeightBin_MV2c10_Continuous",
                      &v_jet_tagWeightBin_Continuous, b_jet_tagWeightBin_Continuous);
    }
    else{ //v31
    	::SetupBranch(t, "jet_tagWeightBin_DL1r_Continuous",
    	             &v_jet_tagWeightBin_Continuous, b_jet_tagWeightBin_Continuous);

    }
    if ( fD3PDversion >= 29 ) {
        // no fixed WP b-tagging
    } else if ( fD3PDversion >= 20 ) {
        ::SetupBranch(t, "jet_isbtagged_MV2c10_60", &v_jet_isbtagged_60, b_jet_isbtagged_60);
        ::SetupBranch(t, "jet_isbtagged_MV2c10_70", &v_jet_isbtagged_70, b_jet_isbtagged_70);
        ::SetupBranch(t, "jet_isbtagged_MV2c10_77", &v_jet_isbtagged_77, b_jet_isbtagged_77);
        ::SetupBranch(t, "jet_isbtagged_MV2c10_85", &v_jet_isbtagged_85, b_jet_isbtagged_85);
    }
    else if ( fD3PDversion >= 13 ) {
        ::SetupBranch(t, "jet_isbtagged_70", &v_jet_isbtagged_70, b_jet_isbtagged_70);
        ::SetupBranch(t, "jet_isbtagged_77", &v_jet_isbtagged_77, b_jet_isbtagged_77);
        ::SetupBranch(t, "jet_isbtagged_85", &v_jet_isbtagged_85, b_jet_isbtagged_85);
    }
    else {
        ::SetupBranch(t, "jet_mv2c00", &v_jet_mv2c00, b_jet_mv2c00);
        ::SetupBranch(t, "jet_mv2c10", &v_jet_mv2c10, b_jet_mv2c10);
        ::SetupBranch(t, "jet_mv2c20", &v_jet_mv2c20, b_jet_mv2c20);
        ::SetupBranch(t, "jet_ip3dsv1", &v_jet_ip3dsv1, b_jet_ip3dsv1);
    }
    if ( fD3PDversion < 20 )
        ::SetupBranch(t, "jet_jvt", &v_jet_jvt, b_jet_jvt);
    if(isMC){ //kkreul data not working
    	::SetupBranch(t, "jet_truthflav", &v_jet_truthflav, b_jet_truthflav);
    }
    ::SetupBranch(t, "met_met", &v_met_met, b_met_met);
    ::SetupBranch(t, "met_phi", &v_met_phi, b_met_phi);
    if ( fD3PDversion >= 12 ) {
    	if(fD3PDversion >= 31){ //v31
    		::SetupBranch(t, "ejets_2015", &v_ejets[0], b_ejets_2015);
    		::SetupBranch(t, "ejets_2016", &v_ejets[0], b_ejets_2016);
    		::SetupBranch(t, "ejets_2017", &v_ejets[0], b_ejets_2017);
    		::SetupBranch(t, "ejets_2018", &v_ejets[0], b_ejets_2018);
    		::SetupBranch(t, "mujets_2015", &v_mujets[0], b_mujets_2015);
    		::SetupBranch(t, "mujets_2016", &v_mujets[0], b_mujets_2016);
    		::SetupBranch(t, "mujets_2017", &v_mujets[0], b_mujets_2017);
    		::SetupBranch(t, "mujets_2018", &v_mujets[0], b_mujets_2018);
    	}
    	else{
			::SetupBranch(t, "leptonic_2015", &v_leptonic[0], b_leptonic_2015);
			::SetupBranch(t, "ejets_2015", &v_ejets[0], b_ejets_2015);
			::SetupBranch(t, "mujets_2015", &v_mujets[0], b_mujets_2015);
			::SetupBranch(t, "ee_2015", &v_ee[0], b_ee_2015);
			::SetupBranch(t, "mumu_2015", &v_mumu[0], b_mumu_2015);
			::SetupBranch(t, "emu_2015", &v_emu[0], b_emu_2015);
			::SetupBranch(t, "eee_2015", &v_eee[0], b_eee_2015);
			::SetupBranch(t, "eemu_2015", &v_eemu[0], b_eemu_2015);
			::SetupBranch(t, "emumu_2015", &v_emumu[0], b_emumu_2015);
			::SetupBranch(t, "mumumu_2015", &v_mumumu[0], b_mumumu_2015);
			::SetupBranch(t, "leptonic_2016", &v_leptonic[1], b_leptonic_2016);
			::SetupBranch(t, "ejets_2016", &v_ejets[1], b_ejets_2016);
			::SetupBranch(t, "mujets_2016", &v_mujets[1], b_mujets_2016);
			::SetupBranch(t, "ee_2016", &v_ee[1], b_ee_2016);
			::SetupBranch(t, "mumu_2016", &v_mumu[1], b_mumu_2016);
			::SetupBranch(t, "emu_2016", &v_emu[1], b_emu_2016);
			::SetupBranch(t, "eee_2016", &v_eee[1], b_eee_2016);
			::SetupBranch(t, "eemu_2016", &v_eemu[1], b_eemu_2016);
			::SetupBranch(t, "emumu_2016", &v_emumu[1], b_emumu_2016);
			::SetupBranch(t, "mumumu_2016", &v_mumumu[1], b_mumumu_2016);
			::SetupBranch(t, "leptonic_2016", &v_leptonic[1], b_leptonic_2016);
					::SetupBranch(t, "ejets_2017", &v_ejets[2], b_ejets_2017);
					::SetupBranch(t, "mujets_2017", &v_mujets[2], b_mujets_2017);
					::SetupBranch(t, "ee_2017", &v_ee[2], b_ee_2017);
					::SetupBranch(t, "mumu_2017", &v_mumu[2], b_mumu_2017);
					::SetupBranch(t, "emu_2017", &v_emu[2], b_emu_2017);
					::SetupBranch(t, "eee_2017", &v_eee[2], b_eee_2017);
					::SetupBranch(t, "eemu_2017", &v_eemu[2], b_eemu_2017);
					::SetupBranch(t, "emumu_2017", &v_emumu[2], b_emumu_2017);
					::SetupBranch(t, "mumumu_2017", &v_mumumu[2], b_mumumu_2017);
			::SetupBranch(t, "ejets_2018", &v_ejets[3], b_ejets_2018);
			::SetupBranch(t, "mujets_2018", &v_mujets[3], b_mujets_2018);
			::SetupBranch(t, "ee_2018", &v_ee[3], b_ee_2018);
			::SetupBranch(t, "mumu_2018", &v_mumu[3], b_mumu_2018);
			::SetupBranch(t, "emu_2018", &v_emu[3], b_emu_2018);
			::SetupBranch(t, "eee_2018", &v_eee[3], b_eee_2018);
			::SetupBranch(t, "eemu_2018", &v_eemu[3], b_eemu_2018);
			::SetupBranch(t, "emumu_2018", &v_emumu[3], b_emumu_2018);
			::SetupBranch(t, "mumumu_2018", &v_mumumu[3], b_mumumu_2018);
    	}
		if ( fD3PDversion < 20 ) {
				::SetupBranch(t, "efatJet_2015", &v_efatJet[0], b_efatJet_2015);
				::SetupBranch(t, "mufatJet_2015", &v_mufatJet[0], b_mufatJet_2015);
				::SetupBranch(t, "efatJet_2016", &v_efatJet[1], b_efatJet_2016);
				::SetupBranch(t, "mufatJet_2016", &v_mufatJet[1], b_mufatJet_2016);
        }
        fPeriodCount = 2;
    }
    else {
        ::SetupBranch(t, "leptonic", &v_leptonic[0], b_leptonic);
        ::SetupBranch(t, "ejets", &v_ejets[0], b_ejets);
        ::SetupBranch(t, "mujets", &v_mujets[0], b_mujets);
        ::SetupBranch(t, "ee", &v_ee[0], b_ee);
        ::SetupBranch(t, "mumu", &v_mumu[0], b_mumu);
        ::SetupBranch(t, "emu", &v_emu[0], b_emu);
        ::SetupBranch(t, "eee", &v_eee[0], b_eee);
        ::SetupBranch(t, "eemu", &v_eemu[0], b_eemu);
        ::SetupBranch(t, "emumu", &v_emumu[0], b_emumu);
        ::SetupBranch(t, "mumumu", &v_mumumu[0], b_mumumu);
        ::SetupBranch(t, "efatJet", &v_efatJet[0], b_efatJet);
        ::SetupBranch(t, "mufatJet", &v_mufatJet[0], b_mufatJet);
        fPeriodCount = 1;
    }
    if ( fD3PDversion < 9 ) {
	::SetupBranch(t, "HLT_e24_lhmedium_iloose_L1EM20VH", &v_HLT_e24_lhmedium_iloose_L1EM20VH, b_HLT_e24_lhmedium_iloose_L1EM20VH);
	::SetupBranch(t, "HLT_e60_lhmedium", &v_HLT_e60_lhmedium, b_HLT_e60_lhmedium);
	::SetupBranch(t, "HLT_mu20_iloose_L1MU15", &v_HLT_mu20_iloose_L1MU15, b_HLT_mu20_iloose_L1MU15);
	::SetupBranch(t, "HLT_mu20_iloose_mu8noL1", &v_HLT_mu20_iloose_mu8noL1, b_HLT_mu20_iloose_mu8noL1);
	::SetupBranch(t, "HLT_mu50", &v_HLT_mu50, b_HLT_mu50);
    } else if ( fD3PDversion >= 9 && fD3PDversion < 15 ) {
	::SetupBranch(t, "HLT_e24_lhmedium_L1EM18VH", &v_HLT_e24_lhmedium_L1EM18VH, b_HLT_e24_lhmedium_L1EM18VH);
	::SetupBranch(t, "HLT_e60_lhmedium", &v_HLT_e60_lhmedium, b_HLT_e60_lhmedium);
	::SetupBranch(t, "HLT_e120_lhloose", &v_HLT_e120_lhloose, b_HLT_e120_lhloose);
	::SetupBranch(t, "HLT_mu20_iloose_L1MU15", &v_HLT_mu20_iloose_L1MU15, b_HLT_mu20_iloose_L1MU15);
	::SetupBranch(t, "HLT_mu50", &v_HLT_mu50, b_HLT_mu50);
    } else if( fD3PDversion < 31 ) {
        if ( fD3PDversion >= 20 && fD3PDversion < 29 ) {
            ::SetupBranch(t, "HLT_e24_lhmedium_nod0_L1EM18VH", &v_HLT_e24_lhmedium_nod0_L1EM18VH, b_HLT_e24_lhmedium_nod0_L1EM18VH);
            ::SetupBranch(t, "HLT_mu24", &v_HLT_mu24, b_HLT_mu24);
            ::SetupBranch(t, "HLT_mu24_ivarmedium", &v_HLT_mu24_ivarmedium, b_HLT_mu24_ivarmedium);
        }
	::SetupBranch(t, "HLT_e24_lhmedium_L1EM20VH", &v_HLT_e24_lhmedium_L1EM20VH, b_HLT_e24_lhmedium_L1EM20VH);
	::SetupBranch(t, "HLT_e26_lhtight_nod0_ivarloose", &v_HLT_e26_lhtight_nod0_ivarloose, b_HLT_e26_lhtight_nod0_ivarloose);
	::SetupBranch(t, "HLT_e60_lhmedium", &v_HLT_e60_lhmedium, b_HLT_e60_lhmedium);
	::SetupBranch(t, "HLT_e60_lhmedium_nod0", &v_HLT_e60_lhmedium_nod0, b_HLT_e60_lhmedium_nod0);
	::SetupBranch(t, "HLT_e120_lhloose", &v_HLT_e120_lhloose, b_HLT_e120_lhloose);
	::SetupBranch(t, "HLT_e140_lhloose_nod0", &v_HLT_e140_lhloose_nod0, b_HLT_e140_lhloose_nod0);
	::SetupBranch(t, "HLT_mu20_iloose_L1MU15", &v_HLT_mu20_iloose_L1MU15, b_HLT_mu20_iloose_L1MU15);
	::SetupBranch(t, "HLT_mu26_ivarmedium", &v_HLT_mu26_ivarmedium, b_HLT_mu26_ivarmedium);
	::SetupBranch(t, "HLT_mu50", &v_HLT_mu50, b_HLT_mu50);
    }
    if ( fD3PDversion < 29 ) {
        ::SetupBranch(t, "el_trigMatch", &v_el_trigMatch, b_el_trigMatch);
        ::SetupBranch(t, "mu_trigMatch", &v_mu_trigMatch, b_mu_trigMatch);
    }
    ::SetupBranch(t, "lbn", &v_lbn, b_lbn);
    ::SetupBranch(t, "Vtxz", &v_Vtxz, b_Vtxz);
    ::SetupBranch(t, "npVtx", &v_npVtx, b_npVtx);
    if ( fD3PDversion < 21 )
        ::SetupBranch(t, "el_n", &v_el_n, b_el_n);
    if ( fD3PDversion < 9 ) {
        ::SetupBranch(t, "el_d0", &v_el_d0, b_el_d0);
        ::SetupBranch(t, "el_z0", &v_el_z0, b_el_z0);
    }
    if ( fD3PDversion > 20 )
        ::SetupBranch(t, "el_isTight", &v_el_isTight, b_el_isTight);
    if ( fD3PDversion < 29 )
        ::SetupBranch(t, "el_d0sig", &v_el_d0sig, b_el_d0sig);
    if ( fD3PDversion < 9 ) {
        ::SetupBranch(t, "el_z0sig", &v_el_z0sig, b_el_z0sig);
        ::SetupBranch(t, "el_topoetcone30", &v_el_topoetcone30, b_el_topoetcone30);
        ::SetupBranch(t, "el_topoetcone40", &v_el_topoetcone40, b_el_topoetcone40);
        ::SetupBranch(t, "el_ptcone30", &v_el_ptcone30, b_el_ptcone30);
        ::SetupBranch(t, "el_ptcone40", &v_el_ptcone40, b_el_ptcone40);
    }
    if ( fD3PDversion >= 29 ) {
        // changed all std::vector<bool> to std::vector<char>
        ::SetupBranch(t, "el_tight", &v_el_tight_nobool, b_el_tight);
    } else {
        ::SetupBranch(t, "el_tight", &v_el_tight, b_el_tight);
    }
    if ( fD3PDversion >= 23 && fD3PDversion < 29) {
        ::SetupBranch(t, "el_isoFixedCutTight", &v_el_isoFixedCutTight, b_el_isoFixedCutTight);
        ::SetupBranch(t, "el_isoFixedCutTightTrackOnly", &v_el_isoFixedCutTightTrackOnly, b_el_isoFixedCutTightTrackOnly);
    }
    if ( fD3PDversion < 21 ) {
        ::SetupBranch(t, "el_MT", &v_el_MT, b_el_MT);
        ::SetupBranch(t, "el_nu_pT", &v_el_nu_pT, b_el_nu_pT);
        ::SetupBranch(t, "el_nu_eta", &v_el_nu_eta, b_el_nu_eta);
        ::SetupBranch(t, "el_nu_phi", &v_el_nu_phi, b_el_nu_phi);
    }
    if ( fD3PDversion < 9 ) {
        ::SetupBranch(t, "el_W_pT", &v_el_W_pT, b_el_W_pT);
        ::SetupBranch(t, "el_W_eta", &v_el_W_eta, b_el_W_eta);
        ::SetupBranch(t, "el_W_phi", &v_el_W_phi, b_el_W_phi);
    }
    ::SetupBranch(t, "el_true_type", &v_el_true_type, b_el_true_type, isMC);
    ::SetupBranch(t, "el_true_origin", &v_el_true_origin, b_el_true_origin, isMC);
    if ( fD3PDversion < 21 ) {
        ::SetupBranch(t, "el_true_typebkg", &v_el_true_typebkg, b_el_true_typebkg, isMC);
        ::SetupBranch(t, "el_true_originbkg", &v_el_true_originbkg, b_el_true_originbkg, isMC);
    }
    ::SetupBranch(t, "el_true_pdg", &v_el_true_pdg, b_el_true_pdg, isMC);
    ::SetupBranch(t, "el_true_pt", &v_el_true_pt, b_el_true_pt, isMC);
    ::SetupBranch(t, "el_true_eta", &v_el_true_eta, b_el_true_eta, isMC);
    if ( fD3PDversion < 21 )
        ::SetupBranch(t, "mu_n", &v_mu_n, b_mu_n);
    if ( fD3PDversion < 9 ) {
        ::SetupBranch(t, "mu_d0", &v_mu_d0, b_mu_d0);
        ::SetupBranch(t, "mu_z0", &v_mu_z0, b_mu_z0);
    }
    if ( fD3PDversion > 20 )
        ::SetupBranch(t, "mu_isTight", &v_mu_isTight, b_mu_isTight);
    if ( fD3PDversion < 29 )
        ::SetupBranch(t, "mu_d0sig", &v_mu_d0sig, b_mu_d0sig);
    if ( fD3PDversion < 9 ) {
        ::SetupBranch(t, "mu_z0sig", &v_mu_z0sig, b_mu_z0sig);
        ::SetupBranch(t, "mu_topoetcone30", &v_mu_topoetcone30, b_mu_topoetcone30);
        ::SetupBranch(t, "mu_topoetcone40", &v_mu_topoetcone40, b_mu_topoetcone40);
        ::SetupBranch(t, "mu_ptcone30", &v_mu_ptcone30, b_mu_ptcone30);
        ::SetupBranch(t, "mu_ptcone40", &v_mu_ptcone40, b_mu_ptcone40);
    }
    if ( fD3PDversion < 21 ) {
        ::SetupBranch(t, "mu_MT", &v_mu_MT, b_mu_MT);
        ::SetupBranch(t, "mu_nu_pT", &v_mu_nu_pT, b_mu_nu_pT);
        ::SetupBranch(t, "mu_nu_eta", &v_mu_nu_eta, b_mu_nu_eta);
        ::SetupBranch(t, "mu_nu_phi", &v_mu_nu_phi, b_mu_nu_phi);
    }
    if ( fD3PDversion < 9 ) {
        ::SetupBranch(t, "mu_W_pT", &v_mu_W_pT, b_mu_W_pT);
        ::SetupBranch(t, "mu_W_eta", &v_mu_W_eta, b_mu_W_eta);
        ::SetupBranch(t, "mu_W_phi", &v_mu_W_phi, b_mu_W_phi);
    }
    if ( fD3PDversion >= 29 ) {
        // changed all std::vector<bool> to std::vector<char>
        ::SetupBranch(t, "mu_tight", &v_mu_tight_nobool, b_mu_tight);
    } else {
        ::SetupBranch(t, "mu_tight", &v_mu_tight, b_mu_tight);
    }
    ::SetupBranch(t, "mu_true_type", &v_mu_true_type, b_mu_true_type, isMC);
    ::SetupBranch(t, "mu_true_origin", &v_mu_true_origin, b_mu_true_origin, isMC);
    if ( fD3PDversion < 9 ) {
        ::SetupBranch(t, "mu_true_typebkg", &v_mu_true_typebkg, b_mu_true_typebkg, isMC);
        ::SetupBranch(t, "mu_true_originbkg", &v_mu_true_originbkg, b_mu_true_originbkg, isMC);
    }
    ::SetupBranch(t, "mu_true_pdg", &v_mu_true_pdg, b_mu_true_pdg, isMC);
    ::SetupBranch(t, "mu_true_pt", &v_mu_true_pt, b_mu_true_pt, isMC);
    ::SetupBranch(t, "mu_true_eta", &v_mu_true_eta, b_mu_true_eta, isMC);
    if ( fD3PDversion < 23 )
        ::SetupBranch(t, "jet_n", &v_jet_n, b_jet_n);
    ::SetupBranch(t, "jet_m", &v_jet_m, b_jet_m);
    ::SetupBranch(t, "met_px", &v_met_px, b_met_px);
    ::SetupBranch(t, "met_py", &v_met_py, b_met_py);
    ::SetupBranch(t, "met_sumet", &v_met_sumet, b_met_sumet);
    if ( fD3PDversion < 20 )
        ::SetupBranch(t, "MT2", &v_MT2, b_MT2);
    if ( fD3PDversion < 21 ) {
        ::SetupBranch(t, "pTsys", &v_pTsys, b_pTsys);
        ::SetupBranch(t, "sigma_pTsys", &v_sigma_pTsys, b_sigma_pTsys);
        ::SetupBranch(t, "Ht", &v_Ht, b_Ht);
    }
    if ( isMC && fD3PDversion < 20 ) {
        ::SetupBranch(t, "genfilter_BHadron", &v_genfilter_BHadron, b_genfilter_BHadron);
        ::SetupBranch(t, "genfilter_CHadronPt4Eta3", &v_genfilter_CHadronPt4Eta3, b_genfilter_CHadronPt4Eta3);
        ::SetupBranch(t, "hasMEphoton_DRgt02_nonhad", &v_hasMEphoton_DRgt02_nonhad, b_hasMEphoton_DRgt02_nonhad);
    }
    if (isMC && fD3PDversion < 31){ //kkreul data not working
    	::SetupBranch(t, "bdt_response", &v_bdt_response, b_bdt_response);
    }
}

//____________________________________________________________________

Bool_t AtlEvtReaderD3PDSgTopR2::Notify() {
    //
    // The Notify() function is called when a new file is opened.
    //

    // Set MC weight positions if requested and not already done
    if ( fMCWeightPosition < -1 and not fMCWeightsProcessed ) {
        ProcessMCWeightsStrings();
        fMCWeightsProcessed = kTRUE;
    }

    // Load truth tree if requested
    if ( IsMC() && (fParent->fDoTruthTree==kTRUE) ) LoadTruthTree();
    return kTRUE;
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTopR2::LoadTruthTree() {
    //
    // Load the MC truth tree for a newly opened input file
    //
    if ( fParent->GetCurrentTree() == 0 ) return;
    fTruthTree = (TTree*)fParent->GetCurrentTree()->GetCurrentFile()->Get("truth");

    // Set branches
    ::SetupBranch(fTruthTree, "runNumber", &v_tt_runNumber, b_tt_runNumber);
    ::SetupBranch(fTruthTree, "eventNumber", &v_tt_eventNumber, b_tt_eventNumber);
    ::SetupBranch(fTruthTree, "MC_b_from_t_pt",  &v_tt_MC_b_from_t_pt,  b_tt_MC_b_from_t_pt);
    ::SetupBranch(fTruthTree, "MC_b_from_t_eta", &v_tt_MC_b_from_t_eta, b_tt_MC_b_from_t_eta);
    ::SetupBranch(fTruthTree, "MC_b_from_t_phi", &v_tt_MC_b_from_t_phi, b_tt_MC_b_from_t_phi);
    ::SetupBranch(fTruthTree, "MC_b_from_t_m",   &v_tt_MC_b_from_t_m,   b_tt_MC_b_from_t_m);
    ::SetupBranch(fTruthTree, "MC_b_from_tbar_pt",  &v_tt_MC_b_from_tbar_pt,  b_tt_MC_b_from_tbar_pt);
    ::SetupBranch(fTruthTree, "MC_b_from_tbar_eta", &v_tt_MC_b_from_tbar_eta, b_tt_MC_b_from_tbar_eta);
    ::SetupBranch(fTruthTree, "MC_b_from_tbar_phi", &v_tt_MC_b_from_tbar_phi, b_tt_MC_b_from_tbar_phi);
    ::SetupBranch(fTruthTree, "MC_b_from_tbar_m",   &v_tt_MC_b_from_tbar_m,   b_tt_MC_b_from_tbar_m);

    // Build index according to run/evt numbers
    fTruthTree->BuildIndex("runNumber", "eventNumber");
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTopR2::BuildEvent() {
    //
    // Build A++ event from D3PD content
    //

    // Build header
    if ( gDebug > 1 )
        Info(__FUNCTION__, "Building Event Header");
    BuildEventHeader();

    // Build trigger
    if ( gDebug > 1 )
        Info(__FUNCTION__, "Building Trigger Decision");
    BuildTriggerDecision();

    // Build energy sum
    if ( gDebug > 1 )
        Info(__FUNCTION__, "Building Energy Sum");
    BuildEnergySum();

    // Build electrons
    if ( gDebug > 1 )
        Info(__FUNCTION__, "Building Electrons");
    BuildElectrons();

    // Build muons
    if ( gDebug > 1 )
        Info(__FUNCTION__, "Building Muons");
    BuildMuons();

    // Build jets
    if ( gDebug > 1 )
        Info(__FUNCTION__, "Building Jets");
    BuildJets();

    // Build from MC truth tree
    if ( IsMC() && (fParent->fDoTruthTree==kTRUE) ) {
        if ( gDebug > 1 )
            Info(__FUNCTION__, "Building from MC truth tree");
        BuildFromTruthTree();
    }

    // Build bdt_response
    fEvent->SetBDTResponse(v_bdt_response);

    if ( gDebug > 1)
        Info(__FUNCTION__, "Event built");
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTopR2::BuildEventHeader() {
    //
    // Build A++ event header
    //
    bool isMC = IsMC();
    double TruthWeightCorrection = 1.;
    if ( isMC ) {
        int IsSherpa22 = ( isnan(v_weight_sherpa_22_vjets) ? 0 : 1 );
        if ( fIsSherpa22 < 0 ) {
            fIsSherpa22 = IsSherpa22;
        }
        else if ( !(fIsSherpa22 == IsSherpa22) ) {
            Fatal(__FUNCTION__, "some events look like Sherpa 2.2, others don't");
        }
        if ( IsSherpa22 ) {
            TruthWeightCorrection *= v_weight_sherpa_22_vjets;
        }
    }

    Double_t mc_weight = 1.;
    if ( isMC ) {
        if ( fMCWeightPosition < -1 ) {
            // multiply several (2) mc weights
            // read from position vector filled in ProcessMCWeightsStrings();
            Double_t wnom = v_weights_mc->at(0);
            Bool_t first = kTRUE;
            for ( auto&& pos: fMCWeightPositionVector ) {
                Double_t w = v_weights_mc->at(pos);
                if ( first ) {
                    mc_weight *= w;
                    first = kFALSE;
                } else {
                    mc_weight *= w/wnom;
                }
            }
        } else {
        	//Info(__FUNCTION__, Form("HELLO2 %d", fMCWeightPosition));
        	//Info(__FUNCTION__, Form("HELLO2 %d", fMcWeightNumber));
            // regular mc weights
            mc_weight = fMcWeightNumber > 0 ? v_weights_mc->at(fMcWeightNumber) : v_weight_mc; //kkreul : was >= 0
        }
    }

    fEvent->SetEventHeader(
        /* RunNr = */ v_runNumber,
        /* EventNr = */ v_eventNumber,
        /* LumiBlock = */ v_lbn,
        /* BeamEnergy = */ 6500.,
        /* IsMC = */ isMC,
        /* EventType = */ AtlEventHeader::kPhysics,
        /* MCWeight = */ mc_weight,
        /* PileupWeight = */ (isMC ? v_weight_pileup : 1.),
        /* ZVertexWeight = */ 1.,
        /* MMWeight_nominal = */ NAN,
        /* MMWeight_fake_mc_up = */ NAN,
        /* MMWeight_fake_mc_down = */ NAN,
        /* MMWeight_fake_alternate = */ NAN,
        /* MMWeight_real_alternate = */ NAN,
        /* TruthWeightCorrection = */ TruthWeightCorrection);
    AtlEventHeader *eventHeader = fEvent->GetEventHeader();
    eventHeader->SetPDFInfo(NAN, NAN, -999, -999, NAN, NAN, NAN);
    {
        BitsBuilder<AtlEventHeader::EPreselectionFlags> bits;
        bits.addIfAny(AtlEventHeader::kElJets, v_ejets, fPeriodCount);
        bits.addIfAny(AtlEventHeader::kMuJets, v_mujets, fPeriodCount);
        bits.addIfAny(AtlEventHeader::kElEl, v_ee, fPeriodCount);
        bits.addIfAny(AtlEventHeader::kMuMu, v_mumu, fPeriodCount);
        bits.addIfAny(AtlEventHeader::kElElEl, v_eee, fPeriodCount);
        bits.addIfAny(AtlEventHeader::kElElMu, v_eemu, fPeriodCount);
        bits.addIfAny(AtlEventHeader::kElMuMu, v_emumu, fPeriodCount);
        bits.addIfAny(AtlEventHeader::kMuMuMu, v_mumumu, fPeriodCount);
        bits.addIfAny(AtlEventHeader::kLeptonic, v_leptonic, fPeriodCount);
        eventHeader->SetPreselectionFlags(bits.get());
    }
    eventHeader->ejets2018 = v_ejets[3];
    eventHeader->mujets2018 = v_mujets[3];
    eventHeader->ejets2017 = v_ejets[2];
    eventHeader->mujets2017 = v_mujets[2];
    eventHeader->ejets2016 = v_ejets[1];
    eventHeader->mujets2016 = v_mujets[1];
    eventHeader->ejets2015 = v_ejets[0];
    eventHeader->mujets2015 = v_mujets[0];
    eventHeader->SetAverageIntPerXing(v_mu);
    eventHeader->SetChannelNumber(v_mcChannelNumber);

    if ( isMC ) {
    	eventHeader->ForwardJVT().SetValue(v_weight_forwardjvt);
    	eventHeader->LeptonTriggerSF().SetValue(v_weight_globalLeptonTriggerSF);  //this LeptonTrigger is already included in LeptonSF
        eventHeader->JetVertexWeight().SetValue(v_weight_jvt);
        eventHeader->LeptonWeight().SetValue(v_weight_leptonSF);
        if(fD3PDversion >= 31){
        	// Set BTagWeight
        	Double_t btag_weight = NAN;
        	if ( fBtagSFVariationComponent < 0 ) {
        	        btag_weight = v_weight_bTagSF_Continuous;
        	} else {
        	        btag_weight = v_weight_bTagSF_Continuous_vec->at(fBtagSFVariationComponent);
        	}
        	eventHeader->BtagWeight_DL1r_85().SetValue(btag_weight);
        	eventHeader->BtagWeight_DL1r_77().SetValue(btag_weight);
        	eventHeader->BtagWeight_DL1r_70().SetValue(btag_weight);
        	eventHeader->BtagWeight_DL1r_60().SetValue(btag_weight);

        }
        // hardcode switch for testing continuous btagging in v25 is fForceContinBTag
        else if ( fD3PDversion >= 29 || (fD3PDversion >= 25 && fForceContinBTag) ) {
            // continuous b-tagging is available in v25+v27,
            // but only used afterwards by default
            Double_t btag_weight = NAN;
            if ( fBtagSFVariationComponent < 0 ) {
                btag_weight = v_weight_bTagSF_Continuous;
            } else {
                btag_weight = v_weight_bTagSF_Continuous_vec->at(fBtagSFVariationComponent);
            }
            eventHeader->BtagWeight_MV2c10_60().SetValue(btag_weight);
            eventHeader->BtagWeight_MV2c10_70().SetValue(btag_weight);
            eventHeader->BtagWeight_MV2c10_77().SetValue(btag_weight);
            eventHeader->BtagWeight_MV2c10_85().SetValue(btag_weight);
        } else if ( fD3PDversion >= 12 ) {
            if ( fD3PDversion >= 20 ) {
                eventHeader->BtagWeight_MV2c10_60().SetValue(( fBtagSFVariationComponent < 0 ? v_weight_bTagSF_60 : v_weight_bTagSF_60_vec->at(fBtagSFVariationComponent) ));
            }
            eventHeader->BtagWeight_MV2c10_70().SetValue(( fBtagSFVariationComponent < 0 ? v_weight_bTagSF_70 : v_weight_bTagSF_70_vec->at(fBtagSFVariationComponent) ));
            eventHeader->BtagWeight_MV2c10_77().SetValue(( fBtagSFVariationComponent < 0 ? v_weight_bTagSF_77 : v_weight_bTagSF_77_vec->at(fBtagSFVariationComponent) ));
            eventHeader->BtagWeight_MV2c10_85().SetValue(( fBtagSFVariationComponent < 0 ? v_weight_bTagSF_85 : v_weight_bTagSF_85_vec->at(fBtagSFVariationComponent) ));
        }
        else {
            eventHeader->BtagWeight_MV2c20_77().SetValue(( fBtagSFVariationComponent < 0 ? v_weight_bTagSF_77 : v_weight_bTagSF_77_vec->at(fBtagSFVariationComponent) ));
            eventHeader->BtagWeight_MV2c20_85().SetValue(( fBtagSFVariationComponent < 0 ? v_weight_bTagSF_85 : v_weight_bTagSF_85_vec->at(fBtagSFVariationComponent) ));
        }
        eventHeader->SetTruthFlavor(v_genfilter_BHadron ? 5 : v_genfilter_CHadronPt4Eta3 ? 4 : 0);
        if ( b_hasMEphoton_DRgt02_nonhad ) {
            eventHeader->SetMEPhoton(v_hasMEphoton_DRgt02_nonhad);
        }
    }
}

//___________________________________________________________

void AtlEvtReaderD3PDSgTopR2::BuildEnergySum() {
    //
    // Build A++ energy sum
    //
    HepEnergySum *energySum = fEvent->GetEnergySum();
    energySum->SetMissingEt(v_met_px*MeV, v_met_py*MeV);
    energySum->SetSumEt(v_met_sumet*MeV);
}

//___________________________________________________________

void AtlEvtReaderD3PDSgTopR2::BuildElectrons() {
    //
    // Build list of A++ electrons
    //
    // The ID quality can be either tight or loose
    //
    // Loop over all electrons
    // el_n is not available in v21; use size of v_el_pt
    Int_t el_n = v_el_n;
    if ( fD3PDversion > 20 )
        el_n = v_el_pt->size();
    for ( Int_t el_i = 0; el_i < el_n; ++el_i ) {
	AtlIDTrack *track = 0;
	if ( fD3PDversion < 9 ) {
	    track = fEvent->AddIDTrack(
		/* Chi2 = */ NAN,
		/* NDoF = */ -999,
		/* Xref = */ NAN,
		/* Yref = */ NAN,
		/* Zref = */ NAN,
		/* Phi0 = */ NAN,
		/* QovP = */ NAN,
		/* D0 = */ v_el_d0->at(el_i)*mm,
		/* Z0 = */ v_el_z0->at(el_i)*mm,
		/* Theta = */ NAN,
		/* CovMat = */ 0);
	    track->SetZ0sig(v_el_z0sig->at(el_i));
	    track->SetD0sig(v_el_d0sig->at(el_i));
	}
        Bool_t istight = kFALSE;
        if ( fD3PDversion >= 29 ) {
            istight = v_el_tight_nobool->at(el_i);
        } else {
            istight = v_el_tight->at(el_i);
        }
        AtlElectron *electron = fEvent->AddElectron(
            /* Px = */ NAN,
            /* Py = */ NAN,
            /* Pz = */ 1.e-99,
            /* E = */ 1.e-99,
            /* IsPositron = */ IsPositive(v_el_charge->at(el_i)),
            /* EMWeight = */ NAN,
            /* BkgWeight = */ NAN,
            /* OQFlag = */ 0,
            /* IsEMBitField = */ 0,
            /* IsEM = */  ( istight )
            ? AtlEMShower::kElectronTight : AtlEMShower::kElectronLoose,
            /* Author = */ AtlElectron::kHighPtElectron,
            /* PCluster = */ TLorentzVector(NAN, NAN, NAN, NAN));
	if ( fD3PDversion < 9 ) {
	    electron->SetPtConeVars(
		/* PtCone20 = */ NAN,
		/* PtCone30 = */ NAN,
		/* PtCone40 = */ v_el_ptcone40->at(el_i)*MeV);
	}
        electron->SetP(PtEtaPhiE(v_el_pt->at(el_i)*MeV, v_el_eta->at(el_i),
				 v_el_phi->at(el_i), v_el_e->at(el_i)*MeV));
	if ( fD3PDversion < 9 ) {
	    electron->SetIDTrack(track);
	}
        electron->SetEtConeVars(NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN,
				NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN,
				NAN, NAN, NAN, NAN, NAN, NAN);
        electron->SetElectronScaleFactors(NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN);
        if ( fD3PDversion < 29 ) {
            electron->SetIsTriggerMatched(v_el_trigMatch->at(el_i));
        } else {
            // using AnalysisTop global trigger matching
            // see SGTOPNTUP-43, ANALYSISTO-646
            electron->SetIsTriggerMatched(kTRUE);
        }
	if ( IsMC() ) {
	    electron->SetMCTruthClassifier((AtlMCTruthClassifier::EParticleOrigin)v_el_true_origin->at(el_i));
	}
    }
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTopR2::BuildMuons() {
    //
    // Build list of A++ muons
    //
    // The ID quality can be either tight or loose
    //

    // Check input validity to get a stack trace for wrong input.
    //   From GCC8 on, vector out of range should report a
    //   stack trace and this check won't be necessary.
    // mu_n is not available in v21; use size of v_mu_pt
    if ( nullptr == v_mu_pt ) Fatal(__FUNCTION__, "v_mu_pt is NULL");
    UInt_t mu_n = v_mu_n;
    if ( fD3PDversion > 20 )
        mu_n = v_mu_pt->size();
    if ( 0 == mu_n )
        return;
    if ( fD3PDversion < 29 && nullptr == v_mu_tight ) Fatal(__FUNCTION__, "v_mu_tight is NULL");
    if ( fD3PDversion >= 29 && nullptr == v_mu_tight_nobool ) Fatal(__FUNCTION__, "v_mu_tight_nobool is NULL");
    if ( nullptr == v_mu_charge ) Fatal(__FUNCTION__, "v_mu_charge is NULL");
    if ( nullptr == v_mu_eta ) Fatal(__FUNCTION__, "v_mu_eta is NULL");
    if ( nullptr == v_mu_phi ) Fatal(__FUNCTION__, "v_mu_phi is NULL");
    if ( nullptr == v_mu_e ) Fatal(__FUNCTION__, "v_mu_e is NULL");
    if ( fD3PDversion < 29 && nullptr == v_mu_trigMatch ) Fatal(__FUNCTION__, "v_mu_trigMatch is NULL");
    if ( IsMC() && nullptr == v_mu_true_origin ) Fatal(__FUNCTION__, "v_mu_true_origin is NULL");

    if ( fD3PDversion < 29 && v_mu_tight->size() != mu_n ) Fatal(__FUNCTION__, "wrong size: v_mu_tight");
    if ( fD3PDversion >= 29 && v_mu_tight_nobool->size() != mu_n ) Fatal(__FUNCTION__, "wrong size: v_mu_tight_nobool");
    if ( v_mu_charge->size() != mu_n ) Fatal(__FUNCTION__, "wrong size: v_mu_charge");
    if ( v_mu_pt->size() != mu_n ) Fatal(__FUNCTION__, "wrong size: v_mu_pt");
    if ( v_mu_eta->size() != mu_n ) Fatal(__FUNCTION__, "wrong size: v_mu_eta");
    if ( v_mu_phi->size() != mu_n ) Fatal(__FUNCTION__, "wrong size: v_mu_phi");
    if ( v_mu_e->size() != mu_n ) Fatal(__FUNCTION__, "wrong size: v_mu_e");
    if ( fD3PDversion < 29 && v_mu_trigMatch->size() != mu_n ) Fatal(__FUNCTION__, "wrong size: v_mu_trigMatch");
    if ( IsMC() && v_mu_true_origin->size() != mu_n ) Fatal(__FUNCTION__, "wrong size: v_mu_true_origin");

    // Loop over all muons
    for ( UInt_t mu_i = 0; mu_i < mu_n; ++mu_i ) {
	AtlIDTrack *track = 0;
	if ( fD3PDversion < 9 ) {
	    track = fEvent->AddIDTrack(
		/* Chi2 = */ NAN,
		/* NDoF = */ -999,
		/* Xref = */ NAN,
		/* Yref = */ NAN,
		/* Zref = */ NAN,
		/* Phi0 = */ NAN,
		/* QovP = */ NAN,
		/* D0 = */ v_mu_d0->at(mu_i)*mm,
		/* Z0 = */ v_mu_z0->at(mu_i)*mm,
		/* Theta = */ NAN,
		/* CovMat = */ 0);
	    track->SetZ0sig(v_mu_z0sig->at(mu_i));
	    track->SetD0sig(v_mu_d0sig->at(mu_i));
	}
        if ( gDebug > 1 )
            Info(__FUNCTION__, "Adding Muon");
        Bool_t istight = kFALSE;
        if ( fD3PDversion >= 29 ) {
            istight = v_mu_tight_nobool->at(mu_i);
        } else {
            istight = v_mu_tight->at(mu_i);
        }
        AtlMuon *muon = fEvent->AddMuon(
            /* Px = */ NAN,
            /* Py = */ NAN,
            /* Pz = */ 1.e-99,
            /* E = */ 1.e-99,
            /* IsMuPlus = */ IsPositive(v_mu_charge->at(mu_i)),
            /* EtCone10 = */ NAN,
            /* EtCone20 = */ NAN,
            /* EtCone30 = */ NAN,
            /* EtCone40 = */ NAN,
            /* NtrkCone10 = */ -999,
            /* NtrkCone20 = */ -999,
            /* NtrkCone30 = */ -999,
            /* NtrkCone40 = */ -999,
            /* PtCone10 = */ NAN,
            /* PtCone20 = */ NAN,
            /* PtCone30 = */ NAN,
            /* PtCone40 = */ NAN,
            /* Author = */ AtlMuon::kMuidCo,
            /* Quality = */ ( istight ) ? AtlMuon::kTight : AtlMuon::kLoose,
            /* MatchingChi2 = */ NAN,
            /* MatchingNDoF = */ -999,
            /* IsCombinedMuon = */ kTRUE,
            /* PMuonSpecExtrapol = */ TLorentzVector(NAN, NAN, NAN, NAN),
            /* MuonSpecExtrapolCharge = */ 0);
        muon->SetP(PtEtaPhiE(v_mu_pt->at(mu_i)*MeV, v_mu_eta->at(mu_i),
			     v_mu_phi->at(mu_i), v_mu_e->at(mu_i)*MeV));
	if ( fD3PDversion < 9 ) {
	    muon->SetIDTrack(track);
	}
        muon->SetMuonScaleFactors(NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN);
        if ( gDebug > 1 )
            Info(__FUNCTION__, "Setting IsTriggerMatched");
        if ( fD3PDversion < 29 ) {
            muon->SetIsTriggerMatched(v_mu_trigMatch->at(mu_i));
        } else {
            // using AnalysisTop global trigger matching
            // see SGTOPNTUP-43, ANALYSISTO-646
            muon->SetIsTriggerMatched(kTRUE);
        }
	if ( IsMC() ) {
            if ( gDebug > 1 )
                Info(__FUNCTION__, "Setting MCTruthClassifier");
	    muon->SetMCTruthClassifier((AtlMCTruthClassifier::EParticleOrigin)v_mu_true_origin->at(mu_i));
	}
    }
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTopR2::BuildJets() {
    //
    // Build A++ jets
    //
    // jet_n is not available in v21; use size of v_jet_pt
    UInt_t jet_n = v_jet_n;
    if ( fD3PDversion >= 23 )
        jet_n = v_jet_pt->size();
    for ( UInt_t jet_i = 0; jet_i < jet_n; ++jet_i ) {
        if ( gDebug > 1 )
            Info(__FUNCTION__, "Adding Jet");
        AtlJet *jet = fEvent->AddJet(
            /* type = */ AtlJet::kDL1r,
            /* E = */ 1.e-99,
            /* Px = */ NAN,
            /* Py = */ NAN,
            /* Pz = */ 1.e-99,
            /* JetQuality = */ AtlJet::kIsGood,
            /* P_EMSCALE = */ TLorentzVector(NAN, NAN, NAN, NAN),
            /* P_JESCorrSCALE = */ TLorentzVector(NAN, NAN, NAN, NAN),
            /* EMJES_EtaCorr = */ NAN,
            /* BCH_CORR_CELL = */ NAN,
            /* BCH_CORR_JET = */ NAN,
            /* eta_offsetJES = */ NAN);
        jet->SetP(PtEtaPhiE(v_jet_pt->at(jet_i)*MeV, v_jet_eta->at(jet_i),
			    v_jet_phi->at(jet_i), v_jet_e->at(jet_i)*MeV));

	// Set Truth flavour (MC only)
	if ( IsMC() ) {
            if ( gDebug > 1 )
                Info(__FUNCTION__, "Setting Truth flavour");
	    AtlJet::EFlavour jet_flavour;
	    switch (abs(v_jet_truthflav->at(jet_i))) {
            case 0:
                jet_flavour = AtlJet::kLightFlavoured;
                break;
            case 4:
                jet_flavour = AtlJet::kCharm;
                break;
            case 5:
                jet_flavour = AtlJet::kBeauty;
                break;
            case 16:
                jet_flavour = AtlJet::kTau;
                break;
            default:
                jet_flavour = AtlJet::kUnknownFlavour;
                break;
	    }
	    jet->SetTruthFlavour(jet_flavour);
	}

	// Set b-tagging
        if ( gDebug > 1 )
            Info(__FUNCTION__, "Setting b-tagging for D3PD version %d", fD3PDversion);
        if (fD3PDversion >= 31){
        	if ( nullptr == v_jet_tagWeightBin_Continuous )
        		Fatal(__FUNCTION__, "v_jet_tagWeightBin_Continuous is nullptr");
        	if ( v_jet_tagWeightBin_Continuous->size() <= jet_i )
        		Fatal(__FUNCTION__, "v_jet_tagWeightBin_Continuous is too small (%lu) for (%u) jets",
        	    v_jet_tagWeightBin_Continuous->size(), jet_i);
            //std::cout<<"HELLO"<<v_jet_tagWeightBin_Continuous->at(jet_i)<<std::endl;
        	jet->AddTag(AtlBTag::kDL1r_85, (v_jet_tagWeightBin_Continuous->at(jet_i) > 1 ? 1. : 0.), kTRUE);
        	jet->AddTag(AtlBTag::kDL1r_77, (v_jet_tagWeightBin_Continuous->at(jet_i) > 2 ? 1. : 0.), kTRUE);
        	jet->AddTag(AtlBTag::kDL1r_70, (v_jet_tagWeightBin_Continuous->at(jet_i) > 3 ? 1. : 0.), kTRUE);
        	jet->AddTag(AtlBTag::kDL1r_60, (v_jet_tagWeightBin_Continuous->at(jet_i) > 4 ? 1. : 0.), kTRUE);
        }
        else if ( fD3PDversion >= 29 || (fD3PDversion >= 25 && fForceContinBTag) ) {
            // use continuous b-tagging
            if ( nullptr == v_jet_tagWeightBin_Continuous )
                Fatal(__FUNCTION__, "v_jet_tagWeightBin_Continuous is nullptr");
            if ( v_jet_tagWeightBin_Continuous->size() <= jet_i )
                Fatal(__FUNCTION__, "v_jet_tagWeightBin_Continuous is too small (%lu) for (%u) jets",
                      v_jet_tagWeightBin_Continuous->size(), jet_i);
            jet->AddTag(AtlBTag::kMV2c10_85, (v_jet_tagWeightBin_Continuous->at(jet_i) > 1 ? 1. : 0.), kTRUE);
            jet->AddTag(AtlBTag::kMV2c10_77, (v_jet_tagWeightBin_Continuous->at(jet_i) > 2 ? 1. : 0.), kTRUE);
            jet->AddTag(AtlBTag::kMV2c10_70, (v_jet_tagWeightBin_Continuous->at(jet_i) > 3 ? 1. : 0.), kTRUE);
            jet->AddTag(AtlBTag::kMV2c10_60, (v_jet_tagWeightBin_Continuous->at(jet_i) > 4 ? 1. : 0.), kTRUE);
        } else if ( fD3PDversion >= 12 ) {
            if ( fD3PDversion >= 20 ) {
                if ( nullptr == v_jet_isbtagged_60 )
                    Fatal(__FUNCTION__, "v_jet_isbtagged_60 is nullptr");
                if ( v_jet_isbtagged_60->at(jet_i) >= 0 )
                    jet->AddTag(AtlBTag::kMV2c10_60, (v_jet_isbtagged_60->at(jet_i) > 0 ? 1. : 0.), kTRUE);
            }
            if ( nullptr == v_jet_isbtagged_70 )
                Fatal(__FUNCTION__, "v_jet_isbtagged_70 is nullptr");
            if ( nullptr == v_jet_isbtagged_77 )
                Fatal(__FUNCTION__, "v_jet_isbtagged_77 is nullptr");
            if ( nullptr == v_jet_isbtagged_85 )
                Fatal(__FUNCTION__, "v_jet_isbtagged_85 is nullptr");
	    if ( v_jet_isbtagged_70->at(jet_i) >= 0 )
		jet->AddTag(AtlBTag::kMV2c10_70, (v_jet_isbtagged_70->at(jet_i) > 0 ? 1. : 0.), kTRUE);
	    if ( v_jet_isbtagged_77->at(jet_i) >= 0 )
		jet->AddTag(AtlBTag::kMV2c10_77, (v_jet_isbtagged_77->at(jet_i) > 0 ? 1. : 0.), kTRUE);
	    if ( v_jet_isbtagged_85->at(jet_i) >= 0 )
		jet->AddTag(AtlBTag::kMV2c10_85, (v_jet_isbtagged_85->at(jet_i) > 0 ? 1. : 0.), kTRUE);
	} else {
	    /* the only tagger with a calibrated working point (77%) in single-top ntuples v02: */
	    jet->AddTag(AtlBTag::kMV2c20, v_jet_mv2c20->at(jet_i), kTRUE);
        }

        // temporary check with v25 that Continuous btagging is same as fixed WP
        if ( fD3PDversion == 25 && gDebug > 1) {
            Info(__FUNCTION__, "comparing contin. btag with fixed WP");
            Bool_t cont = kFALSE;
            Bool_t fixed = kFALSE;
            if ( nullptr == v_jet_tagWeightBin_Continuous )
                Fatal(__FUNCTION__, "v_jet_tagWeightBin_Continuous is nullptr");
            // 85% WP
            cont = ( v_jet_tagWeightBin_Continuous->at(jet_i) > 1 );
            fixed = ( v_jet_isbtagged_85->at(jet_i) > 0 );
            if ( cont != fixed )
                Warning(__FUNCTION__,
                        "BTags for continuous (%s) and fixed (%s) b-tagging at %d%% WP do not agree",
                        cont ? "true" : "false", fixed ? "true" : "false", 85);
            // 77% WP
            cont = ( v_jet_tagWeightBin_Continuous->at(jet_i) > 2 );
            fixed = ( v_jet_isbtagged_77->at(jet_i) > 0 );
            if ( cont != fixed )
                Warning(__FUNCTION__,
                        "BTags for continuous (%s) and fixed (%s) b-tagging at %d%% WP do not agree",
                        cont ? "true" : "false", fixed ? "true" : "false", 77);
            // 70% WP
            cont = ( v_jet_tagWeightBin_Continuous->at(jet_i) > 3 );
            fixed = ( v_jet_isbtagged_70->at(jet_i) > 0 );
            if ( cont != fixed )
                Warning(__FUNCTION__,
                        "BTags for continuous (%s) and fixed (%s) b-tagging at %d%% WP do not agree",
                        cont ? "true" : "false", fixed ? "true" : "false", 70);
            // 60% WP
            cont = ( v_jet_tagWeightBin_Continuous->at(jet_i) > 4 );
            fixed = ( v_jet_isbtagged_60->at(jet_i) > 0 );
            if ( cont != fixed )
                Warning(__FUNCTION__,
                        "BTags for continuous (%s) and fixed (%s) b-tagging at %d%% WP do not agree",
                        cont ? "true" : "false", fixed ? "true" : "false", 60);
        }
        
        jet->SetJetScaleFactors_MV1_60(NAN, NAN, NAN, NAN, NAN, NAN, NAN);
        jet->SetJetScaleFactors_MV1_70(NAN, NAN, NAN, NAN, NAN, NAN, NAN);
        jet->SetJetScaleFactors_MV1_80(NAN, NAN, NAN, NAN, NAN, NAN, NAN);
        jet->SetJetScaleFactors_MV1c_50(NAN, NAN, NAN, NAN, NAN, NAN, NAN);
    }
}

/* !!! end of automatically generated content !!! */

//____________________________________________________________________

void AtlEvtReaderD3PDSgTopR2::BuildFromTruthTree() {
    //
    // Build MC truth information from truth tree
    //

    // Fetch truth tree information for this run/evt
    Int_t btt = fTruthTree->GetEntryWithIndex(v_runNumber, v_eventNumber);
    if ( btt <= 0 ) {
	Error(__FUNCTION__, "Can not find entry for given run and event number in MC truth tree. Abort!");
	gSystem->Abort(1);
    }

    // ttbar: add 2 b-quarks from top decays
    HepMCParticle *prt_b = fEvent->AddMCParticle(5, 0., 0., 0., 1.,
						 0, kFALSE, kFALSE, kFALSE,
						 kFALSE, kFALSE, kFALSE, kFALSE);
    prt_b->SetPtEtaPhiM(v_tt_MC_b_from_t_pt/1000.,
			v_tt_MC_b_from_t_eta,
			v_tt_MC_b_from_t_phi,
			v_tt_MC_b_from_t_m/1000.);
    HepMCParticle *prt_bbar = fEvent->AddMCParticle(-5, 0., 0., 0., 1.,
						    0, kFALSE, kFALSE, kFALSE,
						    kFALSE, kFALSE, kFALSE, kFALSE);
    prt_bbar->SetPtEtaPhiM(v_tt_MC_b_from_tbar_pt/1000.,
			   v_tt_MC_b_from_tbar_eta,
			   v_tt_MC_b_from_tbar_phi,
			   v_tt_MC_b_from_tbar_m/1000.);
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTopR2::BuildTriggerDecision() {
    //
    // Build trigger information for the given event
    //
    // (currently HLT only)
    //

    if ( gDebug > 1 )
        Info(__FUNCTION__, "Building Trigger Config");

    // First event? Build trigger config dbase
    if ( fParent->IsFirstEvent() ) {
	BuildTriggerConfig(fEvent->RunNr(), fEvent->RunNr());
    }

    if ( gDebug > 1 )
        Info(__FUNCTION__, "Checking trigger run no validity range");
    // Run no. changed? Extend validity range of trigger config
    if (  static_cast<Int_t>(fEvent->RunNr()) < fTriggerConfDbase->GetRunStart() ) {
	fTriggerConfDbase->SetRunStart(fEvent->RunNr());
    } else if (  static_cast<Int_t>(fEvent->RunNr()) > fTriggerConfDbase->GetRunEnd() ) {
	fTriggerConfDbase->SetRunEnd(fEvent->RunNr());
    }

    if ( gDebug > 1 )
        Info(__FUNCTION__, "Setting stream");
    // Set stream
    fEvent->GetTrigger()->SetStream(AtlTrigger::kPhysics);

    if ( gDebug > 1 )
        Info(__FUNCTION__, "Writing HLT trigger decision");
    // Write trigger decision, HLT
    TClonesArray *trig_items = fTriggerConfDbase->GetTriggerItemsCA(AtlTriggerConf::kHLT);
    Int_t Entries =  trig_items->GetEntries();
    Entries = 0;
    for ( Int_t idx = 0; idx < Entries; idx++ ) {
    //for ( Int_t idx = 0; idx < trig_items->GetEntries(); idx++ ) {
	AtlTriggerConfItem *trig_item = (AtlTriggerConfItem*)trig_items->At(idx);
	stringstream br_addr_str; br_addr_str << trig_item->GetTitle();
//	strstream br_addr_str; br_addr_str << trig_item->GetTitle();
	void* br_addr;	br_addr_str >> br_addr;
	Int_t br_val = Int_t(*((Char_t*)br_addr));
	fEvent->SetHLTTriggerItem(idx,    // bit no.
				  br_val, // passed ?
				  1.,     // prescale
				  0.);    // pass-through ?
    }
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTopR2::BuildTriggerConfig(UInt_t RunStart,
						 UInt_t RunEnd) {
    //
    // Build trigger configuration for the given run range
    //
    // The configuration os based on the list of branches. Ensure that
    // all required trigger branches are enabled.
    //
    // For the moment only HLT items are supported.
    //

    // Clear old config
    fTriggerConfDbase->Clear();

    // Set validity run range
    fTriggerConfDbase->SetRunStart(RunStart);
    fTriggerConfDbase->SetRunEnd(RunEnd);

    // Loop over all branches and search for trigger branches. Add
    // them to the trigger config
    TIter next_branch(fParent->GetCurrentTree()->GetListOfBranches());
    TBranch *br = 0;
    Int_t idx = 0;
    while ( (br = (TBranch*)next_branch()) ) {
	TString br_name(br->GetName());

	// HLT
	if ( br_name.BeginsWith("HLT_") ) {
	    // Tricky: abuse conf item title for storing the memory
	    // address of the dm connected to the corresponding
	    // trigger branch for fast access when filling the trigger decision later
	    Long64_t br_addr = Long64_t(br->GetAddress());
	    TString br_addr_str = TString::LLtoa(br_addr, 16);
	    fTriggerConfDbase->AddTriggerItem(AtlTriggerConf::kHLT, br_name.Data(),
                                              br_addr_str.Data(), idx++);
	}
    }

    // Re-computer hash tables
    fTriggerConfDbase->Rehash();

    // Prtin config
    //fTriggerConfDbase->Print("CUR");
}

//____________________________________________________________________

AtlEvtReaderD3PDSgTopR2::InitialSumOfWeights_t AtlEvtReaderD3PDSgTopR2::GetInitialSumOfWeights(TFile * inputFile) const {
    //
    // Get initial sum of weights
    //
    // - primarily use info from xAOD::CutBookkeeper
    // - crosscheck with SingleTopAnalysis or cutflow histograms
    //
    AtlEvtReaderD3PDSgTopR2::InitialSumOfWeights_t result;
    if ( !IsMC() )
        return result; /* = undefined */
    // since we use xAOD::CutBookkeeper by default, we can ignore mcweightnumber
    Bool_t use_xaod_cutbookkeeper = (!(fMcWeightNumber >= 0) || fD3PDversion >= 25);

    {
        double sumOfWeights[3] = { NAN, NAN, NAN };

        /* try histograms in SingleTopAnalysis */
        TDirectory * sgtopDir = 0;
        inputFile->GetObject("SingleTopAnalysis", sgtopDir);
        if (sgtopDir) {
            if ( gDebug >0 )
                Info(__FUNCTION__, "Reading histograms from SingleTopAnalysis");
            char const * const histNames[3] = { "one_sum", "mc_weight_sums", "mc_x_pileup_weight_sums" };
            for ( int i = 0; i < 3; ++i ) {
                TH1 * h = 0;
                sgtopDir->GetObject(histNames[i], h);
                assert(h); // FIXME (also check Nbins)
                sumOfWeights[i] = h->GetBinContent(1 + (fMcWeightNumber >= 0 ? fMcWeightNumber : 0));
            }
        }
        else {

            /* analyse cutflow for each preselection -- the preselection should perfectly agree on each of the three initial number of events */
            std::vector<TString> hist_names;
            hist_names.push_back(TString("cutflow"));
            hist_names.push_back(TString("cutflow_mc"));
            hist_names.push_back(TString("cutflow_mc_pu_zvtx"));
            TString tree_name = TString(fTreeName);
            if ( gDebug > 0 )
                Info(__FUNCTION__,"trying to access tree named %s", tree_name.Data());
            if ( tree_name.EndsWith("_Loose") || tree_name.EqualTo("physics") )
                for (uint i=0; i < hist_names.size();i++)
                    hist_names[i].Append("_Loose");
            bool found[3] = { false, false, false };
            for ( TIter nextKey(inputFile->GetListOfKeys()); TKey * key = static_cast<TKey *>(nextKey()); ) {
                if ( !key->IsFolder() )
                    continue;
                TObject * obj = key->ReadObj();
                if ( TDirectory * dir = dynamic_cast<TDirectory *>(obj) ) {
                    for ( int i = 0; i < 3; ++i ) {
                        TH1 * h = 0;
                        dir->GetObject(hist_names[i].Data(), h);
                        if ( !h )
                            continue; /* be tolerant -- maybe the directory isn't related to a preselection */
                        Int_t initialBinIndex = h->GetXaxis()->FindBin("INITIAL");
                        if ( !(initialBinIndex > 0) )
                            continue;
                        if ( !found[i] ) {
                            found[i] = true;
                            sumOfWeights[i] = h->GetBinContent(initialBinIndex);
                        } else if ( !(sumOfWeights[i] == h->GetBinContent(initialBinIndex)) ) {
                            // make this only an error in case we use the xAOD::CutBookkeeper
                            TString debug_text = Form(
                                "Selections do not agree on the initial number of events in "
                                "histogram %s. Previous: %g, %s: %g", hist_names[i].Data(),
                                sumOfWeights[i], dir->GetName(), h->GetBinContent(initialBinIndex));
                            if ( use_xaod_cutbookkeeper ) {
                                Error(__FUNCTION__, debug_text.Data());
                                sumOfWeights[i] = h->GetBinContent(initialBinIndex);
                            } else {
                                Fatal(__FUNCTION__, debug_text.Data());
                            }
                        }
                    }
                }
                delete obj;
            }
            for ( int i = 0; i < 3; ++i ) {
                if ( found[i] )
                    continue;
                Fatal(__FUNCTION__, "no bookkeeping histogram bin found at: %s:/*/%s/%s", inputFile->GetName(), hist_names[i].Data(), "INITIAL");
            }

        }

        result.one = sumOfWeights[0];
        result.mcevt = sumOfWeights[1];
        result.mcevt_pileup_zvtx = sumOfWeights[2];
    }


    if ( use_xaod_cutbookkeeper ) {
        /* cross check with the number from xAOD::CutBookkeeper */
        // mc weight number == 0: read from sumWeights totalEventsWeighted
        // mc weight number > 0: totalEventsWeighted_mc_generator_weights[i]
        // if there is a fMCWeightTotalEvents given, use that instead
        // - if fMCWeightPosition is <-1, need to do more effort to multiply
        // NOTE: multiplying events AND using the sumWeights tree is
        //       currently not implemented

        if ( fMCWeightTotalEvents > 0 ) {
            // manual override, don't use sumWeights tree
            if ( fMCWeightPosition < -1 ) {
                // handle multiplication of event weights
                Double_t tot_nominal = fMCWeightTotalEvents;
                Double_t res = 1.;
                Bool_t first = kTRUE;
                for ( const auto tot: fMCWeightTotalEventsVector ) {
                    if ( first ) {
                        res *= tot;
                        first = kFALSE;
                    } else {
                        res *= tot/tot_nominal;
                    }
                }
                result.mcevt_pileup_zvtx = res;
                result.mcevt = res;
                result.one = res;
            } else {
                // no multiplication, just use fMCWeightTotalEvents
                result.mcevt_pileup_zvtx = fMCWeightTotalEvents;
                result.mcevt = fMCWeightTotalEvents;
                result.one = fMCWeightTotalEvents;
            }
            return result;
        }
        TTree * sumWeightsTree = 0;
        inputFile->GetObject("sumWeights", sumWeightsTree);
        double sumOfWeightsFromCbk = NAN;
        TString expression("totalEventsWeighted");
        if ( fMcWeightNumber > 0 ) {
            expression.Append(Form("_mc_generator_weights[%d]", fMcWeightNumber));
        }
        if ( !SumOverTree(sumWeightsTree, expression.Data(), sumOfWeightsFromCbk) ) {
            Warning(__FUNCTION__, "could not get number of initial events from xAOD::CutBookkeeper");
        } else if ( fabs(sumOfWeightsFromCbk - result.mcevt) > 0. ) {
            /* TODO figure out whether xAOD::CutBookkeeper uses pileup and z-vertex weights, too */
        	//kkreul Note: This Warning is expected for samples except ttbar, single top bc of skimming in some early
        	//processing of the samples (~DAOD)
            Warning(__FUNCTION__, "xAOD::CutBookkeeper reported a different number of initial events (%g) than the cutflow histogram (%g, difference %g)", sumOfWeightsFromCbk, result.mcevt, result.mcevt-sumOfWeightsFromCbk);
        }
        // since this number is more reliable, use it for further processing--> kkreul thats correct
        result.mcevt_pileup_zvtx = sumOfWeightsFromCbk;
        result.mcevt = sumOfWeightsFromCbk; // there is no other MC-only value
        double eventsFromCbk = NAN;
        if ( SumOverTree(sumWeightsTree, "totalEvents", eventsFromCbk) ) {
            result.one = eventsFromCbk;
        } else {
            Warning(__FUNCTION__, "could not get number of unweighted initial events from xAOD::CutBookkeeper");
            Fatal(__FUNCTION__, "No SumWeights Tree");
        }
        delete sumWeightsTree;
    }
    return result; // keep in mind: result is still from cutflow in case we use mc weight number
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTopR2::Print(Option_t *option) const {
    //
    // Print event reader information
    //
    cout << endl
	 << "Event reader configuration:\n"
	 << "  Type = " << ClassName() << "\n"
	 << "  SgTop-D3PD version = v" << fD3PDversion << "\n"
	 << "  Tree name = " << fTreeName << " with\n"
	 << "    weight_mc -> weight_mc" << (fMcWeightNumber > 0 ? Form("[%d]", fMcWeightNumber) : "") << "\n"
	 << "    weight_pileup -> weight_pileup" << fPileupSFVariationName << "\n"
	 << "    weight_jvt -> weight_jvt" << fJvtSFVariationName << "\n"
	 << "    weight_forwardjvt -> weight_forwardjvt" << fForwardJVTSFVariationName << "\n"
	 << "    weight_lepton -> weight_lepton" << fLeptonSFVariationName << "\n"
	 << "    weight_globalLeptonTriggerSF -> weight_globalLeptonTriggerSF" << fGlobalLeptonTriggerSFVariationName << "\n"
	 << "    weight_lepton -> weight_lepton" << fLeptonSFVariationName << "\n"
	 << "    weight_bTagSF_XX -> weight_bTagSF_XX" << fBtagSFVariationName <<
        ( fBtagSFVariationComponent >= 0 ? Form("[%d]", fBtagSFVariationComponent) : "") << "\n"
         << "  bTag WP mode: " << ( (fD3PDversion >= 29 || (fD3PDversion >= 25 && fForceContinBTag))
                                    ? "continuous" : "fixed") << "\n"
	 << endl;
}

//____________________________________________________________________

void AtlEvtReaderD3PDSgTopR2::ProcessMCWeightsStrings() {
    //
    // In case of multiple MC weights to be multiplied, use this function
    //   to fill the internal vectors from the provided strings
    //

    // read position string
    Int_t nsyst = abs(fMCWeightPosition);
    std::unique_ptr<TObjArray> pos_tokens(fMCWeightPositionString.Tokenize("_"));
    if ( pos_tokens->GetEntries() != nsyst ) {
        Fatal(__FUNCTION__, "Check systematic name. Could not find %d systematics"
              " separated by \"_\" in string %s. Abort!",
              nsyst, fMCWeightPositionString.Data());
    }
    for ( Int_t i = 0; i<pos_tokens->GetEntries(); i++ ) {
        // get string from token
        auto obj = pos_tokens->At(i);
        TString pos_str = static_cast<TObjString*>(obj)->GetString();
        // convert to int and add to vector
        fMCWeightPositionVector.push_back(pos_str.Atoi());
    }

    // read total events string
    Int_t nsyst2 = abs(fMCWeightPosition);
    if ( nsyst != nsyst2 ) {
        Fatal(__FUNCTION__, "Number of entries for mc weight positions (%d)"
              " and mc weight totals (%d) do not agree. Abort!", nsyst, nsyst2);
    }
    std::unique_ptr<TObjArray> tot_tokens(fMCWeightTotalEventsString.Tokenize("_"));
    if ( tot_tokens->GetEntries() != nsyst2 ) {
        Fatal(__FUNCTION__, "Check systematic name. Could not find %d systematics"
              " separated by \"_\" in string %s. Abort!",
              nsyst2, fMCWeightTotalEventsString.Data());
    }
    for ( Int_t i = 0; i<tot_tokens->GetEntries(); i++ ) {
        // get string from token
        auto obj =  tot_tokens->At(i);
        TString tot_str = static_cast<TObjString*>(obj)->GetString();
        // convert to int and add to vector
        fMCWeightTotalEventsVector.push_back(tot_str.Atoi());
    }
}
