#include "AtlSubselectionCuts.h"

//____________________________________________________________________

AtlSubselectionCuts::AtlSubselectionCuts(char const * name) :
    TNamed(name, name) {
    fListOfCuts.SetOwner(kTRUE);
}

//____________________________________________________________________

AtlSubselectionCuts::~AtlSubselectionCuts() {
}

