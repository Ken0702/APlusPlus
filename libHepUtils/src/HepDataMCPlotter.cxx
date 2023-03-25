//____________________________________________________________________
//
// DATA/MC Histogram Plotter
//
// Task for creating and plotting lists of stacked or overlayed
// MC histograms with DATA on top
//
// * The same binning for all histograms is required
// * Until now only 1d histograms are supported
//
// The plots are made from input files containing histograms for the
// various MC samples (signal + bckg) as well as DATA. Here, it is
// assumed that the directory and histogram structure is the same for
// each input file. The samples will be normalized w.r.t. the given
// luminosities
//
// The list of plots can be exported in two ways (combination
// possible):
//
// (1) All plots are stored in a single .root file. Here, the
// directory structure of the input files is preserved.
//
// (2) Export to various graphical formats like eps, ps, pdf, png or C
// macro. Since the number of plots is usually high, the directory
// structure of the input files is mapped onto the file system and the
// image files will be stored in their respective directories.  In
// case of an already existing version of the directory tree a backup
// copy is created first under the name $ExportDir_backup where
// $ExportDir indicates the name of the export directory given by
// SetExportDir().
//
// Samples can be combined in groups for better drawing. See the
// examples below. The way of drawing can be changed afterwards via
// HepDataMCPlot::SetGroupHistograms().
//
// Legend: The default behaviour is not to draw a legend inside the
// plots since the number of samples is nornmally high and therefore
// the legend would fit badly into the pad. (However, this behaviour
// can be changed with the help of the SetLegend() member function,
// accessible alspo from the TBrowser) It is recommended to draw the
// legend in an additional pad. In case of an exported .root file this
// is easily possible by using the HepDataMCPlot::DrawLegendPad()
// function (can also be used from the TBrowser). In case of exported
// graphical files an additional file containing only the legend is
// created in toplevel export directory under the name Legend.eps etc.
//
// Example 1 (ungrouped histograms):
// {
//     HepDataMCPlotter *pl = new HepDataMCPlotter("single-top", "Single-Top Analysis");
//     pl->SetExportDir("~/atlas/APlusPlus/libSingleTop/hist/tChannelAnalysis/test");
//     pl->SetLumiDATA(100.);
//     pl->AddMCSample("$LIBSINGLETOP/hist/tChannelAnalysis/tchannel_enu.root",
//                     "MC single-top t-channel e#nu", 120., kOrange);
//     pl->AddMCSample("$LIBSINGLETOP/hist/tChannelAnalysis/tchannel_munu.root",
//                     "MC single-top t-channel #mu#nu", 130., kRed);
//     pl->AddMCSample("$LIBSINGLETOP/hist/tChannelAnalysis/tchannel_taunu.root",
//                     "MC single-top t-channel #tau#nu", 110., kMagenta);
//     pl->AddMCSample("$LIBSINGLETOP/hist/tChannelAnalysis/schannel_enu.root",
//                     "MC single-top s-channel e#nu", 120., kBlue);
//     pl->AddMCSample("$LIBSINGLETOP/hist/tChannelAnalysis/schannel_muunu.root",
//                     "MC single-top s-channel #mu#nu", 120., kCyan);
//     pl->AddMCSample("$LIBSINGLETOP/hist/tChannelAnalysis/Wtproduction.root",
//                     "MC single-top Wt production", 90., kGreen);
//     gROOT->GetListOfBrowsables()->Add(pl);
//     new TBrowser;
// }
//
//
// Example 2 (grouped histograms):
// {
//     HepDataMCPlotter *pl_test = new HepDataMCPlotter("DATA-MC plotter", "Create all Data-MC plots");
//     pl_test->SetWorkingDir("$LIBSINGLETOP/hist/test/plots_datamc");
//     pl_test->SetLumiDATA(100.);
//    
//     // ttbar
//     pl_test->AddMCFolder("ttbar", "ttbar lnln", kRed);
//     (pl_test->AddMCSample("ttbar", "$LIBSINGLETOP/hist/test/ttbar_lnln_np0_hists.root",
//                           "MC ttbar, Np0", 160., kRed-4))->SetLumi(160.);
//     (pl_test->AddMCSample("ttbar", "$LIBSINGLETOP/hist/test/ttbar_lnln_np1_hists.root",
//                           "MC ttbar, Np1", 180., kRed-7))->SetLumi(180.);
//     (pl_test->AddMCSample("ttbar", "$LIBSINGLETOP/hist/test/ttbar_lnln_np2_hists.root",
//                           "MC ttbar, Np2", 150., kRed-9))->SetLumi(150.);
//    
//     // Z+jets
//     pl_test->AddMCFolder("Zjets", "Z+jets", kBlue);
//     (pl_test->AddMCSample("Zjets", "$LIBSINGLETOP/hist/test/zee_np0_hists.root",
//                           "MC Zee, Np0", 70., kBlue-4))->SetLumi(70.);
//     (pl_test->AddMCSample("Zjets", "$LIBSINGLETOP/hist/test/zee_np1_hists.root",
//                           "MC Zee, Np1", 60., kBlue-7))->SetLumi(60.);
//     (pl_test->AddMCSample("Zjets", "$LIBSINGLETOP/hist/test/zmumu_np0_hists.root",
//                           "MC Zmumu, Np0", 90., kBlue-9))->SetLumi(90.);
//     (pl_test->AddMCSample("Zjets", "$LIBSINGLETOP/hist/test/zmumu_np1_hists.root",
//                           "MC Zmumu, Np1", 110., kBlue-10))->SetLumi(110.);
//     (pl_test->AddMCSample("Zjets", "$LIBSINGLETOP/hist/test/zmumu_np2_hists.root",
//                           "MC Zmumu, Np2", 120., kBlue-8))->SetLumi(120.);
// }
//
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepDataMCPlotter
#include <HepDataMCPlotter.h>
#endif
#include <HepDataMCSample.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TH1.h>
#include <TKey.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TObjString.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TObjectTable.h> 
#include <TLegendEntry.h>
#include <TMath.h>
#include <iostream>
#include <TFeldmanCousins.h>
#include <TStyle.h>
#include <TTree.h>

