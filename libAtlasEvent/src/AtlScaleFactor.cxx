//____________________________________________________________________
//
// Store a scale factor for the event weight.
//
// While systematic variations are currently not implemented, support
// will be added, probably using an array of floating-point numbers.
// Such an array would have O(100) entries, so in general you do not
// want to store AtlScaleFactor objects on disk.

#include "AtlScaleFactor.h"

#include <cassert>
#include <cmath>

using namespace std;


AtlScaleFactor const AtlScaleFactor::nil;

//____________________________________________________________________

AtlScaleFactor::AtlScaleFactor()
    : fNominalValue(NAN)
{
}

//____________________________________________________________________

AtlScaleFactor::~AtlScaleFactor() {
}

//____________________________________________________________________

void AtlScaleFactor::Clear(char const *) {
    fNominalValue = NAN;
}

//____________________________________________________________________

double AtlScaleFactor::GetValue() const {
    // In v25, weight_leptonSF_tight is NaN sometimes, but these events
    // should disappear after proper object definitions. The check for
    // nan should therefore be done in GetValue(), not SetValue().
    if ( isnan(fNominalValue) )
        Fatal(__FUNCTION__, "fNominalValue is nan!");
    return fNominalValue;
}

//____________________________________________________________________

void AtlScaleFactor::SetValue(float nominalValue) {
    // In v25, weight_leptonSF_tight is NaN sometimes, but these events
    // should disappear after proper object definitions. The check for
    // nan should therefore be done in GetValue(), not SetValue().
    assert(isnan(fNominalValue));
    fNominalValue = nominalValue;
}
