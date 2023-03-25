//___________________________________________________________
//
//  Top QCD estimation Tool
//
//  This tool is used to estimate the QCD background with the Matrix
//  Method for muons and electrons.
// 
// Author: Umberto Prosperi Porta<mailto: oreste@physik.hu-berlin.de.de>
// Update: $Id$
// Copyright: 2011 (C) Umberto Prosperi Porta 
//
//
//
//
#include <AtlQCDMMTool.h>
#include <AtlEvent.h>                 // for AtlEvent
#include <TCollection.h>              // for TIter
#include <TDirectory.h>               // for TDirectory, gDirectory
#include <TFile.h>                    // for TFile
#include <TH1.h>                      // for TH1F
#include <TList.h>                    // for TList
#include <TString.h>                  // for TString, operator<<, Form
#include <TSystem.h>                  // for TSystem, gSystem
#include <TTree.h>                    // for TTree
#include <TVector2.h>                 // for TVector2
#include <math.h>                     // for fabs
#include <iostream>                   // for operator<<, endl, basic_ostream
#include "AtlAnalysisTool.h"          // for AtlAnalysisTool, AtlAnalysisToo...
#include "AtlEnergySum.h"             // for AtlEnergySum
#include "AtlEventHeader.h"           // for AtlEventHeader
#include "AtlJet.h"                   // for AtlJet
#include "AtlObjectsToolD3PDSgTop.h"  // for AtlObjectsToolD3PDSgTop
#include "AtlSelector.h"              // for AtlSelector
#include "HepParticle.h"              // for HepParticle
#include "TopFakes/FakesWeights.h"    // for FakesWeights, FakesWeights::EJETS
#include "TopFakes/MMEffSet.h"        // for MMLepton, MMEvent

using namespace std;

//___________________________________________________________

AtlQCDMMTool::AtlQCDMMTool(const char* name, const char* title) : 
    AtlAnalysisTool(name, title) {
    //
    // Default constructor
    //
    fHistMMWeight_nominal = 0;
    fHistMMWeight_e_nominal = 0;
    fHistMMWeight_mu_nominal = 0;
    fHistMMEff_fake = 0;
    fHistMMEff_e_fake = 0;
    fHistMMEff_mu_fake = 0;
    fHistMMEff_real = 0;
    fHistMMEff_e_real = 0;
    fHistMMEff_mu_real = 0;
    fObjTool = 0;
    SetCutDefaults();
    
    // Fake weights
    fFakeWeightEJets = new FakesWeights();
    fFakeWeightMuJets = new FakesWeights();
    if ( gDebug > 0 ) {
        fFakeWeightEJets->SetDebug(kTRUE);
        fFakeWeightMuJets->SetDebug(kTRUE);
    }
    fEfficiencyPath2015 = TString("$APP_HOME/external/TopFakes/data/20p7/2015");
    fEfficiencyPath2016 = TString("$APP_HOME/external/TopFakes/data/20p7/2016");
    fEfficiencyPath2017 = TString("");
    fEfficiencyPath = TString("");
    fYear = TString("0000"); 
    // fFakeParamString_fake = TString("NS:pt:dPhi:nbtag");
    // fFakeParamString_real = TString("NS:pt:dR");
    fParamString_fake2015 = TString("2015Test:NS:jetpt:eta:dR");
    fParamString_fake2016 = TString("2016Test:NS:jetpt:eta:dR");
    fParamString_fake2017 = TString("");
    fParamString_fake = TString("Test:NS:jetpt:eta:dR");
    fParamString_real2015 = TString("2015Test:NS:jetpt:eta:dR");
    fParamString_real2016 = TString("2016Test:NS:jetpt:eta:dR");
    fParamString_real2017 = TString("");
    fParamString_real = TString("Test:NS:jetpt:eta:dR");
}

//___________________________________________________________

AtlQCDMMTool::~AtlQCDMMTool() {
    //
    // Default destructor
    //
    if ( fHistMMWeight_nominal != 0 ) delete fHistMMWeight_nominal;
    if ( fHistMMWeight_e_nominal != 0 )  delete fHistMMWeight_e_nominal;
    if ( fHistMMWeight_mu_nominal != 0 )  delete fHistMMWeight_mu_nominal;
    if ( fHistMMEff_fake != 0 )  delete fHistMMEff_fake;
    if ( fHistMMEff_e_fake != 0 )  delete fHistMMEff_e_fake;
    if ( fHistMMEff_mu_fake != 0 )  delete fHistMMEff_mu_fake;
    if ( fHistMMEff_real != 0 )  delete fHistMMEff_real;
    if ( fHistMMEff_e_real != 0 )  delete fHistMMEff_e_real;
    if ( fHistMMEff_mu_real != 0 )  delete fHistMMEff_mu_real;
    if ( fFakeWeightEJets != 0 )  delete fFakeWeightEJets;
    if ( fFakeWeightMuJets != 0 )  delete fFakeWeightMuJets;
}

