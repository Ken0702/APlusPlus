//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlIDTrack
#define ATLAS_AtlIDTrack
#ifndef ATLAS_AtlTRTDigit
#include <AtlTRTDigit.h>
#endif
#ifndef ATLAS_AtlPixelHit
#include "AtlPixelHit.h"
#endif
#ifndef ATLAS_AtlSCT3DHit
#include "AtlSCT3DHit.h"
#endif
#ifndef ROOT_HepVtxTrackHelix
#include <HepVtxTrackHelix.h>
#endif
#ifndef ROOT_TRefArray
#include <TRefArray.h>
#endif
#ifndef HEP_HepMCParticle
#include <HepMCParticle.h>
#endif
#include <iostream>



class AtlIDTrack : public HepVtxTrackHelix {

private:
    TRefArray *fTRTHits;          // List of hits in the TRT belonging to this
			          // track
    TRefArray *fPixelHits;        // List of hits in the Pixels belonging to this
                                  // track
    TRefArray *fSCTHits;          // List of hits in the SCT belonging to this
                                  // track
    TRef      *fMCParticle;       // Link to the MC truth particle matched
			          // by Athena's hi-based matching
    Float_t    fMCParticleProb;   // Probability of hit-based truth matching

    Int_t fN_ContribPixelLayers;       // Number of contributing layers of the pixel detector
    Bool_t fExpectHitInBLayer;         // Boolean to know if b-layer hit is expected
    Int_t fN_BLayerHits;               // These are the hits in the first pixel layer, i.e. b-layer
    Int_t fN_BLayerOutliers;           // Number of blayer outliers
    Int_t fN_BLayerSharedHits;         // Number of Pixel b-layer hits shared by several tracks.
    Int_t fN_PixelHits;                // These are the pixel hits, including the b-layer
    Int_t fN_PixelOutliers;            // Number of pixel outliers
    Int_t fN_PixelHoles;               // Number of pixel layers on track with absence of hits
    Int_t fN_PixelSharedHits;          // Number of Pixel all-layer hits shared by several tracks.
    Int_t fN_GangedPixels;             // Number of pixels which have a ganged ambiguity.
    Int_t fN_GangedFlaggedFakes;       // Number of Ganged Pixels flagged as fakes
    Int_t fN_PixelDeadSensors;         // Number of dead pixel sensors crossed
    Int_t fN_PixelSpoiltHits;          // Number of pixel hits with broad errors (width/sqrt(12))
    Int_t fN_SCTHits;                  // Number of hits in SCT
    Int_t fN_SCTOutliers;              // Number of SCT outliers
    Int_t fN_SCTHoles;                 // Number of SCT holes
    Int_t fN_SCTDoubleHoles;           // Number of Holes in both sides of a SCT module
    Int_t fN_SCTSharedHits;            // Number of SCT hits shared by several tracks.
    Int_t fN_SCTDeadSensors;           // Number of dead SCT sensors crossed
    Int_t fN_SCTSpoiltHits;            // Number of SCT hits with broad errors (width/sqrt(12))
    Int_t fN_TRTHits;                  // Number of TRT hits
    Int_t fN_TRTOutliers;              // Number of TRT outliers
    Int_t fN_TRTHoles;                 // Number of TRT holes
    Int_t fN_TRTHighThresholdHits;     // Number of TRT hits which pass the high threshold
    Int_t fN_TRTHighThresholdOutliers; // Number of TRT high threshold outliers
    Int_t fN_TRTDeadStraws;            // Number of dead TRT straws crossed
    Int_t fN_TRTTubeHits;              // Number of TRT tube hits
    Float_t fPixeldEdx;                // dE/dx estimate, calculated using the pixel clusters 
    
  public:
    AtlIDTrack();
    AtlIDTrack(Int_t Id, Float_t Chi2, Int_t NDoF,
	       Float_t Xref, Float_t Yref, Float_t Zref, Float_t Phi0,
	       Float_t QovP, Float_t D0, Float_t Z0, Float_t Theta0, 
	       const Float_t CovMat[15] = 0);
    virtual ~AtlIDTrack();
    virtual void Clear(Option_t *option = "");
    virtual const HepMagneticField* GetMagneticField() const;
    virtual void Print(Option_t *option = "") const;
    static void PrintHeader();
    static void PrintFooter();
    void PrintHits();
    void AddTRTHit(AtlTRTDigit *hit);
    void AddPixelHit(AtlPixelHit *hit);
    void AddSCTHit(AtlSCT3DHit *hit);
    
