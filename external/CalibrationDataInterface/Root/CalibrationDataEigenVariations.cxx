///////////////////////////////////////////////////////////////////////
// CalibrationDataEigenVariations.cxx, (c) ATLAS Detector software
///////////////////////////////////////////////////////////////////////

#include "CalibrationDataInterface/CalibrationDataContainer.h"
#include "CalibrationDataInterface/CalibrationDataEigenVariations.h"

#include <iomanip>
#include <iostream>
#include <cmath>
#include "TH1.h"
#include "TVectorT.h"
#include "TDecompBK.h"
#include "TMatrixDSymEigen.h"
#include "TROOT.h"

using Analysis::CalibrationDataEigenVariations;
using std::string;
using std::map;
using std::vector;
using std::pair;
using std::setw;
using std::setprecision;

namespace {
  // The covariance matrices constructed by the following two functions adhere to the TH1 binning conventions.
  // But in order to avoid unnecessary overhead, the actual dimensionality of the histograms is accounted for.

  // Construct the (diagonal) covariance matrix for the statistical uncertainties on the "ref" results
  TMatrixDSym getStatCovarianceMatrix(const TH1* hist) {
    Int_t nbinx = hist->GetNbinsX()+2, nbiny = hist->GetNbinsY()+2, nbinz = hist->GetNbinsZ()+2;
    Int_t rows = nbinx;
    if (hist->GetDimension() > 1) rows *= nbiny;
    if (hist->GetDimension() > 2) rows *= nbinz;

    // // the "2" below doesn't actually imply that two bins are used...
    // // this is just to make the loops below work
    // if (ref->GetDimension() <= 1) nbiny = 2;
    // if (ref->GetDimension() <= 2) nbinz = 2;

    TMatrixDSym stat(rows);
    for (Int_t binx = 1; binx < nbinx-1; ++binx)
      for (Int_t biny = 1; biny < nbiny-1; ++biny)
	for (Int_t binz = 1; binz < nbinz-1; ++binz) {
	  Int_t bin = hist->GetBin(binx, biny, binz);
	  double err = hist->GetBinError(bin);
	  stat(bin, bin) = err*err;
	}
    return stat;
  }

  // Construct the covariance matrix assuming that histogram "unc" contains systematic uncertainties
  // pertaining to the "ref" results, and that the uncertainties are fully correlated from bin to bin
  // (unless option "doCorrelated" is false, in which bins are assumed uncorrelated).
  // One exception is made for the uncorrelated case: if we are dealing with a "continuous" calibration
  // object, then a full correlation is still assumed between different tag weight bins.
  TMatrixDSym getSystCovarianceMatrix(const TH1* ref, const TH1* unc,
				      bool doCorrelated, int tagWeightAxis) {
    Int_t nbinx = ref->GetNbinsX()+2, nbiny = ref->GetNbinsY()+2, nbinz = ref->GetNbinsZ()+2;
    Int_t rows = nbinx;
    if (ref->GetDimension() > 1) rows *= nbiny;
    if (ref->GetDimension() > 2) rows *= nbinz;
    
    TMatrixDSym cov(rows);

    // Carry out a minimal consistency check
    if (unc->GetNbinsX()+2 != nbinx || unc->GetNbinsY()+2 != nbiny || unc->GetNbinsZ()+2 != nbinz
	|| unc->GetDimension() != ref->GetDimension()) {
      std::cout << "getSystCovarianceMatrix: inconsistency found in histograms "
		<< ref->GetName() << " and " << unc->GetName() << std::endl;
      return cov;
    }

    // // the "2" below doesn't actually imply that two bins are used...
    // // this is just to make the loops below work
    // if (ref->GetDimension() <= 1) nbiny = 2;
    // if (ref->GetDimension() <= 2) nbinz = 2;
    
    // Special case: uncertainties not correlated from bin to bin.
    // The exception here is for tag weight bins, which are always assumed to be fully correlated.
    if (! doCorrelated) {
      if (tagWeightAxis < 0) {
	// truly uncorrelated uncertainties
	for (Int_t binx = 1; binx < nbinx-1; ++binx)
	  for (Int_t biny = 1; biny < nbiny-1; ++biny)
	    for (Int_t binz = 1; binz < nbinz-1; ++binz) {
	      Int_t bin = ref->GetBin(binx, biny, binz);
	      double err = unc->GetBinContent(bin);
	      cov(bin,bin) = err*err;
	    }
	return cov;
      } else if (tagWeightAxis == 0) {
	// continuous histogram with tag weight X axis
	for (Int_t biny = 1; biny < nbiny-1; ++biny)
	  for (Int_t binz = 1; binz < nbinz-1; ++binz)
	    for (Int_t binx = 1; binx < nbinx-1; ++binx) {
	      Int_t bin = ref->GetBin(binx, biny, binz);
	      double err = unc->GetBinContent(bin);
	      for (Int_t binx2 = 1; binx2 < nbinx-1; ++binx2) {
		Int_t bin2 = ref->GetBin(binx2, biny, binz);
		double err2 = unc->GetBinContent(bin2);
		cov(bin,bin2) = err*err2;
	      }
	    }
	return cov;
      } else if (tagWeightAxis == 1) {
	// continuous histogram with tag weight Y axis
	for (Int_t binx = 1; binx < nbinx-1; ++binx)
	  for (Int_t binz = 1; binz < nbinz-1; ++binz)
	    for (Int_t biny = 1; biny < nbiny-1; ++biny) {
	      Int_t bin = ref->GetBin(binx, biny, binz);
	      double err = unc->GetBinContent(bin);
	      for (Int_t biny2 = 1; biny2 < nbiny-1; ++biny2) {
		Int_t bin2 = ref->GetBin(binx, biny2, binz);
		double err2 = unc->GetBinContent(bin2);
		cov(bin,bin2) = err*err2;
	      }
	    }
	return cov;
      } else if (tagWeightAxis == 2) {
	// continuous histogram with tag weight Z axis
	for (Int_t binx = 1; binx < nbinx-1; ++binx)
	  for (Int_t biny = 1; biny < nbiny-1; ++biny)
	    for (Int_t binz = 1; binz < nbinz-1; ++binz) {
	      Int_t bin = ref->GetBin(binx, biny, binz);
	      double err = unc->GetBinContent(bin);
	      for (Int_t binz2 = 1; binz2 < nbinz-1; ++binz2) {
		Int_t bin2 = ref->GetBin(binx, biny, binz2);
		double err2 = unc->GetBinContent(bin2);
		cov(bin,bin2) = err*err2;
	      }
	    }
	return cov;
      }
    }

    for (Int_t binx = 1; binx < nbinx-1; ++binx)
      for (Int_t biny = 1; biny < nbiny-1; ++biny)
	for (Int_t binz = 1; binz < nbinz-1; ++binz) {
	  Int_t bin = ref->GetBin(binx, biny, binz);
	  double err = unc->GetBinContent(bin);
	  for (Int_t binx2 = 1; binx2 < nbinx-1; ++binx2)
	    for (Int_t biny2 = 1; biny2 < nbiny-1; ++biny2)
	      for (Int_t binz2 = 1; binz2 < nbinz-1; ++binz2) {
		Int_t bin2 = ref->GetBin(binx2, biny2, binz2);
		double err2 = unc->GetBinContent(bin2);
		cov(bin, bin2) = err*err2;
	      }
	}
    return cov;
  }

}