//___________________________________________________________

void AtlQCDMMTool::SetBranchStatus() {
    //
    // Switch on needed branches
    //
    fTree->SetBranchStatus("fEventHeader*", kTRUE);
}

//___________________________________________________________

void AtlQCDMMTool::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fProcessMode = kPreAnalysis;
}

//___________________________________________________________

Bool_t AtlQCDMMTool::Notify() {
    //
    // This function is called every time when a new input file is
    // opened.
    //

    // No further effort for run 1
    if ( fEvent->IsRun1() )
        return kTRUE;

    TDirectory *savdir = gDirectory;

    // Fetch efficiency histograms for the matrix method by year from filename
    TString fname(fParent->GetCurrentTree()->GetCurrentFile()->GetName());
    Int_t pos = fname.Index("data");
    TString year = fname(pos+4, 2);

    if ( year.EqualTo("15") || year.EqualTo("16") || year.EqualTo("17") ) {
        year.Prepend("20");
        if ( year.EqualTo(fYear) ) {
            // don't setup weighter again if year was already correct
            return kTRUE;
        }
        fYear = year;
        TString eff_path;
        TString eff_fake;
        TString eff_real;
        if ( !fEfficiencyPath.IsNull() ) {
            eff_path = Form("%s/%s", fEfficiencyPath.Data(), year.Data());
            eff_fake = Form("%s%s", year.Data(), fParamString_fake.Data());
            eff_real = Form("%s%s", year.Data(), fParamString_real.Data());
        } else if ( year.EqualTo("2015") ) {
            fFakeWeightEJets->SetPeriod(1);
            fFakeWeightMuJets->SetPeriod(1);
            eff_path = fEfficiencyPath2015;
            eff_fake = fParamString_fake2015;
            eff_real = fParamString_real2015;
            eff_fake.ReplaceAll("/TR", ""); // somehow this does not work otherwise
            eff_real.ReplaceAll("/TR", "");
        } else if ( year.EqualTo("2016") ) {
            fFakeWeightEJets->SetPeriod(2);
            fFakeWeightMuJets->SetPeriod(2);
            eff_path = fEfficiencyPath2016;
            eff_fake = fParamString_fake2016;
            eff_real = fParamString_real2016;
        } else {
            fFakeWeightEJets->SetPeriod(2); // note this does nothing without trigger split
            fFakeWeightMuJets->SetPeriod(2);
            eff_path = fEfficiencyPath2017;
            eff_fake = fParamString_fake2017;
            eff_real = fParamString_real2017;
        }
        Info(__FUNCTION__, "Load MM efficiency files from %s", eff_path.Data());
        fFakeWeightEJets->SetDataPath(eff_path.Data());
        fFakeWeightEJets->SetupWeighter(FakesWeights::EJETS, eff_real.Data(), eff_fake.Data());
        fFakeWeightMuJets->SetDataPath(eff_path.Data());
        fFakeWeightMuJets->SetupWeighter(FakesWeights::MUJETS, eff_real.Data(), eff_fake.Data());
    } else {
        Error(__FUNCTION__, "Input data file %s is neither 2015 nor 2016. Abort!",
              fname.Data());
        gSystem->Abort(1);
    }

    savdir->cd();

    return kTRUE;
}

//___________________________________________________________

