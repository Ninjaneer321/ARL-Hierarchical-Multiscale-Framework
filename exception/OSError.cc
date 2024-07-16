//
// File:      OSError.cc
// Package    exception
//
// OSError exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "OSError.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    OSError::OSError( const std::string & what_arg ) throw() :
      RuntimeError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    OSError::~OSError() throw()
    {

      return;
  
    }

  }
}
