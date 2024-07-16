//
// File:      ArithmeticError.cc
// Package    exception
//
// ArithmeticError exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "ArithmeticError.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    ArithmeticError::ArithmeticError( const std::string & what_arg ) throw() :
      RuntimeError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    ArithmeticError::~ArithmeticError() throw()
    {

      return;
  
    }

  }
}
