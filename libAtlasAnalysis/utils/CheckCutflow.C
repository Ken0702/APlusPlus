#include "TSystem.h"
#include "TH1D.h"
#include "TAxis.h"

void CheckCutflow(const char* ext = ".el.root", TFile *fout = 0)
{
  TH1D * cutflow = 0;//new TH1D("cutflow","cutflow",21,-0.5,20.5);

  const char* inDir = "$PWD";

  char* dir = gSystem->ExpandPathName(inDir);
  void* dirp = gSystem->OpenDirectory(dir);

  const char* entry;
  const char* filename[100];
  Int_t n = 0;
  TString str;
  TFile *f;
  TH1D *tmp;

  while((entry = (char*)gSystem->GetDirEntry(dirp))) {
    str = entry;
    if(str.Contains(ext)) {
      f = new TFile(gSystem->ConcatFileName(dir, entry),"READ");
      if ( f == 0 ) {
	  Printf("Error: No file found.");
	  return;
      }	  
      
      tmp = (TH1D*) f->Get("physics_fullCutFlow_noWeights");
      if ( tmp == 0 ) {
       	  // try to get old version
	  tmp = (TH1D*) f->Get("fullCutFlow_noWeights");
      }
      if ( tmp == 0 ) {
	  Printf("Error: No cutflow histogram found.");
	  return;
      }
      
      if ( cutflow == 0 ) {
	cutflow = (TH1D*) tmp->Clone("cutflow");
	cutflow->SetDirectory(0);
      } else {
	cutflow->Add(tmp);
      }
      f->Close();
    }
  }

  if ( cutflow != 0 ) {
      for (Int_t bin = 1; bin < cutflow->GetNbinsX(); bin++)
	  Printf("%s %f", cutflow->GetXaxis()->GetBinLabel(bin), cutflow->GetBinContent(bin));
  }

  if ( fout != 0 ) {
      if ( cutflow != 0 ) {
	  cutflow->SetDirectory(fout);
	  fout->Write();
	  fout->Close();
      }
  }  
  gSystem->FreeDirectory(dirp);
}
