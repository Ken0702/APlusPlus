#ifndef PdfSet_h
#define PdfSet_h
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif
#ifndef ROOT_TList
#include <TList.h>
#endif
#ifndef ROOT_TSystem
#include <TSystem.h>
#endif
#ifndef ROOT_TString
#include <TString.h>
#endif
#include <vector>

class TH1F;
class TH1D;
class TH2D;
namespace LHAPDF {
  class PDF;
}

class PdfSet : public TNamed {
 public:
  enum EVariation { kUp,
		    kDown };
  enum EPdf { kCTEQ = 0,
	      kMSTW,
	      kNNPDF };
  
  static const Int_t fgNumPdfSets = 3;         // Number of Pdf sets available
  static const char* fgPdfNames[fgNumPdfSets]; // PDF sets names

 private:
  const int fNset;                   // Unique LHAPDF ID
  std::vector<LHAPDF::PDF*> fPdfSet; // Storage of pdf set (0=central, >0 error pdfs)
  LHAPDF::PDF *fPdfCT10;             // Central CT10 pdf
  std::vector<double> *fPdfWeights;  // Storage of pdfweights (0=central, >0 error pdfs)

  UInt_t   fNMember;                  // Number of members in pdf set
  TList   *fListOfObservables;        // List of TH2D histograms, one for each observable
  EPdf     fPdfType;                  // Type of Pdf (defines pdf uncertainty calculation)
  std::vector<Double_t> fNormfactors; // Normalization factor for pdf set for each member
  Bool_t   fApplyNormfactor;          // Apply normallization factor in pdf reweighting
  
 public:
  PdfSet(const char* pdfset, const char* path, int nset);
  virtual ~PdfSet();
  Int_t GetPdfId() const { return fNset; }
  void ComputePdfWeights(double x1, double x2, double scale, int pid1, int pid2, double pdf1, double pdf2);
  void AddObservable(TH1F* hist_obs);
  TList* GetListOfObservables() const { return fListOfObservables; }
  void FillObservable(TH1F* hist_obs, float observable, float evt_weight);
  TH1F* PdfUncertainty(TH1F* hist_obs, EVariation var);
  Float_t ComputeVariation(TH1D* hist_proj, EVariation var);
  Float_t ComputeVariationCTEQ(TH1D* hist_proj, EVariation var);
  Float_t ComputeVariationMSTW(TH1D* hist_proj, EVariation var);
  Float_t ComputeVariationNNPDF(TH1D* hist_proj, EVariation var);
  UInt_t GetNMember() const { return fNMember; }
  std::vector<double>* GetPdfWeights() const { return fPdfWeights; }
  EPdf GetTypeByName(const char* name);
  void GetNormfactor(Int_t DSID);
  inline void SetApplyNormfactor(Bool_t set)  { fApplyNormfactor = set; }
  TString GetDescription() const;

  ClassDef(PdfSet,0);
};

#endif
