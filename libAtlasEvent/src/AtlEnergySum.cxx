//____________________________________________________________________
//
// Atlas energy sums in pp collision events
//
// For details of the reconstruction, in particular that of Et_miss
// see the Atlas TWiki pages
// BEGIN_HTML<a href="https://twiki.cern.ch/twiki/bin/view/AtlasProtected/EtMiss">https://twiki.cern.ch/twiki/bin/view/AtlasProtected/EtMiss</a> END_HTML
//
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEnergySum
#include <AtlEnergySum.h>
#endif
#include <iostream>
#include <TString.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlEnergySum);
#endif

//____________________________________________________________________

AtlEnergySum::AtlEnergySum() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlEnergySum::~AtlEnergySum() {
  //
  // Default destructor
  //
}

//____________________________________________________________________

void AtlEnergySum::Clear(Option_t *option) {
  //
  // Clear this object
  //
  HepEnergySum::Clear(option);
  fRefFinalSumEt = 0.;
  fRefFinalMissingEt.Set(0., 0.);
  fRefFinalEMSumEt = 0.;
  fRefFinalEMMissingEt.Set(0., 0.);
  fRefFinalEM_etight_SumEt = 0.;
  fRefFinalEM_etight_MissingEt.Set(0., 0.);
  fRefFinalEM_etight_SumEt_CellOut = 0.;
  fRefFinalEM_etight_MissingEt_CellOut.Set(0., 0.);
  fRefFinalEM_etight_SumEt_SoftJets = 0.;
  fRefFinalEM_etight_MissingEt_SoftJets.Set(0., 0.);
  fRefFinalEM_etightpp_SumEt = 0.;
  fRefFinalEM_etightpp_MissingEt.Set(0., 0.);
  fRefFinalEM_emedium_SumEt = 0.;
  fRefFinalEM_emedium_MissingEt.Set(0., 0.);
  fRefFinalEM_emediumPP_SumEt = 0.;
  fRefFinalEM_emediumPP_MissingEt.Set(0., 0.);
  
  fRefFinalEM_emedium_SumEt_CellOut = 0.;
  fRefFinalEM_emedium_MissingEt_CellOut.Set(0., 0.);
  fRefFinalEM_emedium_SumEt_SoftJets = 0.;
  fRefFinalEM_emedium_MissingEt_SoftJets.Set(0., 0.);
  fRefFinalEM_eloose_SumEt = 0.;
  fRefFinalEM_eloose_MissingEt.Set(0., 0.);
//   fRefFinalEM_ewtm_sgtop_SumEt = 0.;
//   fRefFinalEM_ewtm_sgtop_MissingEt.Set(0., 0.);
  fSimplifiedRefFinalMissingEt.Set(0., 0.);
  fRefCompositionAntiktEmjesMissingEt.Set(0., 0.);
  fAtlFastSumEt = 0.;
  fAtlFastMissingEt.Set(0., 0.);
  fMCIntSumEt = 0.;
  fMCIntMissingEt.Set(0., 0.);
  fMCNonIntSumEt = 0.;
  fMCNonIntMissingEt.Set(0., 0.);
  fMCIntCentralSumEt = 0.;
  fMCIntCentralMissingEt.Set(0., 0.);
  fMCIntFwdSumEt = 0.;
  fMCIntFwdMissingEt.Set(0., 0.);
  fMCIntOutCoverSumEt = 0.;
  fMCIntOutCoverMissingEt.Set(0., 0.);
  fMCMuonsSumEt = 0.;
  fMCMuonsMissingEt.Set(0., 0.);
}

//____________________________________________________________________

void AtlEnergySum::Print(Option_t *option) const {
    //
    // Print object information
    //
    // Options available:
    //   "MC" - print MC truth information also
    //          (only valid for MC events)
    //
    TString opt = option;
    opt.ToUpper();

    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.precision(3);
    cout.width(8);
    cout << endl
	 << "---------------------------------------------------------------------------------------------------" << endl
	 << "Energy Sums                                SumEt (GeV)    |MissingEt| (GeV)    MissingEt_phi (deg)" << endl
	 << "---------------------------------------------------------------------------------------------------" << endl
	 << "Reconstruction (re-fined calib: RefFinalEM_etight) " << fRefFinalEM_etight_SumEt << "             " << RefFinalEM_etight_MissingEt_Mag()
	 << "              " << RefFinalEM_etight_MissingEt_Phi()*TMath::RadToDeg() << endl
	 << "---------------------------------------------------------------------------------------------------" << endl;
    if ( opt.Contains("MC") ) {
	cout << "MC, all non-interacting particles             " << fMCNonIntSumEt
	     << "              " << MCNonIntMissingEt_Mag()
	     << "              " << MCNonIntMissingEt_Phi()*TMath::RadToDeg() << endl
	     << "MC, all interacting particles |eta|<5         " << fMCIntSumEt
	     << "              " << MCIntMissingEt_Mag()
	     << "              " << MCIntMissingEt_Phi()*TMath::RadToDeg() << endl
	     << "MC, all interacting particles |eta|<3.2       " << fMCIntCentralSumEt
	     << "              " << MCIntCentralMissingEt_Mag()
	     << "              " << MCIntCentralMissingEt_Phi()*TMath::RadToDeg() << endl
	     << "MC, all interacting particles 3.2<|eta|<5     " << fMCIntFwdSumEt
	     << "              " << MCIntFwdMissingEt_Mag()
	     << "              " << MCIntFwdMissingEt_Phi()*TMath::RadToDeg() << endl
	     << "MC, all interacting particles 5<|eta|         " << fMCIntOutCoverSumEt
	     << "              " << MCIntOutCoverMissingEt_Mag()
	     << "              " << MCIntOutCoverMissingEt_Phi()*TMath::RadToDeg() << endl
	     << "MC, all muons                                 " << fMCMuonsSumEt
	     << "              " << MCMuonsMissingEt_Mag()
	     << "              " << MCMuonsMissingEt_Phi()*TMath::RadToDeg() << endl
	     << "AtlFast                                       " << fAtlFastSumEt
	     << "              " << AtlFastMissingEt_Mag()
	     << "              " << AtlFastMissingEt_Phi()*TMath::RadToDeg() << endl
	     << "---------------------------------------------------------------------------------------------------" << endl;
    }
}











