//
// File:      ScalarValue.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "ScalarValue.h"

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::ScalarValue)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    ScalarValue::ScalarValue()
    {

      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    ScalarValue::ScalarValue(double value) :
      d_value(value)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    ScalarValue::~ScalarValue()
    {

      //
      //
      //
      return;

    }

    //
    // Get value.
    //
    double 
    ScalarValue::getValue() const
    {

      //
      //
      //
      return d_value;
      
    }

  }
}
