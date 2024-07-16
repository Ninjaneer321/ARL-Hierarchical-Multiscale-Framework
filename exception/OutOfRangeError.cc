//
// File:      OutOfRangeError.cc
// Package    exception
//
// OutOfRangeError exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "OutOfRangeError.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    OutOfRangeError::OutOfRangeError( const std::string & what_arg ) throw() :
      LogicError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    OutOfRangeError::~OutOfRangeError() throw()
    {

      return;
  
    }

  }
}
