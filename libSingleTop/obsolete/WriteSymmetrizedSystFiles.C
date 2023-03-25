//========================================================================================
//
// WriteSymmetrizedSystFiles.C
//
// Tool for creating all symmetrized systematics output histograms of
// the analysis for
//
// -- one-sided systematics,
//
// -- process modelling systematics.
//
// The 2 output histograms are called
// [samplename]_[systematic]_[i]_hists.root where [i]=up,down. The
// systematics base names [systematic] are taken from the fgSystNames
// array below.
//
// Usage (example for electron 2 jet bin nominal):
//
// root -l
// .L WriteSymmetrizedSystFiles.C
// WriteSymmetrizedSystFiles(kElectron,k2Jets,kIFSR,"$LIBSINGLETOP/hist/tChannelAnalysis_","SampleList_plotter_tChannel.txt")
//
// where SampleList_plotter_tChannel.C contains the sample names and
// the cross-sections of the samples that are needed for weighting
// them with their MC luminosities w.r.t. each other. The reference
// data lumi is also applied in the normalization already here (it is
// also taken from the sample list).
//
//========================================================================================
#include <TObjString.h>
#include <TString.h>
#include <TList.h>
#include <TSystem.h>
#include <TFile.h>
#include <TKey.h>
#include <TH1I.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TSystemDirectory.h>
#include <iostream>
#include <fstream>

using namespace std;

enum LeptonMode {
    kElectron,
    kMuon,
    kNumLeptonModes
};

static const char* fgLeptonModeNames[kNumLeptonModes] = {
    "e",
    "mu"
};

enum NJetsMode {
//    k1Jet,
    k2Jets,
    k3Jets,
    k4Jets,
    kNumNJetsModes
};

static const char* fgNJetsModeNames[kNumNJetsModes] = {
//    "1Jet",
    "2Jets",
    "3Jets",
    "4Jets"
};


enum SystMode {
    kNominal,
    kMuSmearedNoScale,
    kJetSmearedSyst,
    kJetGoodReco,
    kIFSR,
    kEventGenerator,
    kPSGenerator,
    kNumSystModes
};

static const char* fgSystNames[kNumSystModes] = {
    "",
    "musc",
    "jer",
    "jeff",
    "ifsr",
    "evgen",
    "psgen"
};


//____________________________________________________________________