using namespace std;

#ifndef __CINT__
ClassImp(HepDataMCPlotter);
#endif


//____________________________________________________________________

HepDataMCPlotter::HepDataMCPlotter(const char* name, const char* title) :
    TTask(name, title) {
    //
    // Default constructor
    //
    fListOfDataFileNames = new TList;
    fListOfDataFiles     = new TList;
    fListOfPlots         = new TList;
    fListOfInFiles       = new TList;
    fMCSamples           = new TList;
    fMCSingleSamples     = new TList;
    fMCFolders           = new TList;
    fNtuplePlotCmds      = new TList;
    fLumiDATA = 1.;
    fExportDir = new TString("./");
    fExportROOT = kTRUE;
    fExportEPS  = kFALSE;
    fExportPS   = kFALSE;
    fExportPDF  = kFALSE;
    fExportPNG  = kFALSE;
    fExportC    = kFALSE;
    fDrawLegend = kFALSE;
    fGroupHistograms = kTRUE;
    fDrawMCError     = kFALSE;
    fDrawDataZeroEntryErrors = kTRUE;
    fUnifyMCStack    = kFALSE;
    fOutputRootFile = 0;
    fLogyaxis = kFALSE;
    fDrawCMSLumiAtlasLabel = kTRUE;
    fUseAtlasStyle = kTRUE;
    fAtlasLabelStatus = "Internal";
    fCenterOfMassEnergyLabel = "7";
    fLuminosityLabel = "4.7 fb^{-1}";
    fDrawSignalOverlay = kFALSE;
    fScaleOverlay = 1.;
    fDrawSignificance = kFALSE;
}

//____________________________________________________________________

HepDataMCPlotter::~HepDataMCPlotter() {
    //
    // Default destructor
    //
    fListOfDataFileNames->Delete(); delete fListOfDataFileNames;
    fListOfDataFiles->Delete();     delete fListOfDataFiles;
    delete fListOfPlots;
    delete fListOfInFiles;
    fMCSamples->Delete(); delete fMCSamples;
    fMCFolders->Delete(); delete fMCFolders;
    fNtuplePlotCmds->Delete(); delete fNtuplePlotCmds;
    delete fExportDir;
    if ( fOutputRootFile != 0 ) delete fOutputRootFile;
}

//____________________________________________________________________

void HepDataMCPlotter::AddDataFile(const char* path, const char* label = "DATA") {
    //
    // Add file to the list of data files.
    // The label is used for the legend
    //
    fListOfDataFileNames->Add(new TNamed(path, label));
}

//____________________________________________________________________

HepDataMCSample* HepDataMCPlotter::AddMCSample(const char* path,
					       const char* label,
					       Color_t color,
					       Style_t line_style,
					       Width_t line_width,
					       Style_t fill_style) {
    //
    // Add MC sample given by the path to its histogram file.
    // The label will be used in the corresponding legend entry.
    // The line and fill attributes are used for drawing the histograms
    // in stacked and overlay mode, resp.
    //
    HepDataMCSample *sample = new HepDataMCSample(path, label, color, line_style,
						  line_width, fill_style); 
    fMCSamples->Add(sample);
    fMCSingleSamples->Add(sample);
    return sample;
}

//____________________________________________________________________

HepDataMCSample* HepDataMCPlotter::AddMCSample(const char* path,
					       const char* label,
					       Color_t color) {
    //
    // Same as above but line style and width, and fill style are set
    // to the defaults given by the current style
    //
    return AddMCSample(path, label, color, gStyle->GetHistLineStyle(),
		       gStyle->GetHistLineWidth(),
		       gStyle->GetHistFillStyle());
}

//____________________________________________________________________

HepDataMCSample* HepDataMCPlotter::AddMCSample(const char* path,
					       const char* label,
					       Float_t xsec,
					       Color_t color) {
    //
    // Same as above but line style and width, and fill style are set
    // to the defaults given by the current style
    //
    if ( gDebug > 0 ) Info("AddMCSample", "path = %s", path);
    HepDataMCSample *sample = AddMCSample(path, label, color,
					  gStyle->GetHistLineStyle(),
					  gStyle->GetHistLineWidth(),
					  gStyle->GetHistFillStyle());
    sample->SetXsection(xsec);
    return sample;
}

//____________________________________________________________________

HepDataMCSample* HepDataMCPlotter::AddMCSample(HepDataMCFolder *folder,
					       const char* path,
					       const char* label,
					       Float_t xsec,
					       Color_t color) {
    //
    // Add an MC sample given by the path to its histogram file to the
    // given folder. The folder can be used to group histograms in
    // order to make the plot and its legend better readable, in
    // particluar when dealing with many samples. The given label will
    // be used in the corresponding legend entry. The line and fill
    // attributes are used for drawing the histograms in stacked and
    // overlay mode, resp. The given x-section is used for the
    // luminosity weighting when drawing the stack of all samples.
    //
    HepDataMCSample *sample = new HepDataMCSample(path, label, color,
						  gStyle->GetHistLineStyle(),
						  gStyle->GetHistLineWidth(),
						  gStyle->GetHistFillStyle());
    folder->Add(sample);
    sample->SetXsection(xsec);
    return sample;
}

