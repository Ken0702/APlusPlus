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
#include "AtlEvtReaderD3PDJetLepton.h"

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

        //assert(pt > 0.);
    	 if(pt < 0.) {
    		 pt = 0.;
    		 Info(__FUNCTION__, "Negative pt");

    	 }
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

AtlEvtReaderD3PDJetLepton::AtlEvtReaderD3PDJetLepton(AtlSelector * parent,
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
    , v_el_topoetcone20(0)
    , v_el_ptvarcone20(0)
    , v_el_d0sig(0)
    , v_el_true_type(0)
    , v_el_true_origin(0)
    , v_el_true_typebkg(0)
    , v_el_true_originbkg(0)
    , v_mu_pt(0)
    , v_mu_eta(0)
    , v_mu_phi(0)
    , v_mu_e(0)
    , v_mu_charge(0)
    , v_mu_topoetcone20(0)
    , v_mu_ptvarcone30(0)
    , v_mu_d0sig(0)
    , v_mu_true_type(0)
    , v_mu_true_origin(0)
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
    , v_jet_m(0)
    , b_weight_mc(0)
    , b_weight_pileup(0)
    , b_weight_sherpa_22_vjets(0)
    , b_weight_leptonSF(0)
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
    , b_el_topoetcone20(0)
    , b_el_ptvarcone20(0)
    , b_mu_pt(0)
    , b_mu_eta(0)
    , b_mu_phi(0)
    , b_mu_e(0)
    , b_mu_charge(0)
    , b_mu_topoetcone20(0)
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
    , b_lbn(0)
    , b_Vtxz(0)
    , b_npVtx(0)
    , b_el_n(0)
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
    , b_mu_isoFixedCutTightTrackOnly(0)
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
    , b_genfilter_BHadron(0)
    , b_genfilter_CHadronPt4Eta3(0)
    , b_hasMEphoton_DRgt02_nonhad(0)
    , fDataType(dataType)
    , fBtagSFVariationComponent(-1)
    , fMcWeightNumber(mcWeightNumber)
    , fIsSherpa22(-1) {
    //
    // Default constructor
    //
    TString vers(D3PDversion);
    vers.Remove(0, 1); // Remove "v" at the beginning
    fD3PDversion = vers.Atoi();
    TString systName(systematicName);
    fPeriodCount = -1;
    fForceContinBTag = kTRUE; // hardcode switch here for now

    bool useNominalTree;
    if ( systName.IsNull() || systName == "nominal" || systName.BeginsWith("Njet_") || systName == "kLEP_FAKE_BDECAYGEN_SHAPE" ) {
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

        if ( systName == "Pileup_SF" ) {
            useNominalTree = true;
            fPileupSFVariationName = Form("_%s", suffixUpper);
        }

        if ( systName == "JVT_SF" ) {
            useNominalTree = true;
            fJvtSFVariationName = Form("_%s", suffixUpper);
        }

    }
    fTreeName = Form("%s%s", ( useNominalTree ? "nominal" : systematicName ), treeNameSuffix);
    fTruthTree = 0;
}

//____________________________________________________________________

AtlEvtReaderD3PDJetLepton::AtlEvtReaderD3PDJetLepton(AtlSelector * parent,
						 EDataType dataType,
						 const char * D3PDversion,
                                                 char const * treeName,
						 Int_t mcWeightNumber)
    : AtlEvtReaderD3PDJetLepton(parent, dataType, D3PDversion, "nominal",
                              "", mcWeightNumber) {
    //
    // Same function as first constructor, but for setting the
    // tree name directly in the constructor
    //
    SetTreeName(treeName);
}

//____________________________________________________________________

AtlEvtReaderD3PDJetLepton::~AtlEvtReaderD3PDJetLepton() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

Int_t AtlEvtReaderD3PDJetLepton::GetEntry(TTree *t, Long64_t entry) {
    //
    // Clear all branches and get tree entry
    //
    ClearBranches();
    return super::GetEntry(t, entry);
}

//____________________________________________________________________

void AtlEvtReaderD3PDJetLepton::ClearBranches() {
    //
    // Clear all tree branches
    //
    v_weight_mc = NAN;
    clear(v_weights_mc);
    v_weight_pileup = NAN;
    v_weight_sherpa_22_vjets = NAN;
    v_weight_jvt = NAN;
    v_weight_leptonSF = NAN;
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
    clear(v_el_topoetcone20);
    clear(v_el_ptvarcone20);
    clear(v_mu_pt);
    clear(v_mu_eta);
    clear(v_mu_phi);
    clear(v_mu_e);
    clear(v_mu_charge);
    clear(v_mu_topoetcone20);
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
    v_lbn = std::numeric_limits<UInt_t>::max();
    v_Vtxz = NAN;
    v_npVtx = std::numeric_limits<UInt_t>::max();
    clear(v_el_d0sig);
    clear(v_el_true_type);
    clear(v_el_true_origin);
    clear(v_el_true_typebkg);
    clear(v_el_true_originbkg);
    v_mu_n = std::numeric_limits<UInt_t>::max();
    clear(v_mu_d0sig);
    clear(v_mu_true_type);
    clear(v_mu_true_origin);
    v_jet_n = std::numeric_limits<UInt_t>::max();
    clear(v_jet_m);
    v_met_px = NAN;
    v_met_py = NAN;
    v_met_sumet = NAN;
    // v_genfilter_BHadron = false;
    // v_genfilter_CHadronPt4Eta3 = false;
    // v_hasMEphoton_DRgt02_nonhad = false;
}

//____________________________________________________________________

void AtlEvtReaderD3PDJetLepton::SetBranches(TTree *t) {
    //
    // Set branch addresses
    //
    if ( gDebug>0 )
        Info(__FUNCTION__, "Starting SetBranches()");
    bool isMC = IsMC();
    InitBranches(t);
    if ( fMcWeightNumber >= 0 ) {
        ::SetupBranch(t, "weights_mc", &v_weights_mc, b_weights_mc, isMC);
    }
    else {
        ::SetupBranch(t, "weight_mc", &v_weight_mc, b_weight_mc, isMC);
    }
    ::SetupBranch(t, Form("weight_pileup%s", fPileupSFVariationName.Data()), &v_weight_pileup, b_weight_pileup, isMC);
    if ( fD3PDversion >= 12 && fD3PDversion < 20) {
        ::SetupBranch(t, "weight_sherpa_22_vjets", &v_weight_sherpa_22_vjets, b_weight_sherpa_22_vjets, isMC);
    }
    ::SetupBranch(t, Form("weight_jvt%s", fJvtSFVariationName.Data()), &v_weight_jvt, b_weight_jvt, isMC);
    ::SetupBranch(t, Form("weight_leptonSF%s", fLeptonSFVariationName.Data()), &v_weight_leptonSF, b_weight_leptonSF, isMC);

    if ( fD3PDversion >= 31 ) {

        	if ( fBtagSFVariationComponent >= 0 ) {
        	            assert(!fBtagSFVariationName.IsNull());
        	            ::SetupBranch(t, Form("weight_bTagSF_DL1r_Continuous%s", fBtagSFVariationName.Data()),
        	                          &v_weight_bTagSF_Continuous_vec, b_weight_bTagSF_Continuous, isMC);
        	        } else {
        	            ::SetupBranch(t, Form("weight_bTagSF_DL1r_Continuous%s", fBtagSFVariationName.Data()),
        	                          &v_weight_bTagSF_Continuous, b_weight_bTagSF_Continuous, isMC);
        	        }
        }

    if ( fD3PDversion >= 25 && fD3PDversion < 31 ) {
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
    }
    else {
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
    ::SetupBranch(t, "runNumber", &v_runNumber, b_runNumber);
    ::SetupBranch(t, "mcChannelNumber", &v_mcChannelNumber, b_mcChannelNumber);
    ::SetupBranch(t, "mu", &v_mu, b_mu);
    ::SetupBranch(t, "el_pt", &v_el_pt, b_el_pt);
    ::SetupBranch(t, "el_eta", &v_el_eta, b_el_eta);
    ::SetupBranch(t, "el_phi", &v_el_phi, b_el_phi);
    ::SetupBranch(t, "el_e", &v_el_e, b_el_e);
    ::SetupBranch(t, "el_charge", &v_el_charge, b_el_charge);
    ::SetupBranch(t, "el_topoetcone20", &v_el_topoetcone20, b_el_topoetcone20);
    ::SetupBranch(t, "el_ptvarcone20", &v_el_ptvarcone20, b_el_ptvarcone20);
    ::SetupBranch(t, "mu_pt", &v_mu_pt, b_mu_pt);
    ::SetupBranch(t, "mu_eta", &v_mu_eta, b_mu_eta);
    ::SetupBranch(t, "mu_phi", &v_mu_phi, b_mu_phi);
    ::SetupBranch(t, "mu_e", &v_mu_e, b_mu_e);
    ::SetupBranch(t, "mu_charge", &v_mu_charge, b_mu_charge);
    ::SetupBranch(t, "mu_topoetcone20", &v_mu_topoetcone20, b_mu_topoetcone20);
    ::SetupBranch(t, "mu_ptvarcone30", &v_mu_ptvarcone30, b_mu_ptvarcone30);
    ::SetupBranch(t, "jet_pt", &v_jet_pt, b_jet_pt);
    ::SetupBranch(t, "jet_eta", &v_jet_eta, b_jet_eta);
    ::SetupBranch(t, "jet_phi", &v_jet_phi, b_jet_phi);
    ::SetupBranch(t, "jet_e", &v_jet_e, b_jet_e);
    if ( fD3PDversion >= 25 && fD3PDversion < 31 ) {
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
    ::SetupBranch(t, "jet_jvt", &v_jet_jvt, b_jet_jvt);
    ::SetupBranch(t, "jet_truthflav", &v_jet_truthflav, b_jet_truthflav);
    ::SetupBranch(t, "met_met", &v_met_met, b_met_met);
    ::SetupBranch(t, "met_phi", &v_met_phi, b_met_phi);
    if ( fD3PDversion >= 12 ) {
        fPeriodCount = 2;
    }
    ::SetupBranch(t, "el_d0sig", &v_el_d0sig, b_el_d0sig);
    ::SetupBranch(t, "el_true_type", &v_el_true_type, b_el_true_type, isMC);
    ::SetupBranch(t, "el_true_origin", &v_el_true_origin, b_el_true_origin, isMC);
    ::SetupBranch(t, "el_true_typebkg", &v_el_true_typebkg, b_el_true_typebkg, isMC);
    ::SetupBranch(t, "el_true_originbkg", &v_el_true_originbkg, b_el_true_originbkg, isMC);
    ::SetupBranch(t, "mu_d0sig", &v_mu_d0sig, b_mu_d0sig);
    ::SetupBranch(t, "mu_true_type", &v_mu_true_type, b_mu_true_type, isMC);
    ::SetupBranch(t, "mu_true_origin", &v_mu_true_origin, b_mu_true_origin, isMC);
}

//____________________________________________________________________

Bool_t AtlEvtReaderD3PDJetLepton::Notify() {
    //
    // The Notify() function is called when a new file is opened.
    //

    // Load truth tree if requested
    if ( IsMC() && (fParent->fDoTruthTree==kTRUE) ) LoadTruthTree();
    return kTRUE;
}

//____________________________________________________________________

void AtlEvtReaderD3PDJetLepton::LoadTruthTree() {
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

void AtlEvtReaderD3PDJetLepton::BuildEvent() {
    //
    // Build A++ event from D3PD content
    //

    // Build header
    if ( gDebug > 1 )
        Info(__FUNCTION__, "Building Event Header");
    BuildEventHeader();

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

    if ( gDebug > 1)
        Info(__FUNCTION__, "Event built");
}

//____________________________________________________________________

void AtlEvtReaderD3PDJetLepton::BuildEventHeader() {
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

    fEvent->SetEventHeader(
        /* RunNr = */ v_runNumber,
        /* EventNr = */ v_eventNumber,
        /* LumiBlock = */ v_lbn,
        /* BeamEnergy = */ 6500.,
        /* IsMC = */ isMC,
        /* EventType = */ AtlEventHeader::kPhysics,
        /* MCWeight = */ (isMC ? (fMcWeightNumber >= 0 ? v_weights_mc->at(fMcWeightNumber) : v_weight_mc) : 1.),
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
    eventHeader->SetAverageIntPerXing(v_mu);
    eventHeader->SetChannelNumber(v_mcChannelNumber);

    if ( isMC ) {
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
    }
}

//___________________________________________________________

void AtlEvtReaderD3PDJetLepton::BuildEnergySum() {
    //
    // Build A++ energy sum
    //
    HepEnergySum *energySum = fEvent->GetEnergySum();
    // energySum->SetMissingEt(v_met_px*MeV, v_met_py*MeV);
    energySum->SetMissingEtMagPhi(v_met_met*MeV, v_met_phi);
    // energySum->SetSumEt(v_met_met*MeV);
}

//___________________________________________________________

void AtlEvtReaderD3PDJetLepton::BuildElectrons() {
    //
    // Build list of A++ electrons
    //
    // The ID quality can be either tight or loose
    //
    // Loop over all electrons
    // el_n is not available in v21; use size of v_el_pt

    Int_t el_n = v_el_n;
    if ( fD3PDversion > 20 ) {
        el_n = v_el_pt->size();
    }
    for ( Int_t el_i = 0; el_i < el_n; ++el_i ) {
        if ( gDebug )
            Info(__FUNCTION__, "creating electron");
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
            /* IsEM = */
            // there are only tight electrons here
            // ( v_el_tight->at(el_i) )            ?
            AtlEMShower::kElectronTight,// : AtlEMShower::kElectronLoose,
            /* Author = */ AtlElectron::kHighPtElectron,
            /* PCluster = */ TLorentzVector(NAN, NAN, NAN, NAN));
        electron->SetP(PtEtaPhiE(v_el_pt->at(el_i)*MeV, v_el_eta->at(el_i),
				 v_el_phi->at(el_i), v_el_e->at(el_i)*MeV));
        electron->SetEtConeVars(NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN,
				NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN,
				NAN, NAN, NAN, NAN, NAN, NAN);
        electron->SetElectronScaleFactors(NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN);
        electron->SetIsTriggerMatched(kTRUE); //v_el_trigMatch->at(el_i));
	// if ( IsMC() ) {
        //     Info(__FUNCTION__, "set el true origin");
	//     electron->SetMCTruthClassifier((AtlMCTruthClassifier::EParticleOrigin)v_el_true_origin->at(el_i));
	// }
    }
}

//____________________________________________________________________

void AtlEvtReaderD3PDJetLepton::BuildMuons() {
    //
    // Build list of A++ muons
    //
    // The ID quality can be either tight or loose
    //

    // Loop over all muons
    // mu_n is not available in v21; use size of v_mu_pt
    Int_t mu_n = v_mu_n;
    if ( fD3PDversion > 20 )
        mu_n = v_mu_pt->size();
    for ( Int_t mu_i = 0; mu_i < mu_n; ++mu_i ) {
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
            /* Quality = */ AtlMuon::kTight,
            /* MatchingChi2 = */ NAN,
            /* MatchingNDoF = */ -999,
            /* IsCombinedMuon = */ kTRUE,
            /* PMuonSpecExtrapol = */ TLorentzVector(NAN, NAN, NAN, NAN),
            /* MuonSpecExtrapolCharge = */ 0);
        muon->SetP(PtEtaPhiE(v_mu_pt->at(mu_i)*MeV, v_mu_eta->at(mu_i),
			     v_mu_phi->at(mu_i), v_mu_e->at(mu_i)*MeV));
        muon->SetMuonScaleFactors(NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN);
        muon->SetIsTriggerMatched(kTRUE); //v_mu_trigMatch->at(mu_i));
	if ( IsMC() ) {
	    muon->SetMCTruthClassifier((AtlMCTruthClassifier::EParticleOrigin)v_mu_true_origin->at(mu_i));
	}
    }
}

//____________________________________________________________________

void AtlEvtReaderD3PDJetLepton::BuildJets() {
    //
    // Build A++ jets
    //

    // jet_n is not available in v21; use size of v_jet_pt
    Int_t jet_n = v_jet_n;
    if ( fD3PDversion >= 23 )
        jet_n = v_jet_pt->size();
    for ( Int_t jet_i = 0; jet_i < jet_n; ++jet_i ) {
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
                Fatal(__FUNCTION__, "v_jet_tagWeightBin_Continuous is too small (%d) for (%d) jets",
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
        jet->SetJetScaleFactors_MV1_60(NAN, NAN, NAN, NAN, NAN, NAN, NAN);
        jet->SetJetScaleFactors_MV1_70(NAN, NAN, NAN, NAN, NAN, NAN, NAN);
        jet->SetJetScaleFactors_MV1_80(NAN, NAN, NAN, NAN, NAN, NAN, NAN);
        jet->SetJetScaleFactors_MV1c_50(NAN, NAN, NAN, NAN, NAN, NAN, NAN);
    }
}

/* !!! end of automatically generated content !!! */

//____________________________________________________________________

void AtlEvtReaderD3PDJetLepton::BuildFromTruthTree() {
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

AtlEvtReaderD3PDJetLepton::InitialSumOfWeights_t AtlEvtReaderD3PDJetLepton::GetInitialSumOfWeights(TFile * inputFile) const {
    //
    // Get initial sum of weights
    //
    if ( gDebug )
        Info(__FUNCTION__, "Getting sum of weights");
    AtlEvtReaderD3PDJetLepton::InitialSumOfWeights_t result;
    if ( !IsMC() )
        return result; /* = undefined */
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
                            Fatal(__FUNCTION__, "selections do not agree on the initial number of events");
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
    if ( !(fMcWeightNumber >= 0) ) {
        /* cross check with the number from xAOD::CutBookkeeper */
        TTree * sumWeightsTree = 0;
        inputFile->GetObject("sumWeights", sumWeightsTree);
        double sumOfWeightsFromCbk = NAN;
        if ( !SumOverTree(sumWeightsTree, "totalEventsWeighted", sumOfWeightsFromCbk) ) {
            Warning(__FUNCTION__, "could not get number of initial events from xAOD::CutBookkeeper");
        } else if ( fabs(sumOfWeightsFromCbk - result.mcevt) > 0. ) {
            /* TODO figure out whether xAOD::CutBookkeeper uses pileup and z-vertex weights, too */
            Warning(__FUNCTION__, "xAOD::CutBookkeeper reported a different number of initial events (%g) than the cutflow histogram (%g, difference %g)", sumOfWeightsFromCbk, result.mcevt, result.mcevt-sumOfWeightsFromCbk);
        }
        // since this number is more reliable, use it for further processing
        result.mcevt_pileup_zvtx = sumOfWeightsFromCbk;
        result.mcevt = sumOfWeightsFromCbk; // there is no other MC-only value
        double eventsFromCbk = NAN;
        if ( SumOverTree(sumWeightsTree, "totalEvents", eventsFromCbk) ) {
            result.one = eventsFromCbk;
        } else {
            Warning(__FUNCTION__, "could not get number of unweighted initial events from xAOD::CutBookkeeper");
        }
        delete sumWeightsTree;
    }
    return result;
}

//____________________________________________________________________

void AtlEvtReaderD3PDJetLepton::Print(Option_t *option) const {
    //
    // Print event reader information
    //
    cout << endl
	 << "Event reader configuration:\n"
	 << "  Type = " << ClassName() << "\n"
	 << "  SgTop-D3PD version = v" << fD3PDversion << "\n"
	 << "  Tree name = " << fTreeName << " with\n"
	 << "    weight_mc -> weight_mc" << (fMcWeightNumber >= 0 ? Form("[%d]", fMcWeightNumber) : "") << "\n"
	 << "    weight_pileup -> weight_pileup" << fPileupSFVariationName << "\n"
	 << "    weight_jvt -> weight_jvt" << fJvtSFVariationName << "\n"
	 << "    weight_lepton -> weight_lepton" << fLeptonSFVariationName << "\n"
	 << "    weight_bTagSF_XX -> weight_bTagSF_XX" << fBtagSFVariationName <<
        ( fBtagSFVariationComponent >= 0 ? Form("[%d]", fBtagSFVariationComponent) : "") << "\n"
         << "  bTag WP mode: " << ( (fD3PDversion >= 29 || (fD3PDversion >= 25 && fForceContinBTag))
                                    ? "continuous" : "fixed") << "\n"
	 << endl;
}
