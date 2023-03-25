//____________________________________________________________________
//
// Atlas detector level jet
//
// One get the truth flavour (Ligth, Charm, Beauty or Tau) of the jet by
// GetTruthFlavour(), wich is filled by Athena. If it's not filled, one
// can get the flavour from fTruthParticles, wich is a reference to HepMCTruth
// particles in jet cone, sorted by Pt in descending order by Athena. The
// first particle in the list, should be the leading particle of the jet.
//
// NOTE:
// The refined quality definitions are tested only for jets of type
// AntiKt4Topo or AntiKt6Topo and ARE NOT SET for the "Tower" and "Cone" jets!
// 
// 
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlJet
#include <AtlJet.h>
#endif
#ifndef HEP_Jet
#include <HepJet.h>
#endif
#ifndef HEP_Particle
#include <HepParticle.h>
#endif
#include <iostream>

using namespace std;

static const char* fgTypeNames[AtlJet::kNumTypes+1] = {
    "InvalidJet",
    "Cone4H1Tower", "Cone7H1Tower", "Cone4H1Topo", "Cone7H1Topo", 
    "MCCone4Hadron", "MCCone7Hadron", 
    "MCAntiKt4Hadron", "MCAntiKt6Hadron", "MCAntiKt4HadronPileup",
    "AntiKt4H1Topo", "AntiKt4H1Tower", "AntiKt6H1Tower",
    "AntiKt4TopoEM", "AntiKt4TopoEMJES", "AntiKt4LCTopo",
    "AntiKt4Tower",  "AntiKt6TopoEM", "AntiKt4Topo", "AntiKt6Tower",
    "AntiKt6LCTopo","AntiKt6Topo",
    "AtlFast"
};

static const char* fgFlavourNames[AtlJet::kNumFlavours] = {
    "Unknown", "Light", "Charm", "Beauty", "Tau"
};


#ifndef __CINT__
ClassImp(AtlJet);
#endif

//____________________________________________________________________

AtlJet::AtlJet() {
    //
    // Default constructor
    //
    fFakeCandidate = new TRef;
    fTruthParticles = new TRefArray;
    fTracks = new TRefArray;
    fK0sCanditates = new TRefArray;
}

//____________________________________________________________________

AtlJet::AtlJet(Int_t Id, Float_t E, Float_t Px, Float_t Py, Float_t Pz,
	       EJetQuality jetquality,
	       TLorentzVector P_EMSCALE, TLorentzVector P_JESCorrSCALE,
	       Double_t EMJES_EtaCorr, 
	       Double_t BCH_CORR_CELL, Double_t BCH_CORR_JET,
	       Float_t eta_offsetJES):
    HepJet(Id, E, Px, Py, Pz), fJetQuality(jetquality),
    fP_EMSCALE(P_EMSCALE), fP_JESCorrSCALE(P_JESCorrSCALE), 
    fEMJES_EtaCorr(EMJES_EtaCorr),
    fBCH_CORR_CELL(BCH_CORR_CELL), fBCH_CORR_JET(BCH_CORR_JET),
    fEta_offsetJES(eta_offsetJES) {
    //
    // Normal constructor
    //
    fFakeCandidate = new TRef;
    fTruthParticles = new TRefArray;
    fTracks = new TRefArray;
    fK0sCanditates = new TRefArray;
    fTruthFlavour = kUnknownFlavour;
    
    // Refined jet quality variables
    fLArQuality      = 0;          
    fHecQuality      = 0;          
    fNegEnergy       = 0;           
    fN90             = 0;                 
    fEMFraction      = 0;          
    fHecF            = 0;                
    fTiming          = 0;              
    fSamplingMax     = 0;         
    fFracSamplingMax = 0;     
    fSumPtTrk        = 0;    

    // Truth DeltaRmin
    fTruthDeltaRminBeauty = 10.e10;
    fTruthDeltaRminCharm  = 10.e10;
    fTruthDeltaRminTau    = 10.e10;
}

