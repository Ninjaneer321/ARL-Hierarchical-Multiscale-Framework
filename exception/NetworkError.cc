//
// File:      NetworkError.cc
// Package    exception
//
// NetworkError exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "NetworkError.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    NetworkError::NetworkError( const std::string & what_arg ) throw() :
      IOError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    NetworkError::~NetworkError() throw()
    {

      return;
  
    }

  }
}