void copy_folder(TDirectory *SrcDir,
		 TDirectory *DestDir,
		 Float_t scale,
		 Bool_t first_file,
		 Bool_t last_file) {
    //
    // Recursively copying of the content (histograms, subfolders) of the given source
    // directory to the destination folder.
    // All histograms will be scaled by the given factor.
    //

    
    // Search source content
    SrcDir->cd();

    TIter next_key(SrcDir->GetListOfKeys());
    TKey *key = 0;
    while ( (key = (TKey*)next_key()) ) {
	if ( strcmp(key->GetClassName(), "TDirectoryFile") == 0 ) {
	    TDirectory *NewSrcDir = (TDirectoryFile*)key->ReadObj();
 	    DestDir->cd();
 	    TDirectory *NewDestDir = 0;
	    if ( first_file ) {
		NewDestDir = DestDir->mkdir(key->GetName());
		copy_folder(NewSrcDir, NewDestDir, scale, first_file, last_file);
	    } else {
		NewDestDir = (TDirectory*)DestDir->FindObject(key->GetName());
		copy_folder(NewSrcDir, NewDestDir, scale, first_file, last_file);
	    }
	} else if ( (strcmp(key->GetClassName(), "TH1F") == 0) ) {
	    DestDir->cd();
	    TH1F *h1f = 0;
	    if ( first_file ) {
		h1f = (TH1F*)key->ReadObj()->Clone();
		h1f->SetDirectory(DestDir);
		h1f->Scale(scale);
	    } else {
		h1f = (TH1F*)key->ReadObj();
		TH1F *h1f_new = (TH1F*)DestDir->FindObject(key->GetName());
		// Add to bin contents to final histogram 
		h1f_new->Add(h1f,scale);
		if ( last_file ) {
		    // Call Sumw2() on final histogram to force recomputation
		    // of bin errors
		    h1f_new->Sumw2();
		}		
	    }
	} else if ( (strcmp(key->GetClassName(), "TH1D") == 0) ) {
	    DestDir->cd();
	    TH1D *h1d = 0;
	    if ( first_file ) {
		h1d = (TH1D*)key->ReadObj()->Clone();
		h1d->SetDirectory(DestDir);
		h1d->Scale(scale);
	    } else {
		h1d = (TH1D*)key->ReadObj();
		TH1D *h1d_new = (TH1D*)DestDir->FindObject(key->GetName());
 		// Add to bin contents of final histogram
		h1d_new->Add(h1d,scale);
		if ( last_file ) {
		    // Call Sumw2() on final histogram to force recomputation
		    // of bin errors
		    h1d_new->Sumw2();
		}
	    }
	} else if ( (strcmp(key->GetClassName(), "TH1I") == 0) ) {
	    DestDir->cd();
	    TH1I *h1i = 0;
	    if ( first_file ) {
		h1i = (TH1I*)key->ReadObj()->Clone();
		h1i->SetDirectory(DestDir);
		h1i->Scale(scale);
	    } else {
		h1i = (TH1I*)key->ReadObj();
		TH1I *h1i_new = (TH1I*)DestDir->FindObject(key->GetName());
 		// Add to bin contents of final histogram
		h1i_new->Add(h1i,scale);
		if ( last_file ) {
		    // Call Sumw2() on final histogram to force recomputation
		    // of bin errors
		    h1i_new->Sumw2();
		}
	    }
	} else if ( (strcmp(key->GetClassName(), "TH2F") == 0) ) {
	    DestDir->cd();
	    TH2F *h2f = 0;
	    if ( first_file ) {
		h2f = (TH2F*)key->ReadObj()->Clone();
		h2f->SetDirectory(DestDir);
		h2f->Scale(scale);
	    } else {
		h2f = (TH2F*)key->ReadObj();
		TH2F *h2f_new = (TH2F*)DestDir->FindObject(key->GetName());
		// Add to bin contents of final histogram
		h2f_new->Add(h2f,scale);
		if ( last_file ) {
		    // Call Sumw2() on final histogram to force recomputation
		    // of bin errors
		    h2f_new->Sumw2();
		}
	    }
	} else if ( (strcmp(key->GetClassName(), "TH2D") == 0) ) {
	    DestDir->cd();
	    TH2D *h2d = 0;
	    if ( first_file ) {
		h2d = (TH2D*)key->ReadObj()->Clone();
		h2d->SetDirectory(DestDir);
		h2d->Scale(scale);
	    } else {
		h2d = (TH2D*)key->ReadObj();
		TH2D *h2d_new = (TH2D*)DestDir->FindObject(key->GetName());
		// Add to bin contents of final histogram
		h2d_new->Add(h2d,scale);
		if ( last_file ) {
		    // Call Sumw2() on final histogram to force recomputation
		    // of bin errors
		    h2d_new->Sumw2();
		}
	    }
	}
    }
}

//____________________________________________________________________

void AddHistogramsOfFiles(TDirectory *f1,
			       TDirectory *f2,
			       TDirectory *f_out,
			       Float_t scale1,
			       Float_t scale2) {
    //
    // Offers functionality to add all histograms of the two files f1 and f2
    // by linearly combining them using the given scale factors:
    // if h1 is a histogram in f1 and h2 is a histogram in f2, all histograms
    // will be added according to
    //
    // h1*scale1 + h2*scale2
    //
    // If a subtraction is required, just provide a negative scale factor!
    // The resulting file is written into f_out.
    //

    copy_folder(f1,
		f_out,
		scale1,
		kTRUE,
		kFALSE);
    
    copy_folder(f2,
		f_out,
		scale2,
		kFALSE,
		kTRUE);

}

//____________________________________________________________________


TList* ListFiles(const char *dirname, TString sample, SystMode syst) {
    //
    // Get files in directory dirname for given systematics
    //
    TSystemDirectory dir(dirname, dirname);
    TList *files = dir.GetListOfFiles();
    TList *sel_files = new TList();
    TObjString *str;
    TString systname = TString(fgSystNames[syst]);
    systname += "_hists.root";
    if (files) {
	TSystemFile *file;
	TString fname;
	
	TIter next(files);
	while ((file=(TSystemFile*)next())) {
	    fname = file->GetName();

	    //Ask for filename containing syst mode
	    if( fname.Contains(sample) &&
		fname.Contains(systname) ) {
		//Ask for filename containing no other syst modes (needed for nominal having no filename tag)
		if(syst>0) {
		    fname = TString(dirname)+"/"+fname;
		    str = new TObjString(fname);
		    sel_files->Add(str);
		    cout << fname << endl;
		} else {
		    Bool_t real_nominal = kTRUE;
		    if ( fname.Contains("ifsr")
			 || fname.Contains("evgen")
			 || fname.Contains("psgen")
			) {
			real_nominal = kFALSE;
		    }
		    if(real_nominal) {
			fname = TString(dirname)+"/"+fname;
			cout << fname << endl;
			str = new TObjString(fname);
			sel_files->Add(str);
		    }
		}
	    }
	}
    }
    
    return sel_files;
    
}