//____________________________________________________________________

AtlJet::~AtlJet() {
  //
  // Default destructor
  //
  delete fFakeCandidate;  fFakeCandidate = 0;
  delete fTruthParticles; fTruthParticles = 0;
  delete fTracks;         fTracks = 0;
  delete fK0sCanditates;  fK0sCanditates = 0;
  
}

//____________________________________________________________________

void AtlJet::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepJet::Clear(option);
    AtlTriggerMatch::Clear(option);
    AtlMETWeights::Clear(option);

    fTruthFlavour = kUnknownFlavour;
    for ( Int_t i = 0; i < AtlBTag::kNumTaggers; i++) {
	fTaggers[i].Clear(option);
    }
    delete fFakeCandidate; fFakeCandidate = 0;
    delete fTruthParticles; fTruthParticles = 0;
    delete fTracks; fTracks = 0;
    delete fK0sCanditates; fK0sCanditates = 0;
    
    fJetQuality = kInvalid;
    
    fLArQuality      = 0;          
    fHecQuality      = 0;          
    fNegEnergy       = 0;           
    fN90             = 0;                 
    fEMFraction      = 0;          
    fHecF            = 0;                
    fTiming          = 0;              
    fSamplingMax     = 0;         
    fFracSamplingMax = 0;     
    fSumPtTrk        = 0;            
    fEMJES_EtaCorr   = 0;

    fTruthDeltaRminBeauty = 10.e10;
    fTruthDeltaRminCharm  = 10.e10;
    fTruthDeltaRminTau    = 10.e10;
}

//____________________________________________________________________

const AtlBTag* AtlJet::GetTag(AtlBTag::ETagger tagger) {
    //
    // Return B-tag object
    //
    return &fTaggers[tagger];
}

//____________________________________________________________________

AtlBTag* AtlJet::AddTag(AtlBTag::ETagger tagger, Double_t weight,
			Bool_t IsValid) {
    //
    // Add B-tag object
    //
    fTaggers[tagger] = AtlBTag(tagger, weight, IsValid);
    return &fTaggers[tagger];
}

//____________________________________________________________________

const char* AtlJet::NameOfType(EType type) {
    //
    // Return name of given jet type
    //
    if (type > kInvalidType && type < kNumTypes)
        return fgTypeNames[type + 1];

    return fgTypeNames[kInvalidType + 1];
}

//____________________________________________________________________

Float_t AtlJet::EMScaleDeltaR(AtlElectron *el, const char* option) {
    //
    // Compute distance in eta-phi plane between this jet and the given
    // electron.
    // Optionally, using the detector position of the jet
    // (eta_EMScale+EMJES_EtaCorr and phi_EMSCALE) and/or the
    // track information of the electron.
    //
    //  "trk"    - Use track information of electron for comparison
    //  "em"     - Use EMSCALE jet position
    //
    TString opt(option);
    opt.ToLower();

    Float_t jet_eta = Eta();
    Float_t el_eta  = el->Eta();
    
    Float_t jet_eta_em = EMScaleEta()+EMJES_EtaCorr();
    Float_t el_eta_trk = el->GetIDTrack()->Eta();

    Float_t dEta = 0;
    Float_t dPhi = 0;
    
    if ( opt.Contains("trk") && opt.Contains("em") ) {
	dEta = jet_eta_em - el_eta_trk;
	dPhi = ( P_EMSCALE().Vect() ).DeltaPhi( el->GetIDTrack()->P() );
    } else if ( opt.Contains("trk") ) {
	dEta = jet_eta    - el_eta_trk;
	dPhi = ( P().Vect() ).DeltaPhi( el->GetIDTrack()->P() );
    } else if ( opt.Contains("em") ) {
	dEta = jet_eta_em - el_eta;
	dPhi = ( P_EMSCALE().Vect() ).DeltaPhi( el->P3() ); 
    } else {
	dEta = jet_eta    - el_eta;
	dPhi = ( P().Vect() ).DeltaPhi( el->P3() );
    }

    return TMath::Sqrt(dEta*dEta + dPhi*dPhi);
}

