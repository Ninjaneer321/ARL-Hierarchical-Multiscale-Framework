//
// File:      InvalidPointer.cc
// Package    exception
//
// InvalidPointer exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "InvalidPointer.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    InvalidPointer::InvalidPointer( const std::string & what_arg ) throw() :
      MemoryError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    InvalidPointer::~InvalidPointer() throw()
    {

      return;
  
    }

  }
}
