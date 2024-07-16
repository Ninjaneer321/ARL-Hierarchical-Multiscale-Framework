//
// File:      LengthError.cc
// Package    exception
//
// LengthError exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "LengthError.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    LengthError::LengthError( const std::string & what_arg ) throw() :
      LogicError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    LengthError::~LengthError() throw()
    {

      return;
  
    }

  }
}
