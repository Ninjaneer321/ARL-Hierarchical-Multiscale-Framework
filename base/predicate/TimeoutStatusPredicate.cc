//
// File:      TimeoutStatusPredicate.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "TimeoutStatusPredicate.h"

#include <base/Command.h>
#include <base/TimeUtils.h>

HMS_SERIALIZATION_EXPORT(arl::hms::TimeoutStatusPredicate)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    TimeoutStatusPredicate::TimeoutStatusPredicate() :
      d_maximumWallClockTime(std::numeric_limits<int>::max())
    {

      //
      //
      //
      return;
      
    }
    
    //
    // Constructor.
    //
    TimeoutStatusPredicate::TimeoutStatusPredicate(int maximumWallClockTime) :
      d_maximumWallClockTime(maximumWallClockTime)
    {

      //
      //
      //
      return;

    }
  
    //
    // Destructor.
    //
    TimeoutStatusPredicate::~TimeoutStatusPredicate()
    {

      //
      //
      //
      return;

    }

    //
    // Evaluate predicate.
    //
    bool
    TimeoutStatusPredicate::evaluate(const Command & command)
    {
      
      //
      // get time
      //
      const struct timespec currentTime = getTime();

      //
      // get start time of the command
      //
      const struct timespec startTime = command.getStartTime();

      //
      // compute the difference in seconds
      //
      const double elapsedTime = timespecToDouble(timeSubtract(currentTime,
							       startTime));

      
      if(elapsedTime > d_maximumWallClockTime) {
	return false;
      }
      
      //
      //
      //
      return true;
      
    }
    
  }
}
