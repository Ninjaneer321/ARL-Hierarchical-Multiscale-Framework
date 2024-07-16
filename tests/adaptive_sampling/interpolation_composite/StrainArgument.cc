//
// File:      StrainArgument.cc
// Package    tests/adaptive_sampling/interpolation_database
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "StrainArgument.h"

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::StrainArgument)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    StrainArgument::StrainArgument()
    {
      
      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    StrainArgument::StrainArgument(const std::vector<double> & strain) :
      d_strain(strain)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    StrainArgument::~StrainArgument()
    {

      //
      //
      //
      return;

    }

    //
    // Get strain.
    //
    const std::vector<double> & 
    StrainArgument::getStrain() const
    {

      //
      //
      //
      return d_strain;

    }
  
  }
}
