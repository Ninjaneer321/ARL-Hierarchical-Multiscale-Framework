//
// File:      NotImplemented.cc
// Package    exception
//
// NotImplemented exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "NotImplemented.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    NotImplemented::NotImplemented( const std::string & what_arg ) throw() :
      RuntimeError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    NotImplemented::~NotImplemented() throw()
    {

      return;
  
    }

  }
}
