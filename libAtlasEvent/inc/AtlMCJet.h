//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlMCJet
#define ATLAS_AtlMCJet
#ifndef ROOT_HepJet
#include <HepJet.h>
#endif

class AtlMCJet : public HepJet {

private:

public:
    AtlMCJet();
    AtlMCJet(Int_t Id, Float_t E, Float_t Px, Float_t Py, Float_t Pz);
    virtual ~AtlMCJet();
    void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();

    ClassDef(AtlMCJet,1) // Atlas MC truth jet
};
#endif

