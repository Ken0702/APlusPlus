//____________________________________________________________________
//
// Extended TDatabase PDG
// 
// Details about the PDG coding can be found at
// BEGIN_HTML
// <a href="http://pdg.lbl.gov/2008/reviews/rpp2008-rev-monte-carlo-numbering.pdf">http://pdg.lbl.gov/2008/reviews/rpp2008-rev-monte-carlo-numbering.pdf</a>
// END_HTML
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepDatabasePDG
#include <HepDatabasePDG.h>
#endif
#include <TSystem.h>

#ifndef __CINT__
ClassImp(HepDatabasePDG);
#endif

HepDatabasePDG *HepDatabasePDG::fgHepInstance = 0;

//____________________________________________________________________

HepDatabasePDG::HepDatabasePDG() : TDatabasePDG() {
    //
    // Default constructor
    //
    
    // Protect against multiple instances
    if ( fgHepInstance != 0 ) {
	Error("HepDatabasePDG", "Multiple instances of this object not allowed. Use HepDatabasePDG::Instance() instead!");
	gSystem->Abort(0);
    }
    fgHepInstance = this;
    
    // Include additional particles
    AddParticlesToPdgDbase();
}

//____________________________________________________________________

HepDatabasePDG::~HepDatabasePDG() {
    //
    // Default destructor
    //
    fgHepInstance = 0;
}

//____________________________________________________________________

HepDatabasePDG* HepDatabasePDG::Instance() {
    //
    // Create dbase singleton
    //
    return ( fgHepInstance != 0 ) ? (HepDatabasePDG*)fgHepInstance
	: new HepDatabasePDG();
}

//____________________________________________________________________

