//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepDatabasePDG
#define HEP_HepDatabasePDG
#ifndef ROOT_TDatabasePDG
#include <TDatabasePDG.h>
#endif

class TSystem;

class HepDatabasePDG : public TDatabasePDG {

private:
    static HepDatabasePDG *fgHepInstance; // HepPDG dbase singleton
    
public:
    HepDatabasePDG();
    virtual ~HepDatabasePDG();
    static HepDatabasePDG* Instance();
    void AddParticlesToPdgDbase();
    
    ClassDef(HepDatabasePDG,0) // Extended PDG dbase
};
#endif

