//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepDigit
#define HEP_HepDigit
#ifndef ROOT_TObject
#include <TObject.h>
#endif

class HepDigit : public TObject {

protected:
    Int_t   fId;   // Id number (for convenience only)
    
public:
    HepDigit();
    HepDigit(Int_t Id);
    virtual ~HepDigit();

    inline Int_t GetId() const { return fId; }
    inline void SetId(Int_t Id) { fId = Id; }
    
    ClassDef(HepDigit,1) // Abstract track hit class
};
#endif

