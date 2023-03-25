//____________________________________________________________________
//
// DATA or MC sample used for an A++ analysis
// 
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlSample
#include <AtlSample.h>
#endif
#include <TString.h>
#include <TClass.h>
#include <TList.h>
#include <TSystem.h>
#include <iostream>

using namespace std;

static const char* fgTypeNames[AtlSample::kUndefined+1] = {
    	"DATA EGamma",
	"DATA Muon",
	"DATA TOPQ1",
	"DATA TOPQ2",
	"DATA TOPQ3",
	"single-top t-channel, e+nu",
	"single-top t-channel, mu+nu",
	"single-top t-channel, tau+nu",
	"single-top s-channel, e+nu",
	"single-top s-channel, mu+nu",
	"single-top s-channel, tau+nu",
	"single-top t-channel, inclusive",
	"single-top t-channel top, inclusive",
	"single-top t-channel antitop, inclusive",
	"single-top s-channel, inclusive",
	"single-top Wt",
	"single-top tZj",
	"ttbar",
	"ttbar+W",
	"ttbar+Z",
	"W+jetsLF Np0, e+nu",
	"W+jetsLF Np0 HFOR 0, e+nu",
	"W+jetsLF Np0 HFOR 1, e+nu",
	"W+jetsLF Np0 HFOR 2, e+nu",
	"W+jetsLF Np0 HFOR 3, e+nu",
	"W+jetsLF Np1, e+nu",
	"W+jetsLF Np1 HFOR 0, e+nu",
	"W+jetsLF Np1 HFOR 1, e+nu",
	"W+jetsLF Np1 HFOR 2, e+nu",
	"W+jetsLF Np1 HFOR 3, e+nu",
	"W+jetsLF Np2, e+nu",
	"W+jetsLF Np2 HFOR 0, e+nu",
	"W+jetsLF Np2 HFOR 1, e+nu",
	"W+jetsLF Np2 HFOR 2, e+nu",
	"W+jetsLF Np2 HFOR 3, e+nu",
	"W+jetsLF Np3, e+nu",
	"W+jetsLF Np3 HFOR 0, e+nu",
	"W+jetsLF Np3 HFOR 1, e+nu",
	"W+jetsLF Np3 HFOR 2, e+nu",
	"W+jetsLF Np3 HFOR 3, e+nu",
	"W+jetsLF Np4, e+nu",
	"W+jetsLF Np4 HFOR 0, e+nu",
	"W+jetsLF Np4 HFOR 1, e+nu",
	"W+jetsLF Np4 HFOR 2, e+nu",
	"W+jetsLF Np4 HFOR 3, e+nu",
	"W+jetsLF Np5, e+nu",
	"W+jetsLF Np5 HFOR 0, e+nu",
	"W+jetsLF Np5 HFOR 1, e+nu",
	"W+jetsLF Np5 HFOR 2, e+nu",
	"W+jetsLF Np5 HFOR 3, e+nu",
	"W+jetsLF Np0, mu+nu",
	"W+jetsLF Np0 HFOR 0, mu+nu",
	"W+jetsLF Np0 HFOR 1, mu+nu",
	"W+jetsLF Np0 HFOR 2, mu+nu",
	"W+jetsLF Np0 HFOR 3, mu+nu",
	"W+jetsLF Np1, mu+nu",
	"W+jetsLF Np1 HFOR 0, mu+nu",
	"W+jetsLF Np1 HFOR 1, mu+nu",
	"W+jetsLF Np1 HFOR 2, mu+nu",
	"W+jetsLF Np1 HFOR 3, mu+nu",
	"W+jetsLF Np2, mu+nu",
	"W+jetsLF Np2 HFOR 0, mu+nu",
	"W+jetsLF Np2 HFOR 1, mu+nu",
	"W+jetsLF Np2 HFOR 2, mu+nu",
	"W+jetsLF Np2 HFOR 3, mu+nu",
	"W+jetsLF Np3, mu+nu",
	"W+jetsLF Np3 HFOR 0, mu+nu",
	"W+jetsLF Np3 HFOR 1, mu+nu",
	"W+jetsLF Np3 HFOR 2, mu+nu",
	"W+jetsLF Np3 HFOR 3, mu+nu",
	"W+jetsLF Np4, mu+nu",
	"W+jetsLF Np4 HFOR 0, mu+nu",
	"W+jetsLF Np4 HFOR 1, mu+nu",
	"W+jetsLF Np4 HFOR 2, mu+nu",
	"W+jetsLF Np4 HFOR 3, mu+nu",
	"W+jetsLF Np5, mu+nu",
	"W+jetsLF Np5 HFOR 0, mu+nu",
	"W+jetsLF Np5 HFOR 1, mu+nu",
	"W+jetsLF Np5 HFOR 2, mu+nu",
	"W+jetsLF Np5 HFOR 3, mu+nu",
	"W+jetsLF Np0, tau+nu",
	"W+jetsLF Np0 HFOR 0, tau+nu",
	"W+jetsLF Np0 HFOR 1, tau+nu",
	"W+jetsLF Np0 HFOR 2, tau+nu",
	"W+jetsLF Np0 HFOR 3, tau+nu",
	"W+jetsLF Np1, tau+nu",
	"W+jetsLF Np1 HFOR 0, tau+nu",
	"W+jetsLF Np1 HFOR 1, tau+nu",
	"W+jetsLF Np1 HFOR 2, tau+nu",
	"W+jetsLF Np1 HFOR 3, tau+nu",
	"W+jetsLF Np2, tau+nu",
	"W+jetsLF Np2 HFOR 0, tau+nu",
	"W+jetsLF Np2 HFOR 1, tau+nu",
	"W+jetsLF Np2 HFOR 2, tau+nu",
	"W+jetsLF Np2 HFOR 3, tau+nu",
	"W+jetsLF Np3, tau+nu",
	"W+jetsLF Np3 HFOR 0, tau+nu",
	"W+jetsLF Np3 HFOR 1, tau+nu",
	"W+jetsLF Np3 HFOR 2, tau+nu",
	"W+jetsLF Np3 HFOR 3, tau+nu",
	"W+jetsLF Np4, tau+nu",
	"W+jetsLF Np4 HFOR 0, tau+nu",
	"W+jetsLF Np4 HFOR 1, tau+nu",
	"W+jetsLF Np4 HFOR 2, tau+nu",
	"W+jetsLF Np4 HFOR 3, tau+nu",
	"W+jetsLF Np5, tau+nu",
	"W+jetsLF Np5 HFOR 0, tau+nu",
	"W+jetsLF Np5 HFOR 1, tau+nu",
	"W+jetsLF Np5 HFOR 2, tau+nu",
	"W+jetsLF Np5 HFOR 3, tau+nu",
	"W+c Np0",
	"W+c Np0 HFOR 0",
	"W+c Np0 HFOR 1",
	"W+c Np0 HFOR 2",
	"W+c Np0 HFOR 3",
	"W+c Np1",
	"W+c Np1 HFOR 0",
	"W+c Np1 HFOR 1",
	"W+c Np1 HFOR 2",
	"W+c Np1 HFOR 3",
	"W+c Np2",
	"W+c Np2 HFOR 0",
	"W+c Np2 HFOR 1",
	"W+c Np2 HFOR 2",
	"W+c Np2 HFOR 3",
	"W+c Np3",
	"W+c Np3 HFOR 0",
	"W+c Np3 HFOR 1",
	"W+c Np3 HFOR 2",
	"W+c Np3 HFOR 3",
	"W+c Np4",
	"W+c Np4 HFOR 0",
	"W+c Np4 HFOR 1",
	"W+c Np4 HFOR 2",
	"W+c Np4 HFOR 3",
	"W+cc Np0",
	"W+cc Np0 HFOR 0",
	"W+cc Np0 HFOR 1",
	"W+cc Np0 HFOR 2",
	"W+cc Np0 HFOR 3",
	"W+cc Np1",
	"W+cc Np1 HFOR 0",
	"W+cc Np1 HFOR 1",
	"W+cc Np1 HFOR 2",
	"W+cc Np1 HFOR 3",
	"W+cc Np2",
	"W+cc Np2 HFOR 0",
	"W+cc Np2 HFOR 1",
	"W+cc Np2 HFOR 2",
	"W+cc Np2 HFOR 3",
	"W+cc Np3",
	"W+cc Np3 HFOR 0",
	"W+cc Np3 HFOR 1",
	"W+cc Np3 HFOR 2",
	"W+cc Np3 HFOR 3",
	"W+bb Np0",
	"W+bb Np0 HFOR 0",
	"W+bb Np0 HFOR 1",
	"W+bb Np0 HFOR 2",
	"W+bb Np0 HFOR 3",
	"W+bb Np1",
	"W+bb Np1 HFOR 0",
	"W+bb Np1 HFOR 1",
	"W+bb Np1 HFOR 2",
	"W+bb Np1 HFOR 3",
	"W+bb Np2",
	"W+bb Np2 HFOR 0",
	"W+bb Np2 HFOR 1",
	"W+bb Np2 HFOR 2",
	"W+bb Np2 HFOR 3",
	"W+bb Np3",
	"W+bb Np3 HFOR 0",
	"W+bb Np3 HFOR 1",
	"W+bb Np3 HFOR 2",
	"W+bb Np3 HFOR 3",
	"Wenu (incl.)",
	"Wenu+B-Hadron (incl.)",
	"Wenu+C-Hadron (incl.)",
	"Wenu+light hadrons (incl.)"
	"Wmunu (incl.)",
	"Wmunu+B-Hadron (incl.)",
	"Wmunu+C-Hadron (incl.)",
	"Wmunu+light hadrons (incl.)"
	"Wtaunu (incl.)",
	"Wtaunu+B-Hadron (incl.)",
	"Wtaunu+C-Hadron (incl.)",
	"Wtaunu+light hadrons (incl.)",
	"Z+jets Np0, ee",
	"Z+jets Np1, ee",
	"Z+jets Np2, ee",
	"Z+jets Np3, ee",
	"Z+jets Np4, ee",
	"Z+jets Np5, ee",
	"Z+jets Np0, mumu",
	"Z+jets Np1, mumu",
	"Z+jets Np2, mumu",
	"Z+jets Np3, mumu",
	"Z+jets Np4, mumu",
	"Z+jets Np5, mumu",
	"Z+jets Np0, tautau",
	"Z+jets Np1, tautau",
	"Z+jets Np2, tautau",
	"Z+jets Np3, tautau",
	"Z+jets Np4, tautau",
	"Z+jets Np5, tautau",
	"Z+cc Np0, ee",
	"Z+cc Np1, ee",
	"Z+cc Np2, ee",
	"Z+cc Np3incl, ee",
	"Z+cc Np0, mumu",
	"Z+cc Np1, mumu",
	"Z+cc Np2, mumu",
	"Z+cc Np3incl, mumu",
	"Z+cc Np0, tautau",
	"Z+cc Np1, tautau",
	"Z+cc Np2, tautau",
	"Z+cc Np3incl, tautau",
	"Z+bb Np0, ee",
	"Z+bb Np1, ee",
	"Z+bb Np2, ee",
	"Z+bb Np3incl, ee",
	"Z+bb Np0, mumu",
	"Z+bb Np1, mumu",
	"Z+bb Np2, mumu",
	"Z+bb Np3incl, mumu",
	"Z+bb Np0, tautau",
	"Z+bb Np1, tautau",
	"Z+bb Np2, tautau",
	"Z+bb Np3incl, tautau",
	"Z+jets incl, ee",
	"Z+jets incl B-Hadron, ee",
	"Z+jets incl C-Hadron, ee",
	"Z+jets incl light hadrons, ee",
	"Z+jets incl, mumu",
	"Z+jets incl B-Hadron, mumu",
	"Z+jets incl C-Hadron, mumu",
	"Z+jets incl light hadrons, mumu",
	"Z+jets incl, tautau",
	"Z+jets incl B-Hadron, tautau",
	"Z+jets incl C-Hadron, tautau",
	"Z+jets incl light hadrons, tautau",
	"Z+jets incl B-Hadron, nunu",
	"Z+jets incl C-Hadron, nunu",
	"Z+jets incl light hadrons, nunu",
	"DiBoson WW",
	"DiBoson WZ",
	"DiBoson ZZ",
	"WW > e nu qq",
	"WZ > e nu qq",
	"WW > mu nu qq",
	"WZ > mu nu qq",
	"WW > tau nu qq",
	"WZ > tau nu qq",
	"ZW > e e qq",
	"ZZ > e e qq",
	"ZW > mu mu qq",
	"ZZ > mu mu qq",
	"ZW > tau tau qq",
	"ZZ > tau tau qq",
	"W Higgs, lnubb",
	"QCD Matrix Method",
	"QCD Matrix Method Tight",
	"QCD Matrix Method Loose",
	"QCD Matrix Method EGamma",
	"QCD Matrix Method Muon",
	"QCD JetElectron Method EGamma",
	"QCD JetElectron Method Muon",
	"QCD AntiMuon Method",
	"QCD JetLepton Method",
	"FCNC utZ K=0.01,LH",
	"FCNC ctZ K=0.01,LH",
	"Undefined"
};

