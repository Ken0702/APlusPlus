//  
// Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Oliver Maria Kind 
//
#ifndef SGTOP_AtlSgTopFCNC_tZ_CtrlZjets3L
#define SGTOP_AtlSgTopFCNC_tZ_CtrlZjets3L
#ifndef SGTOP_AtlSgTopFCNC_tZ_Base3L
#include <AtlSgTopFCNC_tZ_Base3L.h>
#endif

class AtlSgTopFCNC_tZ_CtrlZjets3L : public AtlSgTopFCNC_tZ_Base3L {

  public:
    AtlSgTopFCNC_tZ_CtrlZjets3L(const char* OutputFileName);
    virtual ~AtlSgTopFCNC_tZ_CtrlZjets3L();

    virtual void BookHistograms();
    virtual Bool_t ProcessCut();
    virtual void SetCutDefaults();
    virtual void Print(Option_t *option) const;
    
    ClassDef(AtlSgTopFCNC_tZ_CtrlZjets3L,0) // Analysis class for the single-top FCNC tZ analysis, Z+jets trilepton ctrl region
};
#endif