    inline HepMCParticle* GetMCParticle() const {
	//
	// Return corresponding truth particle to this track (found by
	// Athena's hit-based truth matching algorithm) Please ask with 
	// AtlIDTrack->HasValidTruthMatch(Prob) for an existing MCParticle with
	// corresponding to a required 'Matching Probability' before using
	// GetMCParticle()
	//
	if (fMCParticle->GetObject() == 0)
	    std::cout << std::endl
		      << "WARNING !!!! RETURNED MCPARTICLE IS 0 !!!"  << std::endl
		      << "Please use HasValidTruthMatch(Prob) before!"
		      << std::endl << std::endl;
	return (HepMCParticle*)fMCParticle->GetObject(); 
    }
    inline Float_t GetMCParticleProb() const {
	//
	// Goodness of the hit-based track <-> particle truth matching
	//
	return fMCParticleProb; 
    }
    inline void SetMCParticle(HepMCParticle *mcprt,
			      Float_t prob) {
	//
	// Set hit-based matched MC truth particle
	//
	*fMCParticle = (TObject*)mcprt;
	fMCParticleProb = prob;
    }
    inline Bool_t HasValidTruthMatch(Float_t Prob) {
	//
	// Does this track have a valid MC truth match ?
	//
	if ( fMCParticle->GetObject() == 0) return kFALSE;
	else return ( fMCParticleProb > Prob ) ? kTRUE : kFALSE;
    }
    inline TRefArray* GetTRTHits() const {
	//
	// Get list of TRT hits
	// (only valid in case of ESDs)
	//
	return fTRTHits;
    }
    inline TRefArray* GetPixelHits() const {
	//
	// Get list of pixel detector hits
	// (only valid in case of ESDs)
	//
	return fPixelHits;
    }
    inline TRefArray* GetSCTHits() const {
	//
	// Get list of SCT hits
	// (only valid in case of ESDs)
	//
	return fSCTHits; }