//____________________________________________________________________

HepDataMCSample* HepDataMCPlotter::AddMCSample(const char* folder_name,
					       const char* path,
					       const char* label,
					       Float_t xsec,
					       Color_t color) {
    //
    // Add an MC sample given by the path to its histogram file to the
    // given folder. The folder can be used to group histograms in
    // order to make the plot and its legend better readable, in
    // particluar when dealing with many samples. The given label will
    // be used in the corresponding legend entry. The line and fill
    // attributes are used for drawing the histograms in stacked and
    // overlay mode, resp. The given x-section is used for the
    // luminosity weighting when drawing the stack of all samples.
    //

    // Get folder
    HepDataMCFolder *folder = (HepDataMCFolder*)fMCFolders->FindObject(folder_name);
    if ( folder == 0 ) {
	Error("AddMCSample",
	      "Could not find folder \"%s\" in the list of existing folders. Abort!",
	      folder_name);
	gSystem->Abort(0);
    }

    // Create sample and add it to the given folder
    HepDataMCSample *sample = new HepDataMCSample(path, label, color,
						  gStyle->GetHistLineStyle(),
						  gStyle->GetHistLineWidth(),
						  gStyle->GetHistFillStyle());
    folder->Add(sample);
    sample->SetXsection(xsec);
    return sample;
}

//____________________________________________________________________

void HepDataMCPlotter::InitInput() {
	// 
	// Open all input files for reading and add them to the list of
	// input files.
	// In case single HepDataMCPlots are being produced instead
	// of using HepDataMCPlotter::Exec() this routine must be 
	// called first explicitly.
	//
    if ( fListOfDataFileNames != 0 ) {
	TIter next_file(fListOfDataFileNames);
	TNamed *file = 0;
	TFile *f = 0;
	while ( (file = (TNamed*)next_file()) ) {
            if ( gSystem->AccessPathName(file->GetName()) ) {
                Warning(__FUNCTION__, "Required data file %s does not exist; skipping", file->GetName());
                continue; // only consider existing DATA files
            }
	    f = new TFile( file->GetName(), "read");
	    fListOfInFiles->Add(f);
	    fListOfDataFiles->Add(f);
	}
        Info(__FUNCTION__, "No. of input files after adding data samples: %d", fListOfInFiles->GetEntries());
    }
    TFile *mcfile = 0;
    TIter next_mcfolder(fMCFolders);
    HepDataMCFolder *mc_folder = 0;
    HepDataMCSample *mc_sample = 0;
    while ( (mc_folder = (HepDataMCFolder*)next_mcfolder()) ) {
        TIter next_mcsample(mc_folder->GetMCSamples());
        while ( (mc_sample = (HepDataMCSample*)next_mcsample()) ) {
            mcfile = mc_sample->Open();
            if ( mcfile != 0 ) {
                fListOfInFiles->Add(mcfile);
                if ( gDebug > 0 ) Info("InitInput", "Add file %s", mcfile->GetName());
            }
        }
    }
    Info(__FUNCTION__, "No. of input files after adding grouped MC samples: %d", fListOfInFiles->GetEntries());
    TIter next_mcsample2(fMCSamples);
    while ( (mc_sample = (HepDataMCSample*)next_mcsample2()) ) {
        mcfile = mc_sample->Open();
        if ( mcfile != 0 ) {
            fListOfInFiles->Add(mcfile);
            if ( gDebug > 0 ) Info("InitInput", "Add file %s", mcfile->GetName());
        }
    }
    Info(__FUNCTION__, "No. of input files after adding separate MC samples: %d", fListOfInFiles->GetEntries());
}

//____________________________________________________________________

