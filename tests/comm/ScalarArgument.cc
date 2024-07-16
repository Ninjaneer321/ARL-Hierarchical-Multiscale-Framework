//
// File:      ScalarArgument.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "ScalarArgument.h"

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::ScalarArgument)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    ScalarArgument::ScalarArgument()
    {

      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    ScalarArgument::ScalarArgument(double argument) :
      d_argument(argument)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    ScalarArgument::~ScalarArgument()
    {

      //
      //
      //
      return;

    }

    //
    // Get argument.
    //
    double 
    ScalarArgument::getArgument() const
    {

      //
      //
      //
      return d_argument;

    }

  }
}