//____________________________________________________________________

Float_t AtlJet::EMScaleDeltaR(AtlMuon *mu, const char* option) {
    //
    // Compute distance in eta-phi plane between this jet and the given
    // muon using the detector position of the jet
    // (eta_EMScale+EMJES_EtaCorr and phi_EMSCALE) and optionally the
    // track information of the muon
    //
    //  "trk"    - Use track information of muon for comparison
    //
    TString opt(option);
    opt.ToLower();

    Float_t dEta = 0;
    Float_t dPhi = 0;
 
    if ( opt.Contains("trk") ) {
	dEta = EMScaleEta()+EMJES_EtaCorr() - mu->GetIDTrack()->Eta();
	dPhi = ( P_EMSCALE().Vect() ).DeltaPhi( mu->GetIDTrack()->P() );
    } else {
	dEta = EMScaleEta()+EMJES_EtaCorr() - mu->Eta();
	dPhi = P_EMSCALE().DeltaPhi( mu->P() );
    }

    return TMath::Sqrt(dEta*dEta + dPhi*dPhi);
}

//____________________________________________________________________

void AtlJet::SetQualityVars(Double_t LArQuality,
			    Double_t HecQuality,
			    Double_t NegEnergy,
			    Double_t N90,
			    Double_t EMFraction,
			    Double_t HecF,
			    Double_t Timing,
			    Int_t    SamplingMax,
			    Double_t FracSamplingMax,
			    Double_t SumPtTrk) {
    //
    // Set the jet quality variables
    //
    fLArQuality      = LArQuality;          
    fHecQuality      = HecQuality;          
    fNegEnergy       = NegEnergy;           
    fN90             = N90;            
    fEMFraction      = EMFraction;          
    fHecF            = HecF;                
    fTiming          = Timing;              
    fSamplingMax     = SamplingMax;    
    fFracSamplingMax = FracSamplingMax;     
    fSumPtTrk        = SumPtTrk;

    SetRefinedQualityFlag();
}

//____________________________________________________________________