//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
// CalibrationDataEigenVariations                                                           //
//                                                                                          //
// This class is intended to provide a more proper way to deal with correlations between    //
// calibration bins than would be possible using directly the calibration containers'       //
// methods (the general issue is with the fact that the interface methods are intended to   //
// be use on a jet by jet basis; in this context it is not straightforward to account for   //
// correlations).                                                                           //
//                                                                                          //
// A CalibrationDataEigenVariations object is associated with a specific                    //
// CalibrationDataHistogramContainer. It starts by constructing the covariance matrix from  //
// the information provided by the container. Subsequently it diagonalises this covariance  //
// matrix (this is a standard eigenvalue problem, hence the name of the class), and stores  //
// the result as 'variation' histograms (representing the eigenvectors multiplied by the    //
// square root of the corresponding eigenvalues).                                           //
// Since it is possible for systematic uncertainty contributions to be correlated with      //
// corresponding uncertainties in physics analyses, it is possible to exclude such sources  //
// of uncertainty from being used in the construction of the covariance matrix (this is     //
// since effects from the original sources of uncertainty cannot be traced anymore after    //
// application of the eigenvalue decomposition). It is still possible to evaluate correctly //
// these uncertainties in the form of so-called 'named variations' (see class               //
// CalibrationDataInterfaceROOT); however this will always treat uncertainties as being     //
// fully correlated (or anti-correlated) between calibration bins, so it is recommended not //
// to exclude uncertainties that are not correlated between bins from the eigenvector       //
// decomposition.                                                                           //
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
ClassImp(CalibrationDataEigenVariations)
#endif

//________________________________________________________________________________
CalibrationDataEigenVariations::CalibrationDataEigenVariations(const CalibrationDataHistogramContainer* cnt) :
  m_cnt(cnt), m_initialized(false), m_namedExtrapolation(-1)
{
}

//________________________________________________________________________________
CalibrationDataEigenVariations::~CalibrationDataEigenVariations()
{
  // delete all variation histograms owned by us
  for (vector<pair<TH1*, TH1*> >::iterator it = m_named.begin();
       it != m_named.end(); ++it) {
    delete it->first;
    delete it->second;
  }
  for (vector<pair<TH1*, TH1*> >::iterator it = m_eigen.begin();
       it != m_eigen.end(); ++it) {
    delete it->first;
    delete it->second;
  }
}

