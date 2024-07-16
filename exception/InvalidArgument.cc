//
// File:      InvalidArgument.cc
// Package    exception
//
// InvalidArgument exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "InvalidArgument.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    InvalidArgument::InvalidArgument( const std::string & what_arg ) throw() :
      LogicError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    InvalidArgument::~InvalidArgument() throw()
    {

      return;
  
    }

  }
}
