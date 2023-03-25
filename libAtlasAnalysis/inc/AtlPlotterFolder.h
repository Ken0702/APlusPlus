
#ifndef ATLAS_AtlPlotterFolder
#define ATLAS_AtlPlotterFolder
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif
#ifndef ROOT_TList
#include <TList.h>
#endif
#ifndef ATLAS_AtlSample
#include <AtlSample.h>
#endif


class AtlPlotterFolder : public TNamed {

  public:
    
  private:
    TString fColor;         // Folder color
    
    TList   *fListOfSamples; //List of all samples in this folder
    
  public:
    AtlPlotterFolder(const char* Name, const char* Title,
		     const char* Color);
    virtual ~AtlPlotterFolder();

    void AddSample(AtlSample* sample);
    
    inline TList      *GetListOfSamples() { return fListOfSamples; }
    inline const char *GetColor()         { return fColor; }

    
    ClassDef(AtlPlotterFolder,0) // A++ plotting task MC folder
};
#endif

