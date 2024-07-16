//
// File:      ConnectionTerminationException.cc
// Package    exception
//
// ConnectionTerminationException exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "ConnectionTerminationException.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    ConnectionTerminationException::ConnectionTerminationException( const std::string & what_arg ) throw() :
      NetworkError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    ConnectionTerminationException::~ConnectionTerminationException() throw()
    {

      return;
  
    }

  }
}
