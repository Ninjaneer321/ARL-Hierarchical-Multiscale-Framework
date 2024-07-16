//
// File:      CommandError.cc
// Package    exception
//
// CommandError exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "CommandError.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    CommandError::CommandError( const std::string & what_arg ) throw() :
      RuntimeError( what_arg )
    {

      //
      //
      //
      return;

    }

    //
    // Destructor
    //
    CommandError::~CommandError() throw()
    {

      //
      //
      //
      return;
  
    }

  }
}
