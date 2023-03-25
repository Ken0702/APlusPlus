//____________________________________________________________________
//
// Atlas tau class
// 
// For details on the Tau reconstruction look at the Tau working group wiki page
// BEGIN_HTML
// <a href="https://twiki.cern.ch/twiki/bin/view/AtlasProtected/TauWG">
// https://twiki.cern.ch/twiki/bin/view/AtlasProtected/TauWG</a>
// END_HTML
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTau
#include <AtlTau.h>
#endif
#include <TSystem.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlTau);
#endif

const Int_t AtlTau::fgNAuthors = 3;
const char* AtlTau::fgAuthorNames[] = {
    "Invalid", "TauRec", "Tau1P3P" 
};
const Int_t AtlTau::fgNTauFlags = 20;
const char* AtlTau::fgTauFlagNames[] = {
    "Invalid", "TauCutLoose", "TauCutMedium", "TauCutTight",
    "ElectronVeto", "MuonVeto", "TauLlhTight", "TauLlhMedium",
    "TauLlhLoose", "TauRecOld", "Tau1P3POld", "TauCutSafeLoose",
    "TauCutSafeMedium", "TauCutSafeTight", "TauCutSafeCaloLoose",
    "TauCutSafeCaloMedium", "TauCutSafeCaloTight", "ElectronVetoLoose",
    "ElectronVetoMedium", "ElectronVetoTight"
};

//____________________________________________________________________

AtlTau::AtlTau() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlTau::AtlTau(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
	       Float_t E, Bool_t IsTauPlus, 
	       EAuthor Author, ETauFlag TauFlag):
    HepTau(Id, Px, Py, Pz, E, IsTauPlus), fAuthor(Author),
    fTauFlag(TauFlag) {
    //
    // Normal constructor
    //
  
}

//____________________________________________________________________

AtlTau::~AtlTau() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlTau::Clear(Option_t *option) {
    //
    // Clear object
    //
    HepTau::Clear(option);
    AtlTriggerMatch::Clear(option);
    fAuthor  = kInvalidAuthor;
    fTauFlag = kInvalidTauFlag;
}

//_____________________________________________________________

void AtlTau::Print(Option_t *option) {
    //
    // Print tau information
    //
    // Options available:
    //   "nohead" - No header containing the variable names is
    //              displayed. Useful when printing a whole table
    //              for a list of taus
    //   "flag"   - Print settings of the tau particle id flag
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
    cout.width(12); cout << Pt();
    cout.width(12); cout << E();
    cout.precision(6);
    cout.width(12); cout << M("REC");
    cout.precision(3);
    cout.width(12);
    cout.width(12); cout << Theta()*180/TMath::Pi();
    cout.width(12); cout << Phi()*180/TMath::Pi();
    cout.width(12); cout << Eta();
    cout.width(14);
    cout << NameOfAuthor();
    cout << endl;

    // Print flag (if wanted)
    if ( opt.Contains("flag") ) PrintTauFlag();
    
    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void AtlTau::PrintHeader() {
    //
    // Print information header
    //
    cout << "------------------------------------------------------------------------------------------------"
	 << endl
	 << " Id  Chg        Pt        E         Minv         Theta         Phi         Eta        Author    "
	 << endl
	 << "------------------------------------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void AtlTau::PrintFooter() {
    //
    // Print footer
    //
    cout << "------------------------------------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

const char* AtlTau::NameOfAuthor() {
  //                                                                                                                                                                           
  // Return name(s) of reconstruction algorithm used                                                                                                                           
  //                                                                                                                                                                           
  TString author("");
  if ( IsInvalidAuthor() ) {
    if ( author.Length() > 0 ) author.Append(",");
    author.Append("invalid");
  }
  if ( IsTauRec() ) {
    if ( author.Length() > 0 ) author.Append(",");
    author.Append("rec");
  }
  if ( IsTau1P3P() ) {
    if ( author.Length() > 0 ) author.Append(",");
    author.Append("1P3P");
  }

  return author.Data();
}


//____________________________________________________________________

void AtlTau::PrintTauFlag() {
    //
    // Print reconstruction flags
    //
    cout << endl
	 << "==========================" << endl
	 << " Tau reconstruction" << endl
	 << "--------------------------" << endl
	 << "Invalid                 " << IsInvalidTauFlag() << endl
	 << "MuonVeto                " << IsMuonVeto() << endl
	 << "ElectronVetoLoose       " << IsElectronVetoLoose() << endl
	 << "ElectronVetoMedium      " << IsElectronVetoMedium() << endl
	 << "ElectronVetoTight       " << IsElectronVetoTight() << endl
	 << "JetVetoLoose            " << IsJetVetoLoose() << endl
	 << "JetVetoMedium           " << IsJetVetoMedium() << endl
	 << "JetVetoTight            " << IsJetVetoTight() << endl
	 << "==========================" << endl << endl;
}
