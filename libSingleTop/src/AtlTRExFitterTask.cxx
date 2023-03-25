//____________________________________________________________________
//
// TRExFitter class for extracting the templates from the
// DataMCPlotter files in the required way
//
//
// Author: Stephan Kaphle <mailto: kaphle@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2018 (C) Oliver Maria Kind
//
#include <AtlTRExFitterTask.h>
#include <HepDataMCPlot.h>      // for HepDataMCPlot
#include <TClass.h>             // for TClass
#include <TCollection.h>        // for TIter
#include <TDirectoryFile.h>     // for TDirectoryFile
#include <TFile.h>              // for TFile
#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TH1.h>                // for TH1F, TH1
#include <TKey.h>               // for TKey
#include <TList.h>              // for TList
#include <TObjArray.h>          // for TObjArray
#include <TObjString.h>         // for TObjString
#include <TObject.h>            // for TObject
#include <TString.h>            // for TString, Form, operator==, TString::k...
#include <TSystemDirectory.h>   // for TSystemDirectory
#include <TSystemFile.h>        // for TSystemFile
#include <string.h>             // for strcmp
#include <iostream>             // for std
#include <iterator>             // for move_iterator, make_move_iterator
#include <memory>               // for unique_ptr, make_unique, operator==

using namespace std;

#ifndef __CINT__
ClassImp(AtlTRExFitterTask);
#endif

//____________________________________________________________________

AtlTRExFitterTask::AtlTRExFitterTask(const char* name, const char* title) :
    TTask(name, title) {
    //
    // Default constructor
    //
    fInputDiscrPath  = "";
    fInputDataMCFile = "";
    fOutputDir       = "";
    fOutputDiscrName = "";
}

//____________________________________________________________________

AtlTRExFitterTask::~AtlTRExFitterTask() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlTRExFitterTask::SetInputDataMCFile(const char* InputDataMCFile) {
    //
    // Set input DataMCPlotter file
    //
    fInputDataMCFile = InputDataMCFile;
}

//____________________________________________________________________

void AtlTRExFitterTask::SetInputDiscrPath(const char* InputDiscrPath) {
    //
    // Set name and full path of the inout discriminant inside the
    // plotter file
    //
    fInputDiscrPath = InputDiscrPath;
}

//____________________________________________________________________

void AtlTRExFitterTask::SetOutputDir(const char* OutputDir) {
    //
    // Set output directory
    //
    fOutputDir = OutputDir;
}

//____________________________________________________________________

void AtlTRExFitterTask::SetOutputDiscrName(const char* OutputDiscrName) {
    //
    // Set output discriminant name
    //
    fOutputDiscrName = OutputDiscrName;
}

//____________________________________________________________________