//________________________________________________________________________________
void
CalibrationDataEigenVariations::excludeNamedUncertainty(const std::string& name)
{
  // Exclude the source of uncertainty identified by the given name from being used
  // in the construction of the covariance matrix to be diagonalised.
  // Notes:
  // - Some names returned by CalibrationDataContainer::listUncertainties() are not
  //   meaningful in this context, and specifying them is not allowed.
  // - Once the eigenvector diagonalisation has been carried out, this method may
  //   not be used anymore.

  if (m_initialized)
    std::cerr << "CalibrationDataEigenVariations::excludeNamedUncertainty error:"
	      << " initialization already done" << std::endl;
  else if (name == "comment"    || name == "result"   || name == "systematics" ||
	   name == "statistics" || name == "combined" || name == "extrapolation" ||
	   name == "MCreference" || name == "MChadronisation")
    std::cerr << "CalibrationDataEigenVariations::excludeNamedUncertainty error:"
	      << " name " << name << " not allowed" << std::endl;
  else if (! m_cnt->GetValue(name.c_str()))
    std::cerr << "CalibrationDataEigenVariations::excludeNamedUncertainty error:"
	      << " uncertainty named " << name << " not found" << std::endl;
  else {
    m_named.push_back(std::make_pair<TH1*, TH1*>(0, 0));
    m_namedIndices[name] = m_named.size()-1;
  }
}

//________________________________________________________________________________
TMatrixDSym CalibrationDataEigenVariations::getEigenCovarianceMatrix() const
{
  // Construct the covariance matrix that is to be diagonalised.
  // Note that extrapolation uncertainties (identified by the keyword "extrapolation,
  // this will pertain mostly to the extrapolation to high jet pt) are always excluded
  // since by definition they will not apply to the normal calibration bins. Instead
  // this uncertainty has to be dealt with as a named variation. In addition there are
  // other items ("combined", "systematics") that will not be dealt with correctly
  // either and hence are excluded as well).
  //
  // Note that if an explicit covariance matrix is supplied (at present this may be
  // the case only for statistical uncertainties: in the case of "continuous tagging",
  // multinomial statistics applies so bin-to-bin correlations exist) this will be
  // used instead of constructing the statistical uncertainties' covariance matrix on
  // the fly.

  // retrieve the central calibration
  TH1* result = dynamic_cast<TH1*>(m_cnt->GetValue("result"));

  gROOT->cd();

  // loop over the uncertainties to construct the covariance matrix for all uncertainties
  // to be addressed using the eigenvector method.

  // First, treat the statistics separately.
  // Account for the possibility that this is handled as a (non-trivial) covariance matrix
  TMatrixDSym* sCov = dynamic_cast<TMatrixDSym*>(m_cnt->GetValue("statistics"));
  TMatrixDSym cov = (sCov) ? *sCov : getStatCovarianceMatrix(result);

  // Then loop through the list of (other) uncertainties
  std::vector<string> uncs = m_cnt->listUncertainties();
  for (unsigned int t = 0; t < uncs.size(); ++t) {
    // entries that should never be included
    if (uncs[t] == "comment" || uncs[t] == "result" ||
	uncs[t] == "combined" || uncs[t] == "statistics" ||
        uncs[t] == "systematics" || uncs[t] == "MCreference" ||
	uncs[t] == "MChadronisation" || uncs[t] == "extrapolation") continue;
    // entries that can be excluded if desired
    if (m_namedIndices.find(uncs[t]) != m_namedIndices.end()) continue;

    TH1* hunc = dynamic_cast<TH1*>(m_cnt->GetValue(uncs[t].c_str()));
    cov += getSystCovarianceMatrix(result, hunc, m_cnt->isBinCorrelated(uncs[t]), m_cnt->getTagWeightAxis());
  }

  return cov;
}