//____________________________________________________________________

void WriteSymmetrizedSystFiles(LeptonMode lepton_mode,
			       NJetsMode njets_mode,
			       SystMode syst,
			       const char *histdir_char,
			       const char *sample_file) {
    //
    // Creates two symmetrized systematics output files for chosen
    // systematic separately for each of the required input samples
    // (either all MC or only top-samples). Weighting of the MCs
    // according to their cross-sections and luminosities is applied
    // already here as a scale
    //
    // Lumi_Data/Lumi_MC = Lumi_Data/(N_original_events/Xsec).
    //
    // CAUTION: In the plotter macro, these symmetrized systematics
    // must not be scaled again to data lumi (only SFs applied
    // subsequently: W HF and overall norm. SFs)!
    //
    // NOTE: The merged files for the four hfor types must already
    // exist! Those are not scaled anymore in this macro since they
    // are already scaled to data lumi by the merging macro
    // WriteWhforFiles.C !

    // Save working directory
    TString savedir(gSystem->WorkingDirectory());

    
    // Setting systematics label
    TString systname = TString(fgSystNames[syst]);
    if ( syst == kNominal ){
	cout << endl 
	     << "Mode nominal only for internal use. Nothing to be symmetrized! Abort!"
	     << endl << endl;
	gSystem->Abort();
    } else if ( (syst == kMuSmearedNoScale)
		&& (lepton_mode == kElectron) ){
	cout << endl 
	     << "Mode musc exists only for muon case. Abort!"
	     << endl << endl;
	gSystem->Abort();
    }

    // Setting lepton mode label
    TString lepton_mode_label = TString(fgLeptonModeNames[lepton_mode]);
    
    // Setting njets mode label
    TString njets_label = TString(fgNJetsModeNames[njets_mode]);

    // Generator systematics mode?
    Bool_t IsGenSyst = kFALSE;
    if ( (syst == kIFSR)
	 || (syst == kEventGenerator)
	 || (syst == kPSGenerator) ){
	IsGenSyst = kTRUE;
    }
    
    cout
	<< endl << " Loading MC sample files for systematic "
	<< systname << "..." << endl << endl;
    
    //Prepare input text file
    TString sample_line;
    TObjArray *sublines;
    ifstream sample_list;
    TString group_str, sample_str, path_str, path_str_nom;
    TObjString* group;
    TObjString*sample;
    TObjString *xs;

    // List of all up/down output files 
    TList *file_list_out_up = new TList();
    TList *file_list_out_down = new TList();
    
    // List of all MC files to be symmetrized
    TList *file_list  = new TList();
    TList *file_list2 = new TList(); // some systematics need 2 input
				     // files besides the nominal

    // List of all nominal MC files 
    TList *file_list_nom = new TList();
    
    // List of their respective cross-sections
    TList *xs_string_list = new TList();


    // Generic histogram directory
    TString histdir = TString(histdir_char);

    // Histogram directory of current systematics sample
    path_str=histdir+lepton_mode_label+"/"+njets_label+"/"+systname;

    // Histogram directory of nominal sample
    path_str_nom=histdir+lepton_mode_label+"/"+njets_label+"/nominal";

    // In case of generator systematic, systematics folder is nominal folder
    if ( IsGenSyst ) path_str=path_str_nom;
    
    TString lumi;
    Float_t data_lumi, legend_data_lumi;

    // Pointers of symmetrized systematics up/down output files
    TFile *f_out_up;
    TFile *f_out_down;

    // Needed to ensure that only W samples representative of the 4
    // HFOR types are included
    Bool_t IsW       = kFALSE;
    Bool_t IsWenuNp0 = kFALSE;
    Bool_t IsWcNp0   = kFALSE;
    Bool_t IsWccNp0  = kFALSE;
    Bool_t IsWbbNp0  = kFALSE;

    // Store info on type of top-sample
    Bool_t IsTChan   = kFALSE;
    Bool_t IsTtbar   = kFALSE;

    // 2 files involved?
    Bool_t Need2Files = kFALSE;
    Bool_t Need2Files_was_true = kFALSE;
    
    //Open input MC sample list
    sample_list.open(sample_file);
    if( !sample_list.good() ) {
	cout << endl << " WriteSymmetrizedSystFiles: Error loading sample file !!!" << endl << endl;
      sample_list.close();
    } else {
	// Read input sample list and store information in TLists
	string line("");
	while( !sample_list.eof() ) {
	    line="";
	    getline(sample_list,line);
	    sample_line = TString(line);
	    if(sample_line.BeginsWith("#") || sample_line.Length()<4) continue;
	    sublines = sample_line.Tokenize(" ");
	    group = (TObjString*)sublines->At(0);	    
	    if ( group->GetString().Contains("data") ) {
		lumi = ((TObjString*)sublines->At(2))->GetString();
		legend_data_lumi = lumi.Atof();
		data_lumi = legend_data_lumi*1000.;
		cout << "data_lumi = " << data_lumi << endl << endl;
		continue;
	    }
	    if ( (group->GetString().Contains("qcd")) ) continue;
	    // Only top-samples for generator systematics
	    if ( IsGenSyst
		 && !(group->GetString().Contains("ttbar"))
		 && !(group->GetString().Contains("sgtop")) ) continue;
	    // No s-channel for evgen/psgen systematic
	    if ( ((syst == kEventGenerator) || (syst == kPSGenerator))
		 && (group->GetString().Contains("schannel")) ) continue;
	    
	    sample = (TObjString*)sublines->At(1);
	    xs = (TObjString*)sublines->At(2);
	    
	    group_str = group->GetString();
	    sample_str = sample->GetString();
	    
	    // Keep only one sample (the first) of each hfor type for easier handling
	    IsW = group_str.Contains("wjets");
	    IsWenuNp0 = sample_str.Contains("wenu_np0");
	    IsWcNp0 = sample_str.Contains("wc_np0");
	    IsWccNp0 = sample_str.Contains("wcc_np0");
	    IsWbbNp0 = sample_str.Contains("wbb_np0");
	    if ( IsW && !( IsWenuNp0 || IsWcNp0 || IsWccNp0 || IsWbbNp0 ) ) continue;

	    
	    // Find all systematics sample files
	    TList *tmp_file_list  = 0;
	    TList *tmp_file_list2 = 0;
	    Int_t i_hfor = 0;
	    TString w_sample = TString("");
	    TString sample_str_more = TString("");
	    TString sample_str_less = TString("");
	    
	    // Need 2 files per sample for special systematics
	    Need2Files = kFALSE;
	    if ( (syst == kIFSR) ||
		 ( ( (syst == kPSGenerator) && (sample_str.Contains("ttbar")) )
		   ||  ( (syst == kPSGenerator) && (sample_str.Contains("wtchannel")) ) )
		 ) {
	      Need2Files = kTRUE;
	      Need2Files_was_true = kTRUE;
	    }

	    
	    if ( IsW ) {
		// -------------
		// All W samples	
		// -------------
		if ( IsWenuNp0 ) i_hfor = 3;
		else if ( IsWcNp0 ) i_hfor = 2;
		else if ( IsWccNp0 ) i_hfor = 1;
		w_sample = Form("Wjets_hfor%i",i_hfor);
		tmp_file_list = ListFiles(path_str.Data(), w_sample, syst);
	    } else {
		// 2 files per sample for special systematics
		if ( Need2Files ) {
		    sample_str_more = sample_str+"_more";
		    sample_str_less = sample_str+"_less";
		    tmp_file_list  = ListFiles(path_str.Data(), sample_str_more, syst);
		    tmp_file_list2 = ListFiles(path_str.Data(), sample_str_less, syst);
		} else {
		  if ( (syst == kEventGenerator)
		       && (group_str.Contains("sgtop_tchannel")) ) {
		    tmp_file_list = ListFiles(path_str.Data(), sample_str, kNominal);
		  } else {
		    tmp_file_list = ListFiles(path_str.Data(), sample_str, syst);
		  }
		}
	    }

	    
	    // Find all nominal sample files
	    TList *tmp_file_list_nom = 0;
	    if ( IsW ) {
		tmp_file_list_nom = ListFiles(path_str_nom.Data(), w_sample, kNominal);
	    } else {
		tmp_file_list_nom = ListFiles(path_str_nom.Data(), sample_str, kNominal);
	    }

	    
	    // Found dir and files at all?
	    // Current systematic
	    if ( tmp_file_list->GetEntries() == 0 ) {
		cout << endl << " Directory " << path_str.Data() << " or systematic folder " << systname << " not existent or doesn't contain the requested files!" << endl << endl;
		return;
	    }
	    if ( Need2Files && (tmp_file_list2->GetEntries() == 0) ) {
		cout << endl << " Directory " << path_str.Data() << " or systematic folder " << systname << " not existent or doesn't contain the requested second set of files!" << endl << endl;
		return;
	    }

	    // Nominal
	    if ( tmp_file_list_nom->GetEntries() == 0 ) {
		cout << endl << " Directory " << path_str_nom.Data() << " or systematic folder nominal not existent or doesn't contain the requested files!" << endl << endl;
		return;
	    }

	    // Create symmetrized output files up/down for chosen systematic
	    TString outputfilename_up = 
	      path_str
	      +"/"
	      +sample_str
	      +"_"
	      +systname;

	    TString outputfilename_down = 
	      path_str
	      +"/"
	      +sample_str
	      +"_"
	      +systname;

	    if ( (syst == kEventGenerator) || (syst == kPSGenerator) ) {
	      if ( group_str.Contains("sgtop_tchannel") ) {
		outputfilename_up +=
		  "_tc_up_hists.root";
		outputfilename_down +=
		  "_tc_down_hists.root";
	      } else if ( sample_str.Contains("wtchannel") ) {
		outputfilename_up +=
		  "_wt_up_hists.root";
		outputfilename_down +=
		  "_wt_down_hists.root";
	      } else if ( sample_str.Contains("ttbar") ) {
		outputfilename_up +=
		  "_tt_up_hists.root";
		outputfilename_down +=
		  "_tt_down_hists.root";
	      }
	    } else {
	      outputfilename_up +=
		"_up_hists.root";
	      outputfilename_down +=
		"_down_hists.root";
	    }

	    
	    TString outputWfilename_up=
		path_str
		+"/"
		+w_sample
		+"_"
		+systname
		+"_up_hists.root";
	    
	    TString outputWfilename_down=
		path_str
		+"/"
		+w_sample
		+"_"
		+systname
		+"_down_hists.root";
	    
	    // Store full file paths of up/down output files
	    TObjString *outname_up = 0;
	    TObjString *outname_down = 0;
	    if ( IsW ) {
		outname_up = new TObjString(outputWfilename_up);
		outname_down = new TObjString(outputWfilename_down);
	    } else {
		outname_up = new TObjString(outputfilename_up);
		outname_down = new TObjString(outputfilename_down);
	    }
	    
	    file_list_out_up->Add(outname_up);
	    file_list_out_down->Add(outname_down);
	    
	    // Return to starting directory
	    gSystem->ChangeDirectory(savedir);

	    // Store full MC systematics file paths
	    file_list->AddAll(tmp_file_list);
	    if ( Need2Files ) file_list2->AddAll(tmp_file_list2);

	    // Store full nominal MC file paths
	    file_list_nom->AddAll(tmp_file_list_nom);
	    
	    // Store respective cross-sections
	    for ( Int_t i = 0; i < tmp_file_list->GetEntries(); i++ ) {
		xs_string_list->Add(xs);
		//		cout << ((TObjString*)tmp_file_list->At(i))->String().Data() << endl;
	    }

	    
	    // Clean-up
	    delete tmp_file_list;
	    if ( Need2Files ) delete tmp_file_list2;
	    delete tmp_file_list_nom;
    	}

	// Cross-check for number of entries in file_list and xs_string_list
	cout << endl << " No. of entries in file list = " << file_list->GetEntries()
	     << endl;
	if ( Need2Files_was_true )
	    cout << endl << " No. of entries in file list 2 = " << file_list2->GetEntries()
		 << endl;
	cout << " No. of entries in xsec list = " << xs_string_list->GetEntries()
	     << endl << endl;

	
	// Close input sample list
	sample_list.close();
    }

    
    // =====================
    // Loop over input files
    // =====================
    TObjString *file;
    TObjString *file2;
    TObjString *file_nom;
    TObjString *file_out_up;
    TObjString *file_out_down;
    TString xs_str;
    Float_t xsec = 0;
    Float_t Nevt, scale;
    Float_t Nevt2, scale2;
    Float_t Nevt_nom, scale_nom;
    Double_t lumiMC, lumiMC2, lumiMC_nom;

//    TH1::SetDefaultSumw2(kTRUE);
    
    for ( Int_t i = 0; i < file_list->GetEntries(); i++ ) {
    //   Int_t i=24; // try W syst files!
	// systematics file
	file = (TObjString*)file_list->At(i);
	cout << file->GetString().Data() << endl;

	// Need 2 files per sample for special systematics
	Bool_t Need2Files_now = kFALSE;
	if ( (syst == kIFSR) ||
	     ( ( (syst == kPSGenerator) && (file->GetString().Contains("ttbar")) )
	       ||  ( (syst == kPSGenerator) && (file->GetString().Contains("wtchannel")) ) )
	    ) { // CAUTION: It's only working like this because the
		// t-channel is last in the sample list! Were it not,
		// then there would be holes in the file_list2 for the
		// PSGenerator systematic!!!
	    Need2Files_now = kTRUE;
	    file2 = (TObjString*)file_list2->At(i);
	    cout << file2->GetString().Data() << endl;
	}
	
	// W file?
	IsW = file->GetString().Contains("Wjets");

	// t-channel file?
	IsTChan = file->GetString().Contains("/tchannel");

	// ttbar file?
	IsTtbar  = file->GetString().Contains("ttbar");
	
	xs_str = ((TObjString*)xs_string_list->At(i))->GetString();
	xsec = xs_str.Atof();
	cout << " xsec = " << xsec << endl;

	// nominal file
	file_nom = (TObjString*)file_list_nom->At(i);
	cout << file_nom->GetString().Data() << endl;

	// Get names of up/down output files
	file_out_up = (TObjString*)file_list_out_up->At(i);
	file_out_down = (TObjString*)file_list_out_down->At(i);
	cout
	    << endl << " Creating output files with names: "  << endl << endl
	    << file_out_up->GetString().Data() << endl
	    << file_out_down->GetString().Data() << endl;
	f_out_up   = new TFile(file_out_up->GetString(),"RECREATE");
	f_out_down = new TFile(file_out_down->GetString(),"RECREATE");

	// Dummy file needed for step-wise computations
	TString dummy = path_str+"/dummy.root";
	TFile *f_dummy = new TFile(dummy,"RECREATE");
	
	// Open input files for reading
	TFile  *f_in     = new TFile(file->GetString().Data(), "READ");
	TFile  *f_in2    = 0;
	TFile  *f_in_nom = new TFile(file_nom->GetString().Data(), "READ");
	if ( Need2Files_now ) f_in2 = new TFile(file2->GetString().Data(), "READ");
	
	//
	// Clone and scale histograms and combine them linearly for the
	// respective systematics
	//
	if ( IsW ) {
	    // Is already scaled correctly 
	    scale = 1.;
	    scale_nom = 1.;
	    cout << " scale = " << scale << endl;
	    cout << " scale_nom = " << scale_nom << endl;
	} else {
	    // Get total number of events in this systematics sample
	    Nevt = ((TH1F*)f_in->Get("job_info/h_nevts"))->GetBinContent(2);
	    cout << " Nevt = " << Nevt << endl;

	    // If needed, get total number of events in second file of systematics sample
	    if ( Need2Files_now ) {
		Nevt2 = ((TH1F*)f_in2->Get("job_info/h_nevts"))->GetBinContent(2);
		cout << " Nevt2 = " << Nevt2 << endl;
	    }
	    
	    // Get total number of events in respective nominal sample
	    Nevt_nom = ((TH1F*)f_in_nom->Get("job_info/h_nevts"))->GetBinContent(2);
	    cout << " Nevt_nom = " << Nevt_nom << endl;
	    
	    
	    // Get MC systematics sample lumi for cross-section normalization
	    lumiMC = Nevt/xsec;
	    cout << " lumiMC = " << lumiMC << endl;

	    // If needed, get MC lumi of second systematics file as well
	    if ( Need2Files_now ) {
		lumiMC2 = Nevt2/xsec;
		cout << " lumiMC2 = " << lumiMC2 << endl;
	    }
	    
	    // Get MC nominal sample lumi for cross-section normalization
	    lumiMC_nom = Nevt_nom/xsec;
	    cout << " lumiMC_nom = " << lumiMC_nom << endl;
	    
	    // Get scale factor for histograms of this systematics sample
	    scale =  ( lumiMC == 0. ) ? 0. : data_lumi/lumiMC;
	    cout << " scale = " << scale << endl;
	    
	    // If needed, get scale factor for histograms of second file as well
	    if ( Need2Files_now ) {
		scale2 =  ( lumiMC2 == 0. ) ? 0. : data_lumi/lumiMC2;
		cout << " scale2 = " << scale2 << endl;
	    }
	    
	    // Get scale factor for histograms of respective nominal sample
	    scale_nom =  ( lumiMC_nom == 0. ) ? 0. : data_lumi/lumiMC_nom;
	    cout << " scale_nom = " << scale_nom << endl;
	}

	
	// Clone all histograms from first file and add/subtract all
	// entries for symmetrizing the systematics samples (build up/down files)

	// =================================
	// One-sided object reco systematics
	// =================================
	if ( !IsGenSyst ) {
	    // Compute deviation from nominal
	    //
	    // (variation(scaled to data lumi) - nominal(scaled to data lumi))
	    AddHistogramsOfFiles(f_in,
				 f_in_nom,
				 f_dummy,
				 scale,
				 -scale_nom);
	    
	    f_dummy->Write();
	    delete f_dummy;
	    f_dummy = new TFile(dummy, "READ");
	    
	    cout << " Successfully computed deviation from nominal... " << endl;
	    
	    
	    // -------------
	    // Build up-file
	    // -------------	
	    
	    // nominal(scaled to data lumi) +
	    // (variation(scaled to data lumi) - nominal(scaled to data lumi))
	    AddHistogramsOfFiles(f_in_nom,
				 f_dummy,
				 f_out_up,
				 scale_nom,
				 1.);
	    cout << " Successfully built up variation file... " << endl;
	    
	    
	    // --------------- 	
	    // Build down-file
	    // ---------------
	    
	    // nominal(scaled to data lumi) -
	    // (variation(scaled to data lumi) - nominal(scaled to data lumi))
	    AddHistogramsOfFiles(f_in_nom,
				 f_dummy,
				 f_out_down,
				 scale_nom,
				 -1.);
	    cout << " Successfully built down variation file... " << endl;
	    
	// ===============
	// IFSR systematic
	// ===============
	} else if ( syst == kIFSR ) {
	    // Compute deviation of more_ifsr and less_ifsr samples
	    //
	    // (ifsr_more(scaled to data lumi) - ifsr_less(scaled to data lumi))
	    AddHistogramsOfFiles(f_in,
				 f_in2,
				 f_dummy,
				 scale,
				 -scale2);
	    
	    f_dummy->Write();
	    delete f_dummy;
	    f_dummy = new TFile(dummy, "READ");
	    
	    cout << " Successfully computed deviation of ifsr_more and ifsr_less... " << endl;
	    
	    
	    // -------------
	    // Build up-file
	    // -------------	
	    
	    // nominal(scaled to data lumi) +
	    // 0.5*[(ifsr_more(scaled to data lumi) - ifsr_less(scaled to data lumi))]
	    AddHistogramsOfFiles(f_in_nom,
				 f_dummy,
				 f_out_up,
				 scale_nom,
				 0.5);
	    cout << " Successfully built up variation file... " << endl;
	    
	    
	    // ---------------
	    // Build down-file
	    // ---------------
	    
	    // nominal(scaled to data lumi) -
	    // 0.5*[(ifsr_more(scaled to data lumi) - ifsr_less(scaled to data lumi))]
	    AddHistogramsOfFiles(f_in_nom,
				 f_dummy,
				 f_out_down,
				 scale_nom,
				 -0.5);
	    cout << " Successfully built down variation file... " << endl;
	    
	// ==========================
	// Event generator systematic
	// ==========================
	} else if ( syst == kEventGenerator ) {
	    // ---> t-Channel: simple scale variation by 7.1%
	    if ( IsTChan ) {
		Float_t scale_var = 0.071;
		
		// -------------
		// Build up-file
		// -------------	
		
		// nominal(scaled to data lumi) + 0.071*nominal(scaled to data lumi))
		AddHistogramsOfFiles(f_in,
				     f_in_nom,
				     f_out_up,
				     scale_nom,
				     scale_nom*scale_var);
		cout << " Successfully built up variation file... " << endl;
		
		
		// --------------- 	
		// Build down-file
		// ---------------
		
		// nominal(scaled to data lumi) - 0.071*nominal(scaled to data lumi))
		AddHistogramsOfFiles(f_in,
				     f_in_nom,
				     f_out_down,
				     scale_nom,
				     -scale_nom*scale_var);
		cout << " Successfully built down variation file... " << endl;
	    // ---> Wt-channel and ttbar (for ttbar factor 1., for Wt-channel 0.5)
	    } else {
		// Compute deviation from nominal
		//
		// (variation(scaled to data lumi) - nominal(scaled to data lumi))
		AddHistogramsOfFiles(f_in,
				     f_in_nom,
				     f_dummy,
				     scale,
				     -scale_nom);
		
		f_dummy->Write();
		delete f_dummy;
		f_dummy = new TFile(dummy, "READ");
		
		cout << " Successfully computed deviation from nominal... " << endl;
		
		 if ( IsTtbar ) {
		     // -------------
		     // Build up-file
		     // -------------	

		     // nominal(scaled to data lumi) +
		     // (variation(scaled to data lumi) - nominal(scaled to data lumi))
		     AddHistogramsOfFiles(f_in_nom,
					  f_dummy,
					  f_out_up,
					  scale_nom,
					  1.);
		     cout << " Successfully built up variation file... " << endl;
		     
		     
		     // --------------- 	
		     // Build down-file
		     // ---------------
		     
		     // nominal(scaled to data lumi) -
		     // (variation(scaled to data lumi) - nominal(scaled to data lumi))
		     AddHistogramsOfFiles(f_in_nom,
					  f_dummy,
					  f_out_down,
					  scale_nom,
					  -1.);
		     cout << " Successfully built down variation file... " << endl;
		 } else {
		     // -------------
		     // Build up-file
		     // -------------	

		     // nominal(scaled to data lumi) +
		     // 0.5*[(variation(scaled to data lumi) - nominal(scaled to data lumi))]
		     AddHistogramsOfFiles(f_in_nom,
					  f_dummy,
					  f_out_up,
					  scale_nom,
					  0.5);
		     cout << " Successfully built up variation file... " << endl;
		     
		     
		     // --------------- 	
		     // Build down-file
		     // ---------------
		     
		     // nominal(scaled to data lumi) -
		     // 0.5*[(variation(scaled to data lumi) - nominal(scaled to data lumi))]
		     AddHistogramsOfFiles(f_in_nom,
					  f_dummy,
					  f_out_down,
					  scale_nom,
					  -0.5);
		     cout << " Successfully built down variation file... " << endl;
		 }
		
	    }

	// =======================
	// PS generator systematic
	// =======================
	} else if ( syst == kPSGenerator ) {
	    // ---> t-Channel: variation w.r.t. nominal
	    if ( IsTChan ) {
		// Compute deviation from nominal
		//
		// (variation(scaled to data lumi) - nominal(scaled to data lumi))
		AddHistogramsOfFiles(f_in,
				     f_in_nom,
				     f_dummy,
				     scale,
				     -scale_nom);
		
		f_dummy->Write();
		delete f_dummy;
		f_dummy = new TFile(dummy, "READ");
		
		cout << " Successfully computed deviation from nominal... " << endl;
		
		
		// -------------
		// Build up-file
		// -------------	
	    
		// nominal(scaled to data lumi) +
		// (variation(scaled to data lumi) - nominal(scaled to data lumi))
		AddHistogramsOfFiles(f_in_nom,
				     f_dummy,
				     f_out_up,
				     scale_nom,
				     1.);
		cout << " Successfully built up variation file... " << endl;
		
		
		// --------------- 	
		// Build down-file
		// ---------------
		
		// nominal(scaled to data lumi) -
		// (variation(scaled to data lumi) - nominal(scaled to data lumi))
		AddHistogramsOfFiles(f_in_nom,
				     f_dummy,
				     f_out_down,
				     scale_nom,
				     -1.);
		cout << " Successfully built down variation file... " << endl;
		
	    // ---> Wt-channel and ttbar: difference of variation samples vs nominal
	    } else {
		// Compute deviation of more_psgen and less_psgen samples
		//
		// (psgen_more(scaled to data lumi) - psgen_less(scaled to data lumi))
		AddHistogramsOfFiles(f_in,
				     f_in2,
				     f_dummy,
				     scale,
				     -scale2);
		
		f_dummy->Write();
		delete f_dummy;
		f_dummy = new TFile(dummy, "READ");
		
		cout << " Successfully computed deviation of psgen_more and psgen_less... " << endl;
		
		
		// -------------
		// Build up-file
		// -------------	
	    
		// nominal(scaled to data lumi) +
		// (psgen_more(scaled to data lumi) - psgen_less(scaled to data lumi))
		AddHistogramsOfFiles(f_in_nom,
				     f_dummy,
				     f_out_up,
				     scale_nom,
				     1.);
		cout << " Successfully built up variation file... " << endl;
	    
		
		// ---------------
		// Build down-file
		// ---------------
		
		// nominal(scaled to data lumi) -
		// (psgen_more(scaled to data lumi) - psgen_less(scaled to data lumi))
		AddHistogramsOfFiles(f_in_nom,
				     f_dummy,
				     f_out_down,
				     scale_nom,
				     -1.);
		cout << " Successfully built down variation file... " << endl;
	    		
	    }
	    
	// ===================
	// Unknown systematic!
	// ===================
	} else {
	    cout << endl << endl << "Unknown systematics mode. Abort!" << endl << endl;
	    gSystem->Abort();
	}
	    
	
	// Write output files
	f_out_up->Write();
	f_out_down->Write();

	// Clean-up
	gSystem->Exec(Form("rm -f %s",dummy.Data()));
	delete f_in;
	if ( Need2Files_now ) delete f_in2;
	delete f_in_nom;
	delete f_dummy;
	delete f_out_up;
	delete f_out_down;

    }
	

}

