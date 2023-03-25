{
    //
    // Load main  macro for W hfor files merging in RooT
    //
    gROOT->LoadMacro("WriteWhforFiles.C");


    // Choose lepton flavour
    cout  << endl << " Give lepton mode ( \"e\", \"mu\" ) !" << endl << endl;
    TString lepton_mode = '';
    cin >> lepton_mode;

    // Choose jet bin
    cout  << endl << " Give jet bin mode ( \"1\", \"2\", \"3\", \"4\", \"all\" ) !" << endl << endl;
    TString jet_bin_mode = '';
    cin >> jet_bin_mode;

    // Choose syst mode
    cout  << endl << " Give systematics mode ( 0, 1, 2 etc. or  100 (=all) ) !" << endl << endl;
    TString syst_mode = '';
    cin >> syst_mode;

    cout << endl << endl << " Writing all merged W hfor files for nominal and systematics samples..." << endl << endl;
    
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
    
    if ( jet_bin_mode.Contains("1") ) {
	jet_mode       = k1Jet;
	do_all_jetbins = kFALSE;
    } else if ( jet_bin_mode.Contains("2") ) {
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


    // =========
    // Electrons
    // =========
    if ( do_electron_macros ) {
      if ( do_all_jetbins ) {
	for ( Int_t i = 0; i < kNumNJetsModes; i++ ) {
	    if ( do_all_systs ) {
	      for ( Int_t j = 0; j < kNumSystModes; j++ ) {
		    if ( (j == kMuSFUp) || (j == kMuSFDown)
			|| (j == kMuSmearedIDUp) || (j == kMuSmearedIDDown) 
			 || (j == kMuSmearedMSUp) || (j == kMuSmearedMSDown) 
			 || (j == kMuSmearedNoScale)) continue;
		    WriteWhforFiles(kElectron,i,j,0,samples);
		    WriteWhforFiles(kElectron,i,j,1,samples);
		    WriteWhforFiles(kElectron,i,j,2,samples);
		    WriteWhforFiles(kElectron,i,j,3,samples);
		}
	    } else {
		    WriteWhforFiles(kElectron,i,syst,0,samples);
		    WriteWhforFiles(kElectron,i,syst,1,samples);
		    WriteWhforFiles(kElectron,i,syst,2,samples);
		    WriteWhforFiles(kElectron,i,syst,3,samples);
	    }
	}
      } else {
	  if ( do_all_systs ) {
	      for ( Int_t j = 0; j < kNumSystModes; j++ ) {
		  if ( (j == kMuSFUp) || (j == kMuSFDown)
		      || (j == kMuSmearedIDUp) || (j == kMuSmearedIDDown) 
		       || (j == kMuSmearedMSUp) || (j == kMuSmearedMSDown) 
		       || (j == kMuSmearedNoScale)) continue;
		  WriteWhforFiles(kElectron,jet_mode,j,0,samples);
		  WriteWhforFiles(kElectron,jet_mode,j,1,samples);
		  WriteWhforFiles(kElectron,jet_mode,j,2,samples);
		  WriteWhforFiles(kElectron,jet_mode,j,3,samples);
	      } 
	  } else {
		  WriteWhforFiles(kElectron,jet_mode,syst,0,samples);
		  WriteWhforFiles(kElectron,jet_mode,syst,1,samples);
		  WriteWhforFiles(kElectron,jet_mode,syst,2,samples);
		  WriteWhforFiles(kElectron,jet_mode,syst,3,samples);
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
		    if ( (j == kElSFUp) || (j == kElSFDown)
			 || (j == kElScaledUp) || (j == kElScaledDown) 
			 || (j == kElSmearedUp) || (j == kElSmearedDown)) continue;
		    WriteWhforFiles(kMuon,i,j,0,samples);
		    WriteWhforFiles(kMuon,i,j,1,samples);
		    WriteWhforFiles(kMuon,i,j,2,samples);
		    WriteWhforFiles(kMuon,i,j,3,samples);
		}
	    } else {
		WriteWhforFiles(kMuon,i,syst,0,samples);
		WriteWhforFiles(kMuon,i,syst,1,samples);
		WriteWhforFiles(kMuon,i,syst,2,samples);
		WriteWhforFiles(kMuon,i,syst,3,samples);
	    }   
	}
      } else {
	if ( do_all_systs ) {
	      for ( Int_t j = 0; j < kNumSystModes; j++ ) {
		  if ( (j == kElSFUp) || (j == kElSFDown)
		       || (j == kElScaledUp) || (j == kElScaledDown) 
		       || (j == kElSmearedUp) || (j == kElSmearedDown)) continue;
		  WriteWhforFiles(kMuon,jet_mode,j,0,samples);
		  WriteWhforFiles(kMuon,jet_mode,j,1,samples);
		  WriteWhforFiles(kMuon,jet_mode,j,2,samples);
		  WriteWhforFiles(kMuon,jet_mode,j,3,samples);
	      }
	  } else {
	      WriteWhforFiles(kMuon,jet_mode,syst,0,samples);
	      WriteWhforFiles(kMuon,jet_mode,syst,1,samples);
	      WriteWhforFiles(kMuon,jet_mode,syst,2,samples);
	      WriteWhforFiles(kMuon,jet_mode,syst,3,samples);
	  }   
	  
      }
    }

    cout << endl << endl << " W file merging for nominal and systematics samples done!!!" << endl << endl;
    
}