void AtlQCDMMTool::BookHistograms() {
    //
    // Book histograms
    //
    if ( fObjTool == 0 ) {
	fObjTool = (AtlObjectsToolD3PDSgTop*)
	    GetTool("AtlObjectsToolD3PDSgTop", "",kTRUE);
    }

    fHistMMWeight_nominal = new TH1F("h_MMWeight_nominal",
    				      "QCDMM nominal weight",
    				      30, -1., 2.);
    fHistMMWeight_nominal->SetXTitle("QCDMM weight nominal");
    fHistMMWeight_nominal->SetYTitle("Number of Entries");

    fHistMMWeight_e_nominal = new TH1F("h_MMWeight_e_nominal",
				       "QCDMM nominal weight for electrons",
    				      30, -1., 2.);
    fHistMMWeight_e_nominal->SetXTitle("QCDMM weight nominal");
    fHistMMWeight_e_nominal->SetYTitle("Number of Entries");

    fHistMMWeight_mu_nominal = new TH1F("h_MMWeight_mu_nominal",
    				      "QCDMM nominal weight for muons",
    				      30, -1., 2.);
    fHistMMWeight_mu_nominal->SetXTitle("QCDMM weight nominal");
    fHistMMWeight_mu_nominal->SetYTitle("Number of Entries");

    // Fake Efficiency l,e,nu
    fHistMMEff_fake = new TH1F("h_MMEff_fake",
			       "QCDMM fake efficiency",
			       30, -1., 2.);
    fHistMMEff_fake->SetXTitle("QCDMM fake efficiency");
    fHistMMEff_fake->SetYTitle("Number of Entries");
    fHistMMEff_e_fake = new TH1F("h_MMEff_e_fake",
			       "QCDMM fake efficiency",
			       30, -1., 2.);
    fHistMMEff_e_fake->SetXTitle("QCDMM fake efficiency");
    fHistMMEff_e_fake->SetYTitle("Number of Entries");
    fHistMMEff_mu_fake = new TH1F("h_MMEff_mu_fake",
			       "QCDMM fake efficiency",
			       30, -1., 2.);
    fHistMMEff_mu_fake->SetXTitle("QCDMM fake efficiency");
    fHistMMEff_mu_fake->SetYTitle("Number of Entries");

    // Real Efficiency l,e,nu
    fHistMMEff_real = new TH1F("h_MMEff_real",
			       "QCDMM real efficiency",
			       30, -1., 2.);
    fHistMMEff_real->SetXTitle("QCDMM real efficiency");
    fHistMMEff_real->SetYTitle("Number of Entries");
    fHistMMEff_e_real = new TH1F("h_MMEff_e_real",
			       "QCDMM real efficiency",
			       30, -1., 2.);
    fHistMMEff_e_real->SetXTitle("QCDMM real efficiency");
    fHistMMEff_e_real->SetYTitle("Number of Entries");
    fHistMMEff_mu_real = new TH1F("h_MMEff_mu_real",
			       "QCDMM real efficiency",
			       30, -1., 2.);
    fHistMMEff_mu_real->SetXTitle("QCDMM real efficiency");
    fHistMMEff_mu_real->SetYTitle("Number of Entries");
}

//___________________________________________________________

void AtlQCDMMTool::InitEvent() {
    //
    // Definition of event with determination of physical objects
    //
}

//___________________________________________________________

