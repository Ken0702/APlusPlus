#ifndef PdfReweightingTool_h
#define PdfReweightingTool_h
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TList
#include <TList.h>
#endif
#ifndef ROOT_TSystem
#include <TSystem.h>
#endif
#ifndef PdfSet_h
#include "PdfReweightingTool/PdfSet.h"
#endif

class TH1F;

class PdfReweightingTool : public TObject {

 private:
  static PdfReweightingTool *fgPdfInstance; // unique instance of tool
  TList   *fListOfPdfSets;     // List of PdfSets
  TList   *fListOfObservables; // List of the analysis histograms
  Int_t    fNsets;             // Number of PdfSets stored
  TList   *fVarUp;             // List of histograms (central-value + error-up)
  TList   *fVarDown;           // List of histograms (central-value - error-down)
  Int_t    fDSID;              // DSID corresponding to the current normalization scale factor
  TString *fMCtag;             // MC tag corresponding to the current normalization scale factor

 public:
  PdfReweightingTool(const char* name = "");
  virtual ~PdfReweightingTool();
  static PdfReweightingTool* Instance(const char* name = "");

  void ComputePdfWeights(double x1, double x2, double scale, int pid1, int pid2, double pdf1, double pdf2);
  void AddObservable(TH1F* hist_obs);
  virtual void Print(Option_t* option = "") const;
  void   FillObservable( TH1F* hist_obs, float observable, float evt_weight );
  TList* EnvelopePlot(TH1F* hist_obs, Int_t bin);
  TH1F*  PdfUncertainty(TH1F* hist_obs, PdfSet::EVariation var);

  TList* GetListOfPdfSets() const { return fListOfPdfSets; }
  TList* GetVarUp() const { return fVarUp; }
  TList* GetVarDown() const { return fVarDown; }
  TList* GetReweightedObservable( TH1F* hist_obs, const char* pdf_set);
  void   GetNormfactor(Int_t DSID);
  void   SetApplyNormfactor(Bool_t set);

 private:
  void AddPdfSet(const char* pdfset, const char* path = "");

  ClassDef(PdfReweightingTool,0)
};
#endif
