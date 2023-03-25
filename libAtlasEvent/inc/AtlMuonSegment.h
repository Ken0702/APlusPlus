//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlMuonSegment
#define ATLAS_AtlMuonSegment
#ifndef ROOT_HepVtxTrackHelix
#include <HepVtxTrackHelix.h>
#endif
#ifndef ROOT_HepTrackHelix
#include <HepTrackHelix.h>
#endif

class AtlMuonSegment : public HepTrackHelix {

private:

public:
    AtlMuonSegment();
    AtlMuonSegment(Int_t Id, Float_t Chi2, Int_t NDoF,
		   Float_t Xref, Float_t Yref, Float_t Zref, Float_t Phi0,
		   Float_t QovR, Float_t QxD0, Float_t Z0, Float_t TDip, 
		   Float_t CovMat[15] = 0);
    virtual ~AtlMuonSegment();
    virtual const HepMagneticField* GetMagneticField() const;

    ClassDef(AtlMuonSegment,2) // Muon track segment
};
#endif

