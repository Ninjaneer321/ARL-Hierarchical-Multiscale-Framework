//
// File:      MemoryError.cc
// Package    exception
//
// MemoryError exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "MemoryError.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    MemoryError::MemoryError( const std::string & what_arg ) throw() :
      RuntimeError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    MemoryError::~MemoryError() throw()
    {

      return;
  
    }

  }
}