Bool_t AtlQCDMMTool::AnalyzeEvent() {
    //
    // Invocation of tool, computes the event weights
    // in order to estimate QCD
    //
    if ( gDebug > 0 )
	Info(__FUNCTION__, "Beginning function");
    Bool_t is_electron;
    Bool_t is_tight;

    // Obtain weight
    if ( fEvent->IsRun1() ) {
	fMMWeight = fEvent->GetMMWeightNominal();
    } else if ( fEvent->IsRun2() ) {
	// apply pT cut on loose leptons first
	fObjTool->ApplyLooseLeptonPtCut();
	// skip (weight=0) if not exactly 1 electron/muon
	fMMWeight = 0.;
	HepParticle *lepton;
	if ( fObjTool->GetLeptons()->GetEntries() +
	     fObjTool->GetLooseLeptons()->GetEntries() == 1 ) {
	    if ( fObjTool->GetLeptons()->GetEntries() == 1 ) {
		// tight
		is_tight = kTRUE;
		lepton = (HepParticle*)fObjTool->GetLeptons()->First();
		if ( lepton->IsElectron() ) {
		    is_electron = kTRUE;
		} else if ( lepton->IsMuon() ) {
		    is_electron = kFALSE;
		} else {
		    Warning(__FUNCTION__, "Lepton has invalid ID of %d",
			    lepton->GetPdgCode());
                    return kFALSE;
		}
	    } else {
		// loose
		is_tight = kFALSE;
		lepton = (HepParticle*)fObjTool->GetLooseLeptons()->First();
		if ( lepton->IsElectron() ) {
		    is_electron = kTRUE;
		} else if ( lepton->IsMuon() ) {
		    is_electron = kFALSE;
		} else {
		    // ??
		    Error(__FUNCTION__, "Lepton has invalid ID of %d",
			  lepton->GetPdgCode());
                    return kFALSE;
		}
	    } 
	    
	    // set up trigger variable
	    //   The trigger split is defined in MMEffSet::AddTriggerSplit()
	    //     and it is accessed in MMEffSet::TriggerValue().
	    //   Trigger information available in the ntuples can be found
	    //     in any logfile when running a sample.
	    // TO DO: use trigger match or trigger pass?
	    int trigmatch = 0;
	    if ( is_electron && fYear.EqualTo("2015") ) {
		; // no trigger split available in eff files and MMEffSet
	    }
	    if ( is_electron && fYear.EqualTo("2016") ) {
		// no big influence; first trigger with e26 instead of e24
		if ( fEvent->HasPassedHLT("HLT_e26_lhtight_nod0_ivarloose") )
		    trigmatch |= 0x1 << 0;
		if ( fEvent->HasPassedHLT("HLT_e60_lhmedium_nod0") )
		    trigmatch |= 0x1 << 1;
	    	if ( fEvent->HasPassedHLT("HLT_e24_lhmedium_L1EM20VH") )
		    trigmatch  |= 0x1 << 2;
	    }
	    if ( !is_electron && fYear.EqualTo("2015") ) {
		// third trigger not available in ntuple
	    	if ( fEvent->HasPassedHLT("HLT_mu20_iloose_L1MU15") )
		    trigmatch |= 0x1 << 0;
	    	if ( fEvent->HasPassedHLT("HLT_mu50") )
		    trigmatch |= 0x1 << 1;
	    	// if ( fEvent->HasPassedHLT("HLT_mu20_L1MU15") )
		//     trigmatch  |= 0x1 << 2;
	    }
	    if ( !is_electron && fYear.EqualTo("2016") ) {
		// third trigger not available; first with mu24->mu26 
	    	if ( fEvent->HasPassedHLT("HLT_mu26_ivarmedium") )
		    trigmatch |= 0x1 << 0;
	    	if ( fEvent->HasPassedHLT("HLT_mu50") )
		    trigmatch |= 0x1 << 1;
	    	// if ( fEvent->HasPassedHLT("HLT_mu24") )
		//     trigmatch  |= 0x1 << 2;
	    }

	    // build MMLepton with:
	    // - pt of the lepton
	    // - abs(eta) of the lepton, use the cluster eta for electrons!
	    // - deltaR between the lepton and the closest jet
	    // - deltaPhi between lepton and ETmiss
	    // - trigger only for muons (TO DO: look into this)
	    Float_t met_phi = fEvent->GetEnergySum()->MissingEt_Phi();
	    TIter next_jet(fObjTool->GetJets());
	    AtlJet *jet = 0;
	    Float_t lowest_dR = 10.;
	    while ( (jet = (AtlJet*)next_jet()) ) {
		Float_t dR = jet->DeltaR(lepton);
		if ( dR < lowest_dR ) {
		    lowest_dR = dR;
		}
	    }
	    MMLepton mmlepton;
	    mmlepton.pt = lepton->Pt();
	    mmlepton.eta = fabs(lepton->Eta());
	    mmlepton.dR = lowest_dR;
	    mmlepton.dPhi = fabs(TVector2::Phi_mpi_pi(lepton->Phi()-met_phi));
	    mmlepton.trigger = trigmatch;
	    // build MMEvent
	    // - jet multiplicity
	    // - b-jet multiplicity // default tagger: same as in fObjTool
	    // - pt of the leading jet
            // - add MtW in place of MET
	    Int_t ntag = fObjTool->GetBJets()->GetEntries();
	    MMEvent mmevent;
	    mmevent.njets = fObjTool->GetJets()->GetEntries();
	    mmevent.ntag = ntag;
	    mmevent.jetpt = ((HepParticle*)fObjTool->GetJets()->At(0))->Pt();
            mmevent.sumet = fEvent->W_Mperp(lepton);

	    // debugging
	    if ( gDebug > 1 ) {
		Info(__FUNCTION__, "is_electron      = %d", is_electron);
		Info(__FUNCTION__, "MMLepton.pt      = %f", mmlepton.pt);
		Info(__FUNCTION__, "MMLepton.eta     = %f", mmlepton.eta);
		Info(__FUNCTION__, "MMLepton.dR      = %f", mmlepton.dR);
		Info(__FUNCTION__, "MMLepton.dPhi    = %f", mmlepton.dPhi);
		Info(__FUNCTION__, "MMLepton.trigger = %i", mmlepton.trigger);
		Info(__FUNCTION__, "MMEvent.njets    = %i", mmevent.njets);
		Info(__FUNCTION__, "MMEvent.ntag     = %i", mmevent.ntag);
		Info(__FUNCTION__, "MMEvent.jetpt    = %f", mmevent.jetpt);
	    }

	    // get weight
	    Double_t fake_eff = -1.;
	    Double_t real_eff = -1.;
	    if ( is_electron ) {
		fFakeWeightEJets->SetLepton(mmevent, mmlepton);
		fMMWeight = fFakeWeightEJets->GetFakesWeightLJets(is_tight);
		fake_eff = fFakeWeightEJets->GetFakeEff();
		real_eff = fFakeWeightEJets->GetRealEff();
		fHistMMEff_e_fake->Fill(fake_eff);
		fHistMMEff_e_real->Fill(real_eff);
		fHistMMWeight_e_nominal->Fill(fMMWeight);
	    } else {
		fFakeWeightMuJets->SetLepton(mmevent, mmlepton);
		fMMWeight = fFakeWeightMuJets->GetFakesWeightLJets(is_tight);
		fake_eff = fFakeWeightMuJets->GetFakeEff();
		real_eff = fFakeWeightMuJets->GetRealEff();
		fHistMMEff_mu_fake->Fill(fake_eff);
		fHistMMEff_mu_real->Fill(real_eff);
		fHistMMWeight_mu_nominal->Fill(fMMWeight);
	    }

	    fHistMMEff_fake->Fill(fake_eff);
	    fHistMMEff_real->Fill(real_eff);
	    
	    fEvent->GetEventHeader()->SetMMWeightNominal(fMMWeight);
	}
    } else {
	Error(__FUNCTION__, "Unknown run phase given. Abort!");
	gSystem->Abort(1);
    }

    // Set event weights
    fEvent->SetPreTagEvtWeight(fMMWeight*GetPreTagEvtWeight());
    fEvent->SetTagEvtWeight(fMMWeight*GetTagEvtWeight());

    // fill histograms
    fHistMMWeight_nominal->Fill(fMMWeight);
    
    return kTRUE;
}

