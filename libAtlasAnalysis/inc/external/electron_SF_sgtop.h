#ifndef electron_SF_EPS_sgtop_h
#define electron_SF_EPS_sgtop_h

/****************************************************************************
 * electron_SF_EPS_sgtop.h                                                  *
 *                                                                          *
 * Simple functions which return data/MC scale factors                      *
 * given a electron's eta 													*
 * Also functions for corresponding uncertainties.                          *
 *                                                                          *
 * double ele_ID_SF(double et, double ET)                   	            *
 * double ele_ID_SF_err(double eta, double ET)               	      		*
 * double ele_reco_SF();                                    	            *
 * double ele_reco_SF_err();                                 	      		*
 * double ele_recoID_SF(double eta, double ET);             	            *
 * double ele_recoID_SF_err(double eta, double ET);          	      		*
 * double ele_trigger_SF();                                 	            *
 * double ele_trigger_SF_err();                              	      		*
 *                                                                          *
 * History                                                                  *
 *         26 Jan 2011 -- created by S. Caughron                            *   
 *         05 May 2011 -- updated for pLHC                                  *   
 *         14 May 2011 -- more pLHC updates                                 *   
 *         20 Jun 2011 -- updated for EPS                                   *   
 ***************************************************************************/

#include <iostream>

namespace SgTop {

// forward declaration of functions
double ele_ID_SF(double eta, double ET);
double ele_ID_SF_err(double eta, double ET);
double ele_reco_SF(double eta);
double ele_reco_SF_err(double eta);
double ele_recoID_SF(double eta, double ET);
double ele_recoID_SF_err(double eta, double ET);
double ele_trigger_SF(double eta);
double ele_trigger_SF_err(double eta);


// For ID+iso scale factors
double ele_ID_SF(double eta, double ET)
{

    double ele_eta = eta;
    double ele_ET = ET;
    int etaI=-1; int ET_I=-1;   

    const double etabins[18] = {-2.47,-2.37,-2.01,-1.81,-1.37,
							    -1.15,-0.8,-0.6,-0.1,0,0.1,0.6,0.8,1.15,
							    1.52,1.81,2.01,2.37};
    const double ETbins[5] = {25.,30.,35.,40.,45.}; // lower edges of eta bins

	const double SFmatrix[5][18] = {{0.931, 0.947, 0.986, 0.998, 1.052, 0.994, 0.955, 0.950, 0.972,
									0.952, 0.947, 0.966, 0.985, 1.041, 1.011, 0.992, 0.936, 0.960},
									{0.962, 0.978, 1.018, 1.030, 1.086, 1.026, 0.986, 0.981, 1.004,
									0.983, 0.977, 0.998, 1.016, 1.075, 1.044, 1.025, 0.966, 0.991},
									{0.971, 0.987, 1.028, 1.040, 1.097, 1.036, 0.996, 0.991, 1.014,
									0.993, 0.987, 1.007, 1.026, 1.085, 1.054, 1.035, 0.976, 1.000},
									{0.970, 0.986, 1.027, 1.040, 1.096, 1.035, 0.995, 0.990, 1.013,
									0.992, 0.986, 1.007, 1.026, 1.084, 1.053, 1.034, 0.975, 1.000},
									{0.973, 0.989, 1.031, 1.043, 1.099, 1.038, 0.998, 0.993, 1.016,
									0.995, 0.989, 1.010, 1.029, 1.088, 1.056, 1.037, 0.978, 1.003}};

	const double etamax = 2.47;
	const double etacrack[2] = {1.37,1.52};

    if ( fabs(ele_eta) > etamax || (fabs(ele_eta) > etacrack[0] && fabs(ele_eta) < etacrack[1]) )  // check forward, crack regions

        return 1.0;       

    else {           
        
        for (int i=17; i>=0; i--){    // find eta index
            if ( ele_eta > etabins[i] ) {
                etaI = i;
                break;
            }
        }
        for (int i=4; i>=0; i--){    // find eta index
            if ( ele_ET > ETbins[i] ) {
                ET_I = i;
                break;
            }
        }

        return SFmatrix[ET_I][etaI];

    } //else

    return 0;
}

// For ID+iso scale factor uncertainties (symmetric)
double ele_ID_SF_err(double eta, double ET)
{

    double ele_eta = eta;
    double ele_ET = ET;
    int etaI=-1; int ET_I=-1;   

    const double etabins[18] = {-2.47,-2.37,-2.01,-1.81,-1.37,
							    -1.15,-0.8,-0.6,-0.1,0,0.1,0.6,0.8,1.15,
							    1.52,1.81,2.01,2.37};
    const double ETbins[5] = {25.,30.,35.,40.,45.}; // lower edges of eta bins

	const double errmatrix[5][18] = {{0.045, 0.037, 0.044, 0.035, 0.038, 0.034, 0.032, 0.031, 0.034,
									 0.033, 0.031, 0.032, 0.034, 0.040, 0.036, 0.041, 0.035, 0.040},
									 {0.044, 0.036, 0.043, 0.034, 0.037, 0.034, 0.031, 0.030, 0.034,
									 0.032, 0.030, 0.031, 0.033, 0.040, 0.035, 0.041, 0.035, 0.040},
									 {0.043, 0.034, 0.042, 0.032, 0.036, 0.032, 0.029, 0.028, 0.032,
									 0.030, 0.028, 0.029, 0.031, 0.038, 0.034, 0.039, 0.033, 0.039},
									 {0.043, 0.034, 0.042, 0.032, 0.036, 0.032, 0.029, 0.028, 0.032,
									 0.030, 0.028, 0.029, 0.031, 0.038, 0.034, 0.039, 0.033, 0.039},
									 {0.044, 0.036, 0.043, 0.034, 0.037, 0.033, 0.031, 0.030, 0.033,
									 0.031, 0.030, 0.031, 0.033, 0.040, 0.035, 0.041, 0.034, 0.040}};

	const double etamax = 2.47;
	const double etacrack[2] = {1.37,1.52};

    if ( fabs(ele_eta) > etamax || (fabs(ele_eta) > etacrack[0] && fabs(ele_eta) < etacrack[1]) )  // check forward, crack regions

        return 1.0;       

    else {           
        
        for (int i=17; i>=0; i--){    // find eta index
            if ( ele_eta > etabins[i] ) {
                etaI = i;
                break;
            }
        }
        for (int i=4; i>=0; i--){    // find ET index
            if ( ele_ET > ETbins[i] ) {
                ET_I = i;
                break;
            }
        }
		
        return errmatrix[ET_I][etaI];

    } //else

    return 0;
}

double ele_reco_SF(double eta)
{

    double ele_eta = eta;
    int etaI=-1; 

    const double etabins[3] = {0, 0.8, 2.37};

	const double SFmatrix[3] = {0.9984, 1.0091, 0.9759};
								   
	const double etamax = 2.47;
	const double etacrack[2] = {1.37,1.52};

    if ( fabs(ele_eta) > etamax || (fabs(ele_eta) > etacrack[0] && fabs(ele_eta) < etacrack[1]) )  // check forward, crack regions

        return 1.0;       

    else {           
        
        for (int i=2; i>=0; i--){    // find eta index
            if ( fabs(ele_eta) > etabins[i] ) {
                etaI = i;
                break;
            }
        }

        return SFmatrix[etaI];

    } //else

    return 0;

}

// For reco SF uncertainties (symmetric)
double ele_reco_SF_err(double eta)
{

    double ele_eta = eta;
    int etaI=-1; 

    const double etabins[3] = {0, 0.8, 2.37};

	const double errmatrix[3] = {0.0066, 0.0070, 0.0184};
								   
	const double etamax = 2.47;
	const double etacrack[2] = {1.37,1.52};

    if ( fabs(ele_eta) > etamax || (fabs(ele_eta) > etacrack[0] && fabs(ele_eta) < etacrack[1]) )  // check forward, crack regions

        return 1.0;       

    else {           
        
        for (int i=2; i>=0; i--){    // find eta index
            if ( fabs(ele_eta) > etabins[i] ) {
                etaI = i;
                break;
            }
        }

        return errmatrix[etaI];

    } //else

    return 0;

}

//returns cumulative reco+ID SF
double ele_recoID_SF(double eta, double ET)
{

	double ele_eta = eta;
	double ele_ET = ET;
	double total_SF = ele_ID_SF(ele_eta, ele_ET) * ele_reco_SF(ele_eta);

	return total_SF;

}

//returns cumulative reco+ID SF uncertainty
double ele_recoID_SF_err(double eta, double ET)
{

	double ele_eta = eta;
	double ele_ET = ET;
	double ID_err = ele_ID_SF_err(ele_eta, ele_ET) / ele_ID_SF(ele_eta, ele_ET);  //need relative errors
	double reco_err = ele_reco_SF_err(ele_eta) / ele_reco_SF(ele_eta);

	double tot_rel_err = sqrt( pow(ID_err,2) + pow(reco_err,2) );
	double tot_abs_err = tot_rel_err * ele_recoID_SF(ele_eta, ele_ET);

	return tot_abs_err;

}

// For trigger SFs
double ele_trigger_SF(double eta)
{

    double ele_eta = eta;
    int etaI=-1;

    const double etabins[18] = {-2.47,-2.37,-2.01,-1.81,-1.37,
							    -1.15,-0.8,-0.6,-0.1,0,0.1,0.6,0.8,1.15,
							    1.52,1.81,2.01,2.37};

	const double SFmatrix[18] = {0.9655, 0.9739, 0.9842, 0.9867, 0.9970, 0.9896,
								 0.9847, 0.9930, 0.9912, 0.9928, 0.9923, 0.9971,
								 0.9915, 0.9957, 0.9982, 0.9782, 0.9786, 0.9790};
								   
	const double etamax = 2.47;
	const double etacrack[2] = {1.37,1.52};

    if ( fabs(ele_eta) > etamax || (fabs(ele_eta) > etacrack[0] && fabs(ele_eta) < etacrack[1]) )  // check forward, crack regions

        return 1.0;       

    else {           
        
        for (int i=17; i>=0; i--){    // find eta index
            if ( ele_eta > etabins[i] ) {
                etaI = i;
                break;
            }
        }

        return SFmatrix[etaI];

    } //else

    return 0;

}

// For trigger SF uncertainties (symmetric)
double ele_trigger_SF_err(double eta)
{

    double ele_eta = eta;
    int etaI=-1;

    const double etabins[18] = {-2.47,-2.37,-2.01,-1.81,-1.37,
							    -1.15,-0.8,-0.6,-0.1,0,0.1,0.6,0.8,1.15,
							    1.52,1.81,2.01,2.37};

	const double errmatrix[18] = {0.0065, 0.0059, 0.0043, 0.0043, 0.0058, 0.0046,
								 0.0052, 0.0040, 0.0042, 0.0041, 0.0040, 0.0041,
								 0.0046, 0.0041, 0.0043, 0.0067, 0.0043, 0.0098};
								   
	const double etamax = 2.47;
	const double etacrack[2] = {1.37,1.52};

    if ( fabs(ele_eta) > etamax || (fabs(ele_eta) > etacrack[0] && fabs(ele_eta) < etacrack[1]) )  // check forward, crack regions

        return 1.0;       

    else {           
        
        for (int i=17; i>=0; i--){    // find eta index
            if ( ele_eta > etabins[i] ) {
                etaI = i;
                break;
            }
        }

        return errmatrix[etaI];

    } //else

    return 0;

}

}

#endif
