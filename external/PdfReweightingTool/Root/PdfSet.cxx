////////////////////////////////////////////////////////////////
//                                                            //
// PdfSet                                                    //
//                                                            //
// Wrapper class for LHAPDF - pdfset                          //
//                                                            //
//                                                            //
// Author: Soren Stamm, <soren.stamm@cern.ch>                 //
//                                                            //
////////////////////////////////////////////////////////////////

#include "PdfReweightingTool/PdfSet.h"
#include <LHAPDF/LHAPDF.h>
#include <iostream>
#include <fstream>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TROOT.h>
#include <TString.h>
#include <TObjString.h>
#include <TObjArray.h>

using namespace std;

#ifndef __CINT__
ClassImp(PdfSet);
#endif

// names without ".LHgrid"
const char* PdfSet::fgPdfNames[PdfSet::fgNumPdfSets] = {"CT10nlo",
							"MSTW2008nlo68cl",
							"NNPDF23_nlo_as_0119"};

//__________________________________________________________________

PdfSet::PdfSet(const char* pdfset, const char* path, int nset )
  : TNamed(pdfset, path), fNset(nset) {
  //
  // Default constructor
  //

  // Init PdfSet
  LHAPDF::mkPDFs(pdfset, fPdfSet);
  fPdfCT10 = LHAPDF::mkPDF(fgPdfNames[0], 0);
  fPdfWeights        = new std::vector<double>;
  fNMember           = fPdfSet.size();
  fListOfObservables = new TList;

  fPdfType = GetTypeByName(fName.Data());
  fNormfactors.clear();
}

//__________________________________________________________________

PdfSet::~PdfSet() {
  //
  // Default destructor
  //
  for (unsigned int i = 0; i < fPdfSet.size(); ++i) {
    delete fPdfSet.at(i);
  }
  delete fPdfCT10;
  delete fPdfWeights;
  fListOfObservables->Delete();
  delete fListOfObservables;
}

//__________________________________________________________________

void PdfSet::ComputePdfWeights(double x1, double x2, double scale,
			       int pid1, int pid2,
			       double pdf1, double pdf2) {
  //
  // Compute new pdf weights for central value and all error pdf sets
  // 
  //
 
  // clear pdf weights from previous call
  fPdfWeights->clear();

  double pdf_weight=1.0;

  // reweight with CT10 as default (for all LO generators)
  // CT10 member 0
  double ct10_w1 = fPdfCT10->xfxQ(pid1, x1, scale);
  double ct10_w2 = fPdfCT10->xfxQ(pid2, x2, scale);

  for ( unsigned int i = 0; i < fNMember; i++ ) {
    double nw1 = fPdfSet.at(i)->xfxQ(pid1, x1, scale);
    double nw2 = fPdfSet.at(i)->xfxQ(pid2, x2, scale);

    // Pdf Reweighting
    // ---------------

    // reweighted to NLO
    pdf_weight = nw1*nw2/(ct10_w1*ct10_w2);
    
    // Apply normalization scale factor
    if ( fApplyNormfactor ) {
      pdf_weight = pdf_weight*fNormfactors.at(i);
    }
    fPdfWeights->push_back(pdf_weight);
  }
}

//__________________________________________________________________

void PdfSet::AddObservable(TH1F* hist_obs) {
  //
  // Add observable to PdfSet.
  // For each observable a TH2D plot is created which will store all the neccessary
  // weights for the envelope calculation. The envelope calculation depends on the
  // binning of the original histogram of the observable.
  //
  // TH2D plots might occupy a lot of memory depending on the binning of the observable's
  // histogram and the total number of pdf error sets.
  //

  // Create TH2D histogram with observable binning on the x-axis
  //                     & with error pdfs on the y-axis
  
  // copy variable binning
  TAxis *axis = hist_obs->GetXaxis();
  Double_t edges[axis->GetNbins()];
  //axis->GetLowEdge(edges); // this does not work
  for ( int i = 0; i <= axis->GetNbins(); i++ ) {
    edges[i] = axis->GetBinLowEdge(i+1); // this is a hack
  }
  TH2D *hist = new TH2D(Form("%s_%s",hist_obs->GetName(), fName.Data()),
			Form("%s_%s",hist_obs->GetName(), fName.Data()),
			hist_obs->GetXaxis()->GetNbins(),
			edges,
			fNMember, -0.5, fNMember-0.5);
  hist->Sumw2();
  hist->SetXTitle(hist_obs->GetXaxis()->GetTitle());
  hist->SetYTitle("Pdf Error Sets");
  // prevent histogram from being stored in the user's current location
  // (e.g. outputfile)
  hist->SetDirectory(0);
  fListOfObservables->Add(hist);
}

//__________________________________________________________________

