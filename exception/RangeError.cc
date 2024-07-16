//
// File:      RangeError.cc
// Package    exception
//
// RangeError exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "RangeError.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    RangeError::RangeError( const std::string & what_arg ) throw() :
      RuntimeError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    RangeError::~RangeError() throw()
    {

      return;
  
    }

  }
}
