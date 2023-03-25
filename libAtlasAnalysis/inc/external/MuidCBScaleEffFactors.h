//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 27.04.2011, MCP working group
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef MuidCBScaleEffFactorH
#define MuidCBScaleEffFactorH

/////////////////////////////////
// CLASS MuidCBScaleEffFactors //
/////////////////////////////////

/// \class MuidCBScaleEffFactors
///
/// This class provides efficiency scale factors and their uncertainties
/// for physics analyses using Muid combined muons only. A separate class
/// must be used if the analysis uses Muid combined and tagged muons.
///
/// For 2011 Tier0 data.
///
/// \date 27.04.2011
///
/// \author Oliver.Kortner@CERN.CH

//////////////////
// HEADER FILES //
//////////////////

// ROOT //
#include "TLorentzVector.h"

// STL //
#include <vector>

// Base class //
#include "MuonEfficiencyScaleFactor.h"

// auxiliary class //
#include <EtaPhiBinning.h>

namespace Analysis {
    class MuidCBScaleEffFactors : public MuonEfficiencyScaleFactor {
    public:
    //! Constructor
    MuidCBScaleEffFactors(void);
    //! Default constructor.
    virtual ~MuidCBScaleEffFactors() {}

    // Methods //
    double scaleFactor(const TLorentzVector & tlv) const;
    ///< Get the efficiency scale factor for the given
    ///< fourmomentum.
    double scaleFactorUncertainty(const TLorentzVector & tlv) const;
    ///< Get the uncertainty of the efficiency scale
    ///< factor for the given fourmomentum.
    double scaleFactor(const TLorentzVector & tlv,
                                    const unsigned int & run_nb) const;
    ///< Get the efficiency scale factor for the given
    ///< fourmomentum and the given run number.
    double scaleFactorUncertainty(const TLorentzVector & tlv,
                                    const unsigned int & run_nb) const;
    ///< Get the uncertainty of the efficiency scale
    ///< factor for the given fourmomentum and the given run number..

    double scaleFactorSystematicUncertainty(const TLorentzVector & tlv) const;
    ///< Get the systematic uncertainty of the scale factor. The momentum
    ///< is assumed to be given in MeV.

// private //
  private:
    std::vector<double> m_scale_factor_A;
    std::vector<double> m_scale_factor_uncertainty_A;
    std::vector<double> m_scale_factor_C;
    std::vector<double> m_scale_factor_uncertainty_C;
    std::vector<double> m_scale_factor_A_periodB;
    std::vector<double> m_scale_factor_uncertainty_A_periodB;
    std::vector<double> m_scale_factor_C_periodB;
    std::vector<double> m_scale_factor_uncertainty_C_periodB;

    unsigned m_last_run_periodB;

    EtaPhiBinning m_eta_phi_binning; // auxiliary binning class
  };
}

#endif
