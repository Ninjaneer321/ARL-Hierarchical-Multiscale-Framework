//
// File:      NullPointer.cc
// Package    exception
//
// NullPointer exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "NullPointer.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    NullPointer::NullPointer( const std::string & what_arg ) throw() :
      MemoryError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    NullPointer::~NullPointer() throw()
    {

      return;
  
    }

  }
}