void AtlTRExFitterTask::Exec(Option_t *option) {
    //
    // Task execution
    //
    // The structure in A++ ntuples is
    //
    // |-- <path/to/systematic 1>/MCPlotter.root
    // |   |-- <internal/path/to/HepDataMCPlot for histname 1>
    // |   |   |-- <data>
    // |   |   |-- <MC folder 1>
    // |   |   |-- <...>
    // |   |    -- <MC folder m>
    // |   |-- <...>
    // |    -- <internal/path/to/HepDataMCPlot for histname nh>
    // |-- <...>
    //  -- <path/to/systematic ns>/MCPlotter.root
    //
    //
    // The desired structure for TRExFitter is
    //
    // |-- data.root
    // |-- <MC folder 1>.root
    // |   |-- TH1F <histname 1>_<systname 1>
    // |   |-- <...>
    // |    -- TH1F <histname nh>_<systname ns>
    // |-- <...>
    //  -- <MC folder m>.root
    //
    //
    // Options available:
    //   "ALL"     - use all histograms available in input file, default is to only use fInputDiscrPath;
    //               "ALL" is also used when fInputDiscrPath is not set
    //   "SPLIT"   - ignore folders and create one file per input sample
    //
    // For single histogram mode:
    // - list systematics, start with nominal, open all input files
    // - loop over syst input files
    //   - get HepDataMCPlot for the selected histogram (with fInputDiscrPath)
    //   - open data.root output file as RECREATE, save data hist, close (nominal)
    //   - loop over MC folders
    //     - open <sample>.root output as RECREATE for nominal, UPDATE for syst, write, close
    //
    // For "ALL" histogram mode:
    // - list systematics, start with nominal, open all input files
    // - loop over syst input files
    //   - for first file (nominal), create a list of all available histograms
    //   - loop over available histograms
    //     - get HepDataMCPlot for the selected histogram
    //     - open data.root output file as RECREATE for first hist, UPDATE otherwise, write and close
    //     - loop over MC folders
    //       - open <sample>.root as RECREATE for first nominal, UPDATE otherwise, write and close
 
    Info(__FUNCTION__, "Starting execution of AtlTRExFitterTask.cxx");

    // Process option
    Bool_t use_all_hists = kFALSE;
    TString opt = option;
    opt.ToLower();
    if ( opt.Contains("all") or ( strcmp(fInputDiscrPath, "") == 0 ) ) {
        use_all_hists = kTRUE;
    }

    // Get list of all systematics from directory structure on disk
    Ssiz_t idx = fInputDataMCFile.Index("nominal");
    TString dirname = fInputDataMCFile(0, idx);
    auto dir = std::make_unique<TSystemDirectory>(dirname, dirname);
    std::unique_ptr<TList> systematic_dirs(dir->GetListOfFiles());
    if ( nullptr == systematic_dirs ) {
    	Fatal(__FUNCTION__,"Dirname: " + dirname);
        Fatal(__FUNCTION__, "There are no files in this directory! Abort!");
    }

    // Nominal always at beginning of the list
    TObject *nom = systematic_dirs->FindObject("nominal");
    if ( nullptr == nom ) {
        Fatal(__FUNCTION__, "Did not find directory \"nominal\" in "
              "directory %s. Abort!", dir->GetName());
    }
    systematic_dirs->Remove(nom);
    systematic_dirs->AddFirst(nom);

systematic_dirs->Print();

    // Open each input file, get all histograms, write out.
    std::vector<TString> histogram_paths;
    TString file_specifier("");
    TIter next_dir(systematic_dirs.get());
    TSystemFile *subdir = 0;
    while ( (subdir = dynamic_cast<TSystemFile*>(next_dir())) ) {
        // Open input file if it is valid
	TString subdir_name = subdir->GetName();
	
	std::cout<<subdir_name<<std::endl;
	
        if ( !subdir->IsDirectory() || subdir_name.Contains(".") ) {
        std::cout<<"Skipped"<<subdir_name<<std::endl;
            // File is not a systematics directory, skip silently.
            // User must guarantee non-systematic directories contain a dot.
            continue;
        }
        
        TString systdir_name(fInputDataMCFile);
        systdir_name.ReplaceAll("nominal", subdir->GetName());
        auto f_in = std::make_unique<TFile>(systdir_name.Data(), "READ");
        if ( nullptr == f_in || !f_in->IsOpen() ) {
            Warning(__FUNCTION__, "Could not open file %s, skip.", f_in->GetName());
            continue;
        }
        f_in->SetTitle(subdir->GetName());
        Info(__FUNCTION__, "Reading file %s", f_in->GetName());

        // special treatment for nominal (=first) file
        Bool_t IsNominal = kFALSE;
        if ( subdir->IsEqual(systematic_dirs->First()) ) {
            IsNominal = kTRUE;
            if ( use_all_hists ) {
                histogram_paths = GetHistogramPaths(f_in.get());
            } else {
                histogram_paths.push_back(fInputDiscrPath);
            }
            Info(__FUNCTION__, "Processing %lu histogram%s",
                 histogram_paths.size(),
                 histogram_paths.size() == 1 ? "" : "s");
        } 

        // loop over histograms
        TString hist_output_path;
        TString hist_name;
        TString hist_basedir;
        for ( const auto& hist_path : histogram_paths ) {
            Bool_t is_first_hist = ( hist_path == histogram_paths.front() );
            if ( use_all_hists ) {
                if(((TString)f_in->GetName()).Contains("MemDisc")){
            		//if(hist_path.Contains("MemLlh")) continue;
            		//if(!(hist_path.Contains("ManyBins"))) continue;
                	if(!(hist_path.Contains("Mem"))) continue;
            		if(hist_path.Contains("job_info")) continue;
            		}
                Info(__FUNCTION__, "Processing histogram %s", hist_path.Data());
                
                hist_output_path = hist_path.Copy();
                std::unique_ptr<TObjArray> tokens(hist_path.Tokenize("/"));
                hist_name = dynamic_cast<TObjString*>(
                    tokens->Last())->GetString();
                hist_basedir = hist_output_path.Copy();
                hist_basedir.ReplaceAll(hist_name.Data(),"");
                hist_basedir.Remove(TString::kBoth, '/');
            } else {
                hist_output_path = fOutputDiscrName;
                hist_name = fOutputDiscrName;
            }

            // Fetch DataMC plot
            auto data_mc_plot = dynamic_cast<HepDataMCPlot*>(f_in->Get(hist_path));
            if ( nullptr == data_mc_plot ) {
                Fatal(__FUNCTION__, "The requested DataMCPlot %s in file %s "
                      "does not exist. Abort!", hist_path.Data(), f_in->GetName());
            }

            // ====
            // DATA
            // ====
            // (nominal only)
            if ( IsNominal ) {
                file_specifier = ( is_first_hist ) ? TString("RECREATE") : TString("UPDATE");
                TH1 *h_data = data_mc_plot->GetHistDATA();
                if ( h_data == 0 ) {
                    Info(__FUNCTION__, "DATA histogram does not exist. Skip.");
                } else {
                    h_data->SetName(hist_name);
                    h_data->SetTitle(Form("%s, DATA", hist_name.Data()));

                    if ( is_first_hist ) {
                        Info(__FUNCTION__, "Creating data output file %s/data.root", fOutputDir.Data());
                    }
                    auto f_out = std::make_unique<TFile>(
                        Form("%s/data.root", fOutputDir.Data()),
                        file_specifier.Data());
                    // use directories if we take all histograms
                    if ( use_all_hists ) {
                        if ( nullptr == f_out->GetDirectory(hist_basedir.Data()) ) {
                            Info(__FUNCTION__, "Creating path '%s'", hist_basedir.Data());
                            f_out->mkdir(hist_basedir.Data());
                        }
                        f_out->cd(hist_basedir.Data());
                    } else {
                        f_out->cd();
                    }
                    h_data->Write();
                }
            }

            // ==
            // MC
            // ==
            TList *mc_templates = data_mc_plot->GetListOfMCTemplates("");
            TH1F *h_mc = nullptr;
            TIter next_mc(mc_templates);
            while ( (h_mc = dynamic_cast<TH1F*>(next_mc())) ) {
                TString sample_name = h_mc->GetName();
                sample_name.Remove(TString::kTrailing, '_');
                TString hname = hist_name.Data();
                TString htitle = hist_name.Data();
                if ( !IsNominal ) {
                    hname.Append("_");
                    hname.Append(f_in->GetTitle());
                    if ( hname.EndsWith("down") ) {
                        hname.Remove(hname.Length()-4);
                        hname.Append("_down");
                    } else if ( hname.EndsWith("up") ) {
                        hname.Remove(hname.Length()-2);
                        hname.Append("_up");
                    }
                }
                htitle.Append(" (");
                htitle.Append(f_in->GetTitle());
                htitle.Append("), ");
                if ( htitle.EndsWith("down") ) {
                        htitle.Remove(htitle.Length()-4);
                        htitle.Append("_down");
                } else if ( htitle.EndsWith("up") ) {
                    htitle.Remove(htitle.Length()-2);
                    htitle.Append("_up");
                }
                htitle.Append(sample_name.Data());

                h_mc->SetName(hname.Data());
                h_mc->SetTitle(htitle.Data());

                if ( IsNominal && is_first_hist ) {
                    file_specifier = TString("RECREATE");
                    Info(__FUNCTION__, "Creating MC output file %s/%s.root",
                         fOutputDir.Data(), sample_name.Data());
                } else {
                    file_specifier = TString("UPDATE");
                }
                auto f_out = std::make_unique<TFile>(
                    Form("%s/%s.root", fOutputDir.Data(), sample_name.Data()),
                    file_specifier.Data());
                // use directories if we take all histograms
                if ( use_all_hists ) {
                    if ( nullptr == f_out->GetDirectory(hist_basedir.Data()) ) {
                        Info(__FUNCTION__, "Creating path '%s'", hist_basedir.Data());
                        f_out->mkdir(hist_basedir.Data());
                    }
                    f_out->cd(hist_basedir.Data());
                } else {
                    f_out->cd();
                }
                h_mc->Write();
            }
        }
    }

    Info(__FUNCTION__, "Finished AtlTRExFitterTask.cxx");
}

