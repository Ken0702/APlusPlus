//  
// Author: Sebastian Beumler <mailto: beumler@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Sebastian Beumler
//

#ifndef HEP_Hep2BodyDecay
#include <Hep2BodyDecay.h>
#endif
#ifndef HEP_HepVtxTrackHelix
#include <HepVtxTrackHelix.h>
#endif
#include <HepVertex.h>

#ifndef __CINT__
ClassImp(Hep2BodyDecay);
#endif

//____________________________________________________________________

Hep2BodyDecay::Hep2BodyDecay() {
    //
    // Default constructor
    //
    fDaughter1 = new TRef;
    fDaughter2 = new TRef;
    fVtx       = new TRef;
}

//____________________________________________________________________

Hep2BodyDecay::Hep2BodyDecay(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
			 Float_t E, TObject *Daughter1, TObject *Daughter2,
			 HepVertex *DecayVtx, HepVertex *PrimaryVtx,
			 HepParticle ReFitDaughter1, HepParticle ReFitDaughter2) :
    HepDecayParticle(Id, Px, Py, Pz, E, 0) {
    //
    // Normal constructor
    //
    fDaughter1 = new TRef; *fDaughter1 = Daughter1;
    fDaughter2 = new TRef; *fDaughter2 = Daughter2;
    fVtx       = new TRef; *fVtx       = (TObject*)DecayVtx;
    fReFitDaughter1 = ReFitDaughter1;
    fReFitDaughter2 = ReFitDaughter2;
    fHasSharedTracks = kFALSE;

    // calculate decay length
    TVector3 d = PrimaryVtx->GetPos() - DecayVtx->GetPos();
    fDecayLength = d.Mag();
    // save transverse decay length
    fTransvDecayLength = d.Perp();
    // calculate angle between particle origin and primary vertex
    TLorentzVector p = ReFitDaughter1.P() + ReFitDaughter2.P();
    TVector3 o = DecayVtx->GetPos() - PrimaryVtx->GetPos();
    fAngleToPrim = p.Angle(o);
    
    // calculate opening angle between outgoing tracks in lab frame
    /*
    HepVtxTrackHelix *trk1 = (HepVtxTrackHelix*)Daughter1;
    HepVtxTrackHelix *trk2 = (HepVtxTrackHelix*)Daughter2;
    fOpeningAngle = trk1->P().Angle(trk2->P());
    */
    
    // calculate opening angle between outgoing tracks in lab frame
    fOpeningAngle = ReFitDaughter1.P().Angle(ReFitDaughter2.P().Vect());
    
    // calculate decay angle between outgoing tracks in rest frame
    // (mass hypothesis must be stored in ReFitDaughter
    TLorentzVector PDaughter1 = ReFitDaughter1.P();
    PDaughter1.Boost(-P().BoostVector()); // boost daughter momentum to mother's restframe
    fDecayAngle = P().Angle(PDaughter1.Vect());
}

//____________________________________________________________________

Hep2BodyDecay::~Hep2BodyDecay() {
    //
    // Default destructor
    //
    delete fDaughter1;
    delete fDaughter2;
    delete fVtx;
}

//____________________________________________________________________

void Hep2BodyDecay::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepDecayParticle::Clear(option);
    delete fDaughter1; fDaughter1 = 0;
    delete fDaughter2; fDaughter2 = 0;
    delete fVtx;       fVtx       = 0;
    fDecayLength = 0.;
    fTransvDecayLength = 0;
    fReFitDaughter1.Clear();
    fReFitDaughter2.Clear();
    
}

//____________________________________________________________________

Float_t Hep2BodyDecay::GetLifeTime() {
    //
    // Compute life-time (in ps)
    //
    Double_t BetaGamma = Pmag()/M("pdg");
    return GetDecayLength()/(BetaGamma*TMath::Ccgs())*1.e12;
}