static const char* fgCategoryNames[13] = {
    "SgTop", "Ttbar", "WjetsLight", "WjetsHeavy",
    "W_hfor0", "W_hfor1", "W_hfor2", "W_hfor3",
    "Zjets", "Diboson", "Other", "Wjets", "QCD"
};

#ifndef __CINT__
ClassImp(AtlSample);
#endif

//____________________________________________________________________

AtlSample::AtlSample(const char* Name, const char* Title,
		     const char* color, EType Type, Float_t xsection,
		     const char* PathName ) : 
    TNamed(Name, Title), fColor(color), fType(Type),
    fXsection(xsection) {
    //
    // Default constructor
    //
    fPaths.SetOwner();
    if ( PathName ) {
        fPaths.Add(new TObjString(PathName));
    }
    TString SampleName = Name;
    if ( SampleName.Contains("_syst_") ) {
	fIsSystematicSample = kTRUE;
    } else {
	fIsSystematicSample = kFALSE;
    }	 
}

//____________________________________________________________________

AtlSample::~AtlSample() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

const char* AtlSample::GetTypeName() const {
    //
    // Return name of sample type
    //
    return fgTypeNames[fType];
}

//____________________________________________________________________

const char* AtlSample::GetCategoryName() const {
    //
    // Return name of category for MC samples
    //
    if ( IsSgTop() ) {
	return fgCategoryNames[0];
    } else if ( IsTtbar() ) {
	return fgCategoryNames[1];
    } else if ( IsWjetsLight() ) {
	return fgCategoryNames[2];
    } else if ( IsWjetsHeavy() ) {
	return fgCategoryNames[3];
    } else if ( IsHFOR0() ) {
	return fgCategoryNames[4];
    } else if ( IsHFOR1() ) {
	return fgCategoryNames[5];
    } else if ( IsHFOR2() ) {
	return fgCategoryNames[6];
    } else if ( IsHFOR3() ) {
	return fgCategoryNames[7];
    } else if ( IsZjets() ) {
	return fgCategoryNames[8];
    } else if ( IsDiboson() ) {
	return fgCategoryNames[9];
    } else if ( IsWjets() ) {
        return fgCategoryNames[11];
    } else if ( IsQCD() ) {
	return fgCategoryNames[12];
    } else {
	return fgCategoryNames[10];
    }
}

