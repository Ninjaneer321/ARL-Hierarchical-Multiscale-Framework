//
// File:      DPDArgument.cc
// Package    tests/adaptive_sampling/interpolation_dpd
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "DPDArgument.h"

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::DPDArgument)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    DPDArgument::DPDArgument()
    {
      
      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    DPDArgument::DPDArgument(double energyDensity,
                             double massDensity) :
      d_energyDensity(energyDensity),
      d_massDensity(massDensity)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    DPDArgument::~DPDArgument()
    {

      //
      //
      //
      return;

    }

    //
    // Get energy density.
    //
    double 
    DPDArgument::getEnergyDensity() const
    {

      //
      //
      //
      return d_energyDensity;

    }

    //
    // Get mass density.
    //
    double 
    DPDArgument::getMassDensity() const
    {

      //
      //
      //
      return d_massDensity;
    
    }
  
  }
}
