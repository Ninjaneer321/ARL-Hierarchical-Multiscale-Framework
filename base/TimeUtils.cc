//
// File:      Time.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "TimeUtils.h"

#include <exception/OSError.h>

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

#include <errno.h>
#include <string.h>

//
// OS X
//
#if defined(__MACH__)
#include <mach/clock.h>
#include <mach/mach.h>
#endif // __MACH__

namespace arl {
  namespace hms {

    //
    // Get the current time.
    //
    struct timespec
    getTime()
    {

      struct timespec returnValue;

#if defined(__MACH__) 
      clock_serv_t cclock;
      mach_timespec_t mts;
      if(host_get_clock_service(mach_host_self(), 
				CALENDAR_CLOCK, 
				&cclock) != 0) {
	std::stringstream message;
	message << "Error calling host_get_clock_service(): " 
		<< strerror(errno);
	throw OSError(message.str());
      }
      if(clock_get_time(cclock, 
			&mts) != 0) {
	std::stringstream message;
	message << "Error calling clock_get_time(): "
		<< strerror(errno);
	throw OSError(message.str());
      } 
      if(mach_port_deallocate(mach_task_self(), 
			      cclock) != 0) {
	std::stringstream message;
	message << "Error calling mach_port_deallocate(): "
		<< strerror(errno);
	throw OSError(message.str());
      }
      returnValue.tv_sec = mts.tv_sec;
      returnValue.tv_nsec = mts.tv_nsec;

#else

      if(clock_gettime(CLOCK_REALTIME, 
		       &returnValue) != 0) {
	std::stringstream message;
	message << "Error calling clock_gettime(): " 
		<< strerror(errno);
	throw OSError(message.str());
      }
        
#endif // __MACH__
        
      //
      //
      //
      return returnValue;

    }

    //
    // Convert timespec to double representation in seconds.
    //
    double
    timespecToDouble(const struct timespec & time)
    {

      //
      //
      //
      const double returnValue = time.tv_sec + time.tv_nsec / 1000000000.;

      //
      //
      //
      return returnValue;
      
    }

    //
    // Subtract two timespecs.
    //
    struct timespec
    timeSubtract(const struct timespec & a,
		 const struct timespec & b)
    {

      //
      //
      //
      struct timespec returnValue;
      returnValue.tv_sec = a.tv_sec - b.tv_sec;
      returnValue.tv_nsec = a.tv_nsec - b.tv_nsec;

      //
      //
      //
      return returnValue;
      
    }
    
  }
}
