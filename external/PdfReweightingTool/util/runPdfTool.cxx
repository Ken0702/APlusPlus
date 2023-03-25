#include "PdfReweightingTool/PdfReweightingTool.h"
#include "PdfReweightingTool/PdfSelector.h"
#include <TChain.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <TString.h>

using namespace std;

int main ( int argc, char *argv[] ) {
  // Three command line arguments are needed:
  //
  // 1. InputFile
  // 2. DSID of MC sample
  
  cout <<  endl
       << "------------------------------------------------" << endl
       << "        PDF ReweightingTool GRID run            " << endl
       << "------------------------------------------------" << endl
       << endl;

  TString *filename;
  Int_t DSID;
  TString *treename;

  if ( argc == 1 || (strcasecmp(argv[1], "-h") == 0) ) {
    cout << endl;
    cout << "Usage: " << endl
	 << argv[0] 
	 << " filename "
	 << " DSID "
	 << " treename" << endl
	 << endl;
    cout << "Example: " << endl
	 << argv[0] << " InputFile.dat 117050 physics"
	 << endl;
    exit(-1);
  } else {
    // We need 3 command line arguments in order to continue
    if ( argc != 4 ) { // argv[0] is the exectuable's name
      cerr << "Wrong number of cmd-line arguments! Abort!" << endl
	   << "Please see '" << argv[0] << " -h' for details." << endl
	   << endl;
      exit(-1);
    }
    
    if ( argv[1] == NULL ) {
      cout << "Argument 1 = NULL" << endl;
      exit(-1);
    } else if ( argv[2] == NULL ) {
      cout << "Argument 2 = NULL" << endl;
      exit(-1);
    } else if ( argv[3] == NULL ) {
      cout << "Argument 3 = NULL" << endl;
      exit(-1);
    }

    cout <<  endl
	 << "Initializing command-line arguments." << endl
	 << endl;
    
    // argv[1] is a filename
    filename = new TString(argv[1]);
    
    // argv[2] is the DSID
    DSID = atoi(argv[2]);
    
    // argv[4] is the treename
    treename = new TString(argv[3]);
  }

  cout <<  endl
       << "Creating TChains." << endl
       << endl;
  
  //
  // Create TChain
  // =============
  TChain *ch1 = new TChain(treename->Data());

  //
  // Add all input files
  // ===================
  ifstream input(filename->Data());
  TString *line = new TString;
  
  // Read from input file
  if (input.is_open()) {
    while (! input.eof() ) {
      line->ReadLine(input, kFALSE);
      
      // Suppress empty lines
      // TODO: Add comment lines ('#')
      if ( line->Length() == 0 ) continue;
      cout << "File: " << line->Data() << endl;
      ch1->Add(line->Data());
    }
  } else {
    cerr << "Input file is not open. Abort!" << endl;
    exit(-1);
  }

  //
  // Run Analysis for each selector
  //
  cout << endl
       << "Initializing Selector" << endl
       << "PdfSet1: " << PdfSet::fgPdfNames[0] << endl
       << "PdfSet2: " << PdfSet::fgPdfNames[1] << endl
       << "PdfSet3: " << PdfSet::fgPdfNames[2] << endl
       << "DSID:    " << DSID << endl
       << endl;
  
  PdfSelector *sel = new PdfSelector(DSID);
  ch1->Process(sel);
  delete sel;
  
  delete filename;
  delete treename;
  
  cout << " ----------------------------- " << endl
       << "           Finished!           " << endl
       << " ----------------------------- " << endl;

  return 0;
}  
