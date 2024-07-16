//
// File:      DomainError.cc
// Package    exception
//
// DomainError exception.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "DomainError.h"

//
//
//

namespace arl {
  namespace hms {
    //
    // Constructor
    //
    DomainError::DomainError( const std::string & what_arg ) throw() :
      LogicError( what_arg )
    {

      return;

    }

    //
    // Destructor
    //
    DomainError::~DomainError() throw()
    {

      return;
  
    }

  }
}
