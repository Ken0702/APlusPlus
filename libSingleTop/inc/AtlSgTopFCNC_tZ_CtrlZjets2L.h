//  
// Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Oliver Maria Kind 
//
#ifndef SGTOP_AtlSgTopFCNC_tZ_CtrlZjets2L
#define SGTOP_AtlSgTopFCNC_tZ_CtrlZjets2L
#ifndef SGTOP_AtlSgTopFCNC_tZ_Base
#include <AtlSgTopFCNC_tZ_Base.h>
#endif

class AtlSgTopFCNC_tZ_CtrlZjets2L : public AtlSgTopFCNC_tZ_Base {

  public:
    AtlSgTopFCNC_tZ_CtrlZjets2L(const char* OutputFileName);
    virtual ~AtlSgTopFCNC_tZ_CtrlZjets2L();

    virtual void BookHistograms();
    virtual void FillHistograms() {;}
    Bool_t ProcessPreCut() /* override */;
    virtual Bool_t ProcessCut();
    virtual void SetCutDefaults();
    virtual void Print(Option_t *option) const;
    
    ClassDef(AtlSgTopFCNC_tZ_CtrlZjets2L,0) // Analysis class for the single-top FCNC tZ analysis, Z+jets di-lepton ctrl region
};
#endif

