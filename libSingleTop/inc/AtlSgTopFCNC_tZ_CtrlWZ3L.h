//  
// Author: Ana Peixoto <mailto: ana.paula.pereira.peixoto@cern.ch>
//
#ifndef SGTOP_AtlSgTopFCNC_tZ_CtrlWZ3L
#define SGTOP_AtlSgTopFCNC_tZ_CtrlWZ3L
#ifndef SGTOP_AtlSgTopFCNC_tZ_Base3L
#include <AtlSgTopFCNC_tZ_Base3L.h>
#endif

class AtlSgTopFCNC_tZ_CtrlWZ3L : public AtlSgTopFCNC_tZ_Base3L {

  public:
    AtlSgTopFCNC_tZ_CtrlWZ3L(const char* OutputFileName);
    virtual ~AtlSgTopFCNC_tZ_CtrlWZ3L();

    virtual void BookHistograms();
    virtual void FillHistograms() {;}
    virtual Bool_t ProcessCut();
    virtual void SetCutDefaults();
    virtual void Print(Option_t *option) const;

    ClassDef(AtlSgTopFCNC_tZ_CtrlWZ3L,0) // Analysis class for the single-top FCNC tZ analysis, signal region
};
#endif

