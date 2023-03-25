//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlFastTau
#define ATLAS_AtlFastTau
#ifndef HEP_HepTau
#include <HepTau.h>
#endif

class AtlFastTau : public HepTau {

private:

public:
    AtlFastTau();
    AtlFastTau(Int_t Id, Float_t Px, Float_t Py, Float_t Pz, Float_t E,
	   Bool_t IsTauPlus);
    virtual ~AtlFastTau();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();

    ClassDef(AtlFastTau,1) // Atlas tau class
};
#endif
