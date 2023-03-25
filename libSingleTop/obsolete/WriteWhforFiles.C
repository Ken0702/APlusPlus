//========================================================================================
//
// WriteWhforFiles.C
//
// Tool for merging all W+jets output histograms of the analysis according to
// their hfor (heavy flavour overlap removal) type (0=Wbb,1=Wcc,2=Wc,3=Wll).
// The 4 output histograms are called 
//   Wjets_hfor[i][_systematic]_hists.root
// where [i]=0,1,2,3 and [_systematic] is not appended for the nominal case; 
// for the systematics it's taken from the fgSystNames array below.
//
// Usage (example for electron 2 jet bin nominal):
//
// root -l
// .L WriteWhforFiles.C
// WriteWhforFiles(kElectron,k2Jets,kNominal,0,"SampleList_plotter_tChannel.txt")
//
// where SampleList_plotter_tChannel.C contains the groups (WLF,WHF), the names
// and the cross-sections of the W samples that are needed for weighting them with
// their MC luminosities w.r.t. each other. The reference data lumi is also applied 
// in the normalization already here (it is also taken from the sample list).
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
    k1Jet,
    k2Jets,
    k3Jets,
    k4Jets,
    kNumNJetsModes
};

static const char* fgNJetsModeNames[kNumNJetsModes] = {
    "1Jet",
    "2Jets",
    "3Jets",
    "4Jets"
};


enum SystMode {
    kNominal,
    kElScaledUp,
    kElScaledDown,
    kElSmearedUp,
    kElSmearedDown,
    kMuSmearedIDUp,
    kMuSmearedIDDown,
    kMuSmearedMSUp,
    kMuSmearedMSDown,
    kMuSmearedNoScale,
    kJetScaledUp,
    kJetScaledDown,
    kJetSmearedSyst,
    kJetGoodReco,
//    kOutsoftUp,
//    kOutsoftDown,
    kPileUpUp,
    kPileUpDown,
    kElSFUp,
    kElSFDown,
    kMuSFUp,
    kMuSFDown,
    kJetBTagSFUp,
    kJetBTagSFDown,
    kJetCTagSFUp,
    kJetCTagSFDown,
    kJetMisTagSFUp,
    kJetMisTagSFDown,
    kJVFWeightUp,
    kJVFWeightDown,
    kCellOutUp,
    kCellOutDown,
    kPDFUp,
    kPDFDown,
    kNumSystModes
};

static const char* fgSystNames[kNumSystModes] = {
    "",
    "eesu",
    "eesd",
    "eeru",
    "eerd",
    "muidu",
    "muidd",
    "mumsu",
    "mumsd",
    "musc",
    "jesu",
    "jesd",
    "jer",
    "jeff",
//    "softjetu",
//    "softjetd",
    "pileupu",    
    "pileupd",
    "lep_sf_up", // unified for simplifying application of BILL tool
    "lep_sf_down",
    "lep_sf_up", // unified for simplifying application of BILL tool
    "lep_sf_down",
    "jet_btag_sf_up",
    "jet_btag_sf_down",
    "jet_ctag_sf_up",
    "jet_ctag_sf_down",
    "jet_mistag_sf_up",
    "jet_mistag_sf_down",
    "jvf_weight_up",
    "jvf_weight_down",
    "celloutu",
    "celloutd",
    "pdf_up",
    "pdf_down"
};

//____________________________________________________________________

