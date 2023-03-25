//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlFastJet
#define ATLAS_AtlFastJet
#ifndef HEP_HepJet
#include <HepJet.h>
#endif

class AtlFastJet : public HepJet {

private:

public:
    AtlFastJet();
    AtlFastJet(Int_t Id, Float_t E, Float_t Px, Float_t Py, Float_t Pz);
    virtual ~AtlFastJet();
    
    ClassDef(AtlFastJet,1) // Atlas MC truth jet
};
#endif

