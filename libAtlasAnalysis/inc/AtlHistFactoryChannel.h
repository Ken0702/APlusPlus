//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryChannel
#define ATLAS_AtlHistFactoryChannel
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif

class AtlHistFactorySystematic;
class AtlHistFactorySample;
class TList;

class AtlHistFactoryChannel : public TNamed {

protected:
    TString fDiscriminant;
    TString fMCPlotterBaseDir;

    TString *fShapeDiscriminantRef;
    TString *fShapeDiscriminantSyst;
    
    TList *fListOfSystematics;
    TList *fListOfSamples;

public:
    AtlHistFactoryChannel(const char* name,
			  const char* discriminant,
			  const char* MCPlotterBaseDir);
    virtual ~AtlHistFactoryChannel();

    void AddSample(AtlHistFactorySample *sample);
    void AddSystematic(AtlHistFactorySystematic *syst);
    void AddSystematics(TList *list);

    inline const char* GetMCPlotterBaseDir() const { return fMCPlotterBaseDir.Data(); }
    inline const char* GetDiscriminant() const { return fDiscriminant.Data(); }
    inline TString* GetShapeDiscriminantRef() { return fShapeDiscriminantRef; }
    inline TString* GetShapeDiscriminantSyst() { return fShapeDiscriminantSyst; }
    inline TList* GetListOfSamples() const { return fListOfSamples; }
    inline TList* GetListOfSystematics() const { return fListOfSystematics; }

    void SetShapeDiscriminants(const char* ShapeRef, const char* ShapeSyst);
    
    ClassDef(AtlHistFactoryChannel,0) // 
};
#endif

