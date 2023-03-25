//____________________________________________________________________
//
// Atlas muon class
// 
// For details about the muon reconstruction at Atlas see the muon
// working group wiki page at
// BEGIN_HTML
// <a href="https://twiki.cern.ch/twiki/bin/view/AtlasProtected/MuonPerformance">
// https://twiki.cern.ch/twiki/bin/view/AtlasProtected/MuonPerformance</a>
// END_HTML
// or the MuonRecoPedia
// BEGIN_HTML
// <a href="https://twiki.cern.ch/twiki/bin/view/AtlasProtected/MuonRecoPedia">
// https://twiki.cern.ch/twiki/bin/view/AtlasProtected/MuonRecoPedia</a>
// END_HTML
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlMuon
#include <AtlMuon.h>
#endif
#include <TSystem.h>
#include <iostream>
#include <iomanip>

using namespace std;

#ifndef __CINT__
ClassImp(AtlMuon);
#endif

const char* AtlMuon::fgAuthorNames[] = {
    "Invalid", "HighPt", "LowPt", "MediumPt",
    "MuonBoySP", "MuonBoy", "STACO", "MuTag",
    "Unused", "Unused2", // this should be amended!
    "MOORE", "MuidSA", "MuidCo", "MuGirl",
    "CaloMuonId", "MuGirlLowBeta", "CaloTag",
    "CaloLikelihood", "MuTagIMO", "MuCombRefit", "MuExtrapolateIP"
};
const char* AtlMuon::fgQualityNames[] = {
    "Invalid", "Loose", "Medium", "Tight"
};

//____________________________________________________________________

AtlMuon::AtlMuon() {
    //
    // Default constructor
    //
    
    // Initialize Chi2 and No. of degrees of freedom for combined
    // Muons. Values will be set for combined muons during the
    // conversion. For not combined muons, please look for the chi2
    // and NDof from the originally IDTrack if existing
    fMatchingChi2 = 0.;      
    fMatchingNDoF = 0;      
    fIDTrack = new TRef;
    fMETrack = new TRef;
    fTrackRefit = new HepTrackHelix;
}

//____________________________________________________________________

AtlMuon::AtlMuon(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		 Float_t E, Bool_t IsMuPlus, Float_t EtCone10,
		 Float_t EtCone20, Float_t EtCone30, Float_t EtCone40, 
		 Int_t NtrkCone10, Int_t NtrkCone20, Int_t NtrkCone30,
		 Int_t NtrkCone40, Float_t PtCone10, Float_t PtCone20,
		 Float_t PtCone30, Float_t PtCone40, EAuthor Author,
		 EQuality Quality, 
		 Float_t MatchingChi2, Int_t MatchingNDoF,
		 Bool_t IsCombinedMuon,
		 TLorentzVector PMuonSpecExtrapol, 
		 Int_t MuonSpecExtrapolCharge) : 
  HepMuon(Id, Px, Py, Pz, E, IsMuPlus), fAuthor(Author), fQuality(Quality),
  fEtCone10(EtCone10), fEtCone20(EtCone20), fEtCone30(EtCone30), fEtCone40(EtCone40),
  fNtrkCone10(NtrkCone10), fNtrkCone20(NtrkCone20), fNtrkCone30(NtrkCone30),
  fNtrkCone40(NtrkCone40), fPtCone10(PtCone10), fPtCone20(PtCone20),
  fPtCone30(PtCone30), fPtCone40(PtCone40), 
  fMatchingChi2(MatchingChi2), fMatchingNDoF(MatchingNDoF),
  fIsCombinedMuon(IsCombinedMuon),
  fPMuonSpecExtrapol(PMuonSpecExtrapol), 
  fMuonSpecExtrapolCharge(MuonSpecExtrapolCharge) { 
  //
  // Normal constructor
  //
  fTrackRefit = new HepTrackHelix;
  fIDTrack = new TRef;
  fMETrack = new TRef;
}

//____________________________________________________________________

AtlMuon::~AtlMuon() {
    //
    // Default destructor
    //
    delete fTrackRefit;
    delete fIDTrack; fIDTrack=0;
    delete fMETrack; fMETrack=0;
}

//____________________________________________________________________

void AtlMuon::Clear(Option_t *option) {
    //
    // Clear object
    //
    HepMuon::Clear(option);
    AtlMETWeights::Clear(option);
    AtlTriggerMatch::Clear(option);

    fAuthor = kInvalidAuthor;
    fQuality = kInvalid;
    fEtCone10     = 0.;
    fEtCone20     = 0.;
    fEtCone30     = 0.;
    fEtCone40     = 0.;
    fPtCone10     = 0.;
    fPtCone20     = 0.;
    fPtCone30     = 0.;
    fPtCone40     = 0.;
    fNtrkCone10   = 0;
    fNtrkCone20   = 0;
    fNtrkCone30   = 0;
    fNtrkCone40   = 0;
    fMatchingChi2 = 0.;
    fMatchingNDoF = 0;
    fPMuonSpecExtrapol.SetPxPyPzE(0, 0, 1., 0); // unit-vector in Pz direction
    fMuonSpecExtrapolCharge = 0;
    delete fIDTrack; fIDTrack = 0;    
    delete fMETrack; fMETrack = 0;
    delete fTrackRefit; fTrackRefit = 0;
    fMCTruthClassifier.Clear(option);
}

//_____________________________________________________________