void WriteWhforFiles(LeptonMode lepton_mode, NJetsMode njets_mode, SystMode syst, Int_t hfor_type, const char *w_sample_file) {
    //
    // Creates ONE merged W file for chosen systematic and hfor type
    // from all different W input samples that belong to that hfor type
    // ( weighting of the W+jets MCs according to their cross-sections and
    // luminosities is applied as a scale
    //
    // Lumi_Data/Lumi_MC = Lumi_Data/(N_original_events/Xsec)
    //

    // Save working directory
    TString savedir(gSystem->WorkingDirectory());

    
    // Setting systematics label
    TString systname = TString(fgSystNames[syst]);
    if ( systname == "" ) systname = "nominal";

    // Setting lepton mode label
    TString lepton_mode_label = TString(fgLeptonModeNames[lepton_mode]);
    
    // Setting njets mode label
    TString njets_label = TString(fgNJetsModeNames[njets_mode]);
    
    // Set hfor label
    TString hfor_label = "";
    switch ( hfor_type ) {
	case  0: {
	    hfor_label+="0";
	    break;
	}
	case  1: {
	    hfor_label+="1";
	    break;
	}
	case  2: {
	    hfor_label+="2";
	    break;
	}
	case  3: {
	    hfor_label+="3";
	    break;
	}
	// nominal w.r.t. W reweighting and rescaling
	default: {
	    cout << endl << " No valid hfor type given! Abort!" << endl << endl;
	    gSystem->Abort();
	}

    }
    
    cout
	<< endl << " Loading W+jets sample files for systematic "
	<< systname << "..." << endl << endl;
    
    //Prepare input text file
    TString sample_line;
    TObjArray *sublines;
    ifstream sample_list;
    TString group_str, sample_str, path_str;
    TObjString* group;
    TObjString*sample;
    TObjString *xs;

    // List of all files to be merged
    TList *file_list = new TList();
    
    // List of their respective cross-sections
    TList *xs_string_list = new TList();


    // Generic histogram directory
    TString histdir = "/users/eeh/stamm/atlas/APlusPlus/libSingleTop/hist/tChannelAnalysis_";
    path_str=histdir+lepton_mode_label+"/"+njets_label+"/"+systname;
    
    TString lumi;
    Float_t data_lumi, legend_data_lumi;
    
    //Open input W sample list
    sample_list.open(w_sample_file);
    if( !sample_list.good() ) {
	cout << endl << " WriteWhforFiles: Error loading sample file !!!" << endl << endl;
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
	    }
	    if ( !(group->GetString().Contains("wjetsLF"))
		 && !(group->GetString().Contains("wjetsHF")) ) continue;
	    sample = (TObjString*)sublines->At(1);
	    xs = (TObjString*)sublines->At(2);
	    
	    group_str = group->GetString();
	    sample_str = sample->GetString();

	    // Find all sample files
	    TList *tmp_file_list = 0;
	    tmp_file_list = ListFiles(path_str.Data(), sample_str, syst, hfor_label);

	    // Found dir and files at all?
	    if ( tmp_file_list->GetEntries() == 0 ) {
		cout << endl << " Directory " << path_str.Data() << " or systematic folder " << systname << " not existent or doesn't contain files for HFOR type " << hfor_label.Data() << " !" << endl << endl;
		return;
	    }
	    
	    // Return to starting directory
	    gSystem->ChangeDirectory(savedir);

	    // Store full file paths
	    file_list->AddAll(tmp_file_list);

	    // Store respective cross-sections
	    for ( Int_t i = 0; i < tmp_file_list->GetEntries(); i++ ) {
		xs_string_list->Add(xs);
	    }
	    
	    // Clean-up
	    delete tmp_file_list;
    	}

	// Cross-check for number of entries in file_list and xs_string_list
	cout << endl << " No. of entries in file list = " << file_list->GetEntries() << endl
	     << " No. of entries in xsec list = " << xs_string_list->GetEntries() << endl << endl;

	
	// Close input sample list
	sample_list.close();
    }

    
    // Create merged output file for chosen systematic and hfor type
    const char* outputfilename=
	path_str+
	"/Wjets_hfor"
	+hfor_label
	+"_"
	+systname
	+"_hists.root";

    const char* outputfilename_nominal=
	path_str+
	"/Wjets_hfor"
	+hfor_label
	+"_hists.root";
    
    if ( syst == kNominal )
	cout
	    << endl << " Creating output file with name "  << endl
	    << endl << outputfilename_nominal << endl << endl
	    << "from" << endl << endl;
    else
	cout
	    << endl << " Creating output file with name "  << endl
	    << endl << outputfilename << endl << endl
	    << "from" << endl << endl;
    
    TFile *f_out;
    if ( syst == kNominal )
	f_out = new TFile(outputfilename_nominal,"RECREATE");
    else
	f_out = new TFile(outputfilename,"RECREATE");

    // =====================
    // Loop over input files
    // =====================
    TObjString *file;
    Bool_t first_file = kTRUE;
    Bool_t last_file = kFALSE;
    TString xs_str;
    Float_t xsec = 0;
    Float_t Nevt, scale;
    Double_t lumiMC;