    inline Int_t GetN_ContribPixelLayers() const {
	//
	// Get the number of contributing layers of the pixel detector
	//
	return fN_ContribPixelLayers;
    }
    inline Bool_t ExpectHitInBLayer() const {
      //
      // Get the info if b-layer hit is expected
      //
      return fExpectHitInBLayer;
    }
    inline Int_t GetN_BLayerHits() const {
	//
	// Get the number of hits in the first pixel layer, i.e. b-layer
	//
	return fN_BLayerHits;
    }
    inline Int_t Get_BLayerOutliers() const {
	//
	// Get the number of blayer outliers
	//
	return fN_BLayerOutliers;
    }
    inline Int_t GetN_BLayerSharedHit() const {
	//
	// Get the number of Pixel b-layer hits shared by several
	// tracks
	//
	return fN_BLayerSharedHits;
    }
    inline Int_t GetN_PixelHits() const {
	//
	// Get the number of pixel hits, including the b-layer
	//
	return fN_PixelHits;
    }
    inline Int_t GetN_PixelOutliers() const {
        //
        // Get the number of pixel hits, including the b-layer
        //
        return fN_PixelOutliers;
    }
    inline Int_t GetN_PixelHoles() const {
	//
	// Get the number of pixel layers on track with absence of
	// hits
	//
	return fN_PixelHoles;
    }
    inline Int_t GetN_PixelSharedHits() const {
	//
	// Get the number of Pixel all-layer hits shared by several
	// tracks
	//
	return fN_PixelSharedHits;
    }
    inline Int_t GetN_GangedPixels() const {
	//
	// Get the number of pixels which have a ganged ambiguity
	//
	return fN_GangedPixels;
    }
    inline Int_t GetN_GangedFlaggedFakes() const {
	//
	// Get the number of Ganged Pixels flagged as fakes
	//
	return fN_GangedFlaggedFakes;
    }
    inline Int_t GetN_PixelDeadSensors() const {
	//
	// Get the number of dead pixel sensors crossed
	//
	return fN_PixelDeadSensors;
    }
    inline Int_t GetN_PixelSpoiltHits() const {
	//
	// Get the number of pixel hits with broad errors
	// (width/sqrt(12))
	return fN_PixelSpoiltHits;
    }
    inline Int_t GetN_SCTHits() const {
	//
	// Get the number of hits in SCT
	//
	return fN_SCTHits;
    }
    inline Int_t GetN_SCTOutliers() const {
	//
	// Return the number of SCT outliers
	//
	return fN_SCTOutliers;
    }
    inline Int_t GetN_SCTHoles() const {
	//
	// Get the number of SCT holes
	//
	return fN_SCTHoles;
    }
    inline Int_t GetN_SCTDoubleHoles() const {
	//
	// Get the number of Holes in both sides of a SCT module
	//
	return fN_SCTDoubleHoles;
    }
    inline Int_t GetN_SCTSharedHits() const {
	//
	// Get the number of SCT hits shared by several tracks
	//
	return fN_SCTSharedHits;
    }
    inline Int_t GetN_SCTDeadSensors() const {
	//
	// Get the number of dead SCT sensors crossed
	//
	return fN_SCTDeadSensors;
    }
    inline Int_t GetN_SCTSpoiltHits() const {
	//
	// Get the number of SCT hits with broad errors
	// (width/sqrt(12))
	//
	return fN_SCTSpoiltHits;
    }
    inline Int_t GetN_TRTHits() const {
	//
	// Get the number of TRT hits
	//
	return fN_TRTHits;
    }
    inline Int_t GetN_TRTOutliers() const {
	//
	// Get the number of TRT outliers
	//
	return fN_TRTOutliers;
    }
    inline Int_t GetN_TRTHoles() const {
	//
	// Get the number of TRT holes
	//
	return fN_TRTHoles;
    }
    inline Int_t GetN_TRTHighThresholdHits() const {
	//
	// Get the number of TRT hits which pass the high threshold
	//
	return fN_TRTHighThresholdHits;
    }
    inline Int_t GetN_TRTHighThresholdOutliers() const {
	//
	// Gett the number of TRT high threshold outliers
	//
	return fN_TRTHighThresholdOutliers;
    }
    inline Int_t GetN_TRTDeadStraws() const {
	//
	// Get the number of dead TRT straws crossed
	//
	return fN_TRTDeadStraws;
    }
    inline Int_t GetN_TRTTubeHits() const {
	//
	// Get the number of TRT tube hits
	//
	return fN_TRTTubeHits;
    }
    inline Float_t GetPixeldEdx() const {
	//
	// Get the number of TRT tube hits
	//
	return fPixeldEdx;
    }
    inline void SetN_ContribPixelLayers(Int_t N_ContribPixelLayers) {
	//
	// Set the number of contributing layers of the pixel detector
	//
	fN_ContribPixelLayers = N_ContribPixelLayers;
    }
    inline void SetExpectHitInBLayer(Bool_t ExpectHitInBLayer) {
	//
	// Define if b-layer hit is expected
	//
	fExpectHitInBLayer = ExpectHitInBLayer;
    }
    inline void SetN_BLayerHits(Int_t N_BLayerHits) {
	//
	// Set the number of hits in the first pixel layer, i.e. b-layer
	//
	fN_BLayerHits = N_BLayerHits;
    }
    inline void SetN_BLayerOutliers(Int_t N_BLayerOutliers) {
	//
	// Set the number of blayer outliers
	//
	fN_BLayerOutliers = N_BLayerOutliers;
    }
    inline void SetN_BLayerSharedHits(Int_t N_BLayerSharedHits) {
	//
	// Set the number of Pixel b-layer hits shared by several
	// tracks
	//
	fN_BLayerSharedHits = N_BLayerSharedHits;
    }
    inline void SetN_PixelHits(Int_t N_PixelHits) {
	//
	// Set the number of pixel hits, including the b-layer
	//
	fN_PixelHits = N_PixelHits;
    }
    inline void SetN_PixelOutliers(Int_t N_PixelOutliers) {
        //
        // Set the number of pixel hits, including the b-layer
        //
        fN_PixelOutliers = N_PixelOutliers;
    }
    inline void SetN_PixelHoles(Int_t N_PixelHoles) {
	//
	// Setx the number of pixel layers on track with absence of
	// hits
	//
	fN_PixelHoles = N_PixelHoles;
    }
    inline void SetN_PixelSharedHits(Int_t N_PixelSharedHits) {
	//
	// Set the number of Pixel all-layer hits shared by several
	// tracks
	//
	fN_PixelSharedHits = N_PixelSharedHits;
    }
    inline void SetN_GangedPixels(Int_t N_GangedPixels) {
	//
	// Set the number of pixels which have a ganged ambiguity
	//
	fN_GangedPixels = N_GangedPixels;
    }
    inline void SetN_GangedFlaggedFakes(Int_t N_GangedFlaggedFakes) {
	//
	// Set the number of Ganged Pixels flagged as fakes
	//
	fN_GangedFlaggedFakes = N_GangedFlaggedFakes;
    }
    inline void SetN_PixelDeadSensors(Int_t N_PixelDeadSensors) {
	//
	// Set the number of dead pixel sensors crossed
	//
	fN_PixelDeadSensors = N_PixelDeadSensors;
    }
    inline void SetN_PixelSpoiltHits(Int_t N_PixelSpoiltHits) {
	//
	// Set the number of pixel hits with broad errors
	// (width/sqrt(12))
	fN_PixelSpoiltHits = N_PixelSpoiltHits;
    }
    inline void SetN_SCTHits(Int_t N_SCTHits) {
	//
	// Set the number of hits in SCT
	//
	fN_SCTHits = N_SCTHits;
    }
    inline void SetN_SCTOutliers(Int_t N_SCTOutliers) {
	//
	// the number of SCT outliers
	//
	fN_SCTOutliers = N_SCTOutliers;
    }
    inline void SetN_SCTHoles(Int_t N_SCTHoles) {
	//
	// Set the number of SCT holes
	//
	fN_SCTHoles = N_SCTHoles;
    }
    inline void SetN_SCTDoubleHoles(Int_t N_SCTDoubleHoles) {
	//
	// Set the number of Holes in both sides of a SCT module
	//
	fN_SCTDoubleHoles = N_SCTDoubleHoles;
    }
    inline void SetN_SCTSharedHits(Int_t N_SCTSharedHits) {
	//
	// Set the number of SCT hits shared by several tracks
	//
	fN_SCTSharedHits = N_SCTSharedHits;
    }
    inline void SetN_SCTDeadSensors(Int_t N_SCTDeadSensors) {
	//
	// Set the number of dead SCT sensors crossed
	//
	fN_SCTDeadSensors = N_SCTDeadSensors;
    }
    inline void SetN_SCTSpoiltHits(Int_t N_SCTSpoiltHits) {
	//
	// Set the number of SCT hits with broad errors
	// (width/sqrt(12))
	//
	fN_SCTSpoiltHits = N_SCTSpoiltHits;
    }
    inline void SetN_TRTHits(Int_t N_TRTHits) {
	//
	// Set the number of TRT hits
	//
	fN_TRTHits = N_TRTHits;
    }
    inline void SetN_TRTOutliers(Int_t N_TRTOutliers) {
	//
	// Set the number of TRT outliers
	//
	fN_TRTOutliers = N_TRTOutliers;
    }
    inline void SetN_TRTHoles(Int_t N_TRTHoles) {
	//
	// Set the number of TRT holes
	//
	fN_TRTHoles = N_TRTHoles;
    }
    inline void SetN_TRTHighThresholdHits(Int_t N_TRTHighThresholdHits) {
	//
	// Set the number of TRT hits which pass the high threshold
	//
	fN_TRTHighThresholdHits = N_TRTHighThresholdHits;
    }
    inline void SetN_TRTHighThresholdOutliers(Int_t N_TRTHighThresholdOutliers) {
	//
	// Sett the number of TRT high threshold outliers
	//
	fN_TRTHighThresholdOutliers = N_TRTHighThresholdOutliers;
    }
    inline void SetN_TRTDeadStraws(Int_t N_TRTDeadStraws) {
	//
	// Set the number of dead TRT straws crossed
	//
	fN_TRTDeadStraws = N_TRTDeadStraws;
    }
    inline void SetN_TRTTubeHits(Int_t N_TRTTubeHits) {
	//
	// Set the number of TRT tube hits
	//
	fN_TRTTubeHits = N_TRTTubeHits;
    }
    inline void SetPixeldEdx  (Float_t PixeldEdx  ) {
	//
	// Set dE/dx, calculated using the pixel clusters 
	//
	fPixeldEdx  = PixeldEdx ;
    }

    ClassDef(AtlIDTrack,11) // ATLAS inner detector track
};
#endif
