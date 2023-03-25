{
    //
    // Small script for testing the EVE event display
    //
    // Author: Oliver Maria Kind
    // Date:   July 2009
    // $Id$
    //
    TFile *f = new TFile("/datm1/atlas/MC/W+jets/enu/Np5/app/mc08.107685.AlpgenJimmyWenuNp5_pt20.recon.AOD.e368_s462_r563_tid029548.root", "read");
    TTree *t = (TTree*)gDirectory->Get("t_app");
    AtlEVE *eve = new AtlEVE;
    AtlEVE *eve = new AtlEVE;
    eve->DrawEvent(f,t,0);
}
