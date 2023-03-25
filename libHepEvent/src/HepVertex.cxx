//____________________________________________________________________
//
// Vertex class for reconstructed vertices
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepVertex
#include <HepVertex.h>
#endif
#include <HepVtxTrackHelix.h>
#include <iostream>
#include <TSystem.h>

using namespace std;

#ifndef __CINT__
ClassImp(HepVertex);
#endif

//____________________________________________________________________

HepVertex::HepVertex() {
    //
    // Default constructor
    //
    fPos.SetXYZ(0, 0, 1); // unit-vector in Z dircetion (instead of
			  // zero-vector). This is to avoid
			  // difficulties when using polar coordinates
    fDaughters = new TRefArray;
    fErrX = 0;
    fErrY = 0;
    fErrZ = 0;
    fNDaughters = -999;
}

//____________________________________________________________________

HepVertex::HepVertex(Int_t Id, Float_t X, Float_t Y, Float_t Z,
		     Float_t Chi2, Int_t NDoF) :
fId(Id), fChi2(Chi2), fNDoF(NDoF) {
    //
    // Normal constructor
    //
    fPos.SetXYZ(X, Y, Z);
    fDaughters = new TRefArray;
    fType = kInvalid;
    fErrX = 0;
    fErrY = 0;
    fErrZ = 0;
    fNDaughters = -999;
}

//____________________________________________________________________

HepVertex::~HepVertex() {
    //
    // Default destructor
    //
    delete fDaughters; fDaughters = 0;
}

//____________________________________________________________________

void HepVertex::Clear(Option_t *option) {
    //
    // Clear this object
    //
    fPos.SetXYZ(0, 0, 1);
    fType = kInvalid;
    fErrX = 0;
    fErrY = 0;
    fErrZ = 0;
    delete fDaughters; fDaughters = 0;
    fNDaughters = -999;
}

//____________________________________________________________________

void HepVertex::AddDaughter(HepVtxTrackHelix *trk) {
    //
    // Add outgoing helix to the list of daughters
    // and set this vertex as mother in the track object
    //
    fDaughters->Add((TObject*)trk);
    HepVertex *vtx = trk->ProducedAt();
    if ( vtx != 0 ) {
	Error("AddDaughter",
	      "Track points already to a different vertex. Abort");
	gSystem->Abort(0);
    }
    trk->SetVertex(this);
}

//____________________________________________________________________

void HepVertex::Print(Option_t *option) const {
    //
    // Print object information
    //
    // Options available:
    //   "daughters" - Print all daughter tracks also
    //   "nohead"    - No header containing the variable names is
    //                 displayed. Useful when printing a whole table
    //                 for a list of vertices
    //
    TString opt = option;
    opt.ToLower();

    // Print header
    if ( !opt.Contains("nohead") ) PrintHeader();

    // Print
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.precision(3);
    cout.width(4);  cout << fId;
    cout.width(12); cout << X();
    cout.width(12); cout << Y();
    cout.width(12); cout << Z();
    cout.width(15); cout << GetVtxTypeStr()->Data();
    cout.width(6);  cout << GetNDaughters();
    cout.width(9);  cout << fChi2 << "/";
    cout.width(3);  cout << fNDoF;
    cout << endl;
        
    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();

    // Print daughter tracks (if wished)
    if ( opt.Contains("daughters") ) {
	TIter next_trk(GetDaughters());
	HepVtxTrackHelix *trk = 0;
	cout << endl << "Vtx daughters:" << endl;
	HepVtxTrackHelix::PrintHeader();
	while ( (trk=(HepVtxTrackHelix*)next_trk()) ) {
	    trk->Print("nohead");
	}
	HepVtxTrackHelix::PrintFooter();
    }
}

//____________________________________________________________________

void HepVertex::PrintHeader() {
    //
    // Print information header
    //
    cout << "---------------------------------------------------------------------------" << endl
         << " Id        X (cm)      Y (cm)      Z (cm)       Type     Ntrk    Chi2/DoF  " << endl
         << "---------------------------------------------------------------------------" << endl; 
}

//____________________________________________________________________

void HepVertex::PrintFooter() {
    //
    // Print footer
    //
    cout << "---------------------------------------------------------------------------" << endl;
}

//____________________________________________________________________

TString* HepVertex::GetVtxTypeStr() const {
    //
    // Return character string of vertex type(s)
    //
    TString *type = new TString("");
    if ( fType & kInvalid ) {
	type->Append("invalid");
    } else if ( fType & kDummy ) {
	type->Append("dummy");
    } else if ( fType & kNotSpecified ) {
	type->Append("not specified");
    } else if ( fType & kPrimary ) {
	type->Append("primary");
    } else if ( fType & kPileUp ) {
	type->Append("pile-up");
    } else if ( fType & kSecondary ) {
	type->Append("sec");
	if ( fType & kTrkKink ) type->Append(",kink");
	if ( fType & kConversion ) type->Append(",conv");
	if ( fType & kV0Vtx ) type->Append(",V0");
	if ( fType & kV0K0s ) type->Append(",K0s");
	if ( fType & kV0Lambda ) type->Append(",Lambda");
	if ( fType & kV0LambdaBar ) type->Append(",LambdaBar");
	if ( fType & kV0Rho ) type->Append(",Rho");
    } else {
	Error("GetVtxTypeStr", "Unknown vertex type %d. Abort!", fType);
	gSystem->Abort(0);
    }
    return type;
}

//____________________________________________________________________

Int_t HepVertex::Compare(const TObject *obj) const {
    //
    // Compares chi2/ndof of this vertex with another.
    // Necessary for sorting
    //
    Float_t chi2 = GetChi2overNDoF();
    if ( chi2 >  ((HepVertex*)obj)->GetChi2overNDoF() ) return -1;
    if ( chi2 == ((HepVertex*)obj)->GetChi2overNDoF() ) return  0;
    if ( chi2 <  ((HepVertex*)obj)->GetChi2overNDoF() ) return  1;
    return 0;
}
