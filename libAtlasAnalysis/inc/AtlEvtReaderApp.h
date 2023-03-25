//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtReaderApp
#define ATLAS_AtlEvtReaderApp
#ifndef ATLAS_AtlEvtReaderBase
#include <AtlEvtReaderBase.h>
#endif

class AtlEvtReaderApp : public AtlEvtReaderBase {

private:

public:
    explicit AtlEvtReaderApp(AtlSelector *parent);
    virtual ~AtlEvtReaderApp();
    virtual void SetBranches(TTree *t) override;
    virtual Int_t GetEntry(TTree *t, Long64_t entry = 0) override;

protected:
    virtual void BuildEvent() override {;}
    virtual void BuildEventHeader() override {;}

    ClassDefOverride(AtlEvtReaderApp,0) // Event reader for A++ events
};
#endif
