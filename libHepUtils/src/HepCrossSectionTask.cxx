//____________________________________________________________________
//
// Cross-section class
// 
// This class is used to calculate the cross section and its error
//  
// Author: Umberto Prosperi Porta <mailto: umberto.prosperi@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2011 (C) Umberto Prosperi Porta
//
#ifndef HEP_HepCrossSectionTask
#include <HepCrossSectionTask.h>
#endif
#include <HepDataMCPlot.h>
#include <HepDataMCSample.h>
#include <TString.h>
#include <TFile.h>
#include <TList.h>
#include <TMath.h>
#include <TH1F.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepCrossSectionTask);
#endif

//____________________________________________________________________

HepCrossSectionTask::HepCrossSectionTask(const char* name,
					 const char* title) : TTask(name, title) {
    //
    // Default constructor
    //
//     fInputFilename = new TString("");
//     fCutFlowName = new TString("");
//     fSignalName = new TString("");
    fCutFlowPlot = 0;
    signal_sample = 0;

    Nbins = 0;                 
    Ndata = 0;
    NdataErr = 0;
    Nevents_sig = 0;
    Nevents_sigErr = 0;
    Nevents_sig_gen = 0;       
    Nevents = 0;
    NeventsErr = 0;
    Nevents_bkg = 0;           
    Nevents_sig_data = 0;      
    Nevents_sig_dataErr = 0;  
    Acceptance = 0;            
    AcceptanceErr = 0;       
    AcceptanceStatErr = 0;   
    AcceptanceSystErr = 0;   
    Luminosity = 0;            
    LuminosityErr = 0;       
    Cross_Section = 0;         
    Cross_SectionErr = 0;    
    Cross_SectionErr_Syst_up = 0;
    Cross_SectionErr_Syst_down = 0;

    
}

//____________________________________________________________________

HepCrossSectionTask::~HepCrossSectionTask() {
    //
    // Default destructor
    //
    delete fInputFilename;
    delete fCutFlowName;
    if ( fCutFlowPlot != 0 ) {
	delete fCutFlowPlot;
	fCutFlowPlot = 0;
    }
    
if ( signal_sample != 0 ) {
	delete signal_sample;
	signal_sample = 0;
    }
    
    
}

//____________________________________________________________________

void HepCrossSectionTask::SetInputFilename(const char* InputFilename) {
    //
    // Set name of input file containing plots (including path)
    //

    fInputFilename.Remove(0);
    fInputFilename.Append(InputFilename);
}

//____________________________________________________________________

void HepCrossSectionTask::SetOutputFilename(const char* OutputFilename) {
    //
    // Set name of output file containing plots (including path)
    //

    fOutputFilename.Remove(0);
    fOutputFilename.Append(OutputFilename);
}





//____________________________________________________________________

void HepCrossSectionTask::SetCutFlowName(const char* CutFlowName) {
    //
    // Set name of cut-flow plot (including path)
    //
    fCutFlowName.Remove(0);
    fCutFlowName.Append(CutFlowName);
}

//____________________________________________________________________

void HepCrossSectionTask::SetSignalName(const char* SignalName) {
    //
    // Set name of signal 
    //
    fSignalName.Remove(0);
    fSignalName.Append(SignalName);
}



//____________________________________________________________________

void HepCrossSectionTask::Exec(Option_t *option) {
    //
    // Execute task
    //    Step 1: Acceptance calculation
    //    Step 2: Cross-section calculation
    //    Step 3: Cross-section analysis


  
    AcceptanceCalc();
    XsecCalc();
    Print();
 

    gDirectory->cd();
    gDirectory->mkdir("XsecHistograms");
    gDirectory->cd("XsecHistograms");
    
    fHistXsecErrors = new TH1F("h_XsecErrors","Cross Section Errors",3,0,3);
    fHistXsecErrors->SetYTitle("Cross Section (pb)");

    
    fHistXsecErrors->SetBinContent(1,Cross_Section);
    fHistXsecErrors->SetBinError(1,Cross_SectionErr);

  
    fHistXsecErrors->SetBinContent(2,Cross_Section + Cross_SectionErr_Syst_up);
    fHistXsecErrors->SetBinContent(3,Cross_Section - Cross_SectionErr_Syst_down);


    fHistXsecErrors->GetXaxis()->SetBinLabel(1, "#sigma #pm #Delta#sigma_{stat.}");
    fHistXsecErrors->GetXaxis()->SetBinLabel(2, "#sigma + L_{up}");
    fHistXsecErrors->GetXaxis()->SetBinLabel(3, "#sigma + L_{down}");

    fHistXsecErrors->GetYaxis()->SetLimits(0,23);



    
    fHistXsecErrors->SetMarkerColor(8);
    fHistXsecErrors->SetMarkerStyle(8);
    fHistXsecErrors->SetMarkerSize(2);

    
    fHistXsecErrors->SetLineColor(1);



    
    f_output = new TFile(fOutputFilename,"RECREATE");
    fHistXsecErrors->Write();
    
    gDirectory->cd("..");
    
}

