//  
// Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2016 (C) Oliver Maria Kind 
//
#ifndef SGTOP_AtlSgTopFCNC_tZ_CtrlTtbar
#define SGTOP_AtlSgTopFCNC_tZ_CtrlTtbar
#ifndef SGTOP_AtlSgTopFCNC_tZ_Base3L
#include <AtlSgTopFCNC_tZ_Base3L.h>
#endif

class AtlSgTopFCNC_tZ_CtrlTtbar : public AtlSgTopFCNC_tZ_Base3L {

  public:
    AtlSgTopFCNC_tZ_CtrlTtbar(const char* OutputFileName);
    virtual ~AtlSgTopFCNC_tZ_CtrlTtbar();

    virtual void BookHistograms();
    virtual void FillHistograms() {;}
    virtual Bool_t ProcessCut();
    virtual void SetCutDefaults();
    virtual void Print(Option_t *option) const;

    ClassDef(AtlSgTopFCNC_tZ_CtrlTtbar,0) // Analysis class for the single-top FCNC tZ analysis, ttbar ctrl region
};
#endif

