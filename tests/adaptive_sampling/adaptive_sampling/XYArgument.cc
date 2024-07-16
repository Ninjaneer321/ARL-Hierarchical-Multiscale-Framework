//
// File:      XYArgument.cc
// Package    tests/adaptive_sampling/interpolation_database
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "XYArgument.h"

HMS_SERIALIZATION_EXPORT(arl::hms::XYArgument)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    XYArgument::XYArgument()
    {
      
      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    XYArgument::XYArgument(double x,
                           double y) :
      d_x(x),
      d_y(y)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    XYArgument::~XYArgument()
    {

      //
      //
      //
      return;

    }

    //
    // Get X.
    //
    double 
    XYArgument::getX() const
    {

      //
      //
      //
      return d_x;

    }
  
    //
    // Get Y.
    //
    double 
    XYArgument::getY() const
    {

      //
      //
      //
      return d_y;

    }

  }
}