void PdfSet::FillObservable(TH1F* hist_obs, float observable, float evt_weight ) {
  //
  // Fill pdf reweighting histogram (TH2D) for pdf set
  // Use same histogram as in AddObservable(TH1F*). 
  //

  TH2D *hist = (TH2D*) fListOfObservables->FindObject( Form("%s_%s",hist_obs->GetName(), fName.Data()) );
  if ( !hist ) { gSystem->Abort(); }

  // Loop over all pdf weights
  for ( unsigned int member = 0; member < fNMember; member++ ) {
    hist->Fill( observable, member, fPdfWeights->at(member)*evt_weight );
  }
}

//__________________________________________________________________

TH1F* PdfSet::PdfUncertainty(TH1F* hist_obs, EVariation var ) {
  //
  // Compute uncertainty for each observable (binwise)
  //

  TH1F* hout;

  // copy variable binning
  TAxis *axis = hist_obs->GetXaxis();
  Double_t edges[axis->GetNbins()];
  for ( int i = 0; i <= axis->GetNbins(); i++ )
    edges[i] = axis->GetBinLowEdge(i+1); // this is a hack
  
  if ( var == PdfSet::kUp ) {
    hout = new TH1F( Form("%s_%s_up", hist_obs->GetName(), fName.Data()),
		     Form("%s_%s_up", hist_obs->GetName(), fName.Data()),
		     hist_obs->GetXaxis()->GetNbins(),
		     edges);
    hout->SetDirectory(0); // do not save this in the histogram
  } else {
    hout = new TH1F( Form("%s_%s_down", hist_obs->GetName(), fName.Data()),
		     Form("%s_%s_down", hist_obs->GetName(), fName.Data()),
		     hist_obs->GetXaxis()->GetNbins(),
		     edges);
    hout->SetDirectory(0); // do not save this in the histogram
  }
  
  TH2D* hist = (TH2D*) fListOfObservables->FindObject( Form("%s_%s",hist_obs->GetName(), fName.Data()) );
  TH1D* h_proj;
  
  // Loop over all bins of the observable's histogram
  for ( Int_t bin = 1; bin <= hist->GetNbinsX(); bin++ ) {
    h_proj = hist->ProjectionY("_py", bin, bin);
    h_proj->SetDirectory(0); // move to ROOT-Directory
    hout->SetBinContent( bin, ComputeVariation(h_proj, var) );
  }
  return hout;
}

//__________________________________________________________________

Float_t PdfSet::ComputeVariation(TH1D* hist_proj, EVariation var ) {
  //
  // Compute up / down variation of observable
  //
  Float_t ret_val = 1.0;
  if ( fPdfType == PdfSet::kCTEQ ) {
    ret_val = ComputeVariationCTEQ(hist_proj, var);
  } else if ( fPdfType == PdfSet::kMSTW ) {
    ret_val = ComputeVariationMSTW(hist_proj, var);
  } else if ( fPdfType == PdfSet::kNNPDF ) {
    ret_val = ComputeVariationNNPDF(hist_proj, var);
  } else {
    Error("PdfSet::ComputeVariation", "Unknown Pdf Type. Abort");
    gSystem->Abort();
  }
  return ret_val;
}

//__________________________________________________________________

Float_t PdfSet::ComputeVariationCTEQ(TH1D* hist_proj, EVariation var ) {
  //
  // Compute variation for CTEQ
  //
  // Symmetric hessian
  //

  // PdfSet Member x is stored in bin x+1 !!!
  Float_t NewObs = hist_proj->GetBinContent(1); // observable is member 0
  Float_t DeltaX = 0.;                          // variation of observable in this bin
  Float_t C90 = 1.64485;                        // 90% CL --> 68% CL

  Float_t diffx = 0;
  Float_t sumx2 = 0;
  
  Int_t Nbins = hist_proj->GetNbinsX();
  Int_t member = 0;
  for ( Int_t bin = 2; bin <= (0.5*(Nbins+1)); bin++ ) {
    member = bin - 1; // bin x holds member x-1
    // extract '2*member-1' - 2*member --> add +1 for bin shift
    // i.e. extract '2*member' - '2*member+1'
    // does not really matter, since it is symmetric...
    diffx = hist_proj->GetBinContent( 2*member ) - hist_proj->GetBinContent( 2*member + 1 );
    sumx2 += diffx*diffx;
  }
  DeltaX = 0.5*TMath::Sqrt( sumx2 )/C90;
  if ( var == PdfSet::kUp ) {
    NewObs += DeltaX;
  } else {
    NewObs -= DeltaX;
  }
  return NewObs;
}

//__________________________________________________________________

Float_t PdfSet::ComputeVariationMSTW(TH1D* hist_proj, EVariation var ) {
  //
  // Compute variation for MSTW
  //
  // Asymmetric hessian
  //

  // PdfSet Member x is stored in bin x+1 !!!
  Float_t NewObs = hist_proj->GetBinContent(1); // observable is member 0 <-> bin 1
  Float_t ErrUp = 0;                            // Asymetric error up
  Float_t ErrDown = 0;                          //    "        "   down

  Float_t DeltaX = 0;  
  Int_t Nbins = hist_proj->GetNbinsX();
  for ( Int_t bin = 2; bin <= Nbins; bin++ ) {
    // bin x holds member x-1
    DeltaX = hist_proj->GetBinContent( bin ) - NewObs;
    if ( DeltaX > 0 ) {
      ErrUp += DeltaX*DeltaX;
    } else {
      ErrDown += DeltaX*DeltaX;
    }
  }
  ErrUp = TMath::Sqrt( ErrUp );
  ErrDown = TMath::Sqrt( ErrDown );
  if ( var == PdfSet::kUp ) {
    NewObs += ErrUp;
  } else {
    NewObs -= ErrDown;
  }
  return NewObs;
}