//    TH1::SetDefaultSumw2(kTRUE);
    
    for ( Int_t i = 0; i < file_list->GetEntries(); i++ ) {
        // Keep track of last file
        if ( i == ( file_list->GetEntries() - 1 ) ) {
	    last_file = kTRUE;
	    cout << endl << " This is the last file! " << endl << endl;
	}
	
	file = (TObjString*)file_list->At(i);
	cout << file->GetString().Data() << endl;
	xs_str = ((TObjString*)xs_string_list->At(i))->GetString();
	xsec = xs_str.Atof();
	cout << " xsec = " << xsec << endl;
	
	// Open input file for reading
	TFile  *f_in = new TFile(file->GetString().Data(), "READ");
	
	//
	// Copy and scale histograms
	//

	// Get original total number of events in this sample before hfor splitting
	Nevt = ((TH1F*)f_in->Get("job_info/h_nevts"))->GetBinContent(2);
	cout << " Nevt = " << Nevt << endl;

	// Get MC sample lumi for cross-section normalization
	lumiMC = Nevt/xsec;
	cout << " lumiMC = " << lumiMC << endl;

	// Get scale factor for histograms of this sample
	scale =  ( lumiMC == 0. ) ? 0. : data_lumi/lumiMC;
	cout << " scale = " << scale << endl;

	// Clone all histograms from first file and add all entries 
	// for subsequent samples (merging)
	copy_folder(f_in, f_out, scale, first_file, last_file);

	// Close input file
	f_in->Close();
	
	if ( first_file == kTRUE ) first_file = kFALSE;
    }


    // Write output file
    f_out->Write();

    
    // Clean-up
    delete f_out;
}

//____________________________________________________________________

void copy_folder(TDirectory *SrcDir, TDirectory *DestDir, Float_t scale,
		 Bool_t first_file, Bool_t last_file) {
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


TList* ListFiles(const char *dirname, TString sample, SystMode syst, TString hfor_label) {
    //
    // Get files in directory dirname for given systematics
    //
    TSystemDirectory file_dir(dirname, dirname);
    TList *files = file_dir.GetListOfFiles();
    TList *sel_files = new TList();
    TObjString *str;
    TString hfor_suffix = "hfor"+hfor_label;
    if (files) {
	TSystemFile *file;
	TString fname;
	
	TIter next(files);
	while ((file=(TSystemFile*)next())) {
	    fname = file->GetName();

	    //Ask for filename containing syst mode
	    if( fname.Contains(sample) &&
//		fname.Contains(fgSystNames[syst]) &&
		fname.Contains(hfor_suffix) &&
		fname.Contains(".root") ) {
		//Ask for filename containing no other syst modes (needed for nominal having no filename tag)
		if(syst>0) {
		    fname = TString(dirname)+"/"+fname;
		    str = new TObjString(fname);
		    sel_files->Add(str);
		} else {
// 		    Bool_t real_nominal = kTRUE;
// 		    for(Int_t i=1; i<kNumSystModes; i++) {
// 			cout << i << endl;
// 			if(fname.Contains(fgSystNames[i])) {
// 			    real_nominal = kFALSE;
// 			    cout << fname << endl;
// 			}
// 		    }
// 		    if(real_nominal) {
			fname = TString(dirname)+"/"+fname;
			cout << fname << endl;
			str = new TObjString(fname);
			sel_files->Add(str);
// 		    }
		}
		
	    }
	}
    }
    
    return sel_files;
    
}

