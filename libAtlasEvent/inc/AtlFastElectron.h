//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlFastElectron
#define ATLAS_AtlFastElectron
#ifndef HEP_HepElectron
#include <HepElectron.h>
#endif

class AtlFastElectron : public HepElectron {

private:

public:
    AtlFastElectron();
    AtlFastElectron(Int_t Id, Float_t Px, Float_t Py, Float_t Pz, Float_t E,
	Bool_t IsPositron);
    virtual ~AtlFastElectron();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();

    ClassDef(AtlFastElectron,1) // Atlas electron class
};
#endif