//___________________________________________________________

void AtlQCDMMTool::FillHistogramsNoEvtSel() {
    //
    // Method for filling histograms, without any selection cuts
    // applied
    //
    // fHistMMWeight_nominal->Fill(fMMWeight);
}

//___________________________________________________________

void AtlQCDMMTool::FillHistograms() {
    //
    // Method for filling histograms, only events that survive 
    // AnalyzeEvent() are taken into account
    //
}

//___________________________________________________________

void AtlQCDMMTool::Print() const {
    //
    // Print tool configuration
    //
    cout << endl
         << "========================================================" << endl
         << "  QCDMM Tool \"" << GetName() << "\"" << endl
         << "========================================================"
	 << endl;
    AtlAnalysisTool::Print();
    cout << endl;
    if ( fEfficiencyPath.IsNull() ) {
        cout << "  Eff. Path  2015 = " << fEfficiencyPath2015 << endl
             << "  Real param 2015 = " << fParamString_real2015 << endl
             << "  Fake param 2015 = " << fParamString_fake2015 << endl;
        cout << "  Eff. Path  2016 = " << fEfficiencyPath2016 << endl
             << "  Real param 2016 = " << fParamString_real2016 << endl
             << "  Fake param 2016 = " << fParamString_fake2016 << endl;
        cout << "  Eff. Path  2017 = " << fEfficiencyPath2017 << endl
             << "  Real param 2017 = " << fParamString_real2017 << endl
             << "  Fake param 2017 = " << fParamString_fake2017 << endl;
    } else {
        cout << "  Efficiency Path = " << fEfficiencyPath << endl
             << "  Real param.     = " << fParamString_real << endl
             << "  Fake param.     = " << fParamString_fake << endl;
    }
    cout     << " "
         << "========================================================" << endl
	 << endl;
}

//___________________________________________________________

void AtlQCDMMTool::Terminate() {
    //
    // Executed at the end of the tool
    //
}