void HepDataMCPlotter::Exec(Option_t *option) {
    //
    // Execute task
    //
    // The plot building process is performed in two steps: First a
    // list of plots is created by inspecting all keys in the input
    // files. In the next step for each list entry a plot is created
    // and all existing histograms are added. The directory structure
    // is preserved. The two steps are needed because only one plot
    // can be kept in memory at a time. Otherwise one would have to
    // keep all plots in memory in parallel which would blow the
    // available RAM.
    //
    fListOfInFiles->Delete();
    
    // Execute in batch mode to prevent from drawing all the histograms
    // (very time-consuming when working from remote)
    Bool_t savmode = gROOT->IsBatch();
    gROOT->SetBatch(kTRUE);

    // remove warning issued from h.Sumw2() in normalize()
    TH1::SetDefaultSumw2();
    
    cout << endl;
    Info("Exec", "Begin of task \"%s\"", GetName());
    
    // Open all input files for reading and add them to the list of
    // input files
    InitInput();
	
    // Build the list of plots
    BuildListOfPlots();

    // Save a backup copy of the output directory in case the export
    // directory already exists
    if ( !gSystem->AccessPathName(gSystem
				  ->ExpandPathName(fExportDir
						   ->Data())) ) {
	Info("Exec", "Save backup copy of export directory \"%s\"",
	     fExportDir->Data());
	TString path2 = fExportDir->Strip(TString::kTrailing, '/');
	Info("Exec", "mv --force --strip-trailing-slashes %s %s_backup",
			   path2.Data(),
			   path2.Data());
	gSystem->Exec(Form("cp -r %s %s_backup",
			   path2.Data(),
			   path2.Data()));
	gSystem->Exec(Form("rm -fr  %s", fExportDir->Data()));
    }
    gSystem->Exec(Form("mkdir  %s", fExportDir->Data()));

    // Open .root output file for writing
    if ( fExportROOT ) {
	if ( fOutputRootFile != 0 ) delete fOutputRootFile;
	fOutputRootFile = new TFile(Form("%s/%s.root",
					 fExportDir->Data(),
					 GetName()), "recreate");
    }
    
    // Loop over the list of plot names and build all plots and export
    // them
    TIter next_plot(fListOfPlots);
    TNamed *pname = 0;
    TString qcdtool = TString("/QCDMMTool");
    while ( (pname = (TNamed*)next_plot()) ) {
	if ( qcdtool.EqualTo(pname->GetTitle()) ) {
	    Warning(__FUNCTION__, "Using hardcoded tool name to skip "
		    "histograms created only in QCDMMTool");
	    continue; 
	} // SK ugly hack
	if ( gDebug > 0 ) {
	    Info(__FUNCTION__, "Start building of plot %s %s",
		 pname->GetName(), pname->GetTitle());
	}
        HepDataMCPlot *pl = BuildPlot(pname);  
	if ( pl == 0 ) continue;
 	if ( gDebug > 0 ) {
	    Info(__FUNCTION__, "Created plot %s %s",
		 pl->GetName(), pl->GetTitle());
	}
 	Export(pl, pname);   
 	delete pl->GetHistDATA();
 	delete pl;
    }

    // Loop over the list of ntuple plot commands and create plots
    TIter next_cmd(fNtuplePlotCmds);
    HepNtuplePlotCmd *cmd = 0;
    while ( (cmd = (HepNtuplePlotCmd*)next_cmd()) ) {
        Info("Exec", "Start building of ntuple plot %s %s",
	    cmd->GetPlotName(), cmd->GetPlotTitle());
        HepDataMCPlot *pl = BuildNtuplePlot(cmd);  
	if ( pl == 0 ) continue;
 	Info("Exec", "Created plot %s %s",
 	     pl->GetName(), pl->GetTitle());
	Export(pl, new TNamed(pl->GetName(), ""));   
	//OMK 	delete pl->GetHistDATA();
	delete pl;
    }
    
    // Close all input files
    TFile *infile = 0;
    TIter next_infile(fListOfInFiles);
    while ( (infile = (TFile*)next_infile()) ) {
	infile->Close();
    }

    // Close output .root file
    delete fOutputRootFile; fOutputRootFile = 0;
    
    Info("Exec", "End of task \"%s\"", GetName());
    cout << endl;

    // Re-set mode
    gROOT->SetBatch(savmode);
}

//____________________________________________________________________

void HepDataMCPlotter::BuildListOfPlots() {
    //
    // Loop over all histograms in the DATA file (if exisiting) or the
    // 1st MC file otherwise and create a list containg the names of
    // all plots
    //
    TFile *infile = 0;
    TIter next_infile(fListOfInFiles);
    while ( (infile = (TFile*)next_infile()) ) {
	if ( gDebug > 0 ) {
	    Info("BuildListOfPlots", "Processing input file %s", infile->GetName());
	}
	ReadHistogramsFromDir(infile);
    }
}
	
//____________________________________________________________________

void HepDataMCPlotter::ReadHistogramsFromDir(TDirectory *dir) {
    //
    // Read all histograms from given directory
    // and all its sub-directories
    //
    dir->cd();
    if ( gDebug > 0 ) Info("ReadHistogramsFromDir", "cd %s", dir->GetName());

    TIter next_key(dir->GetListOfKeys());
    TKey *key = 0;
    while ( (key = (TKey*)next_key()) ) {
	if ( gDebug > 0 ) Info("ReadHistogramsFromDir",
                               "key Name = %s, Title = %s, Class = %s",
                               key->GetName(), key->GetTitle(), key->GetClassName());
	TString keyname = key->GetName();
	if ( keyname.Contains("fullCutFlow") ||
	     keyname.Contains("allEventsHFOR") ) {
	    // skip cutflow histograms since naming is different
	    // for data one and mc with systematics...
	    // physics_systematic_fullCutFlow...
	    // for data it's always nominal, which causes problems
	    continue;
	}

	if ( strcmp(key->GetClassName(), "TDirectoryFile") == 0 ) {
	    // Change into sub-directory
	    TDirectoryFile *dir = (TDirectoryFile*)key->ReadObj();
	    ReadHistogramsFromDir(dir);
	    delete dir;
	} else if ( (strcmp(key->GetClassName(), "TH1F") == 0)
		    || (strcmp(key->GetClassName(), "TH1D") == 0) ) {
	    // Add name and path of 1-dim histogram to list of plots
	    // if the plot has not been added before already
	    TString path = HepDataMCPlot::GetPathInsideFile(dir);
	    
	    Bool_t alreadyInList = kFALSE;
	    TIter next_plot(fListOfPlots);
	    while(TNamed* pl = (TNamed*)next_plot()){
		if((strcmp(pl->GetName(),key->GetName()) == 0) &&
		   (strcmp(pl->GetTitle(),path.Data()) == 0))
		  alreadyInList = kTRUE;
	    }
	    if (!alreadyInList) {
		fListOfPlots->Add(new TNamed(key->GetName(), path.Data()));
		if ( gDebug > 0 ) Info("ReadHistogramsFromDir",
                                       "Add plot %s, %s", key->GetName(), path.Data());
	    }
	}
    }
}