void HepDatabasePDG::AddParticlesToPdgDbase() {
    //
    // Add more particles to ROOT's PDG databse
    // Courtesy of the ALICE collaboration. See
    // BEGIN_HTML
    // <a href="http://alisoft.cern.ch/viewvc/trunk/STEER/AliPDG.cxx?root=AliRoot&view=markup">http://alisoft.cern.ch/viewvc/trunk/STEER/AliPDG.cxx?root=AliRoot&view=markup</a>
    // END_HTML
    //
    static Bool_t bAdded = kFALSE;
    // Check if already called
    if(bAdded)return;
    bAdded = true;
    
    // PDG nuclear states are 10-digit numbers
    // 10LZZZAAAI e.g. deuteron is 
    // 1000010020
    const Int_t kion=1000000000;
    
    const Double_t kAu2Gev=0.9314943228;
    const Double_t khSlash = 1.0545726663e-27;
    const Double_t kErg2Gev = 1/1.6021773349e-3;
    const Double_t khShGev = khSlash*kErg2Gev;
    const Double_t kYear2Sec = 3600*24*365.25;
    
    //
    // Bottom mesons
    // mass and life-time from PDG
    //
    this->AddParticle("Upsilon(3S)","Upsilon(3S)",10.3552,kTRUE,
		       0,1,"Bottonium",200553);
    
    // QCD diffractive states
    this->AddParticle("rho_diff0","rho_diff0",0,kTRUE,
		       0,0,"QCD diffr. state",9900110);
    this->AddParticle("pi_diffr+","pi_diffr+",0,kTRUE,
		       0,1,"QCD diffr. state",9900210);
    this->AddParticle("omega_di","omega_di",0,kTRUE,
		       0,0,"QCD diffr. state",9900220);
    this->AddParticle("phi_diff","phi_diff",0,kTRUE,
		       0,0,"QCD diffr. state",9900330);
    this->AddParticle("J/psi_di","J/psi_di",0,kTRUE,
		       0,0,"QCD diffr. state",9900440);
    this->AddParticle("n_diffr0","n_diffr0",0,kTRUE,
		       0,0,"QCD diffr. state",9902110);
    this->AddParticle("p_diffr+","p_diffr+",0,kTRUE,
		       0,1,"QCD diffr. state",9902210);
    
    // From Herwig
    this->AddParticle("PSID    ", " ", 3.7699, kFALSE, 0.0, 0, "meson",   30443);
    
    this->AddParticle("A_00    ", " ", 0.9960, kFALSE, 0.0, 0, "meson",  9000111); 
    this->AddParticle("A_0+    ", " ", 0.9960, kFALSE, 0.0,+1, "meson",  9000211);  
    this->AddParticle("A_0-    ", " ", 0.9960, kFALSE, 0.0,-1, "meson", -9000211);  
    
    this->AddParticle("F0P0    ", " ", 0.9960, kFALSE, 0.0, 0, "meson",  9010221); 
    
    this->AddParticle("KDL_2+  ", " ", 1.773,  kFALSE, 0.0,+1, "meson",   10325); 
    this->AddParticle("KDL_2-  ", " ", 1.773,  kFALSE, 0.0,-1, "meson",  -10325); 
    
    this->AddParticle("KDL_20  ", " ", 1.773,  kFALSE, 0.0, 0, "meson",   10315); 
    this->AddParticle("KDL_2BR0", " ", 1.773,  kFALSE, 0.0, 0, "meson",  -10315); 
    
    this->AddParticle("PI_2+   ", " ", 1.670,  kFALSE, 0.0,+1, "meson",   10215);
    this->AddParticle("PI_2-   ", " ", 1.670,  kFALSE, 0.0,-1, "meson",  -10215);
    this->AddParticle("PI_20   ", " ", 1.670,  kFALSE, 0.0, 0, "meson",   10115);
    
    this->AddParticle("KD*+    ", " ", 1.717,  kFALSE, 0.0,+1, "meson",   30323); 
    this->AddParticle("KD*-    ", " ", 1.717,  kFALSE, 0.0,-1, "meson",  -30323); 

    this->AddParticle("KD*0    ", " ", 1.717,  kFALSE, 0.0, 0, "meson",   30313); 
    this->AddParticle("KDBR*0  ", " ", 1.717,  kFALSE, 0.0, 0, "meson",  -30313); 
    
    this->AddParticle("RHOD+   ", " ", 1.700,  kFALSE, 0.0,+1, "meson",   30213); 
    this->AddParticle("RHOD-   ", " ", 1.700,  kFALSE, 0.0,-1, "meson",  -30213); 
    this->AddParticle("RHOD0   ", " ", 1.700,  kFALSE, 0.0, 0, "meson",   30113); 
    
    this->AddParticle("ETA_2(L)", " ", 1.632,  kFALSE, 0.0, 0, "meson",   10225); 
    this->AddParticle("ETA_2(H)", " ", 1.854,  kFALSE, 0.0, 0, "meson",   10335); 
    this->AddParticle("OMEGA(H)", " ", 1.649,  kFALSE, 0.0, 0, "meson",   30223);
    
    this->AddParticle("KDH_2+  ", " ", 1.816,  kFALSE, 0.0,+1, "meson",   20325);
    this->AddParticle("KDH_2-  ", " ", 1.816,  kFALSE, 0.0,-1, "meson",  -20325);
    
    this->AddParticle("KDH_20  ", " ", 1.816,  kFALSE, 0.0, 0, "meson",   20315);
    this->AddParticle("KDH_2BR0", " ", 1.816,  kFALSE, 0.0, 0, "meson",  -20315);
    
    this->AddParticle("KD_3+   ", " ", 1.773,  kFALSE, 0.0,+1, "meson",     327);
    this->AddParticle("KD_3-   ", " ", 1.773,  kFALSE, 0.0,-1, "meson",    -327);
    
    this->AddParticle("KD_30   ", " ", 1.773,  kFALSE, 0.0, 0, "meson",     317);
    this->AddParticle("KD_3BR0 ", " ", 1.773,  kFALSE, 0.0, 0, "meson",    -317);
    
    this->AddParticle("RHO_3+  ", " ", 1.691,  kFALSE, 0.0,+1, "meson",     217);
    this->AddParticle("RHO_3-  ", " ", 1.691,  kFALSE, 0.0,-1, "meson",    -217);
    this->AddParticle("RHO_30  ", " ", 1.691,  kFALSE, 0.0, 0, "meson",     117);
    this->AddParticle("OMEGA_3 ", " ", 1.667,  kFALSE, 0.0, 0, "meson",     227);
    this->AddParticle("PHI_3   ", " ", 1.854,  kFALSE, 0.0, 0, "meson",     337);
    
    this->AddParticle("CHI2P_B0", " ", 10.232, kFALSE, 0.0, 0, "meson", 110551);
    this->AddParticle("CHI2P_B1", " ", 10.255, kFALSE, 0.0, 0, "meson", 120553);
    this->AddParticle("CHI2P_B2", " ", 10.269, kFALSE, 0.0, 0, "meson", 100555);
    this->AddParticle("UPSLON4S", " ", 10.580, kFALSE, 0.0, 0, "meson", 300553);
    
    // Ions
    //
    // Done by default now from Pythia6 table
    // Needed for other generators
    // So check if already defined
    Int_t ionCode = kion+10020;
    if(!this->GetParticle(ionCode)){
	this->AddParticle("Deuteron","Deuteron",2*kAu2Gev+8.071e-3,kTRUE,
			   0,1,"Ion",ionCode);
    }
    
    this->AddAntiParticle("Anti Deuteron", - ionCode);
    
    ionCode = kion+10030;
    if(!this->GetParticle(ionCode)){
	this->AddParticle("Triton","Triton",3*kAu2Gev+14.931e-3,kFALSE,
			   khShGev/(12.33*kYear2Sec),1,"Ion",ionCode);
    }
    
    ionCode = kion+20030;
    if(!this->GetParticle(ionCode)){
	this->AddParticle("HE3","HE3",3*kAu2Gev+14.931e-3,kFALSE,
			   0,2,"Ion",ionCode);
    }
    
    ionCode = kion+20040;
    if(!this->GetParticle(ionCode)){
	this->AddParticle("Alpha","Alpha",4*kAu2Gev+2.424e-3,kTRUE,
			   khShGev/(12.33*kYear2Sec),2,"Ion",ionCode);
    }

    // Special particles
    const Int_t kspe=50000000;
    this->AddParticle("Cherenkov","Cherenkov",0,kFALSE,
		       0,0,"Special",kspe+50);
    this->AddParticle("FeedbackPhoton","FeedbackPhoton",0,kFALSE,
		       0,0,"Special",kspe+51);
    this->AddParticle("Lambda1520","Lambda1520",1.5195,kFALSE,
		       0.0156,0,"Resonance",3124);
    this->AddAntiParticle("Lambda1520bar",-3124);
}

