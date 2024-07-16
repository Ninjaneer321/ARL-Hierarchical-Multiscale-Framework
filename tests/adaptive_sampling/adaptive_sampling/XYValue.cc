//
// File:      XYValue.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "XYValue.h"

HMS_SERIALIZATION_EXPORT(arl::hms::XYValue)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    XYValue::XYValue()
    {

      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    XYValue::XYValue(double value) :
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
    XYValue::~XYValue()
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
    XYValue::getValue() const
    {

      //
      //
      //
      return d_value;
      
    }

  }
}
