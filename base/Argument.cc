//
// File:      Argument.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "Argument.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    Argument::Argument() :
      d_toCleanupFlag(false)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    Argument::~Argument()
    {

      //
      //
      //
      return;

    }

    //
    // Get to delete flag.
    //
    bool
    Argument::getToCleanup() const
    {

      //
      //
      //
      return d_toCleanupFlag;
      
    }
      
    //
    // Set to delete flag.
    //
    void
    Argument::setToCleanup(bool toCleanup)
    {

      //
      //
      //
      d_toCleanupFlag = toCleanup;

      //
      //
      //
      return;

    }
    
  }
}