//____________________________________________________________________

TH1F* HepDataMCPlotter::GetHistFromFile(TFile *f, TNamed *PlotName) {
    TH1 * h = 0;
    f->GetObject(Form("%s/%s", PlotName->GetTitle(), PlotName->GetName()), h);
    if ( !h ) {
        if ( gDebug > 0 )
            Warning(__FUNCTION__, "Plot named %s/%s not found in file %s.",
                    PlotName->GetTitle(), PlotName->GetName(), f->GetName());
        //gSystem->Abort();
        return nullptr;
    }
    TH1F * h1 = dynamic_cast<TH1F *>(h);
    if ( !h1 ) {
        TDirectory::TContext directoryContext(h->GetDirectory());
        h1 = new TH1F();
        h->Copy(*h1);
        h->SetDirectory(gDirectory);
    }
    return h1;
}

//____________________________________________________________________

HepDataMCPlot* HepDataMCPlotter::CreatePlotFromHist(TH1F *h) const {
    //
    // Create plot and use the given histogram for setting name, title
    // and axis titles of the plot
    //
    h->SetStats(kFALSE);
    if ( gDebug > 0 ) Info("CreatePlotFromHist", "plot = %s %s", h->GetName(), h->GetTitle());
    HepDataMCPlot *plot = new HepDataMCPlot(h->GetName(), h->GetTitle());
    plot->SetXTitle(h->GetXaxis()->GetTitle());
    plot->SetYTitle(h->GetYaxis()->GetTitle());
    plot->SetUseAtlasStyle(fUseAtlasStyle);
    plot->SetAtlasLabelStatus(fAtlasLabelStatus);
    plot->SetCenterOfMassEnergyLabel(fCenterOfMassEnergyLabel);
    plot->SetLuminosityLabel(fLuminosityLabel);
    plot->SetDrawCMSLumiAtlasLabel(fDrawCMSLumiAtlasLabel);
    plot->SetDrawDataZeroEntryErrors(fDrawDataZeroEntryErrors);
    plot->SetDrawMCError(fDrawMCError, kFALSE);
    plot->SetUseOverflowUnderflow(fUseOverflowUnderflow);
    plot->SetDrawSignalOverlay(fDrawSignalOverlay);
    plot->SetScaleOverlay(fScaleOverlay);
    plot->SetDrawSignificance(fDrawSignificance);

    // stop here because samples/folders are added in BuildPlot()
    return plot; // <<<<==== Why here ??? OMK
    
    // Copy the existing folder structure to the plot
    TIter next_folder(fMCFolders);
    HepDataMCFolder *folder = 0;
    while ( (folder = (HepDataMCFolder*)next_folder()) ) {
	plot->AddMCFolder(folder);
    }

    // Copy single samples to the plot
    TIter next_singlesample(fMCSingleSamples);
    HepDataMCSample *single_sample = 0;
    while ( (single_sample = (HepDataMCSample*)next_singlesample()) ) {
	plot->AddMCSingleSample(single_sample);
    }

    //    return plot; <<<<==== ??? OMK
}

//____________________________________________________________________

void HepDataMCPlotter::AddHistMC(HepDataMCPlot *plot, TH1F *h,
				 HepDataMCSample *mc_sample,
				 HepDataMCFolder *folder,
				 Float_t LumiScale) const {
    //
    // Add MC histogram to given plot.
    // The histogram ist normalized.
    // In case of non-existing folder the zero pointer is used (default).
    //
    Normalize(*h, mc_sample, LumiScale);
    if ( folder == 0 ) {
	plot->AddHistMC(h, mc_sample);
	if ( gDebug > 0 ) {
	    Info("AddHistMC", "hist %s   sample %s", h->GetName(),
		 mc_sample->GetFile()->GetName());
	}
    } else { 
	plot->AddHistMC(h, folder, mc_sample);
	if ( gDebug > 0 ) {
	    Info("AddHistMC",
		 "hist %s   sample %s   folder %s", h->GetName(),
		 mc_sample->GetFile()->GetName(), folder->GetName());
	}
    }
}

//____________________________________________________________________