//__________________________________________________________________

Float_t PdfSet::ComputeVariationNNPDF(TH1D* hist_proj, EVariation var ) {
  //
  // Compute variation for NNPDF
  //
  // Standard deviation 
  //

  // PdfSet Member x is stored in bin x+1 !!!
  
  // observable is not member 0 in our case (since <pdf1*pdf2> != <pdf1>*<pdf2>)
  Float_t NewObs = 0.;
  Int_t Nbins = hist_proj->GetNbinsX();
  for ( Int_t bin = 2; bin <= Nbins; bin++ ) {
    NewObs += hist_proj->GetBinContent(bin);
  }
  NewObs = NewObs/(Nbins-1);
  Float_t DeltaX = 0;                           // Difference between member 0 and error set
  Float_t Sumx2 = 0;
  for ( Int_t bin = 2; bin <= Nbins; bin++ ) {
    // bin x holds member x-1
    DeltaX = hist_proj->GetBinContent( bin ) - NewObs;
    Sumx2 += DeltaX*DeltaX;
  }
  Sumx2 = Sumx2/(Nbins - 2); // STD with (N-1), Nbins = N+1
  DeltaX = TMath::Sqrt(Sumx2);
  if ( var == PdfSet::kUp ) {
    NewObs += DeltaX;
  } else {
    NewObs -= DeltaX;
  }
  return NewObs;
}

//__________________________________________________________________

PdfSet::EPdf PdfSet::GetTypeByName(const char* name) {
  //
  // Get PDF set by name
  //
  for ( Int_t i = 0; i < fgNumPdfSets; i++ ) {
    if ( strcasecmp(fgPdfNames[i], name ) == 0 )
      return (EPdf)i;
  }
  Error("GetTypeByName", "PdfSet of name \"%s\" not found.", name);
  Error("GetTypeByName", "Abort !");
  gSystem->Abort(1);
  return (EPdf)0;
}

//__________________________________________________________________

void PdfSet::GetNormfactor(Int_t DSID) {
  //
  // Get Normfactor from file
  // (MC tag, e.g. p1400 is not used!)
  //
  
  const char* filename = Form("%s/data/PdfReweightingTool/%s.txt",
			      gSystem->Getenv("ROOTCOREDIR"),
			      GetName());

  cout << "Reading file '" << filename << "'" << endl;

  ifstream input;
  input.open(filename);
  if ( !input.is_open() ) {
    Error("GetNormFactor",
	  "Unable to open file: %s. Abort!",
	  filename);
    gSystem->Abort();
  }
  
  // I know this will be slow. Hopefully still faster
  // than running the event loop. If you have a better idea
  // to read / interface the normfactors, please let me know. 
  TString line = "";
  while ( !input.eof() ) {
    line.ReadLine(input, kFALSE);

    // Ignore comments (#) or empty lines
    if ( line.BeginsWith("#") || line.Length() == 0 ) continue;

    // Extract information for each line:
    TObjArray *tokens = line.Tokenize(" ");
    Int_t    dsid     = ((TObjString*)tokens->At(0))->GetString().Atoi();
    TString  PDFset   = ((TObjString*)tokens->At(1))->GetString();
    Int_t    member   = ((TObjString*)tokens->At(2))->GetString().Atoi();
    Double_t sf       = ((TObjString*)tokens->At(3))->GetString().Atof();
    
    if ( DSID == dsid &&
	 PDFset.Contains(GetName(), TString::kExact) ) {
      // cout << "DSID = " << dsid << " "
      // 	   << GetName() << " = " << PDFset.Data() 
      // 	   << " member " << member 
      // 	   << " sf = " << sf << endl;
	fNormfactors.push_back(sf);
    }

    // Once all info is read, stop here
    if ( (UInt_t) member == fNMember )
      break;
  }

  // Check if for each member, one scale factor exist
  if ( (UInt_t)fNormfactors.size() != fNMember ) {
    Error("GetNormFactor",
	  "Missing some scale factor for PDF set %s.",
	  GetName());
    Error("GetNormFactor",
	  "I have only %ld scale factors, but I should have %d.",
	  fNormfactors.size(), fNMember);
    Error("GetNormFactor",
	  "Abort!");
    gSystem->Abort();
  }

}

//__________________________________________________________________

TString PdfSet::GetDescription() const {
  //
  // LHAPDF info 
  //
  return fPdfSet.at(0)->description();
}
