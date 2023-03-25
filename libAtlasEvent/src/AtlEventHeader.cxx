//____________________________________________________________________
//
// ATLAS event header
//
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEventHeader
#include <AtlEventHeader.h>
#endif
#include <TDatime.h>
#include <cmath>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlEventHeader);
#endif

AtlEvtTreeInfo *AtlEventHeader::fgTreeInfo = 0;

//____________________________________________________________________

AtlEventHeader::AtlEventHeader()
    : fPreselectionFlags(-1)
    , fTruthFlavor(-1)
    , fMEPhotonFlag(-1)
    , fJetVertexWeight(new AtlScaleFactor())
    , fLeptonWeight(new AtlScaleFactor())
    , fBtagWeight_MV2c10_60(new AtlScaleFactor())
    , fBtagWeight_MV2c10_70(new AtlScaleFactor())
    , fBtagWeight_MV2c10_77(new AtlScaleFactor())
    , fBtagWeight_MV2c10_85(new AtlScaleFactor())
    , fBtagWeight_MV2c20_77(new AtlScaleFactor())
    , fBtagWeight_MV2c20_85(new AtlScaleFactor())
    , fBtagWeight_DL1r_85(new AtlScaleFactor())
	, fBtagWeight_DL1r_77(new AtlScaleFactor())
	, fBtagWeight_DL1r_70(new AtlScaleFactor())
	, fBtagWeight_DL1r_60(new AtlScaleFactor())
    , fForwardJVTWeight(new AtlScaleFactor())
    , fLeptonTriggerSF(new AtlScaleFactor())
          {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlEventHeader::~AtlEventHeader() {
    //
    // Default destructor
    //
    delete fBtagWeight_MV2c10_60;
    delete fBtagWeight_MV2c10_70;
    delete fBtagWeight_MV2c10_77;
    delete fBtagWeight_MV2c10_85;
    delete fBtagWeight_MV2c20_77;
    delete fBtagWeight_MV2c20_85;
    delete fBtagWeight_DL1r_85;
    delete fBtagWeight_DL1r_77;
    delete fBtagWeight_DL1r_70;
    delete fBtagWeight_DL1r_60;
    delete fLeptonWeight;
    delete fJetVertexWeight;
    delete fForwardJVTWeight;
    delete fLeptonTriggerSF;
}

//____________________________________________________________________

void AtlEventHeader::Clear(Option_t *option) {
    //
    // Clear event header
    //
    fRunNr           = 0;
    fChannelNr       = 0;
    fHFOR_type       = -1;
    fEventNr         = 0;
    fLumiBlock       = 0;
    fBeamEnergy      = 0;
    fEventType       = kPhysics;
    fIsMC            = kFALSE;
    fPreTagEvtWeight = 1.;
    fTagEvtWeight    = 1.;
    fMCWeight        = 1.;
    fPileupWeight    = 1.;
    fZVtxWeight      = 1.;
    fTruthWeightCorrection = NAN;
    fActualIntPerXing     = 0;
    fAverageIntPerXing    = 0;
    fmcevt_pdf1 = 0;
    fmcevt_pdf2 = 0;
    fmcevt_pdf_id1 = 0;
    fmcevt_pdf_id2 = 0;
    fmcevt_pdf_scale = 0;
    fmcevt_pdf_x1 = 0;
    fmcevt_pdf_x2 = 0;
    fPreselectionFlags = -1;
    fTruthFlavor = -1;

    fJetVertexWeight->Clear();
    fLeptonWeight->Clear();
    fBtagWeight_MV2c10_60->Clear();
    fBtagWeight_MV2c10_70->Clear();
    fBtagWeight_MV2c10_77->Clear();
    fBtagWeight_MV2c10_85->Clear();
    fBtagWeight_MV2c20_77->Clear();
    fBtagWeight_MV2c20_85->Clear();
    fBtagWeight_DL1r_85->Clear();
    fBtagWeight_DL1r_77->Clear();
    fBtagWeight_DL1r_70->Clear();
    fBtagWeight_DL1r_60->Clear();
    fForwardJVTWeight->Clear();
    fLeptonTriggerSF->Clear();
}

//____________________________________________________________________

void AtlEventHeader::Print(Option_t *option) const {
    //
    // Print object information
    //
    cout.width(25); cout << "Run : " << fRunNr << endl;
    cout.width(25); cout << "Event : " << fEventNr << endl;
    cout.width(25); cout << "Lumi Block : " << fLumiBlock << endl;
    cout.width(25); cout << "BeamEnergy : " << fBeamEnergy/1000. << " TeV" << endl;
    cout.width(25); cout << "AverageIntPerXing : " << fAverageIntPerXing << endl;
    cout.width(25); cout << "ActualIntPerXing  : " << fActualIntPerXing << endl;

    cout.width(25); cout  << "Event type : ";
    PrintEventType();
    cout << endl;
    cout.width(25); cout << "PreTag-Event weight : " << fPreTagEvtWeight << endl;
    cout.width(25); cout << "   Tag-Event weight : " << fTagEvtWeight << endl;
    cout << endl;
}

//____________________________________________________________________

void AtlEventHeader::PrintEventType() const {
    //
    // Print event type
    //
    if ( IsMC() ) {
	cout << "MC";
    } else {
	cout << "DATA";
    }
    if ( IsPhysics() )     cout << " physics run";
    if ( IsTestBeam() )    cout << " test-beam operation";
    if ( IsCalibration() ) cout << " calibration run";
    if ( IsCosmics() )     cout << " cosmics run";
}

//____________________________________________________________________

void AtlEventHeader::SetPreselectionFlags(ULong_t PreselectionFlags) {
    fPreselectionFlags = PreselectionFlags;
}

//____________________________________________________________________

void AtlEventHeader::SetTruthFlavor(Long_t TruthFlavor) {
    fTruthFlavor = TruthFlavor;
}

//____________________________________________________________________

void AtlEventHeader::SetMEPhoton(bool flag) {
    fMEPhotonFlag = (flag ? 1 : 0);
}

//____________________________________________________________________

bool AtlEventHeader::HasMEPhoton() const {
    if (fMEPhotonFlag < 0)
       Fatal(__FUNCTION__, "MEPhoton flag not defined");
    return (fMEPhotonFlag > 0);
}