//____________________________________________________________________

std::vector<TString> AtlTRExFitterTask::GetHistogramPaths(TDirectoryFile *dir) {
    //
    // Get a complete list of histogram paths in a directory or file
    //
    std::vector<TString> histogram_paths;
    TIter next(dir->GetListOfKeys());
    TKey *key;
    while (( key = dynamic_cast<TKey*>(next()) )) {
        TClass *cl = TClass::GetClass(key->GetClassName());
        if (cl->InheritsFrom("HepDataMCPlot")) {
            // get full path of histogram inside file and add to list
            TString fullpath(dir->GetPath());
            std::unique_ptr<TObjArray> tokens(fullpath.Tokenize(":"));
            TString path(dynamic_cast<TObjString*>(tokens->Last())->GetString());
            path.Append("/"); path.Append(key->GetName());
            histogram_paths.push_back(path);
        } else if (cl->InheritsFrom("TDirectoryFile")) {
            // get this subdirectory's histogram list, and then add to list
            TDirectoryFile *subdir = dynamic_cast<TDirectoryFile*>(key->ReadObj());
            std::vector<TString> sublist = GetHistogramPaths(subdir);
            histogram_paths.insert(histogram_paths.end(),
                                   std::make_move_iterator(sublist.begin()),
                                   std::make_move_iterator(sublist.end()));
        }
    }
    return histogram_paths;
}
