#ifndef electron_SF_EPS_h
#define electron_SF_EPS_h

/****************************************************************************
 * electron_SF_EPS.h                                                        *
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

namespace TTBar {

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

	const double SFmatrix[5][18] = {{0.972, 0.964, 1.007, 1.016, 1.071, 1.007, 0.967, 0.957, 0.979, 
 									0.963, 0.953, 0.975, 0.999, 1.057, 1.024, 1.007, 0.957, 0.993},
									{1.003, 0.996, 1.040, 1.049, 1.106, 1.039, 0.998, 0.988, 1.010, 
 									0.994, 0.984, 1.006, 1.031, 1.091, 1.057, 1.039, 0.988, 1.025},
									{1.013, 1.005, 1.050, 1.059, 1.117, 1.050, 1.008, 0.998, 1.020,
 									1.004, 0.993, 1.016, 1.041, 1.102, 1.068, 1.050, 0.997, 1.035},
									{1.012, 1.005, 1.049, 1.059, 1.116, 1.049, 1.007, 0.997, 1.019,
 									1.003, 0.993, 1.015, 1.041, 1.101, 1.067, 1.049, 0.997, 1.034},
									{1.015, 1.008, 1.053, 1.062, 1.120, 1.052, 1.010, 1.000, 1.023,
 									1.006, 0.996, 1.019, 1.044, 1.105, 1.070, 1.052, 1.000, 1.037}};
								   
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

	const double errmatrix[5][18] = {{0.038, 0.029, 0.037, 0.027, 0.029, 0.027, 0.027, 0.026, 0.030,
 								 	 0.027, 0.026, 0.027, 0.027, 0.033, 0.028, 0.035, 0.028, 0.033},
								 	 {0.037, 0.028, 0.036, 0.026, 0.028, 0.026, 0.025, 0.025, 0.028,
 								 	 0.026, 0.025, 0.026, 0.026, 0.032, 0.026, 0.034, 0.026, 0.032},
								 	 {0.035, 0.025, 0.034, 0.023, 0.026, 0.023, 0.023, 0.022, 0.026,
 								 	 0.023, 0.022, 0.023, 0.023, 0.029, 0.023, 0.031, 0.024, 0.030},
								 	 {0.035, 0.025, 0.034, 0.023, 0.026, 0.023, 0.023, 0.022, 0.026,
 								 	 0.023, 0.022, 0.023, 0.023, 0.029, 0.023, 0.032, 0.024, 0.030},
								 	 {0.037, 0.027, 0.036, 0.025, 0.028, 0.025, 0.025, 0.024, 0.028,
 								 	 0.025, 0.024, 0.025, 0.025, 0.031, 0.025, 0.033, 0.025, 0.031}};


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
