//
// File:      IOError.cc
// Package    exception
//
// IOError exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "IOError.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    IOError::IOError( const std::string & what_arg ) throw() :
      RuntimeError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    IOError::~IOError() throw()
    {

      return;
  
    }

  }
}
