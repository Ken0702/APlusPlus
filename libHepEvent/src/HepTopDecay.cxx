//____________________________________________________________________
//
// Top decay class
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
//
#ifndef HEP_HepTopDecay
#include <HepTopDecay.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepTopDecay);
#endif

//____________________________________________________________________

HepTopDecay::HepTopDecay() {
    //
    // Default constructor
    //
    fWDecay   = new TRef;
    fBJetOrig = new TRef;
    fTruthTop = new TRef;
}

//____________________________________________________________________

HepTopDecay::HepTopDecay(Int_t Id, Float_t Px_T, Float_t Py_T, Float_t Pz_T,
			 Float_t E_T, HepWDecay *WDecay, HepJet *BJetOrig,
			 Float_t Px_j, Float_t Py_j, Float_t Pz_j,
			 Float_t E_j, ProductionMode mode) :
    HepDecayParticle(Id, Px_T, Py_T, Pz_T, E_T,
		     (WDecay->IsPositive()) ? 6 : -6) {
    //
    // Normal constructor
    //
    fWDecay   = new TRef;
    fBJetOrig = new TRef;
    fTruthTop = new TRef;
    *fWDecay   = (TObject*)WDecay;
    *fBJetOrig = (TObject*)BJetOrig;
    fP_BJet.SetPxPyPzE(Px_j, Py_j, Pz_j, E_j);
    fMode = mode;
}

//____________________________________________________________________

HepTopDecay::~HepTopDecay() {
    //
    // Default destructor
    //
    delete fWDecay;   fWDecay   = 0;
    delete fBJetOrig; fBJetOrig = 0;
    delete fTruthTop; fTruthTop = 0;
//    fP_BJet.SetPxPyPzE(0, 0, 1, 0);
}

//____________________________________________________________________

void HepTopDecay::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepDecayParticle::Clear(option);
    delete fWDecay;   fWDecay   = 0;
    delete fBJetOrig; fBJetOrig = 0;
    delete fTruthTop; fTruthTop = 0;
    fP_BJet.SetPxPyPzE(0, 0, 1, 0);
}

//____________________________________________________________________

void HepTopDecay::Print(Option_t *option) {
    //
    // Print object information
    //
    cout << endl << endl
	 << "Semi-leptonic t-quark decay #" << fId << endl
	 << "============================" << endl
	 << "Kinematic fit result: chi^2/NDoF = "
	 << fChi2 << " / " << fNDoF << "   Prob = " << fProb << endl << endl
	 << "Reconstructed top-quark:" << endl;
    HepParticle::Print();
    cout << endl
	 << "Outgoing B-jet:" << endl;
    GetBJetOrig()->Print();
    cout << "Improved B-jet 4-momentum: ";
    HepParticle::PrintLorentzVector(GetP_BJet());
    cout << "Reconstructed W-boson:" << endl;
    GetWDecay()->Print();
    cout << endl << endl;
}
    
//____________________________________________________________________

Int_t HepTopDecay::Compare(const TObject *obj) const {
    //
    // Compare invariant mass of top decays
    //
    // Return values:
    //    +1 = invariant mass of this decay is closer to the pole-mass of
    //         172.5GeV than the given decay
    //     0 = both invariant masses are equal
    //    -1 = invariant mass of the compared decay is closer to the
    //         pole-mass than this decay
    //
    static const Double_t m_top = 172.5;
    if ( TMath::Abs(M("rec") - m_top)
	 < TMath::Abs(((HepTopDecay*)obj)->M("rec") - m_top) ) return  1;
    if ( TMath::Abs(M("rec") - m_top)
	 > TMath::Abs(((HepTopDecay*)obj)->M("rec") - m_top) ) return -1;
    return 0;
}