//________________________________________________________________________________
void
CalibrationDataEigenVariations::initialize()
{
  // This is this class's most important method, in the sense that it does all the
  // math and constructs all "variation" histograms (for both eigenvector and named
  // named variations). This constitutes the full initialisation of the class.
  // This method is meant to be called only after all calls (if any) to the
  // CalibrationDataEigenVariations::excludeNamedUncertainty() method.

  // retrieve the central calibration
  TH1* result = dynamic_cast<TH1*>(m_cnt->GetValue("result"));

  // first, construct the original covariance matrix
  TMatrixDSym cov = getEigenCovarianceMatrix();

  // Second step: create the variations for the named sources of uncertainty (easy...)
  for (map<string, unsigned int>::iterator it = m_namedIndices.begin();
       it != m_namedIndices.end(); ++it) {
    pair<TH1*, TH1*>& p = m_named[it->second];
    TH1* hunc = (TH1*) m_cnt->GetValue(it->first.c_str());
    TString namedvar("namedVar");
    namedvar += it->first.c_str();
    TString namedvarUp(namedvar);   namedvarUp   += "_up";
    TString namedvarDown(namedvar); namedvarDown += "_down";
    TH1* resultVariedUp   = (TH1*)result->Clone(namedvarUp);   resultVariedUp->Add(hunc, 1.0);
    TH1* resultVariedDown = (TH1*)result->Clone(namedvarDown); resultVariedDown->Add(hunc, -1.0);
    p.first  = resultVariedUp;
    p.second = resultVariedDown;
  }
  // Refinement: add the "extrapolation" uncertainty as a named uncertainty, if the histogram is provided
  // This is a bit special, since the extrapolation uncertainty histogram has a different size than other histograms
  if (TH1* hunc = (TH1*) m_cnt->GetValue("extrapolation")) {
    TH1* resultVariedUp   = (TH1*) hunc->Clone("extrapolation_up");
    TH1* resultVariedDown = (TH1*) hunc->Clone("extrapolation_down");
    Int_t nbinx = hunc->GetNbinsX()+2, nbiny = hunc->GetNbinsY()+2, nbinz = hunc->GetNbinsZ()+2;
    Int_t firstbinx = hunc->GetXaxis()->FindFixBin(result->GetXaxis()->GetBinCenter(1));
    Int_t firstbiny = result->GetDimension() > 1 ? hunc->GetYaxis()->FindFixBin(result->GetYaxis()->GetBinCenter(1)) : 1;
    Int_t firstbinz = result->GetDimension() > 2 ? hunc->GetZaxis()->FindFixBin(result->GetZaxis()->GetBinCenter(1)) : 1;
    for (Int_t binx = 1; binx < nbinx-1; ++binx) {
      Int_t binxResult = binx - firstbinx + 1;
      if (binxResult < 1) binxResult = 1;
      if (binxResult > result->GetNbinsX()) binxResult = result->GetNbinsX();
      for (Int_t biny = 1; biny < nbiny-1; ++biny) {
	Int_t binyResult = biny - firstbiny + 1;
	if (binyResult > result->GetNbinsY()) binyResult = result->GetNbinsY();
	for (Int_t binz = 1; binz < nbinz-1; ++binz) {
	  Int_t binzResult = binz - firstbinz + 1;
	  if (binzResult > result->GetNbinsZ()) binzResult = result->GetNbinsZ();
	  Int_t bin = hunc->GetBin(binx, biny, binz);
	  double contResult = result->GetBinContent(binxResult, binyResult, binzResult);
	  resultVariedUp->SetBinContent(bin, contResult + hunc->GetBinContent(bin));
	  resultVariedDown->SetBinContent(bin, contResult - hunc->GetBinError(bin));
	}
      }
    }
    m_named.push_back(std::make_pair(resultVariedUp, resultVariedDown));
    m_namedExtrapolation = m_namedIndices["extrapolation"] = m_named.size()-1;
  }

  // Third step: compute the eigenvector variations corresponding to the remaining sources of uncertainty
  int nbins = result->GetNbinsX()+2;
  int ndim  = result->GetDimension();
  if (ndim > 1) nbins*= (result->GetNbinsY()+2);
  if (ndim > 2) nbins*= (result->GetNbinsZ()+2);
  
  // Start by "compressing" the covariance matrix (removing columns/rows containing zeros only)
  int nZeros=0;
  std::vector<int> zeroComponents;
  if (cov.GetNrows() != nbins) {
    std::cerr << " error: covariance matrix size (" << cov.GetNrows() << ") doesn't match histogram size (" << nbins << ")" << std::endl;
    return;
  }

  // First flag all the zeros
  for (int i = 0; i < nbins; ++i) {
    // Directly identify the under- and overflow bins
    Int_t binx, biny, binz;
    result->GetBinXYZ(i, binx, biny, binz);
    if ((binx == 0 || binx == result->GetNbinsX()+1) ||
	(ndim > 1 && (biny == 0 || biny == result->GetNbinsY()+1)) ||
	(ndim > 2 && (binz == 0 || binz == result->GetNbinsZ()+1))) {
      ++nZeros;
      zeroComponents.push_back(i);
      // std::cout << "flagging bin " << i << " as under- or overflow" << std::endl;
    }
    // Try a first (quick) identification of rows/columns of zeros by the first element in each row
    // If "successful", check the whole row in more detail
    else if (fabs(cov(i,0)) < 1e-10) {
      bool isThereANonZero(false);
      for (int j = 0; j < nbins; ++j) {
        if (fabs(cov(i,j)) > 1e-10) {
          isThereANonZero=true; break;
        }
      }
      if (! isThereANonZero) {
        ++nZeros;
        zeroComponents.push_back(i);
      }
    }
  }
  // std::cout << "number of null rows: " <<  nZeros << std::endl;

  // Determine whether the container is for "continuous" calibration.
  // This is important since the number of independent scale factors (for each pt or eta bin)
  // is reduced by 1 compared to the number of tag weight bins (related to the fact that the fractions
  // of events in tag weight bins have to sum up to unity).
  int axis = m_cnt->getTagWeightAxis();
  bool doContinuous = false; unsigned int weightAxis = 0;
  if (axis >= 0) {
    doContinuous = true;
    weightAxis = (unsigned int) axis;
    // In this case, verify that the special "uncertainty" entry that is in fact the reference MC tag
    // weight fractions is present. These tag weight fractions are needed in order to carry out the
    // diagonalisation successfully.
    if (! dynamic_cast<TH1*>(m_cnt->GetValue("MCreference"))) {
      std::cerr << " Problem: continuous calibration object found without MC reference tag weight histogram " << std::endl;
      return;
    }
  }

  // Only relevant for continuous calibration containers, but in order to void re-computation we
  // retrieve them here
  Int_t nbinx = result->GetNbinsX()+2, nbiny = result->GetNbinsY()+2, nbinz = result->GetNbinsZ()+2;

  // If we are indeed dealing with a "continuous" calibration container, ignore one tag weight row
  const int skipTagWeightBin = 1; // NB this follows the histogram's bin numbering
  if (doContinuous) {
    for (Int_t binx = 1; binx < nbinx-1; ++binx)
      for (Int_t biny = 1; biny < nbiny-1; ++biny)
	for (Int_t binz = 1; binz < nbinz-1; ++binz) {
	  if ((weightAxis == 0 && binx == skipTagWeightBin) ||
	      (weightAxis == 1 && biny == skipTagWeightBin) ||
	      (weightAxis == 2 && binz == skipTagWeightBin)) {
	    // At this point we simply add these to the 'null' elements
	    ++nZeros;
	    zeroComponents.push_back(result->GetBin(binx, biny, binz));
	  }
	}
  }

  if (nZeros >= nbins) {
    std::cerr << " Problem. Found n. " << nZeros << " while size of matrix is " << nbins << std::endl;
    return;
  }

  int size = nbins - nZeros;

  TMatrixT<double> matrixVariationJacobian(size,nbins);
  int nMissed=0;
  for (int i = 0; i < nbins; ++i) { //full size
    bool missed=false;
    for (unsigned int s = 0; s < zeroComponents.size(); ++s) {
      if (zeroComponents.at(s) == i) {
        missed = true;
        break;
      }
    }
    if (missed) {
      ++nMissed;
      continue;
    }
    matrixVariationJacobian(i-nMissed,i)=1;
  }

  // reduce the matrix to one without the zeros, using a "similarity" transformation
  // std::cout << "#rows before similarity transform: " << cov.GetNrows() << std::endl;
  TMatrixDSym redSystematicCovMatrix = cov.Similarity(matrixVariationJacobian);
  // std::cout << "#rows after  similarity transform: " << cov.GetNrows() << std::endl;

  // GP uncomment to debug covariance matrix
  //  std::cout << " resulting size of cov matrix " <<  redSystematicCovMatrix.GetNrows()<< std::endl;
  //  std::cout << " determinant " << redSystematicCovMatrix.Determinant() << std::endl;
  //  for (int i=0;i<size;i++)
  //  {
  //    for (int j=0;j<size;j++)
  //    {
  //      std::cout << " ["<<i<<","<<j<<"]: " << redSystematicCovMatrix(i,j) << std::endl;
  //    }
  //  }

  const TMatrixDSym matrixCovariance = redSystematicCovMatrix;

  TVectorT<double> vectorConstants(redSystematicCovMatrix.GetNrows());
  for (int i = 0; i < redSystematicCovMatrix.GetNrows(); ++i) vectorConstants(i) = 0;
  
  TDecompBK myDecomposition(matrixCovariance);
  TMatrixDSym matrixWeight2=myDecomposition.Invert();
  
  //now you have the weight matrix
  TMatrixDSymEigen eigenValueMaker(matrixWeight2);
  TVectorT<double> eigenValues   = eigenValueMaker.GetEigenValues();
  TMatrixT<double> eigenVectors  = eigenValueMaker.GetEigenVectors();
  TMatrixT<double> eigenVectorsOriginal = eigenVectors;
  TMatrixT<double> eigenVectorsT = eigenVectors.T();

  TVectorT<double> constValues(size);
  for (int i = 0; i < size; ++i) {
    double constValue = 0;
    for (int j = 0; j < size; ++j) {
      double comb = eigenVectorsT[i][j];
      constValue += comb*vectorConstants[j];
    }
    constValues[i]=constValue;
  }

  TVectorT<double> randomEigenValues(size);
  TMatrixT<double> matrixVariations(size,size);
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) randomEigenValues(j)=0;
    randomEigenValues[i]=sqrt(1./eigenValues[i]);//here you can replace with a random gaussian sigma
  
    //now go back
    TVectorT<double> randomValues(size);
    for (int r = 0; r < size; ++r) {
      //first row of eigenVectors (without T) gives needed combinations
      double constValue = 0;
      for (int j = 0; j < size; ++j) {
        double comb = eigenVectorsOriginal[r][j];
        constValue += comb*randomEigenValues[j];
      }
      randomValues[r] = constValue;
      matrixVariations(i,r) = randomValues[r];//first index is the variation number, second corresponds to the pT bin
    }
  }

  TMatrixT<double> matrixVariationsWithZeros = matrixVariations * matrixVariationJacobian;

  // //now setup the real ucertainties
  // std::vector<TObject*> eigenvectorsUncUpProvider;
  // std::vector<TObject*> eigenvectorsUncDownProvider; 
  // std::vector<TObject*> eigenvectorsUncProvider;

  TH1* MC = (doContinuous) ? dynamic_cast<TH1*>(m_cnt->GetValue("MCreference")) : 0;
  // first carry out basic cross-checks: data and MC fractions summed over tag weight bins must always be 1
  if (doContinuous) {
    if (weightAxis == 2) {
      for (Int_t binx = 1; binx < nbinx-1; ++binx)
	for (Int_t biny = 1; biny < nbiny-1; ++biny) {
	  double effMC = 0, effData = 0;
	  for (Int_t binz = 1; binz < nbinz-1; ++binz) {
	    Int_t bin = result->GetBin(binx, biny, binz);
	    effData += result->GetBinContent(bin) * MC->GetBinContent(bin);
	    effMC   +=                              MC->GetBinContent(bin);
	  }
	  std::cout << "\t(x, y) = (" << setw(3) << binx << "," << setw(3) << biny
		    << ") summed MC: " << setw(6) << effMC << ", summed data: " << setw(6) << effData << std::endl;
	}
    } else if (weightAxis == 1) {
      for (Int_t binx = 1; binx < nbinx-1; ++binx)
	for (Int_t binz = 1; binz < nbinz-1; ++binz) {
	  double effMC = 0, effData = 0;
	  for (Int_t biny = 1; biny < nbiny-1; ++biny) {
	    Int_t bin = result->GetBin(binx, biny, binz);
	    effData += result->GetBinContent(bin) * MC->GetBinContent(bin);
	    effMC   +=                              MC->GetBinContent(bin);
	    // std::cout << "\tcnt = (" << setw(3) << binx << "," << setw(3) << biny << "," << setw(3) << binz
	    // 		<< ") MC: " << setw(10) << MC->GetBinContent(bin) << " SF: (nom = " << setw(8) << result->GetBinContent(bin)
	    // 		<< ", up = " << setw(8) << resultVariedUp->GetBinContent(bin) << "), up(cumulative): "
	    // 		<< setw(8) << effUp << ", nom(cumulative): " << setw(8) << effNom << std::endl;
	  }
	  std::cout << "\t(x, z) = (" << setw(3) << binx << "," << setw(3) << binz
		    << ") summed MC: " << setw(6) << effMC << ", summed data: " << setw(6) << effData << std::endl;
	}
    } else {
      for (Int_t biny = 1; biny < nbiny-1; ++biny)
	for (Int_t binz = 1; binz < nbinz-1; ++binz) {
	  double effMC = 0, effData = 0;
	  for (Int_t binx = 1; binx < nbinx-1; ++binx) {
	    Int_t bin = result->GetBin(binx, biny, binz);
	    effData += result->GetBinContent(bin) * MC->GetBinContent(bin);
	    effMC   +=                              MC->GetBinContent(bin);
	  }
	  std::cout << "\t(y, z) = (" << setw(3) << biny << "," << setw(3) << binz
		    << ") summed MC: " << setw(6) << effMC << ", summed data: " << setw(6) << effData << std::endl;
	}
    }
  }

  for (int i = 0; i < size; ++i) {
    // TString superstring(result->GetName());
    // superstring += "_eigenVar";
    TString superstring("eigenVar");
    superstring+=i;

    TString nameUp(superstring);   nameUp   += "_up";
    TString nameDown(superstring); nameDown += "_down";
    // TString nameUnc(superstring);  nameUnc+= "_unc";

    TH1* resultVariedUp   = (TH1*)result->Clone(nameUp);
    TH1* resultVariedDown = (TH1*)result->Clone(nameDown);

    for (int u = 0; u < nbins; ++u) {
      resultVariedUp->SetBinContent(u,result->GetBinContent(u)+
				    matrixVariationsWithZeros(i,u));
      resultVariedDown->SetBinContent(u,result->GetBinContent(u)-
				      matrixVariationsWithZeros(i,u));
    }

    // The "continuous" case is special, since we need to re-insert the variations for the tag weight bins that were
    // removed to make the covariance matrix non-singular
    if (doContinuous) {
      // std::cout << "assuming a continuous calibration object, weight axis = " << weightAxis << std::endl;

      // Ugly code duplication since nfortunately there doesn't seem to be a compact way to go
      // through the following manipulations without a priori knowledge of the histogram axes!
      if (weightAxis == 2) {
	for (Int_t binx = 1; binx < nbinx-1; ++binx)
	  for (Int_t biny = 1; biny < nbiny-1; ++biny) {
	    double effUp = 1, effDown = 1;
	    for (Int_t binz = 1; binz < nbinz-1; ++binz) {
	      if (binz == skipTagWeightBin) continue;
	      Int_t bin = result->GetBin(binx, biny, binz);
	      effUp   -= resultVariedUp->GetBinContent(bin)   * MC->GetBinContent(bin);
	      effDown -= resultVariedDown->GetBinContent(bin) * MC->GetBinContent(bin);
	    }
	    // What remains is the "data" tag weight fraction in the skipped bin.
	    // Divide it by its corresponding MC tag weight fraction to obtain the varied scale factor
	    Int_t bin = result->GetBin(binx, biny, skipTagWeightBin);
	    resultVariedUp->SetBinContent(bin,  effUp/MC->GetBinContent(bin));
	    resultVariedDown->SetBinContent(bin,effDown/MC->GetBinContent(bin));
	  }
      } else if (weightAxis == 1) {
	for (Int_t binx = 1; binx < nbinx-1; ++binx)
	  for (Int_t binz = 1; binz < nbinz-1; ++binz) {
	    double effUp = 1, effDown = 1; //, effNom = 1;
	    for (Int_t biny = 1; biny < nbiny-1; ++biny) {
	      if (biny == skipTagWeightBin) continue;
	      Int_t bin = result->GetBin(binx, biny, binz);
	      // effNom  -= result->GetBinContent(bin)           * MC->GetBinContent(bin);
	      effUp   -= resultVariedUp->GetBinContent(bin)   * MC->GetBinContent(bin);
	      effDown -= resultVariedDown->GetBinContent(bin) * MC->GetBinContent(bin);
	      // std::cout << "\tcnt = (" << setw(3) << binx << "," << setw(3) << biny << "," << setw(3) << binz
	      // 		<< ") MC: " << setw(10) << MC->GetBinContent(bin) << " SF: (nom = " << setw(8) << result->GetBinContent(bin)
	      // 		<< ", up = " << setw(8) << resultVariedUp->GetBinContent(bin) << "), up(cumulative): "
	      // 		<< setw(8) << effUp << ", nom(cumulative): " << setw(8) << effNom << std::endl;
	    }
	    // What remains is the "data" tag weight fraction in the skipped bin.
	    // Divide it by its corresponding MC tag weight fraction to obtain the varied scale factor
	    Int_t bin = result->GetBin(binx, skipTagWeightBin, binz);
	    resultVariedUp->SetBinContent(bin,  effUp/MC->GetBinContent(bin));
	    resultVariedDown->SetBinContent(bin,effDown/MC->GetBinContent(bin));
	    // std::cout << "\tcnt = (" << setw(3) << binx << "," << setw(3) << skipTagWeightBin << "," << setw(3) << binz
	    // 	      << ") MC: " << setw(10) << MC->GetBinContent(bin) << ", resulting SF ( nom = " << setw(8) << effNom/MC->GetBinContent(bin)
	    // 	      << ", check = " << setw(8) << result->GetBinContent(bin) << ", up = " << setw(8) << effUp/MC->GetBinContent(bin) << ")"
	    // 	      << std::endl;
	  }
      } else {
	for (Int_t biny = 1; biny < nbiny-1; ++biny)
	  for (Int_t binz = 1; binz < nbinz-1; ++binz) {
	    double effUp = 1, effDown = 1;
	    for (Int_t binx = 1; binx < nbinx-1; ++binx) {
	      if (binx == skipTagWeightBin) continue;
	      Int_t bin = result->GetBin(binx, biny, binz);
	      effUp   -= resultVariedUp->GetBinContent(bin)   * MC->GetBinContent(bin);
	      effDown -= resultVariedDown->GetBinContent(bin) * MC->GetBinContent(bin);
	    }
	    // What remains is the "data" tag weight fraction in the skipped bin.
	    // Divide it by its corresponding MC tag weight fraction to obtain the varied scale factor
	    Int_t bin = result->GetBin(skipTagWeightBin, biny, binz);
	    resultVariedUp->SetBinContent(bin,  effUp/MC->GetBinContent(bin));
	    resultVariedDown->SetBinContent(bin,effDown/MC->GetBinContent(bin));
	  }
      }

    }

    m_eigen.push_back(std::make_pair(resultVariedUp, resultVariedDown));

  } //end eigenvector size

  // // Cross-check: the sum of squares of the variations should be equal to the original covariance matrix
  // // 1. create the cross-check matrix
  // TMatrixDSym covCheck(nbins);
  // for (int u = 0; u < nbins; ++u)
  //   for (int v = 0; v < nbins; ++v) covCheck(u,v) = 0;
  // // 2. for each of the "up" variations, convert back to the difference (i.e., eigenvalue times eigenvector)
  // for (unsigned int i = 0; i < (unsigned int) size; ++i) { 
  //   double variation[nbins];
  //   TH1* resultVariedUp = m_eigen[i].first;
  //   for (unsigned int u = 0; u < (unsigned int) nbins; ++u)
  //     variation[u] = resultVariedUp->GetBinContent(u) - result->GetBinContent(u);
  //   // 3. these differences can be converted directly to contributions to the covariance matrix
  //   for (int u = 0; u < nbins; ++u)
  //     for (int v = 0; v < nbins; ++v)
  // 	covCheck(u, v) += variation[u]*variation[v];
  // }
  // 4. the cross-check: for each row report the largest deviation from unity in the ratio of the re-constructed matrix and the original matrix
  //    For this to work, re-retrieve the original covariance matrix (as it's modified by the similarity transformation...)
  // TMatrixDSym cov2 = getEigenCovarianceMatrix();
  // std::cout << " checking re-constructed covariance matrix against original matrix (diagonal elements only)" << std::endl;
  // for (int u = 0; u < nbins; ++u) {
  //   std::cout << "bin" << setw(2) << u << setprecision(3) << setw(10) << cov2(u,u) << setprecision(3) << setw(10) << covCheck(u,u) << std::endl;
  // }
  // cov2.Print("f=  %8.2g  ");
  // std::cout << " reconstructed matrix: " << std::endl;
  // covCheck.Print("f=  %8.2g  ");
  // for (int u = 0; u < nbins; ++u) {
  //   double maxdiff = 0.;
  //   for (int v = 0; v < nbins; ++v) {
  //     if (cov2(u, v) != 0) {
  // 	double diff = covCheck(u, v) / cov2(u, v) - 1.;
  // 	if (fabs(diff) > maxdiff) maxdiff = fabs(diff);
  //     }
  //   }
  //   if (maxdiff > 0.) {
  //     Int_t binx, biny, binz;
  //     result->GetBinXYZ(u, binx, biny, binz);
  //     std::cout << " \trow " << u << " (binx,biny,binz) = (" << binx << "," << biny << "," << binz << "), relative |diff| = " << maxdiff << std::endl;
  //   }
  // }

  // last step: order the named uncertainties (so that the user will not have to deal with this)
  
  m_initialized = true;
}