//____________________________________________________________________

Bool_t AtlSample::IsSgTop() const {
    //
    // Is single-top production ?
    //
    return ( fType == kSgTop_tChannel_e 
	     || fType == kSgTop_tChannel_mu
	     || fType == kSgTop_tChannel_tau
	     || fType == kSgTop_sChannel_e 
	     || fType == kSgTop_sChannel_mu
	     || fType == kSgTop_sChannel_tau
	     || fType == kSgTop_Wt
	     || fType == kSgTop_tChannel_incl
	     || fType == kSgTop_tChannel_top_incl
	     || fType == kSgTop_tChannel_antitop_incl
	     || fType == kSgTop_sChannel_incl
	) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsSgTopTChannel() const {
    //
    // Is single-top production ?
    //
    return ( fType == kSgTop_tChannel_e 
	     || fType == kSgTop_tChannel_mu
	     || fType == kSgTop_tChannel_tau
	     || fType == kSgTop_tChannel_incl
	     || fType == kSgTop_tChannel_top_incl
	     || fType == kSgTop_tChannel_antitop_incl) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsSgTopSChannel() const {
    //
    // Is single-top production ?
    //
    return ( fType == kSgTop_sChannel_e 
	     || fType == kSgTop_sChannel_mu
	     || fType == kSgTop_sChannel_tau
	     || fType == kSgTop_sChannel_incl ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsSgTopWt() const {
    //
    // Is single-top production ?
    //
    return ( fType == kSgTop_Wt ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IstZj() const {
    //
    // Is single-top tZ SM production ?
    //
    return ( fType == kSgTop_tZj ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsTtbar() const {
    //
    // Is ttbar production ?
    //
    return ( fType == kTtbar ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsTtbarV() const {
    //
    // Is ttbar+W|Z production ?
    //
    return ( fType == kTtbarPlusW
	     || fType == kTtbarPlusZ
	) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsWjetsLight() const {
    //
    // Is W+light jets production ?
    //
    return ( fType == kWenuNp0
	     || fType == kWenuNp1
	     || fType == kWenuNp2
	     || fType == kWenuNp3
	     || fType == kWenuNp4
	     || fType == kWenuNp5
	     || fType == kWmunuNp0
	     || fType == kWmunuNp1
	     || fType == kWmunuNp2
	     || fType == kWmunuNp3
	     || fType == kWmunuNp4
	     || fType == kWmunuNp5
	     || fType == kWtaunuNp0
	     || fType == kWtaunuNp1
	     || fType == kWtaunuNp2
	     || fType == kWtaunuNp3
	     || fType == kWtaunuNp4
	     || fType == kWtaunuNp5
	     || fType == kWenuInclLight
	     || fType == kWmunuInclLight
	     || fType == kWtaunuInclLight ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsWjetsHeavy() const {
    //
    // Is W+light jets production ?
    //
    return ( fType == kWcNp0
	     || fType == kWcNp1
	     || fType == kWcNp2
	     || fType == kWcNp3
	     || fType == kWcNp4
	     || fType == kWccNp0
	     || fType == kWccNp1
	     || fType == kWccNp2
	     || fType == kWccNp3
	     || fType == kWbbNp0
	     || fType == kWbbNp1
	     || fType == kWbbNp2
	     || fType == kWbbNp3 
	     || fType == kWenuInclB
	     || fType == kWenuInclC
	     || fType == kWmunuInclB
	     || fType == kWmunuInclC
	     || fType == kWtaunuInclB
	     || fType == kWtaunuInclC ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsWjetsC() const {
    //
    // Is W + c production ?
    //
    return ( fType == kWcNp0
	     || fType == kWcNp1
	     || fType == kWcNp2
	     || fType == kWcNp3
	     || fType == kWcNp4
	     || fType == kWccNp0
	     || fType == kWccNp1
	     || fType == kWccNp2
	     || fType == kWccNp3
	     || fType == kWenuInclC
	     || fType == kWmunuInclC
	     || fType == kWtaunuInclC ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsWjetsB() const {
    //
    // Is W + b production ?
    //
    return ( fType == kWbbNp0
	     || fType == kWbbNp1
	     || fType == kWbbNp2
	     || fType == kWbbNp3 
	     || fType == kWenuInclB
	     || fType == kWmunuInclB
	     || fType == kWtaunuInclB ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsWjets() const {
    //
    // Is W production ?
    //
    switch ( fType ) {
    case kWenuIncl:
    case kWmunuIncl:
    case kWtaunuIncl:
        return kTRUE;
    default:
        return (IsWjetsLight() || IsWjetsHeavy());
    }
}

//____________________________________________________________________

Bool_t AtlSample::IsHFOR0() const {
    //
    // Is HFOR type 0 ?
    //
    return ( fType == kWenuNp0_hfor0
	     || fType == kWenuNp1_hfor0
	     || fType == kWenuNp2_hfor0
	     || fType == kWenuNp3_hfor0
	     || fType == kWenuNp4_hfor0
	     || fType == kWenuNp5_hfor0
	     || fType == kWmunuNp0_hfor0
	     || fType == kWmunuNp1_hfor0
	     || fType == kWmunuNp2_hfor0
	     || fType == kWmunuNp3_hfor0
	     || fType == kWmunuNp4_hfor0
	     || fType == kWmunuNp5_hfor0
	     || fType == kWtaunuNp0_hfor0
	     || fType == kWtaunuNp1_hfor0
	     || fType == kWtaunuNp2_hfor0
	     || fType == kWtaunuNp3_hfor0
	     || fType == kWtaunuNp4_hfor0
	     || fType == kWtaunuNp5_hfor0
	     || fType == kWcNp0_hfor0
	     || fType == kWcNp1_hfor0
	     || fType == kWcNp2_hfor0
	     || fType == kWcNp3_hfor0
	     || fType == kWcNp4_hfor0
	     || fType == kWccNp0_hfor0
	     || fType == kWccNp1_hfor0
	     || fType == kWccNp2_hfor0
	     || fType == kWccNp3_hfor0
	     || fType == kWbbNp0_hfor0
	     || fType == kWbbNp1_hfor0
	     || fType == kWbbNp2_hfor0
	     || fType == kWbbNp3_hfor0 ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsHFOR1() const {
    //
    // Is HFOR type 1 ?
    //
    return ( fType == kWenuNp0_hfor1
	     || fType == kWenuNp1_hfor1
	     || fType == kWenuNp2_hfor1
	     || fType == kWenuNp3_hfor1
	     || fType == kWenuNp4_hfor1
	     || fType == kWenuNp5_hfor1
	     || fType == kWmunuNp0_hfor1
	     || fType == kWmunuNp1_hfor1
	     || fType == kWmunuNp2_hfor1
	     || fType == kWmunuNp3_hfor1
	     || fType == kWmunuNp4_hfor1
	     || fType == kWmunuNp5_hfor1
	     || fType == kWtaunuNp0_hfor1
	     || fType == kWtaunuNp1_hfor1
	     || fType == kWtaunuNp2_hfor1
	     || fType == kWtaunuNp3_hfor1
	     || fType == kWtaunuNp4_hfor1
	     || fType == kWtaunuNp5_hfor1
	     || fType == kWcNp0_hfor1
	     || fType == kWcNp1_hfor1
	     || fType == kWcNp2_hfor1
	     || fType == kWcNp3_hfor1
	     || fType == kWcNp4_hfor1
	     || fType == kWccNp0_hfor1
	     || fType == kWccNp1_hfor1
	     || fType == kWccNp2_hfor1
	     || fType == kWccNp3_hfor1
	     || fType == kWbbNp0_hfor1
	     || fType == kWbbNp1_hfor1
	     || fType == kWbbNp2_hfor1
	     || fType == kWbbNp3_hfor1 ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsHFOR2() const {
    //
    // Is HFOR type 2 ?
    //
    return ( fType == kWenuNp0_hfor2
	     || fType == kWenuNp1_hfor2
	     || fType == kWenuNp2_hfor2
	     || fType == kWenuNp3_hfor2
	     || fType == kWenuNp4_hfor2
	     || fType == kWenuNp5_hfor2
	     || fType == kWmunuNp0_hfor2
	     || fType == kWmunuNp1_hfor2
	     || fType == kWmunuNp2_hfor2
	     || fType == kWmunuNp3_hfor2
	     || fType == kWmunuNp4_hfor2
	     || fType == kWmunuNp5_hfor2
	     || fType == kWtaunuNp0_hfor2
	     || fType == kWtaunuNp1_hfor2
	     || fType == kWtaunuNp2_hfor2
	     || fType == kWtaunuNp3_hfor2
	     || fType == kWtaunuNp4_hfor2
	     || fType == kWtaunuNp5_hfor2
	     || fType == kWcNp0_hfor2
	     || fType == kWcNp1_hfor2
	     || fType == kWcNp2_hfor2
	     || fType == kWcNp3_hfor2
	     || fType == kWcNp4_hfor2
	     || fType == kWccNp0_hfor2
	     || fType == kWccNp1_hfor2
	     || fType == kWccNp2_hfor2
	     || fType == kWccNp3_hfor2
	     || fType == kWbbNp0_hfor2
	     || fType == kWbbNp1_hfor2
	     || fType == kWbbNp2_hfor2
	     || fType == kWbbNp3_hfor2 ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsHFOR3() const {
    //
    // Is HFOR type 3 ?
    //
    return ( fType == kWenuNp0_hfor3
	     || fType == kWenuNp1_hfor3
	     || fType == kWenuNp2_hfor3
	     || fType == kWenuNp3_hfor3
	     || fType == kWenuNp4_hfor3
	     || fType == kWenuNp5_hfor3
	     || fType == kWmunuNp0_hfor3
	     || fType == kWmunuNp1_hfor3
	     || fType == kWmunuNp2_hfor3
	     || fType == kWmunuNp3_hfor3
	     || fType == kWmunuNp4_hfor3
	     || fType == kWmunuNp5_hfor3
	     || fType == kWtaunuNp0_hfor3
	     || fType == kWtaunuNp1_hfor3
	     || fType == kWtaunuNp2_hfor3
	     || fType == kWtaunuNp3_hfor3
	     || fType == kWtaunuNp4_hfor3
	     || fType == kWtaunuNp5_hfor3
	     || fType == kWcNp0_hfor3
	     || fType == kWcNp1_hfor3
	     || fType == kWcNp2_hfor3
	     || fType == kWcNp3_hfor3
	     || fType == kWcNp4_hfor3
	     || fType == kWccNp0_hfor3
	     || fType == kWccNp1_hfor3
	     || fType == kWccNp2_hfor3
	     || fType == kWccNp3_hfor3
	     || fType == kWbbNp0_hfor3
	     || fType == kWbbNp1_hfor3
	     || fType == kWbbNp2_hfor3
	     || fType == kWbbNp3_hfor3 ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsZjets() const {
    //
    // Is Z+jets production ?
    //
    return ( fType == kZeeNp0
	     || fType == kZeeNp1
	     || fType == kZeeNp2
	     || fType == kZeeNp3
	     || fType == kZeeNp4
	     || fType == kZeeNp5
	     || fType == kZmumuNp0
	     || fType == kZmumuNp1
	     || fType == kZmumuNp2
	     || fType == kZmumuNp3
	     || fType == kZmumuNp4
	     || fType == kZmumuNp5
	     || fType == kZtautauNp0
	     || fType == kZtautauNp1
	     || fType == kZtautauNp2
	     || fType == kZtautauNp3
	     || fType == kZtautauNp4
	     || fType == kZtautauNp5
	     || fType == kZeeccNp0
	     || fType == kZeeccNp1
	     || fType == kZeeccNp2
	     || fType == kZeeccNp3incl
	     || fType == kZmumuccNp0
	     || fType == kZmumuccNp1
	     || fType == kZmumuccNp2
	     || fType == kZmumuccNp3incl
	     || fType == kZtautauccNp0
	     || fType == kZtautauccNp1
	     || fType == kZtautauccNp2
	     || fType == kZtautauccNp3incl
	     || fType == kZeebbNp0
	     || fType == kZeebbNp1
	     || fType == kZeebbNp2
	     || fType == kZeebbNp3incl
	     || fType == kZmumubbNp0
	     || fType == kZmumubbNp1
	     || fType == kZmumubbNp2
	     || fType == kZmumubbNp3incl
	     || fType == kZtautaubbNp0
	     || fType == kZtautaubbNp1
	     || fType == kZtautaubbNp2
	     || fType == kZtautaubbNp3incl
	     || fType == kZeeIncl
	     || fType == kZeeInclB
	     || fType == kZeeInclC
	     || fType == kZeeInclLight
	     || fType == kZmumuIncl
	     || fType == kZmumuInclB
	     || fType == kZmumuInclC
	     || fType == kZmumuInclLight
	     || fType == kZtautauIncl
	     || fType == kZtautauInclB
	     || fType == kZtautauInclC
	     || fType == kZtautauInclLight
	     || fType == kZnunuInclB
	     || fType == kZnunuInclC
	     || fType == kZnunuInclLight ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsZjetsLF() const {
    //
    // Is Z+jets production ?
    //
    return ( fType == kZeeNp0
	     || fType == kZeeNp1
	     || fType == kZeeNp2
	     || fType == kZeeNp3
	     || fType == kZeeNp4
	     || fType == kZeeNp5
	     || fType == kZmumuNp0
	     || fType == kZmumuNp1
	     || fType == kZmumuNp2
	     || fType == kZmumuNp3
	     || fType == kZmumuNp4
	     || fType == kZmumuNp5
	     || fType == kZtautauNp0
	     || fType == kZtautauNp1
	     || fType == kZtautauNp2
	     || fType == kZtautauNp3
	     || fType == kZtautauNp4
	     || fType == kZtautauNp5
	     || fType == kZeeInclLight
	     || fType == kZmumuInclLight
	     || fType == kZtautauInclLight
	     || fType == kZnunuInclLight ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsZjetsHF() const {
    //
    // Is Z+jets production ?
    //
    return ( fType == kZeeccNp0
	     || fType == kZeeccNp1
	     || fType == kZeeccNp2
	     || fType == kZeeccNp3incl
	     || fType == kZmumuccNp0
	     || fType == kZmumuccNp1
	     || fType == kZmumuccNp2
	     || fType == kZmumuccNp3incl
	     || fType == kZtautauccNp0
	     || fType == kZtautauccNp1
	     || fType == kZtautauccNp2
	     || fType == kZtautauccNp3incl
	     || fType == kZeebbNp0
	     || fType == kZeebbNp1
	     || fType == kZeebbNp2
	     || fType == kZeebbNp3incl
	     || fType == kZmumubbNp0
	     || fType == kZmumubbNp1
	     || fType == kZmumubbNp2
	     || fType == kZmumubbNp3incl
	     || fType == kZtautaubbNp0
	     || fType == kZtautaubbNp1
	     || fType == kZtautaubbNp2
	     || fType == kZtautaubbNp3incl
	     || fType == kZeeInclB
	     || fType == kZeeInclC
	     || fType == kZmumuInclB
	     || fType == kZmumuInclC
	     || fType == kZtautauInclB
	     || fType == kZtautauInclC
	     || fType == kZnunuInclB
	     || fType == kZnunuInclC ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsZjetsC() const {
    //
    // Is Z+jets production ?
    //
    return ( fType == kZeeccNp0
	     || fType == kZeeccNp1
	     || fType == kZeeccNp2
	     || fType == kZeeccNp3incl
	     || fType == kZmumuccNp0
	     || fType == kZmumuccNp1
	     || fType == kZmumuccNp2
	     || fType == kZmumuccNp3incl
	     || fType == kZtautauccNp0
	     || fType == kZtautauccNp1
	     || fType == kZtautauccNp2
	     || fType == kZtautauccNp3incl
	     || fType == kZeeInclC
	     || fType == kZmumuInclC
	     || fType == kZtautauInclC
	     || fType == kZnunuInclC ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsZjetsB() const {
    //
    // Is Z+jets production ?
    //
    return ( fType == kZeebbNp0
	     || fType == kZeebbNp1
	     || fType == kZeebbNp2
	     || fType == kZeebbNp3incl
	     || fType == kZmumubbNp0
	     || fType == kZmumubbNp1
	     || fType == kZmumubbNp2
	     || fType == kZmumubbNp3incl
	     || fType == kZtautaubbNp0
	     || fType == kZtautaubbNp1
	     || fType == kZtautaubbNp2
	     || fType == kZtautaubbNp3incl
	     || fType == kZeeInclB
	     || fType == kZmumuInclB
	     || fType == kZtautauInclB
	     || fType == kZnunuInclB ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsDiboson() const {
    //
    // Is Diboson production ?
    //
    return ( fType == kDibosonWW
	     || fType == kDibosonWZ
	     || fType == kDibosonZZ
	     || fType == kWWenuqq
	     || fType == kWZenuqq
	     || fType == kWWmunuqq
	     || fType == kWZmunuqq
	     || fType == kWWtaunuqq
	     || fType == kWZtaunuqq
	     || fType == kZWeeqq
	     || fType == kZZeeqq
	     || fType == kZWmumuqq
	     || fType == kZZmumuqq
	     || fType == kZWtautauqq
	     || fType == kZZtautauqq ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsFCNCtZ() const {
    //
    // Is FCNC tZ production ?
    //
    return ( fType == kFCNC_utZ_K01L || fType == kFCNC_ctZ_K01L )  ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsQCD() const {
    //
    // Is QCD Multijet production ?
    //
    return ( fType == kQCDMatrixMethod
	     || fType == kQCDMatrixMethod_Tight
	     || fType == kQCDMatrixMethod_Loose
	     || fType == kQCDMatrixMethod_EGamma
	     || fType == kQCDMatrixMethod_Muon
	     || fType == kQCDJetElectron_EGamma
	     || fType == kQCDJetElectron_Muon
	     || fType == kQCD_AntiMuon
             || fType == kQCD_JetLepton
        ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsDATA() const {
    //
    // Sample is real data ?
    //
    switch ( fType ) {
    case kDATA_EGamma:
    case kDATA_Muon:
    case kDATA_TOPQ1:
    case kDATA_TOPQ2:
    case kDATA_TOPQ3:
        return kTRUE;
    default:
        return kFALSE;
    }
}

//____________________________________________________________________

Bool_t AtlSample::IsDATAEGamma() const {
    //
    // Sample is Egamma stream (DATA) ?
    //
    return ( fType == kDATA_EGamma ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsDATAMuon() const {
    //
    // Sample is Muon stream (DATA) ?
    //
    return ( fType == kDATA_Muon ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsMC() const {
    //
    // Sample is MC ?
    //
    return (!IsDATA() && !IsQCD());
}

//____________________________________________________________________

Bool_t AtlSample::IsQCDTight() const {
    //
    // Is QCD Multijet (tight) ?
    //
    return ( fType == kQCDMatrixMethod_Tight ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsQCDLoose() const {
    //
    // Is QCD Multijet (loose) ?
    //
    return ( fType == kQCDMatrixMethod_Loose ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsQCDEGamma() const {
    //
    // Is QCD Multijet (EGamma) ?
    //
    return ( fType == kQCDMatrixMethod_EGamma
	     || fType == kQCD_JetLepton 
	     || fType == kQCDJetElectron_EGamma ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

Bool_t AtlSample::IsQCDMuon() const {
    //
    // Is QCD Multijet (Muon) ?
    //
    return ( fType == kQCDMatrixMethod_Muon
	     || fType == kQCD_AntiMuon
	     || fType == kQCDJetElectron_Muon ) ? kTRUE : kFALSE;
}

//____________________________________________________________________

void AtlSample::Print(Option_t *option) const {
    //
    // Print sample information
    //
    cout << endl
	 << "Sample " << GetName() << endl
	 << "Sample type     : " << GetTypeName() << endl
	 << "Sample Xsection : " << GetXsection() << endl
//	 << "Sample category : " << GetCategoryName() << endl
	 << "Paths           :";
    for ( TIter next(&fPaths); TObjString const * path = static_cast<TObjString const *>(next()); ) {
        cout << " " << path->GetName();
    }
    cout << "\n" << endl;
}

//____________________________________________________________________

Int_t AtlSample::ReadType(const char* Type) {
    //
    // Get sample type by string, eg "kDATA_EGamma".
    // Needed for reading from text files
    //

    TList *dm = Class()->GetListOfDataMembers();
    TObject *obj0 = dm->FindObject("kDATA_EGamma");
    TObject *obj1 = dm->FindObject(Type);
    if ( obj0 == 0 )  {
	cerr << "<AtlSample::ReadType> Could not find valid data"
	     << "member for enum \"kDATA_EGamma\". Abort!" << endl;
	gSystem->Abort(1);
    }
    if ( obj1 == 0 )  {
	cerr << "<AtlSample::ReadType> Could not find valid data member"
	     << "for enum \"" << Type << "\". Abort!" << endl;
	gSystem->Abort(1);
    }

    Int_t idx0 = dm->IndexOf(obj0);
    Int_t idx1 = dm->IndexOf(obj1);
    return idx1 - idx0;
}

//____________________________________________________________________

void AtlSample::AddPath(char const * path) {
    //
    // Add path names
    //
    fPaths.Add(new TObjString(path));
}