//____________________________________________________________________
void HepCrossSectionTask::Print() {
    
    
    cout<<
	endl
	<<"Nevents_sig gen       = "<< Nevents_sig_gen  <<endl
	<<"Nevents               = "<< Nevents          <<endl
	<<"Nevents_sig           = "<< Nevents_sig      <<endl
	<<"Nevents_bkg           = "<< Nevents_bkg      <<endl
	
	<<"Ndata                 = "<< Ndata <<endl
	<<"Nevents_sig_data      = "<< Nevents_sig_data << endl

	<<"Acceptance            = "<< Acceptance       <<endl
	<<"Acceptance Error      = "<< AcceptanceErr    <<endl
	
	<<"Luminosity            = "<< Luminosity   << " pb^-1 " <<endl<<endl
	<<"Luminosity Error      = "<< LuminosityErr    <<endl
	
	<<"Cross Section         = "<< Cross_Section<<    " pb "    << endl
	<<"CS Statistical Error  = "<< Cross_SectionErr<< " pb "    << endl
    
	<<"CS Systematic Error(+)= "<< Cross_SectionErr_Syst_up<< " pb "    << endl
	<<"CS Systematic Error(-)= "<< Cross_SectionErr_Syst_down<< " pb "    << endl;

    
    
}
//____________________________________________________________________


void HepCrossSectionTask::AcceptanceCalc() {
    //
    //Compute acceptance from MC-DATA of cut-flow plots
    //
    
    f_input = new TFile(fInputFilename,"READ");
    
    if (!f_input) cout << endl <<
	"ERROR: Cannot find MCPlotter file..." << endl;
    cout<<"test file : "<<f_input<<endl;
    cout<<"test hist : "<<fCutFlowName<<endl;
    
    h_cutflow =  (HepDataMCPlot*)f_input->Get(fCutFlowName);
    
    Nbins = h_cutflow->GetHistDATA()->GetNbinsX() + 1;
    Ndata = h_cutflow->GetHistDATA()->GetBinContent(Nbins-1);
    signal = ((HepDataMCFolder*)(h_cutflow->GetMCFolders())->FindObject(fSignalName))->GetMCSamples();

    signal_sample = (HepDataMCSample*)signal->At(0);

    
    
    Nevents_sig     = signal_sample->GetHistogram()->GetBinContent(Nbins-1);
    Nevents_sig_gen = signal_sample->GetHistogram()->GetBinContent(1);
   


    Acceptance = Nevents_sig / Nevents_sig_gen;
    AcceptanceStatErr = (TMath::Sqrt(Acceptance*Acceptance*Nevents_sig_gen+Nevents_sig*(1-2*Acceptance)))/Nevents_sig_gen;


    AcceptanceErr = AcceptanceStatErr + AcceptanceSystErr;
    
}

//_____________________________________________________________________
void HepCrossSectionTask::XsecCalc() {

    for (int i = 0; i < h_cutflow->GetMCHistStack()->GetHists()->GetEntries(); i++) {
	Nevents += ((TH1F*)h_cutflow->GetMCHistStack()->GetHists()->At(i))->GetBinContent(Nbins-1);
    }
    
    //Nevents = h_cutflow->GetMCHistStack()->GetHistogram()->GetBinContent(Nbins-1);

    Nevents_bkg = Nevents - Nevents_sig;

    Nevents_sig_data = Ndata - Nevents + Nevents_sig;

    Luminosity = h_cutflow->GetLumiDATA();
    Cross_Section = (Nevents_sig_data)/(Luminosity*Acceptance);

    cout<<"lumi f = "<<Luminosity<<endl;
    cout<<"lumi p = "<<h_cutflow->GetLumiDATA()<<endl;
    NdataErr = h_cutflow->GetHistDATA()->GetBinError(Nbins-1);
    NeventsErr = h_cutflow->GetMCHistStack()->GetHistogram()->GetBinError(Nbins-1);
    Nevents_sigErr = signal_sample->GetHistogram()->GetBinError(Nbins-1);
    
    Nevents_sig_dataErr = TMath::Sqrt(NdataErr*NdataErr+NeventsErr*NeventsErr+Nevents_sigErr*Nevents_sigErr);
    
    LuminosityErr = Luminosity*0.045;

    
    Cross_SectionErr = Cross_Section*(TMath::Sqrt( (

							 (TMath::Power(AcceptanceErr/Acceptance,2))+
							 (TMath::Power(Nevents_sig_dataErr/Nevents_sig_data,2))
						       )  )         );


    Cross_SectionErr_Syst_up = (Nevents_sig_data/Acceptance)*(LuminosityErr/(TMath::Power(Luminosity+LuminosityErr,2)));
    Cross_SectionErr_Syst_down = (Nevents_sig_data/Acceptance)*(LuminosityErr/(TMath::Power(Luminosity-LuminosityErr,2)));



    // if(Cross_SectionErr>Cross_SectionErr_Syst_up)   Cross_SectionErr_Syst_up     = 0;
    // if(Cross_SectionErr>Cross_SectionErr_Syst_down) Cross_SectionErr_Syst_down   = 0;

    
    
}




    
