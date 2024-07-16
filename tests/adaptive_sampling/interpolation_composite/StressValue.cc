//
// File:      StressValue.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "StressValue.h"

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::StressValue)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    StressValue::StressValue()
    {

      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    StressValue::StressValue(const std::vector<double> & stress) :
      d_stress(stress)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    StressValue::~StressValue()
    {

      //
      //
      //
      return;

    }

    //
    // Get value.
    //
    const std::vector<double> & 
    StressValue::getStress() const
    {

      //
      //
      //
      return d_stress;
      
    }

  }
}
