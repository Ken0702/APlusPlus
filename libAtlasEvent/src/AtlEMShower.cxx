//____________________________________________________________________
//
// Helper class containing EM shower properties
// Used by AtlElectron and AtlPhoton classes
//
// IsEm flag:
// ==========
// The IsEM flag is used to identify electrons
// or photons by means of selection cuts (see the references below for
// the exact selection criteria). In case a cut has NOT been passed a
// bit corresponding to this cut in the IsEM bit pattern is set.
//
// The bit pattern definitions of the IsEM flag are taken from
// BEGIN_HTML<a href="http://atlas-computing.web.cern.ch/atlas-computing/links/AtlasEventDir/Reconstruction/egamma/egammaEvent/egammaEvent/egammaPIDdefs.h">http://atlas-computing.web.cern.ch/atlas-computing/links/AtlasEventDir/Reconstruction/egamma/egammaEvent/egammaEvent/egammaPIDdefs.h</a>END_HTML
//
// See also the EgammaAOD page at
// BEGIN_HTML<a href="https://twiki.cern.ch/twiki/bin/view/AtlasProtected/EgammaAOD">https://twiki.cern.ch/twiki/bin/view/AtlasProtected/EgammaAOD</a>END_HTML
// as well as the TWiki page of the Egamma working group:
// BEGIN_HTML<a href="https://twiki.cern.ch/twiki/bin/view/AtlasProtected/ElectronGamma">https://twiki.cern.ch/twiki/bin/view/AtlasProtected/ElectronGamma</a>END_HTML
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEMShower
#include <AtlEMShower.h>
#endif
#include <TSystem.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlEMShower);
#endif

const char* AtlEMShower::fgIsEMNames[] = {
    "Invalid",
    "el_loose", "el_fwdloose", "el_medium",
    "el_tight", "el_fwdtight", "el_tightiso",
    "ph_loose", "ph_tight", "ph_tightiso",
    "el_mediumiso", "el_medium++", "el_tight++"
};
const char* AtlEMShower::fgAuthorNames[] = {
    "Invalid", "el_highPt", "el_soft", "el_fwd", "photon", "conv_photon"
};

//____________________________________________________________________

AtlEMShower::AtlEMShower() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlEMShower::AtlEMShower(EAuthor Author, 
			 UInt_t IsEMBitField,
			 EIsEM IsEM, Float_t EMWeight, Float_t BkgWeight,
			 UInt_t OQFlag,
    			 TLorentzVector PCluster) :
    fAuthor(Author), 
    fIsEMBitField(IsEMBitField),
    fIsEM(IsEM), fEMWeight(EMWeight), fBkgWeight(BkgWeight), 
    fOQFlag(OQFlag), 
    fPCluster(PCluster){
    //
    // Normal constructor
    //
    if ( fAuthor == 0 ) {
	cout << "<AtlEMShower:AtlEMShower> Author variable not set. Abort!"
	     << endl;
	gSystem->Abort(0);
    }
}

//____________________________________________________________________

AtlEMShower::~AtlEMShower() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlEMShower::Clear(Option_t *option) {
    //
    // Clear this object
    //

    AtlMETWeights::Clear(option);

    fAuthor       = kInvalidAuthor;
    fEtCone20     = 0.;
    fEtCone30     = 0.;
    fEtCone40     = 0.;
    fEtCone45     = 0.;
    fEtCone20_pt_corrected = 0.;
    fEtCone30_pt_corrected = 0.;
    fEtCone40_pt_corrected = 0.;
    fEtCone20_pt_corrected_error = 0; 
    fEtCone30_pt_corrected_error = 0;
    fEtCone40_pt_corrected_error = 0;
    fEtCone20_ED_corrected = 0.;
    fEtCone30_ED_corrected = 0.;
    fEtCone40_ED_corrected = 0.;
    fEtCone20_NPV_corrected = 0.;
    fEtCone30_NPV_corrected = 0.;
    fEtCone40_NPV_corrected = 0.;
    fEtCone20_corrected = 0.;
    fEtCone30_corrected = 0.;
    fEtCone40_corrected = 0.;
    fEtCone20_PtNPV_corrected = 0.;
    fEtCone30_PtNPV_corrected = 0.;
    fEtCone40_PtNPV_corrected = 0.;
    fEMWeight     = 0.;
    fBkgWeight    = 0.;
    fIsEMBitField = 0;
    fIsEM         = kInvalid;
    fOQFlag       = 0;
}

//____________________________________________________________________

TString* AtlEMShower::NameOfAuthor(EAuthor eg_Author) {
    //
    // Return name(s) of reconstruction algorithm used
    //
    // !!! The user is responsible for deleting this string     !!!
    // !!! when it is not needed anymore to avoid memory leaks. !!!
    TString *author = new TString("");
    for ( Int_t i = 0; i < fgNAuthors; i++ ) {
	if ( eg_Author & (0x1<<i) ) {
	    if ( author->Length() > 0 ) {
		author->Append(",");
	    }
	    author->Append(fgAuthorNames[i]);
	}
    }
    if ( author->Length() == 0 ) author->Append(" --- ");
    
    return author;
}

//____________________________________________________________________

TString* AtlEMShower::NameOfIsEM(EIsEM eg_IsEM) {
    //
    // Return name(s) of IsEM selection used
    //
    // !!! The user is responsible for deleting this string     !!!
    // !!! when it is not needed anymore to avoid memory leaks. !!!

    TString *isem = new TString("");
    for ( Int_t i = 0; i < fgNIsEM; i++ ) {
	if ( eg_IsEM & (0x1<<i) ) {
	    if ( isem->Length() > 0 ) isem->Append(",");
	    isem->Append(fgIsEMNames[i]);
	}
    }
    if ( isem->Length() == 0 ) isem->Append(" --- ");
    return isem;
}

//____________________________________________________________________

TString* AtlEMShower::GetAuthorName() {
    //
    // Return name(s) of reconstruction algorithm used
    //
    // !!! The user is responsible for deleting this string     !!!
    // !!! when it is not needed anymore to avoid memory leaks. !!!
    return NameOfAuthor(fAuthor);
}

//____________________________________________________________________

TString* AtlEMShower::GetIsEMName() {
    //
    // Return name(s) of IsEM selection used
    //
    // !!! The user is responsible for deleting this string     !!!
    // !!! when it is not needed anymore to avoid memory leaks. !!!
    return NameOfIsEM(fIsEM);
}

//____________________________________________________________________

Float_t AtlEMShower::ClusterDeltaR(HepJet *jet) {
    //
    // Compute distance in eta-phi plane between cluster of this
    // electron and the given jet
    //
    Float_t dEta = ClusterEta() - jet->Eta();
    Float_t dPhi = ClusterPhi() - jet->Phi();
    return TMath::Sqrt(dEta*dEta + dPhi*dPhi);
}