//________________________________________________________________________________
unsigned int
CalibrationDataEigenVariations::getNumberOfNamedVariations() const
{
  // Returns the number of named variations

  return m_namedIndices.size();
}

//________________________________________________________________________________
vector<string>
CalibrationDataEigenVariations::listNamedVariations() const
{
  // Provides the list of named variations

  vector<string> names;
  for (map<string, unsigned int>::const_iterator it = m_namedIndices.begin();
       it != m_namedIndices.end(); ++it)
    names.push_back(it->first);
  return names;
}

//________________________________________________________________________________
unsigned int
CalibrationDataEigenVariations::getNumberOfEigenVariations() const
{
  if (! m_initialized) const_cast<CalibrationDataEigenVariations*>(this)->initialize();
  return m_eigen.size();
}

//________________________________________________________________________________
bool
CalibrationDataEigenVariations::getEigenvectorVariation(unsigned int variation,
							TH1*& up, TH1*& down) const
{
  // Return the pointers to the up- and downward variation histogram for the specified
  // eigenvector variation. In case of an invalid variation number, null pointers will
  // be returned and the return value will be false.
  //
  //     variation:   eigenvector variation number
  //     up:          (reference to) pointer to upward variation histogram
  //     down:        (reference to) pointer to downward variation histogram

  if (! m_initialized) const_cast<CalibrationDataEigenVariations*>(this)->initialize();
  if (variation < m_eigen.size()) {
    up   = m_eigen[variation].first;
    down = m_eigen[variation].second;
    return true;
  }

  up = down = 0;
  return false;
}