HepDataMCPlot* HepDataMCPlotter::BuildPlot(TNamed *PlotName) {
    //
    // Build plot of given name
    //
    HepDataMCPlot *plot = 0;
    TFile *f;
    TH1F *h = 0;
    


    //
    // Step 1: Add DATA histogram to plot (if existing)
    // ================================================
    if ( fListOfDataFiles->GetEntries() > 0 ) {
	TIter next_file(fListOfDataFiles);
	f = (TFile*) next_file();           // first file in list
	h = GetHistFromFile(f, PlotName);   // init hist

	while ( (f = (TFile*)next_file()) ) // loop over remaining files
	    if ( h != 0) { h->Add(GetHistFromFile(f, PlotName)); }
	
	if ( h != 0 ) {
	    if ( plot == 0 ) plot = CreatePlotFromHist(h);
	    TNamed *data = (TNamed*) fListOfDataFileNames->At(0);
	    plot->SetHistDATA(h, data->GetTitle(), fLumiDATA);

	}
    }
    
    //
    // Step 2: Loop over all grouped samples and add normalized MC
    // histograms to MC stack of the plot
    // ===========================================================
    TIter next_folder(fMCFolders);
    HepDataMCSample *mc_sample = 0;
    HepDataMCFolder *folder = 0;
    Int_t skip_h = 0;
    Int_t tot_h = 0;
    while ( (folder = (HepDataMCFolder*)next_folder()) ) {
	TIter next_mcsample(folder->GetMCSamples());
	while ( (mc_sample = (HepDataMCSample*)next_mcsample()) ) {
	    f = mc_sample->GetFile();  
	    h = GetHistFromFile(f, PlotName);
	    if ( h != 0 ) {
		if ( plot == 0 ) plot = CreatePlotFromHist(h);
		AddHistMC(plot, h, mc_sample, folder);
	    } else {
                // don't throw or crash, just warn and count the missing samples
                Warning(__FUNCTION__, "Couldn't find sample %s", mc_sample->GetName());
                skip_h++;
            }
            tot_h++;
	}
	if ( plot != 0 ) plot->AddMCFolder(folder);
    }
    if ( skip_h > 0 )
        Warning(__FUNCTION__, "Skipped %d/%d MC samples for plot %s/%s",
                skip_h, tot_h, PlotName->GetTitle(), PlotName->GetName());

    // temporary solution: skip this entirely
    if ( skip_h > 0 ) {
        Error(__FUNCTION__, "There were some samples to skip; skip entire plot.");
        return nullptr;
    }
    
    //
    // Step 3: Loop over all remaining un-grouped samples and add
    // normalized MC histograms to MC stack of the plot
    // ==========================================================
    TIter next_mcsample(fMCSamples);
    while ( (mc_sample = (HepDataMCSample*)next_mcsample()) ) {
        f = mc_sample->GetFile();  
	h = GetHistFromFile(f, PlotName);
	if ( h != 0 ) {
	    if ( plot == 0 ) plot = CreatePlotFromHist(h);
	    AddHistMC(plot, h, mc_sample);
	    if ( plot != 0 ) plot->AddMCSingleSample(mc_sample);
	}
    }
    
    plot->SetLumiDATA(fLumiDATA);
    
    return plot;
}

//____________________________________________________________________

void HepDataMCPlotter::Export(HepDataMCPlot *Plot, TNamed *PlotName) {
    //
    // Export the given plot to the .root output file and to the file
    // system in various graphical formats
    //
    
    // Set drawing of MC errors
    Plot->SetDrawMCError(fDrawMCError, kFALSE);
    
    // Set unifying the MC stack
    Plot->SetUnifyMCStack(fUnifyMCStack, kFALSE);

    // Set drawing of 0 data entry bin upper errors
    Plot->SetDrawDataZeroEntryErrors(fDrawDataZeroEntryErrors);
    
    // Export to .root file
    TDirectory *dir = 0;
    if ( fExportROOT ) {
	dir = (TDirectory*)fOutputRootFile
	    ->FindObject(PlotName->GetTitle());
	if ( dir == 0 )
	    dir = MkDirWithParents(fOutputRootFile, PlotName->GetTitle());
	dir->cd();
	if ( Plot->GetHistDATA() != 0 ) Plot->GetHistDATA()->SetDirectory(dir);
	if ( Plot->GetMCHistStack()->GetHists() == 0 ) {
	    Warning("Export", "No hists in stack");
	    cerr.flush();
	} else {
	    TIter next_hist(Plot->GetMCHistStack()->GetHists());  
	    TH1F *h = 0;
	    while ( (h = (TH1F*)next_hist()) ) {   
		h->SetDirectory(dir);
	    }
	    Plot->Write();  
	}
    }
    
    // Create a new (corresponding) directory on the file system
    TString fullpath;
    if ( fExportEPS || fExportPS || fExportPDF || fExportPNG || fExportC ) {
	fullpath.Append(fExportDir->Data());
	fullpath.Append(PlotName->GetTitle());
	gSystem->Exec(Form("mkdir -p %s", fullpath.Data()));
	
	// Create canvas
        TCanvas *c = new TCanvas("c1", "c1");
        c->SetName(Form("c_%s",  Plot->GetName()));
        c->SetTitle(Plot->GetTitle());
        if ( fLogyaxis ) c->SetLogy();

        Plot->Draw(fDrawLegend?"":"noleg");
        
        // Save plot in different formats
        if ( fExportEPS )
	    c->SaveAs(Form("%s/%s.eps", fullpath.Data(), Plot->GetName()));
        if ( fExportPS )
	    c->SaveAs(Form("%s/%s.ps",  fullpath.Data(), Plot->GetName()));
        if ( fExportPDF )
	    c->SaveAs(Form("%s/%s.pdf", fullpath.Data(), Plot->GetName()));
        if ( fExportPNG )
	    c->SaveAs(Form("%s/%s.png", fullpath.Data(), Plot->GetName()));
        if ( fExportC )
        c->SaveAs(Form("%s/%s.C",   fullpath.Data(), Plot->GetName()));  
        
        // Export graphics file(s) containing the legend
        Plot->DrawLegend();
        Plot->DrawLegendPad();
	    if ( fExportEPS )
	    c->SaveAs(Form("%s/Legend.eps", fExportDir->Data()));
	    if ( fExportPS )
	    c->SaveAs(Form("%s/Legend.ps",  fExportDir->Data()));
	    if ( fExportPDF )
	    c->SaveAs(Form("%s/Legend.pdf", fExportDir->Data()));
	    if ( fExportPNG )
		c->SaveAs(Form("%s/Legend.png", fExportDir->Data()));
	    if ( fExportC )
		c->SaveAs(Form("%s/Legend.C",   fExportDir->Data()));
	    
	    // Clean-up of canvas
	    delete c; 
    }
    
    // Delete clones histogram objects. Needed until ROOT bug in TLegend::Clone() is fixed.
    TLegend *LegendLarge = Plot->GetLegendLarge();
    if (LegendLarge != 0)  {
	TIter next_obj(LegendLarge->GetListOfPrimitives());  
	TLegendEntry *obj = 0;
	while ( (obj = (TLegendEntry*)next_obj()) ) {
	    delete (TH1F*)obj->GetObject();
	}   
    }
}