void AtlMuon::Print(Option_t *option) {
    //
    // Print muon information
    //
    // Note that in the standard printout only the main authors are
    // displayed. For a detailed view use the option "author".
    //
    // Options available:
    //   "nohead" - No header containing the variable names is
    //              displayed. Useful when printing a whole
    //              table for a list of muons
    //   "author" - Print the reconstruction algorithm(s) the
    //              muon candidate had been found by
    //
    TString opt = option;
    opt.ToLower();
    
    // Print header
    if ( !opt.Contains("nohead") ) PrintHeader();
    
    // Print
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.precision(3);
    cout.width(4);  cout << fId;
    cout.width(3);  cout << (( IsPositive() ) ? "+" : "-");
    cout.width(10); cout << Pt();
    cout.width(9); cout << E();
    cout.precision(6);
    cout.width(11); cout << M("REC");
    cout.precision(3);
    cout.width(9); cout << Theta()*180/TMath::Pi();
    cout.width(9); cout << Phi()*180/TMath::Pi();
    cout.width(7); cout << Eta();
    cout.width(9); cout << GetEtCone20();
    cout.width(11); cout << GetNtrkCone20();
    cout.precision(1);
    cout.width(5); cout << GetMatchingChi2();
    cout << " /";
    cout.width(2); cout << GetMatchingNDoF();
    cout << "  ";
    if ( HasInvalidQuality() ) {
	cout << "i";
    } else {
	cout << "-";
    }
    if ( IsLoose() ) {
	cout << "l";
    } else {
	cout << "-";
    }
    if ( IsMedium() ) {
	cout << "m";
    } else {
	cout << "-";
    }
    if ( IsTight() ) {
	cout << "t";
    } else {
	cout << "-";
    }
    cout << " ";
    if ( HasInvalidAuthor() ) {
	cout << "i";
    } else {
	cout << "-";
    }
    if ( IsSTACO() ) {
	cout << "s";
    } else {
	cout << "-";
    }
    if ( IsMuidCo() ) {
	cout << "c";
    } else {
	cout << "-";
    }
    if ( IsMuGirl() ) {
	cout << "g";
    } else {
	cout << "-";
    }
    if ( IsMuonBoy() ) {
	cout << "b";
    } else {
	cout << "-";
    }
    if ( IsMuidSA() ) {
	cout << "a";
    } else {
	cout << "-";
    }
    cout << endl;

    // Print author (if wanted)
    if ( opt.Contains("author") ) PrintAuthor();
    
    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void AtlMuon::PrintHeader() {
    //
    // Print information header
    //
    cout << "--------------------------------------------------------------------------------------------------------"
	 << endl
	 << " Id  Chg Pt (GeV)  E (GeV) Minv (GeV)    Theta      Phi    Eta EtCone20 NtrkCone20 Chi2/NDoF Qual Author"
	 << endl
	 << "--------------------------------------------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void AtlMuon::PrintFooter() {
    //
    // Print footer
    //
    cout << "   i=invalid l=loose m=medium t=tight s=staco c=muid-comb g=mu_girl b=mu-boy a=muid-sa"
	 << endl
	 << "--------------------------------------------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void AtlMuon::PrintAuthor() const {
    //
    // Print names of the reconstruction algorithm(s) used
    //
    cout << endl
	 << "==========================" << endl
	 << " Muon reconstruction" << endl
	 << "--------------------------" << endl;
    for ( Int_t i = 0; i < fgNAuthors; i++ ) {
	if ( i != 8 && i != 9 ) { // these bits are not used
	    Bool_t set = fAuthor & 1<<i; 
	    cout << setw(20) << setiosflags(ios::left)
		 << fgAuthorNames[i] << set << endl;
	}
    }
    cout << "==========================" << endl << endl;
}

//____________________________________________________________________

TString* AtlMuon::NameOfAuthor() {
    //
    // Return name(s) of reconstruction algorithm used. The user is
    // responsible for deleting the returned TString object.
    //
    return NameOfAuthor(fAuthor);
}

//____________________________________________________________________

TString* AtlMuon::NameOfAuthor(AtlMuon::EAuthor author) {
    //
    // Return name(s) of given reconstruction algorithm(s). The user
    // is responsible for deleting the returned TString object.
    //
    TString *author_str = new TString("");
    for ( Int_t i = 0; i < fgNAuthors; i++ ) {
	if ( author & (0x1<<i) ) {
	    if ( author_str->Length() > 0 ) author_str->Append(",");
	    author_str->Append(fgAuthorNames[i]);
	}
    }
    if ( author_str->Length() == 0 ) author_str->Append(" --- ");
    return author_str;
}

//____________________________________________________________________

TString* AtlMuon::NameOfQuality() {
    //
    // Return name(s) of selection used. The user is responsible for
    // deleting the returned TString object.
    //
    return NameOfQuality(fQuality);
}
    
//____________________________________________________________________

TString* AtlMuon::NameOfQuality(AtlMuon::EQuality quality) {
    //
    // Return name(s) of given quality. The user is responsible for
    // deleting the returned TString object.
    //
    TString *quality_str = new TString("");
    for ( Int_t i = 0; i < fgNQuality; i++ ) {
	if ( quality & (0x1<<i) ) {
	    if ( quality_str->Length() > 0 ) quality_str->Append(",");
	    quality_str->Append(fgQualityNames[i]);
	}
    }
    if ( quality_str->Length() == 0 ) quality_str->Append(" --- ");
    return quality_str;
}



