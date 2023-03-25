#ifndef muon_SF_EPS_h
#define muon_SF_EPS_h

/******************************************************************************
 * muon_SF_EPS.h                                                              *
 *                                                                            *
 * Simple functions which return data/MC scale factors                        *
 * given a muon's (or two muons') eta and phi (defined b/w -pi and pi).       *
 * Also functions for corresponding uncertainties.                            *
 *                     .                                                      *
 * double mu_trigger_SF(double eta, double phi)                               * 
 * double mu_trigger_SF_err(double eta, double phi, bool isUp)                * 
 * double mu_trigger_SF_staterr(double eta, double phi)                       * 
 * double mu_trigger_SF_systerr(double eta, double phi, bool isUp)            * 
 *                                                                            * 
 * double mu_ID_SF()                                    	                  *
 * double mu_ID_SF_err()                                          	          *
 * double mu_ID_SF_staterr()                                                  *
 * double mu_ID_SF_systerr()                                   	              *
 *                                                                            *
 * History                                                                    *
 *         08 Dec 2010 -- created by S. Caughron                              *   
 *         11 Feb 2011 -- Finalization for Moriond                            *   
 *         15 Mar 2011 -- removing reco SF (provided by MCP now)              *   
 *         09 May 2011 -- update for pLHC                                     *   
 *         14 Jun 2011 -- update for EPS                                      *   
 *****************************************************************************/

#include <iostream>
#include <math.h>

// forward declarations
double mu_trigger_SF(double eta, double phi, double pt);
double mu_trigger_SF_err(double eta, double phi, double pt, bool isUp);
//double mu_trigger_SF_staterr(double eta, double phi);
//double mu_trigger_SF_systerr(double eta, double phi, bool isUp);
double mu_ID_SF();
double mu_ID_SF_err();
double mu_ID_SF_staterr();
double mu_ID_SF_systerr();
double check_phi_range(double phi);
void printWarning();

//these are deprecated
double mu_recoID_SF();
double mu_recoID_SF_err();
double mu_recoID_SF_staterr();
double mu_recoID_SF_systerr();


// For trigger scale factors 
double mu_trigger_SF(double eta, double phi, double pt)
{

    double mu_eta = eta;
    double mu_phi = check_phi_range(phi);
    double mu_pt = pt;
    int etaI=-1, phiI=-1;   

    const double pi = acos(-1.);

    const double EC_eta = 1.05;
    const double etabins[7] = {-1.05,-0.75,-0.45,-0.15,0.15,0.45,0.75};   // lower edges of eta bins
    const double phibins[3] = {-pi,0.,pi/2.};   

    const double EC_SF_pos = 1.0140;
    const double EC_SF_neg = 1.0097;
    const double trig_SFmatrix_lo[7][3] = {{1.0388,1.0660,1.0388},   // rows are eta, columns are phi
                              			   {1.0196,1.0660,1.0196},     // period B2 numbers
                              			   {0.9996,1.0660,0.9996},
                              			   {1.0315,1.0315,1.0315},
                              			   {1.0026,1.0026,1.0026},
                              			   {0.9070,1.0369,1.0369},
                              			   {0.9070,0.9946,0.9946}};
    const double trig_SFmatrix_mid[7][3] = {{1.0893,1.1525,1.0893},   // rows are eta, columns are phi
                              				{1.0767,1.1525,1.0767},     // period B2 numbers
                              				{1.0880,1.1525,1.0880},
                              				{1.1071,1.1071,1.1071},
                              				{1.0609,1.0609,1.0609},
                              				{0.9400,1.1205,1.1205},
                              				{0.9400,1.0684,1.0684}};
    const double trig_SFmatrix_hi[7][3] = {{1.3432,0.8027,1.3432},   // rows are eta, columns are phi
                              			   {1.1803,0.8027,1.1803},     // period B2 numbers
                              			   {0.9868,0.8027,0.9868},
                              			   {1.2126,1.2126,1.2126},
                              			   {0.8520,0.8520,0.8520},
                              			   {1.5726,1.2885,1.2885},
                              			   {1.5726,1.3099,1.3099}};
    
    if ( mu_eta > EC_eta )    // check if in endcap
        return EC_SF_pos;       
	else if ( mu_eta < -EC_eta)
        return EC_SF_neg;       
    else {                          // if in barrel, look up appropriate SF 
        
        for (int i=2; i>=0; i--){    // find phi index
            if ( mu_phi > phibins[i] ) {
                phiI = i;
                break;
            }
        }
        for (int i=6; i>=0; i--){    // find eta index
            if ( mu_eta > etabins[i] ) {
                etaI = i;
                break;
            }
        }

		if (mu_pt < 60.)
        	return trig_SFmatrix_lo[etaI][phiI];   
		else if (mu_pt < 120.)
        	return trig_SFmatrix_mid[etaI][phiI];   
		else 
        	return trig_SFmatrix_hi[etaI][phiI];   

    } //else

    return 0;
}

