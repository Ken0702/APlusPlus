//  
// Author:  <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2016 (C) 
//
#ifndef SGTOP_AtlSgTopFCNC_tZ_Base3L
#define SGTOP_AtlSgTopFCNC_tZ_Base3L
#ifndef ROOT_AtlSgTopFCNC_tZ_Base
#include <AtlSgTopFCNC_tZ_Base.h>
#endif

class AtlSgTopFCNC_tZ_Base3L : public AtlSgTopFCNC_tZ_Base {

private:
    
public:
    AtlSgTopFCNC_tZ_Base3L(const char* OutputFileName);
    virtual ~AtlSgTopFCNC_tZ_Base3L();
    void Clear(Option_t *) /* override */;
    
    virtual void BookHistograms();
    virtual void FillHistograms() {;}
    Bool_t ProcessPreCut() /* override */;
    virtual Bool_t ProcessCut() { return kTRUE; }
    virtual void SetCutDefaults();
    virtual void Print(Option_t *option) const;
    virtual void Terminate();
       
protected:
    void ReconstructZ0Top();
    void ReconstructTopDecay(HepParticle *lepton, AtlJet *jet);
    void ReconstructWDecay(HepParticle *lep);
    void ReconstructNeutrino(HepParticle *lep);
    virtual void BookHistogramsTop(AtlHistogramTool *htool);
    virtual void FillHistogramsTop(AtlHistogramTool *htool, Double_t w);
    virtual void CreatePieCharts();

    void const * fReconstructedTopFrom; // whether top reconstruction has run
    void const * fReconstructedWFrom; // whether W reconstruction has run
    void const * fReconstructedNeutrinoFrom; // whether neutrino reconstruction has run
    
    ClassDef(AtlSgTopFCNC_tZ_Base3L,0) // SgTop FCNC tZ 3L base class
};
#endif

