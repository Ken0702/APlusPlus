//
// Author: Stephan Kaphle <mailto: kaphle@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2018 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTRExFitterTask
#define ATLAS_AtlTRExFitterTask

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Option_t
#include <TString.h>     // for TString, ROOT_TString
#include <TTask.h>       // for TTask
#include <vector>        // for vector
class TBuffer;
class TClass;
class TDirectoryFile;
class TMemberInspector;
class TObjArray;

class AtlTRExFitterTask : public TTask {

private:
    TString        fInputDataMCFile; // Input DataMCPlot file name
    TString        fInputDiscrPath;  // Name and full path of the input dicriminant
    TString        fOutputDir;       // Output directory
    TString        fOutputDiscrName; // Name of the output discriminant templates
    std::vector<TString> GetHistogramPaths(TDirectoryFile *dir);

public:
    AtlTRExFitterTask(const char* name, const char* title);
    virtual ~AtlTRExFitterTask();
    virtual void Exec(Option_t *option);

    void SetInputDataMCFile(const char* InputDataMCFile); //! *MENU*
    void SetInputDiscrPath(const char* InputDiscrPath); //! *MENU*
    void SetOutputDir(const char* OutputDir); // *MENU*
    void SetOutputDiscrName(const char* OutputDiscrName); // *MENU*

    ClassDef(AtlTRExFitterTask,0) // TRExFitter task
};
#endif