void AtlJet::SetRefinedQualityFlag() {
    //
    // Computes and sets the refined jet quality flag
    //

    //
    // Compute the criteria
    //
    Bool_t IsLooseBad  = kFALSE;
    Bool_t IsMediumBad = kFALSE;
    Bool_t IsTightBad  = kFALSE;
    Double_t chargeFrac = fSumPtTrk / JES_Pt();

    // Non-collision background & cosmics
    if ( ( TMath::Abs(fTiming) > 25 )
	 || ( fEMFraction < 0.05
	      && chargeFrac < 0.05
	      && TMath::Abs(EMScaleEta()) < 2 )
	 || ( fEMFraction < 0.05
	      && TMath::Abs(EMScaleEta()) >= 2 )
	 || ( fFracSamplingMax > 0.99
	      && TMath::Abs(EMScaleEta()) < 2 )
	) {
	IsLooseBad  = kTRUE;
	IsMediumBad = kTRUE;
	IsTightBad  = kTRUE;
    }

    if ( ( TMath::Abs(fTiming) > 10 )
	 || ( fEMFraction < 0.05
	      && chargeFrac < 0.1
	      && TMath::Abs(EMScaleEta()) < 2 )
	 || ( fEMFraction > 0.95
	      && chargeFrac < 0.05
	      && TMath::Abs(EMScaleEta()) < 2 )
	) {
	IsMediumBad = kTRUE;
	IsTightBad  = kTRUE;
    }

    if ( ( fEMFraction < 0.1
	   && chargeFrac < 0.2
	   && TMath::Abs(EMScaleEta()) < 2 )
	 || ( fEMFraction < 0.1
	      && TMath::Abs(EMScaleEta()) >= 2 )
	 || ( fEMFraction > 0.9
	      && chargeFrac < 0.02
	      && TMath::Abs(EMScaleEta()) < 2 )
	) {
	IsTightBad  = kTRUE;
    }

    // HEC spikes
    if ( ( TMath::Abs(fNegEnergy) > 60 )
	 || ( fHecF > 0.5
	      && TMath::Abs(fHecQuality) > 0.5 )
	) {
	IsLooseBad  = kTRUE;
	IsMediumBad = kTRUE;
	IsTightBad  = kTRUE;
    }

    if ( fHecF > ( 1 - TMath::Abs(fHecQuality) ) ) {
	IsMediumBad = kTRUE;
	IsTightBad  = kTRUE;
    }

    // EM coherent noise
    if ( ( fEMFraction > 0.95 )
	 && ( TMath::Abs(fLArQuality) > 0.8 )
	 && ( TMath::Abs(EMScaleEta()) < 2.8 )
	) {
	IsLooseBad  = kTRUE;
	IsMediumBad = kTRUE;
	IsTightBad  = kTRUE;
    }   
    
    if ( ( fEMFraction > 0.9 )
	 && ( TMath::Abs(fLArQuality) > 0.8 )
	 && ( TMath::Abs(EMScaleEta()) < 2.8 )
	) {
	IsMediumBad = kTRUE;
	IsTightBad  = kTRUE;
    }

    if ( ( TMath::Abs(fLArQuality) > 0.95 )
	 || ( fEMFraction > 0.98
	      && TMath::Abs(fLArQuality) > 0.05 )
	) {
	IsTightBad  = kTRUE;
    }

    //
    // Set the refined flag criteria
    //
    if ( IsLooseBad )
	fJetQuality = AtlJet::EJetQuality(fJetQuality | AtlJet::kIsLooseBad);
    if ( IsMediumBad )
	fJetQuality = AtlJet::EJetQuality(fJetQuality | AtlJet::kIsMediumBad);
    if ( IsTightBad )
	fJetQuality = AtlJet::EJetQuality(fJetQuality | AtlJet::kIsTightBad);
}

//____________________________________________________________________

void AtlJet::Print(Option_t *option) {
    //
    // Print jet information
    //
    //   "nohead" - No header containing the variable names is
    //              displayed. Useful when printing a whole table
    //              for a list of jets
    //   "btag"   - Print b-tagger results for this jet
    //   "prt"    - Print list of associated MC truth particles
    //   "trk"    - Print list of tracks inside the jet
    //   "emscale"- Print kinematic variables at EMScale
    //
    TString opt = option;
    opt.ToLower();

    // =======
    // General
    // =======

    // Print header
    if ( !opt.Contains("nohead") ) PrintHeader();
    
    // Print
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.precision(3);
    cout.width(4);  cout << fId;
    if ( opt.Contains("emscale") ) {
	cout.width(12); cout << EMScaleEt();
	cout.width(12); cout << EMScaleM();
	cout.width(12); cout << EMScaleTheta()*180/TMath::Pi();
	cout.width(12); cout << EMScalePhi()*180/TMath::Pi();
	cout.width(12); cout << EMScaleEta();
    } else {
	cout.width(12); cout << Et();
	cout.width(12); cout << M();
	cout.width(12); cout << Theta()*180/TMath::Pi();
	cout.width(12); cout << Phi()*180/TMath::Pi();
	cout.width(12); cout << Eta();
    }
    HepParticle *fake = GetFakeCandidate();
    if ( fake == 0 )  {
	cout << "          --    ";
    } else {
	cout.width(10); cout << fake->GetPdgName()
			     << ",Id=" << fake->GetId();
    }
    cout.width(11); cout << fgFlavourNames[fTruthFlavour];
    cout.width(3);
    if ( IsUglyJet() ) {
	cout << "u";
    } else {
	cout << "-";
    }
    if ( IsBadJet() ) {
	cout << "b";
    } else {
	cout << "-";
    }
    if ( IsGoodJet() ) {
	cout << "g";
    } else {
	cout << "-";
    }
    if ( IsLooseBadJet() ) {
	cout << "l";
    } else {
	cout << "-";
    }
    if ( IsMediumBadJet() ) {
	cout << "m";
    } else {
	cout << "-";
    }
    if ( IsTightBadJet() ) {
	cout << "t";
    } else {
	cout << "-";
    }
    cout << "    ";
    cout << endl;

    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();

    // ==========================
    // Print B-tagger information
    // ==========================
    if ( opt.Contains("btag") ) {

        // Print header
        cout << endl;
        AtlBTag::PrintHeader();

        // Loop over all taggers and call their Print() function
        for ( Int_t i = 0; i < AtlBTag::kNumTaggers; i++ ) {
	    GetTag((AtlBTag::ETagger)i)->Print("nohead");
	}
	
        // Print footer
        AtlBTag::PrintFooter();
    }

    // ================================
    // Print list of MC truth particles
    // ================================
    if ( opt.Contains("prt") ) {

	// Print header
	cout << endl;
	HepMCParticle::PrintHeader();

	// Print list
	TIter next_prt(fTruthParticles);
	HepMCParticle *prt = 0;
	while ( (prt = (HepMCParticle*)next_prt()) ) {
	    prt->Print("nohead");
	}

	// Print footer
	HepMCParticle::PrintFooter();
    }

    // ====================
    // Print list of tracks
    // ====================
    if ( opt.Contains("trk") ) {

	// Print header
	cout << endl;
	AtlIDTrack::PrintHeader();

	// Print list
	TIter next_trk(fTracks);
	AtlIDTrack *trk = 0;
	while ( (trk = (AtlIDTrack*)next_trk()) ) {
	    trk->Print("nohead");
	}

	// Print footer
 	AtlIDTrack::PrintFooter();
    }
}