//____________________________________________________________________

TDirectory* HepDataMCPlotter::MkDirWithParents(TFile *f, const char* dir) {
    //
    // Create the given directoy and all of its parents if necessary
    // in the given file
    //
    TString fulldir(dir);
    TObjArray *subdirs = fulldir.Tokenize("/");
    f->cd();
    TIter next_dir(subdirs);
    TObjString *subdir = 0;
    while ( (subdir = (TObjString*)next_dir()) ) {
	if ( gDirectory->FindObject(subdir->GetString().Data()) == 0 ) {
	    gDirectory->mkdir(subdir->GetString().Data());
	}
	gDirectory->cd(subdir->GetString().Data());
    }

    //Clear objects
    delete subdirs;
    delete subdir;

    return gDirectory;
}

//____________________________________________________________________

void HepDataMCPlotter::SetGroupHistograms(Bool_t GroupHistograms) {
    //
    // Switch grouping of histograms on or off
    //
    fGroupHistograms = GroupHistograms;
    TIter next_plot(fListOfPlots);
    HepDataMCPlot *plot = 0;
    while ( (plot = (HepDataMCPlot*)next_plot()) ) {
	plot->SetGroupHistograms(GroupHistograms);
    }
}

//____________________________________________________________________

void HepDataMCPlotter::SetDrawMCError(Bool_t DrawMCError) {
    //
    // Switch drawing of MC errors on or off.
    //
    fDrawMCError = DrawMCError;
}

//____________________________________________________________________


void HepDataMCPlotter::SetDrawDataZeroEntryErrors(Bool_t DrawDataZeroEntryErrors) {
    //
    // Draw the Data error bars even for 0 data entry bins
    //
    fDrawDataZeroEntryErrors = DrawDataZeroEntryErrors;
}

//____________________________________________________________________

void HepDataMCPlotter::SetUnifyMCStack(Bool_t UnifyMCStack) {
    //
    // Unify MC stack histograms into a single histogram
    //
    fUnifyMCStack = UnifyMCStack;
}

//____________________________________________________________________

void HepDataMCPlotter::SetLogyaxis(Bool_t Logyaxis) {
    //
    // Plot y axis in logarithmic scale
    //
    fLogyaxis = Logyaxis;
}

//____________________________________________________________________

void HepDataMCPlotter::AddMCFolder(const char* name, const char* title,
				   Color_t color) {
    //
    // Add folder for grouping histograms
    //
    // Line and fill style are set to the defaults given by the
    // current style
    //
    HepDataMCFolder *folder = new HepDataMCFolder(name, title, color,
						  gStyle->GetHistLineStyle(),
						  gStyle->GetHistLineWidth(),
						  gStyle->GetHistFillStyle());
    fMCFolders->Add(folder);
    Info(__FUNCTION__,
	 "Added folder \"%s\" with title \"%s\" to list of MCFolders",
	 name, title);

    // Add folder to all plots also for later use
    TIter next_plot(fListOfPlots);
    HepDataMCPlot *plot = 0;
    while ( (plot = (HepDataMCPlot*)next_plot()) ) {
	plot->AddMCFolder(folder);
    }
}

//____________________________________________________________________

void HepDataMCPlotter::Normalize(TH1F &h, HepDataMCSample *sample,
    Float_t LumiScale) const {
    //
    // Weight given histogram with the lumi of the given MC sample and
    // normalize it to DATA luminosity.
    //
    // LumiScale is an additional factor which can be given to the MC
    // lumi (default = 1)
    //
	if(((TString)sample->GetTitle()).Contains("QCDANTIMUON")) LumiScale = 10506.0 / 50000.0; //kkreul: Why?
	if(((TString)sample->GetTitle()).Contains("QCDJetLepton")) LumiScale = 5549.0 / (18000.0+17700.0);
	Int_t first = 1;
	Double_t n = h.GetBinContent(first);
	     Double_t err = h.GetBinError(first);
	     cout<<endl;
	     cout<<h.GetName()<<"Sum = "<<n<<" Errr= "<<err<<endl;

	h.Sumw2();
    Double_t lumiMC = sample->GetLumi() * LumiScale;
    Double_t scale = ( lumiMC == 0. ) ? 0. : fLumiDATA/lumiMC;
    cout<<"NORMALIZE"<<sample->GetTitle()<<" "<<scale<<" "<<lumiMC<<" "<<LumiScale<<" "<<fLumiDATA<<endl;
    sample->Print();

    		//Double_t err = 0;
    		//h.GetXaxis()->GetFirst() ; // overflow underflow
    	    Int_t last  = 2;//h.GetXaxis()->GetLast()  ;


    h.Scale(scale);
    h.SetStats(kFALSE);
    n = h.GetBinContent(first);
        	     err = h.GetBinError(first);


        cout<<h.GetTitle()<<"Sum = "<<n<<" Errr= "<<err<<endl;
}

