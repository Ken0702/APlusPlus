//____________________________________________________________________
//
// Atlas B-tagger class
//
// Basically the tag weight and a validity flag is stored for each
// tagger. For some taggers some additinoal information is stored. In
// detail:
//
//   * Impact paramter 2d/3d
//       - number of tracks
//       - b/u likelihoods
//
//   * SV1 and SV2
//       - b/u likelihoods
//
//   * SVInfoPlus and SV0InfoPlus
//       - invariant mass at sec. vertex
//       - number of 2-track vertices
//       - flag for good vertex
//       - total number of tracks
//       - number of good tracks at vertex
//       - number of good tracks in jet
//       - energy fraction sec. vtx/jet of charged tracks
//
//   * JetFitterTag
//       - b/u likelihoods
//       - number of vertices with more than one track
//       - number of single-track vertices
//       - number of tracks at vertices (with more than one track - sum)
//       - invariant mass of the tracks fitted to the vertices with at least 2 tracks
//       - energy fraction sec. vtx/jet of charged tracks
//       - 3-dim significance of all vertices with more than one track or -if not there-
//       - delta(phi) between sum of all momenta at vertices and the fitted B-meson flight direction 
//       - delta(eta) between sum of all momenta at vertices and the fitted B-meson flight direction
//
//
// Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#include "AtlBTag.h"
#include <TString.h>
#include <iostream>

using namespace std;

static const char* fgTaggerNames[AtlBTag::kNumTaggers] = {
    "IP2D",
    "IP3D",
    "SVInfoPlus",
    "SV1",
    "SV2",
    "SV0",
    "SV0InfoPlus",
    "SoftMuonTag",
    "JetProb",
    "JetFitterTagNN",
    "JetFitterCOMBNN",
    "Baseline",
    "MV1",
    "MV1c",
    "MV2c20",
    "MV2c10_60",
    "MV2c10_70",
    "MV2c10_77",
    "MV2c10_85",
	"DL1r_85",
	"DL1r_77",
	"DL1r_70",
	"DL1r_60",
};

//____________________________________________________________________

AtlBTag::AtlBTag() {
    //
    // Default constructor
    //
    fTagger = kInvalidTagger;
}

//____________________________________________________________________

AtlBTag::AtlBTag(enum ETagger tagger, Double_t weight, Bool_t Valid)
    : fTagger(tagger), fWeight(weight), fValid(Valid) {
    //
    // Normal constructor
    //
    fNTrk = 0;
    fPb = 0.;
    fPu = 0.;
    fSecVtxMass = 0.;
    fN1TrkSecVtx = 0;
    fN2TrkSecVtx = 0;
    fNManyTrkSecVtx = 0;
    fGoodSecVtx = kFALSE;
    fNGoodVtxTrk = 0;
    fNGoodJetTrk = 0;
    fEFrac = 0.;
    fSignificance3d = 0.;
    fDeltaPhi = 0.;
    fDeltaEta = 0.;
}

//____________________________________________________________________

AtlBTag::~AtlBTag() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlBTag::Clear(Option_t *option) {
    //
    // Clear this object
    //
    fTagger = kInvalidTagger;
    fWeight = 0.;
    fValid = kFALSE;

    fNTrk = 0;
    fPb = 0.;
    fPu = 0.;
    fSecVtxMass = 0.;
    fN1TrkSecVtx = 0;
    fN2TrkSecVtx = 0;
    fNManyTrkSecVtx = 0;
    fGoodSecVtx = kFALSE;
    fNGoodVtxTrk = 0;
    fNGoodJetTrk = 0;
    fEFrac = 0.;
    fSignificance3d = 0.;
    fDeltaPhi = 0.;
    fDeltaEta = 0.;
}

//____________________________________________________________________

const char* AtlBTag::GetTaggerName(ETagger tagger) {
    //
    // Get human-readable name of tagger
    //
    return fgTaggerNames[tagger];
}

//____________________________________________________________________

AtlBTag::ETagger AtlBTag::GetTaggerByName(const char *name) {
    //
    // Get enum value for full name of tagger
    //
    // In case no tagger was found for the given name the value
    // kInvalidTagger is returned
    //
    for ( Int_t i = 0; i < kNumTaggers; i++) {
	if ( strcasecmp(fgTaggerNames[i], name ) == 0 )
	    return (ETagger)i;
    }
    return kInvalidTagger;
}

//____________________________________________________________________

void AtlBTag::Print(Option_t *option) const {
    //
    // Print object information
    //
    if ( fTagger == kInvalidTagger ) return;
    TString opt = option;
    opt.ToLower();

    // Print header
    if ( !opt.Contains("nohead") ) PrintHeader();

    // Print
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.precision(3);
    cout.width(20); cout << GetName();
    cout.width(12); cout << fWeight;
    cout.width(6);  cout << IsValid();
    if ( fTagger == kIP2d || fTagger == kIP3d
	 || fTagger == kSV1 || fTagger == kSV2 ) {
	cout.width(7); cout << fPb;
	cout.width(7); cout << fPu;
    } else {
	cout.width(7); cout << "---";
	cout.width(7); cout << "---";
    }
    cout.width(13); cout << "---";

    if ( fTagger == kIP2d || fTagger == kIP3d ) {
	cout.width(6); cout << fNTrk;
    } else {
	cout.width(6); cout << "--";
    }
    cout << endl;

    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void AtlBTag::PrintHeader() {
    //
    // Print information header
    //
    cout << "-----------------------------------------------------------------------" << endl
	 << "B-Tagger                  Weight Valid     Pb     Pu   Mass (GeV)  NTrk" << endl
	 << "-----------------------------------------------------------------------" << endl;
}

//____________________________________________________________________

void AtlBTag::PrintFooter() {
    //
    // Print footer
    //
    cout << "-----------------------------------------------------------------------" << endl;
}
