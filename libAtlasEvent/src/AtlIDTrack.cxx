//____________________________________________________________________
//
// ATLAS inner detector track
// 
// Note that the list of hits for the sub-detectors are filled only in
// case of converted ESDs. The AODs do not contain such information
// anymore. Here only the number of hits per track is stored.
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlIDTrack
#include <AtlIDTrack.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlIDTrack);
#endif

//____________________________________________________________________

AtlIDTrack::AtlIDTrack() {
    //
    // Default constructor
    //
    fTRTHits    = new TRefArray;
    fPixelHits  = new TRefArray;
    fSCTHits    = new TRefArray;
    fMCParticle = new TRef;
}

//____________________________________________________________________

AtlIDTrack::AtlIDTrack(Int_t Id, Float_t Chi2, Int_t NDoF,
		       Float_t Xref, Float_t Yref, Float_t Zref,
		       Float_t Phi0, Float_t QovP, Float_t D0,
		       Float_t Z0, Float_t Theta0, const Float_t CovMat[15]) :
    HepVtxTrackHelix(Id, Chi2, NDoF, Xref, Yref, Zref, Phi0, QovP, D0, Z0,
		     Theta0, CovMat),
    
    fN_ContribPixelLayers(0),
    fExpectHitInBLayer(0),
    fN_BLayerHits(0),
    fN_BLayerOutliers(0),
    fN_BLayerSharedHits(0),
    fN_PixelHits(0),
    fN_PixelOutliers(0),
    fN_PixelHoles(0),
    fN_PixelSharedHits(0), 
    fN_GangedPixels(0),
    fN_GangedFlaggedFakes(0),
    fN_PixelDeadSensors(0), 
    fN_PixelSpoiltHits(0),
    fN_SCTHits(0),   
    fN_SCTOutliers(0),
    fN_SCTHoles(0),
    fN_SCTDoubleHoles(0),
    fN_SCTSharedHits(0),
    fN_SCTDeadSensors(0),
    fN_SCTSpoiltHits(0),
    fN_TRTHits(0),
    fN_TRTOutliers(0),
    fN_TRTHoles(0),
    fN_TRTHighThresholdHits(0),
    fN_TRTHighThresholdOutliers(0),
    fN_TRTDeadStraws(0),
    fN_TRTTubeHits(0),
    fPixeldEdx(0) {
  
  //
  // Normal constructor
  //
  fTRTHits    = new TRefArray;
  fPixelHits  = new TRefArray;
  fSCTHits    = new TRefArray;
  fMCParticle = new TRef;
}

//____________________________________________________________________

AtlIDTrack::~AtlIDTrack() {
    //
    // Default destructor
    //
    delete fTRTHits;    fTRTHits    = 0;
    delete fPixelHits;  fPixelHits  = 0;
    delete fSCTHits;    fSCTHits    = 0;
    delete fMCParticle; fMCParticle = 0;
}

//____________________________________________________________________

void AtlIDTrack::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepVtxTrackHelix::Clear(option);
    
    // Clear references to hits
    delete fTRTHits;    fTRTHits    = 0;
    delete fPixelHits;  fPixelHits  = 0;
    delete fSCTHits;    fSCTHits    = 0;
    delete fMCParticle; fMCParticle = 0; 
    fMCParticleProb = 0.;

    fN_ContribPixelLayers = 0;
    fExpectHitInBLayer = kFALSE;
    fN_BLayerHits = 0;        
    fN_BLayerOutliers = 0;    
    fN_BLayerSharedHits = 0;  
    fN_PixelHits = 0;
    fN_PixelOutliers = 0;        
    fN_PixelHoles = 0;      
    fN_PixelSharedHits = 0;   
    fN_GangedPixels = 0;      
    fN_GangedFlaggedFakes = 0;
    fN_PixelDeadSensors = 0;  
    fN_PixelSpoiltHits = 0;   
    fN_SCTHits = 0;          
    fN_SCTOutliers = 0;      
    fN_SCTHoles = 0;         
    fN_SCTDoubleHoles = 0;   
    fN_SCTSharedHits = 0;    
    fN_SCTDeadSensors = 0;   
    fN_SCTSpoiltHits = 0;    
    fN_TRTHits = 0;          
    fN_TRTOutliers = 0;      
    fN_TRTHoles = 0;         
    fN_TRTHighThresholdHits = 0;
    fN_TRTHighThresholdOutliers = 0;
    fN_TRTDeadStraws = 0;           
    fN_TRTTubeHits = 0;    
    fPixeldEdx = 0;
}

