//
// File:      LogicError.cc
// Package    excption
//
// LogicError exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "LogicError.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    LogicError::LogicError( const std::string & what_arg ) throw() :
      Exception( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    LogicError::~LogicError() throw()
    {

      return;
  
    }

  }
}
