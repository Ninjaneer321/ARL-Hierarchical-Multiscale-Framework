//
// File:      RuntimeError.cc
// Package    exception
//
// RuntimeError exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "RuntimeError.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    RuntimeError::RuntimeError( const std::string & what_arg ) throw() :
      Exception( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    RuntimeError::~RuntimeError() throw()
    {

      return;
  
    }

  }
}