// Trigger scale factor uncertainties 
double mu_trigger_SF_err(double eta, double phi, double pt, bool isUp)
{

    double mu_eta = eta;
    double mu_phi = check_phi_range(phi);
    double mu_pt = pt;
    int etaI=-1, phiI=-1;   

    const double pi = acos(-1.);

    const double EC_eta = 1.05;
    const double etabins[7] = {-1.05,-0.75,-0.45,-0.15,0.15,0.45,0.75};   // lower edges of eta bins
    const double phibins[3] = {-pi,0.,pi/2.};   
    
    const double EC_SF_pos_up = 0.00215;
    const double EC_SF_pos_down = 0.00206;
    const double EC_SF_neg_up = 0.00213;;
    const double EC_SF_neg_down = 0.00208;;

    const double trig_SFmatrix_up_lo[7][3] = {{0.00644,0.00543,0.00644},   // rows are eta, columns are phi
                                		      {0.00577,0.00543,0.00577},   // positive errors
                                		      {0.00527,0.00543,0.00527},
                                		      {0.00609,0.00609,0.00609},
                                		      {0.00459,0.00459,0.00459},
                                		      {0.00578,0.00613,0.00613},
                                		      {0.00578,0.00726,0.00726}};
    const double trig_SFmatrix_down_lo[7][3] = {{0.00640,0.00528,0.00640},   // rows are eta, columns are phi
                                		   	    {0.00609,0.00536,0.00609},   // negative errors
                                		   	    {0.00533,0.00536,0.00533},
                                		   	    {0.00615,0.00615,0.00615},
                                		   	    {0.00473,0.00473,0.00473},
                                		   	    {0.00524,0.00581,0.00581},
                                		   	    {0.00524,0.00739,0.00739}};
    
    const double trig_SFmatrix_up_mid[7][3] = {{0.03443,0.02918,0.03443},   // rows are eta, columns are phi
                                		   	   {0.03345,0.02918,0.03345},   // negative errors
                                		   	   {0.03101,0.02918,0.03101},
                                		   	   {0.03677,0.03677,0.03677},
                                		   	   {0.02670,0.02670,0.02670},
                                		   	   {0.03053,0.03296,0.03296},
                                		   	   {0.03053,0.03869,0.03869}};
    const double trig_SFmatrix_down_mid[7][3] = {{0.03426,0.03131,0.03426},   // rows are eta, columns are phi
                                		         {0.03308,0.03131,0.03308},   // positive errors
                                		         {0.03456,0.03131,0.03456},
                                		         {0.03503,0.03503,0.03503},
                                		         {0.02646,0.02646,0.02646},
                                		         {0.02877,0.03211,0.03211},
                                		         {0.02877,0.04138,0.04138}};

    const double trig_SFmatrix_up_hi[7][3] = {{0.1446,0.1963,0.1446},   // rows are eta, columns are phi
                                		      {0.1480,0.1963,0.1480},   // positive errors
                                		      {0.1463,0.1963,0.1463},
                                		      {0.1445,0.1445,0.1445},
                                		      {0.1340,0.1340,0.1340},
                                		      {0.1312,0.1704,0.1704},
                                		      {0.1312,0.1763,0.1763}};
    const double trig_SFmatrix_down_hi[7][3] = {{0.3481,0.1010,0.3481},   // rows are eta, columns are phi
                                		   	    {0.1426,0.1010,0.1426},   // negative errors
                                		   	    {0.1524,0.1010,0.1524},
                                		   	    {0.2142,0.2142,0.2142},
                                		   	    {0.1315,0.1315,0.1315},
                                		   	    {0.2393,0.2549,0.2549},
                                		   	    {0.2393,0.1763,0.1763}};

    if ( mu_eta > EC_eta && isUp)    // check if in endcap
        return EC_SF_pos_up;       
    else if ( mu_eta > EC_eta && !isUp)    
        return EC_SF_pos_down;       
	else if ( mu_eta < -EC_eta && isUp)
        return EC_SF_neg_up;       
	else if ( mu_eta < -EC_eta && !isUp)
        return EC_SF_neg_down;       
    else {                          // if in barrel, look up appropriate SF 
        
        for (int i=2; i>=0; i--){    // find phi index
            if ( mu_phi > phibins[i] ) {
                phiI = i;
                break;
            }
        }
        for (int i=6; i>=0; i--){    // find eta index
            if ( mu_eta > etabins[i] ) {
                etaI = i;
                break;
            }
        }

		if (mu_pt < 60.) {
			if (isUp)
        		return trig_SFmatrix_up_lo[etaI][phiI];   
			else
        		return trig_SFmatrix_down_lo[etaI][phiI];   
		} else if (mu_pt < 120.) {
			if (isUp)
        		return trig_SFmatrix_up_mid[etaI][phiI];   
			else
        		return trig_SFmatrix_down_mid[etaI][phiI];   
		} else {
			if (isUp)
        		return trig_SFmatrix_up_hi[etaI][phiI];   
			else
        		return trig_SFmatrix_down_hi[etaI][phiI];   
		}

    } //else

    return 0;
}

