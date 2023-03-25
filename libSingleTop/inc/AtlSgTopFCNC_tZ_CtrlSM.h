//  
// Author: Ana Peixoto  <mailto: ana.paula.pereira.peixoto@cern.ch>
// Update: $Id$
// Copyright: 2015 (C) Ana Peixoto
//
#ifndef SGTOP_AtlSgTopFCNC_tZ_CtrlSM
#define SGTOP_AtlSgTopFCNC_tZ_CtrlSM
#ifndef SGTOP_AtlSgTopFCNC_tZ_Base3L
#include <AtlSgTopFCNC_tZ_Base3L.h>
#endif

class AtlSgTopFCNC_tZ_CtrlSM : public AtlSgTopFCNC_tZ_Base3L {

  public:
    AtlSgTopFCNC_tZ_CtrlSM(const char* OutputFileName);
    virtual ~AtlSgTopFCNC_tZ_CtrlSM();

    virtual void BookHistograms();
    virtual void FillHistograms() {;}
    virtual Bool_t ProcessCut();
    virtual void SetCutDefaults();
    virtual void Print(Option_t *option) const;
    
    ClassDef(AtlSgTopFCNC_tZ_CtrlSM,0) // Analysis class for the single-top FCNC tZ analysis, SM trilepton ctrl region
};
#endif