//____________________________________________________________________

void HepDataMCPlotter::AddNtuplePlotCmd(const char* TreeName,
					const char* VarExp,
					const char* Selection,
					const char* PlotName,
					const char* PlotTitle, Int_t Nbins,
					Double_t Xlow, Double_t Xup,
					const char* XTitle,
					const char* YTitle) {
    //
    // Add ntuple draw command (see TTree::Draw()) to be used by the
    // plotter
    //
    fNtuplePlotCmds->Add(new HepNtuplePlotCmd(TreeName, VarExp,
					      Selection, PlotName,
					      PlotTitle, Nbins,
					      Xlow, Xup, XTitle,
					      YTitle));
}

//____________________________________________________________________

HepDataMCPlot* HepDataMCPlotter::BuildNtuplePlot(HepNtuplePlotCmd *cmd) const {
    //
    // Build plot from ntuple by using TTree::Draw()
    //
    HepDataMCPlot *plot = 0;
    TFile *f = 0;
    TH1F  *h = 0;
    TTree *t = 0;

    // DATA
    if ( fListOfDataFiles->GetEntries() > 0 ) {
        TIter next_datafile(fListOfDataFiles);
	h = new TH1F(Form("%s_data", cmd->GetPlotName()),
		     cmd->GetPlotTitle(), cmd->GetNbins(),
		     cmd->GetXlow(), cmd->GetXup());
	h->Sumw2();
	h->SetXTitle(cmd->GetXTitle());
	h->SetYTitle(cmd->GetYTitle());
	while ( (f = (TFile*)next_datafile()) ) {
	     t = (TTree*)f->Get(cmd->GetTreeName());
	     t->Draw(Form("%s>>+%s", cmd->GetVarExp(), h->GetName()),
		     cmd->GetSelection());
	     if ( plot == 0 ) plot = CreatePlotFromHist(h);
	}
	TNamed *data = (TNamed*) fListOfDataFileNames->At(0);
	plot->SetName(cmd->GetPlotName());
	plot->SetTitle(cmd->GetPlotTitle());
	plot->SetHistDATA(h, data->GetTitle(), fLumiDATA);
    }

    // MC folders
    TIter next_folder(fMCFolders);
    HepDataMCSample *mc_sample = 0;
    HepDataMCFolder *folder = 0;
    // TH1F *h_nevt = 0;
    // Float_t NSelEvents = 0.;
    // Long64_t NTreeEntries = 0;
    while ( (folder = (HepDataMCFolder*)next_folder()) ) {
	TIter next_mcsample(folder->GetMCSamples());
	while ( (mc_sample = (HepDataMCSample*)next_mcsample()) ) {
	    f = mc_sample->GetFile();
	    t = (TTree*)f->Get(cmd->GetTreeName());
	    h = new TH1F(Form("%s_%s", cmd->GetPlotName(),
			      mc_sample->GetTitle()),
			 cmd->GetPlotTitle(), cmd->GetNbins(),
			 cmd->GetXlow(), cmd->GetXup());
	    h->SetXTitle(cmd->GetXTitle());
	    h->SetYTitle(cmd->GetYTitle());
	    h->Sumw2();
	    
	    // Check if tree exists
	    if ( t == 0 ) {
		Error("BuildNtuplePlot",
		      "Could not find tree %s in file %s! Abort!",
		      cmd->GetTreeName(), f->GetName());
		gSystem->Abort(0);
	    }
	    
	    t->Draw(Form("%s>>%s", cmd->GetVarExp(), h->GetName()),
		    cmd->GetSelection());
	    if ( plot == 0 ) {
		plot = CreatePlotFromHist(h);
		plot->SetName(cmd->GetPlotName());
		plot->SetTitle(cmd->GetPlotTitle());
	    }
		
	    // Check if no. of selected events and number of events inside the tree are the same.
	    // If not scale MC lumi accordingly
	    // Look for JobInfo histogram. If this doesn't exist try
	    // cut-flow (from SgTop-D3PDs)
	    // h_nevt = (TH1F*)f->Get("job_info/h_nevts");
	    // if ( h_nevt != 0 ) {
	    // 	NSelEvents = h_nevt->GetBinContent(10);
	    // }
	    // NTreeEntries = t->GetEntries();
	    Float_t LumiScale = 1.;

	    AddHistMC(plot, h, mc_sample, folder, LumiScale);
	}
	if ( plot != 0 ) plot->AddMCFolder(folder);
    }
    return plot;
}

//____________________________________________________________________

void HepDataMCPlotter::SetUseOverflowUnderflow(Bool_t status) {
    //
    // Set flag for drawing the under- and overflow bins
    //
    fUseOverflowUnderflow = status;
}

//____________________________________________________________________

void HepDataMCPlotter::SetDrawSignalOverlay(Bool_t status) {
    //
    // Set flag for drawing the signal MC (assumed to be the least
    // entry in the MC histogram stack) as overlay instead as part of
    // the stack
    //
    fDrawSignalOverlay = status;
}

//____________________________________________________________________

void HepDataMCPlotter::SetScaleOverlay(Float_t scale) {
    //
    // Set scale factor of the overlay histogram (see
    // SetDrawSignalOverlay())
    //
    fScaleOverlay = scale;
}

