//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepEvent
#define HEP_HepEvent
#ifndef ROOT_TObject
#include <TObject.h>
#endif

class HepEvent : public TObject {

private:
    
public:
    HepEvent();
    virtual ~HepEvent();

    ClassDef(HepEvent,2) // Abstract HEP event class
};
#endif

