#ifndef ATLAS_AtlSubselectionCuts
#define ATLAS_AtlSubselectionCuts

#include <TNamed.h>
#include <TList.h>


class AtlSubselectionCuts : public TNamed {

public:

    explicit AtlSubselectionCuts(char const * name);
    ~AtlSubselectionCuts() /*override*/;

    virtual TList * GetListOfCuts() { return &fListOfCuts; }

protected:
    TList fListOfCuts;

    ClassDef(AtlSubselectionCuts, 0)
};
#endif
