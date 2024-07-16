//
// File:      SleepArgument.cc
// Package    tests/adaptive_sampling/multiplexer
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "SleepArgument.h"

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::SleepArgument)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    SleepArgument::SleepArgument()
    {
      
      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    SleepArgument::SleepArgument(int sleepTime) :
      d_sleepTime(sleepTime)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    SleepArgument::~SleepArgument()
    {

      //
      //
      //
      return;

    }

    //
    // Get sleep time.
    //
    double
    SleepArgument::getSleepTime() const
    {

      //
      //
      //
      return d_sleepTime;

    }
  
  }
}