//________________________________________________________________________________
bool
CalibrationDataEigenVariations::getNamedVariation(const string& name,
						  TH1*& up, TH1*& down) const
{
  // Return the pointers to the up- and downward variation histogram for the specified
  // named variation. In case of an invalid named variation, null pointers will
  // be returned and the return value will be false.
  //
  //     name:        named variation
  //     up:          (reference to) pointer to upward variation histogram
  //     down:        (reference to) pointer to downward variation histogram

  map<string, unsigned int>::const_iterator it = m_namedIndices.find(name);
  if (it != m_namedIndices.end()) return getNamedVariation(it->second, up, down);

  up = down = 0;
  return false;
}

//________________________________________________________________________________
bool
CalibrationDataEigenVariations::getNamedVariation(unsigned int nameIndex,
						  TH1*& up, TH1*& down) const
{
  // Return the pointers to the up- and downward variation histogram for the specified
  // named variation. In case of an invalid named variation number, null pointers will
  // be returned and the return value will be false.
  //
  //     nameIndex:   named variation number
  //     up:          (reference to) pointer to upward variation histogram
  //     down:        (reference to) pointer to downward variation histogram

  if (! m_initialized) const_cast<CalibrationDataEigenVariations*>(this)->initialize();

  if (nameIndex < m_named.size()) {
    up   = m_named[nameIndex].first;
    down = m_named[nameIndex].second;
    return true;
  }

  up = down = 0;
  return false;
}

//________________________________________________________________________________
unsigned int
CalibrationDataEigenVariations::getNamedVariationIndex(const std::string& name) const
{
  // Return the integer index corresponding to the named variation.
  // Note that no checks are made on the validity of the name provided.

  map<string, unsigned int>::const_iterator it = m_namedIndices.find(name);
  return it->second;
}

//________________________________________________________________________________
bool
CalibrationDataEigenVariations::isExtrapolationVariation(unsigned int nameIndex) const
{
  // Verifies whether the given named variation index corresponds to the extrapolation
  // uncertainty.

  return (m_namedExtrapolation == int(nameIndex));
}