//____________________________________________________________________

void AtlJet::PrintHeader() {
    //
    // Print information header
    //
    cout
	<< "---------------------------------------------------------------------------------------------------------"
	<< endl
	<< " Id         Et          M         Theta         Phi         Eta        FakedBy      MC Flav   Quality   "
	<< endl
	<< "---------------------------------------------------------------------------------------------------------"
	<< endl;
}

//____________________________________________________________________

void AtlJet::PrintFooter() {
    //
    // Print footer
    //
    cout
	<< "---------------------------------------------------------------------------------------------------------"
	<< endl;
}

//____________________________________________________________________

AtlJet::EFlavour AtlJet::GetTruthFlavourByDeltaRmin(Float_t DeltaRmax) const {
    //
    // Return truth flavour by DeltaR information of next hadrons/taus
    // within a search cone of DeltaRmax. The flavour of the next
    // hadron/tau will be returned.
    //
    // NOTE: This method requires the data members fTruthDeltaRminBeauty,
    // fTruthDelatRminCharm and fTruthDeltaRminTau to be filled.
    //
    if ( fTruthDeltaRminBeauty <= DeltaRmax &&
	 fTruthDeltaRminBeauty < fTruthDeltaRminCharm &&
	 fTruthDeltaRminBeauty < fTruthDeltaRminTau ) return kBeauty;
    if ( fTruthDeltaRminCharm <= DeltaRmax &&
	 fTruthDeltaRminCharm < fTruthDeltaRminTau ) return kCharm;
    if ( fTruthDeltaRminTau <= DeltaRmax ) return kTau;
    return kLightFlavoured;
}


//____________________________________________________________________

void AtlJet::AddTrack(AtlIDTrack *trk, Float_t deltaR_max) {
//
//   Add track to jet if deltaR < deltaR_max 
//   
 
    Double_t dEta = Eta() - trk->Eta();
    Double_t dPhi = TVector2::Phi_mpi_pi(Phi() - trk->Phi());
    
    if(  sqrt( dEta*dEta+dPhi*dPhi ) < deltaR_max){
      this->AddTrack(trk);//this entspricht dann jet, da dieses Objekt die AddTrack Funktion aufruft
    }

  
}
