#ifndef ATLAS_AtlScaleFactor
#define ATLAS_AtlScaleFactor

#include <Rtypes.h>
#include <TObject.h>


class AtlScaleFactor : public TObject {

public:

    AtlScaleFactor();
    ~AtlScaleFactor() /*override*/;

    void Clear(char const * = "") /*override*/;
    virtual double GetValue() const;
    virtual void SetValue(float nominalValue);

    static AtlScaleFactor const nil; // placeholder for an undefined AtlScaleFactor instance

private:

    Float_t fNominalValue; //! scale factor

    ClassDef(AtlScaleFactor, 0);
};

#endif
