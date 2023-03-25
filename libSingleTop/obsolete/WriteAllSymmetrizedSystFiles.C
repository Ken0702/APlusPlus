{
    //
    // Load main  macro for writing of symmetrized systematics files
    //
    gROOT->LoadMacro("WriteSymmetrizedSystFiles.C");


    // Choose lepton flavour
    cout  << endl << " Give lepton mode ( \"e\", \"mu\" ) !" << endl << endl;
    TString lepton_mode = '';
    cin >> lepton_mode;

    // Choose jet bin
    cout  << endl << " Give jet bin mode ( \"2\", \"3\", \"4\", \"all\" ) !" << endl << endl;
//    cout  << endl << " Give jet bin mode ( \"1\", \"2\", \"3\", \"4\", \"all\" ) !" << endl << endl;
    TString jet_bin_mode = '';
    cin >> jet_bin_mode;

    // Choose syst mode
    cout  << endl << " Give systematics mode ( 0, 1, 2 etc. or  100 (=all) ) !" << endl << endl;
    TString syst_mode = '';
    cin >> syst_mode;

    cout << endl << endl << " Writing all symmetrized systematics files for nominal and systematics samples..." << endl << endl;
    
    //
    // Global settings
    //
    Bool_t do_electron_macros; 
    Bool_t do_muon_macros;     
    Bool_t do_all_jetbins;
    Bool_t do_all_systs;
    NJetsMode  jet_mode;

    if ( lepton_mode.Contains("e") ) {
	do_electron_macros = kTRUE;
	do_muon_macros     = kFALSE;
      } else if ( lepton_mode.Contains("mu") ) {
	do_electron_macros = kFALSE;
	do_muon_macros     = kTRUE;
    } else {
	cout <<  endl << " Incorrect lepton mode given - abort!" << endl << endl;
	gSystem->Abort();
    }
    
//     if ( jet_bin_mode.Contains("1") ) {
// 	jet_mode       = k1Jet;
// 	do_all_jetbins = kFALSE;
//     } else
    if ( jet_bin_mode.Contains("2") ) {
	jet_mode       = k2Jets;
	do_all_jetbins = kFALSE;
    } else if ( jet_bin_mode.Contains("3") ) {
	jet_mode       = k3Jets;
	do_all_jetbins = kFALSE;
    } else if ( jet_bin_mode.Contains("4") ) {
	jet_mode       = k4Jets;
	do_all_jetbins = kFALSE;
    } else if ( jet_bin_mode.Contains("all") ) {
	do_all_jetbins = kTRUE;
    } else {
	cout <<  endl << " Incorrect jet bin mode given - abort!" << endl << endl;
	gSystem->Abort();
    }

    
    SystMode syst = syst_mode.Atoi(); 
    
    if ( (syst != 100) && (syst >= 0) && (syst < kNumSystModes) ) {
      // Setting systematics label
      TString systname = TString(fgSystNames[syst]);
      if ( systname == "" ) systname = "nominal";
      cout << endl << " Choosing systematics " << systname.Data() << endl << endl;
      do_all_systs = kFALSE;
    } else if ( syst == 100 ) {
      cout << endl << " Doing all systematics!" << endl << endl;
      do_all_systs = kTRUE;
    } else {
      cout <<  endl << " Incorrect systematics mode given - abort!" << endl << endl;
      gSystem->Abort();
    }

    
    //
    // Execute it for all systematics
    //
    const char* samples = "SampleList_plotter_tChannel.txt";
    const char* histdir = "/users/eeh/ruthh/APlusPlus/libSingleTop/hist/tChannelAnalysis_";

    // =========
    // Electrons
    // =========
    if ( do_electron_macros ) {
	if ( do_all_jetbins ) {
	    for ( Int_t i = 0; i < kNumNJetsModes; i++ ) {
		if ( do_all_systs ) {
		    for ( Int_t j = 0; j < kNumSystModes; j++ ) {
		      if ( (j == kNominal) || (j == kMuSmearedNoScale) ) continue;
			WriteSymmetrizedSystFiles(kElectron,i,j,histdir,samples);
		    }
		} else {
		    if ( syst == kNominal ) {
			cout << " Does not make sense for nominal! Abort!";
			gSystem->Abort();		    
		    } else if ( syst == kMuSmearedNoScale ) {
			cout << " Does not make sense in e-channel! Abort!";
			gSystem->Abort();
		    }
		    WriteSymmetrizedSystFiles(kElectron,i,syst,histdir,samples);
		}
	    }
	} else {
	  if ( do_all_systs ) {
	    for ( Int_t j = 0; j < kNumSystModes; j++ ) {
	      if ( (j == kNominal) || (j == kMuSmearedNoScale) ) continue;
	      WriteSymmetrizedSystFiles(kElectron,jet_mode,j,histdir,samples);
	    } 
	  } else {
	    if ( syst == kNominal ) {
	      cout << " Does not make sense for nominal! Abort!";
	      gSystem->Abort();		    
	    } else if ( syst == kMuSmearedNoScale ) {
	      cout << " Does not make sense in e-channel! Abort!";
	      gSystem->Abort();
	    }
	    WriteSymmetrizedSystFiles(kElectron,jet_mode,syst,histdir,samples);
	  }
	}
    }
    
    
    // =========
    // Muons
    // =========
    if ( do_muon_macros ) {
	if ( do_all_jetbins ) {
	    for ( Int_t i = 0; i < kNumNJetsModes; i++ ) {
		if ( do_all_systs ) {
		    for ( Int_t j = 0; j < kNumSystModes; j++ ) {
		      if ( j == kNominal ) continue;
		      WriteSymmetrizedSystFiles(kMuon,i,j,histdir,samples);
		    }
		} else {
		  if ( syst == kNominal ) {
		    cout << " Does not make sense for nominal! Abort!";
		    gSystem->Abort();		    
		  } 
		  WriteSymmetrizedSystFiles(kMuon,i,syst,histdir,samples);
		}   
	    }
	} else {
	    if ( do_all_systs ) {
	      for ( Int_t j = 0; j < kNumSystModes; j++ ) {
		if ( j == kNominal ) continue;
		WriteSymmetrizedSystFiles(kMuon,jet_mode,j,histdir,samples);
	      }
	    } else {
	      if ( syst == kNominal ) {
		cout << " Does not make sense for nominal! Abort!";
		gSystem->Abort();		    
	      } 
	      WriteSymmetrizedSystFiles(kMuon,jet_mode,syst,histdir,samples);
	    }   
	    
	}
    }
    
    cout << endl << endl << " Symmetrized output files for all systematics samples written out!!!" << endl << endl;
    
}