//____________________________________________________________________

const HepMagneticField* AtlIDTrack::GetMagneticField() const {
    //
    // Return magnetic field
    //
    static HepMagneticField *b_field = 0;
    if ( b_field == 0 ) {
	b_field = new HepMagneticField(0., 0., 1.4,
				       HepMagneticField::kSolenoid);
    }
    return b_field;
}

//____________________________________________________________________

void AtlIDTrack::PrintHits() {
    //
    // Print all digitisations belonging to this track
    //
    cout << endl << "#TRT hits: " << GetN_TRTHits()
	 << "   #HT hits: " << GetN_TRTHighThresholdHits() << endl;
    AtlTRTDigit::PrintHeader();
    TIter next_hit(fTRTHits);
    AtlTRTDigit *hit = 0;
    while ( (hit = (AtlTRTDigit*)next_hit()) )
	hit->Print("nohead");
    AtlTRTDigit::PrintFooter();
}

//____________________________________________________________________

void AtlIDTrack::AddTRTHit(AtlTRTDigit *hit) {
    //
    // Add TRT hit
    //
    fTRTHits->Add((TObject*)hit);
}

//____________________________________________________________________

void AtlIDTrack::AddPixelHit(AtlPixelHit *hit) {
    //
    // Add Pixel hit
    //
    fPixelHits->Add((TObject*)hit);
}

//____________________________________________________________________

void AtlIDTrack::AddSCTHit(AtlSCT3DHit *hit) {
    //
    // Add SCT hit
    //
    fSCTHits->Add((TObject*)hit);
}

//____________________________________________________________________

void AtlIDTrack::Print(Option_t *option) const {
    //
    // Print track content
    //   "PRT" - Print corresponding MC truth particle
    //   "covmat" - Print content of covariance matrix
    //   "nohead" - No header containing the variable names is
    //              displayed. Useful when printing a whole table
    //              for a list of particles
    //
    TString opt = option;
    opt.ToLower();
    
    // Header
    if ( !opt.Contains("nohead") ) AtlIDTrack::PrintHeader();

    // Main
    PrintHelixParameters();
    PrintVtxType();
    PrintClone();
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.width(2); cout << fN_BLayerHits;
    cout.width(2); cout << fN_PixelHits;
    cout.width(3); cout << fN_SCTHits;
    cout.width(3); cout << fN_TRTHits;
    cout << endl;
    
    // Covariance matrix
    if ( opt.Contains("covmat") ) PrintCovMatrix();
    
    // Print MC truth partcle
    if ( opt.Contains("prt") ) {
	cout << endl;
	HepMCParticle *prt = GetMCParticle();
	if ( prt == 0 ) {
	    cout << "No valid truth match found." << endl;
	} else {
	    cout << "MC truth match probability = "
		 << fMCParticleProb << endl;
	    prt->Print();
	}
    }

    // Footer
    if ( !opt.Contains("nohead") ) AtlIDTrack::PrintFooter();
}

//____________________________________________________________________

void AtlIDTrack::PrintHeader() {
    //
    // Print information header
    //
    cout << "--------------------------------------------------------------------------------------------------------------------------"
	 << endl
         << " Id   P (GeV)  Pt (GeV)  Pz (GeV)    Theta      Phi    Eta  Q D0 (cm) Z_0 (cm)   chi^2/DoF       Vtx type  Clone B P SC TR"
	 << endl
	 << "--------------------------------------------------------------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void AtlIDTrack::PrintFooter() {
    //
    // Print footer
    //
    cout << "--------------------------------------------------------------------------------------------------------------------------"
	 << endl;
    cout << "(#Hits: B=BLayer P=Pixel SC=SCT TR=TRT)" << endl;
}