// For reco+ID scale factors
double mu_recoID_SF()
{

	printWarning();   //this is not to be used anymore
    return -99.;

}

// For reco+ID scale factor total uncertainties. 
double mu_recoID_SF_err()
{

    printWarning();   //this is not to be used anymore
    return -99.;

}

// For reco+ID scale factor statistical uncertainties (symmetric)
double mu_recoID_SF_staterr()
{

    printWarning();   //this is not to be used anymore
	return -99.;    

}

// For reco+ID scale factor systematic uncertainties 
double mu_recoID_SF_systerr()
{

    printWarning();   //this is not to be used anymore
	return -99.;

}

//-----------------------------------------------------------------

// For ID scale factors
double mu_ID_SF()
{

	const double ID_SF = 0.9989;  // in rel16, just a number
    return ID_SF;

}

// For ID scale factor total uncertainties. 
double mu_ID_SF_err()
{

    double staterr, systerr, toterr;

    staterr = mu_ID_SF_staterr();
    systerr = mu_ID_SF_systerr();

    toterr = sqrt(pow(staterr,2) + pow(systerr,2));

    return toterr;

}

// For ID scale factor statistical uncertainties (symmetric)
double mu_ID_SF_staterr()
{

	const double SFerr = 0.0004;  //in rel16, just a number
	return SFerr;    

}

// For ID scale factor systematic uncertainties 
double mu_ID_SF_systerr()
{

	const double SFerr = 0.0003;
	return SFerr;

}

// Helper function to deal with possible phi ambiguity
double check_phi_range(double phi)
{

	double newphi = phi;
	const double pi = acos(-1.);

	if (newphi > pi) {
		printf("<muon_SF>: WARNING: Muon phi %4.2f > pi! Using (phi-2*pi) \n", phi);
		newphi -= 2*pi;
	}
	if (newphi < -pi) {
		printf("<muon_SF>: WARNING: Muon phi %4.2f < -pi! Using (phi+2*pi) \n", phi);
		newphi += 2*pi;
	}

	return newphi;
}

void printWarning()
{

   printf("<muon_SF>: WARNING: Reco SFs no longer provided by this tool! Use MCP group tool instead.\n");
   printf("<muon_SF>: See https://twiki.cern.ch/twiki/bin/view/AtlasProtected/MCPAnalysisGuidelinesRel16\n");
   return;

}

#endif
