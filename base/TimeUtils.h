//
// File:      Time.h
// Package    base
//
// Base classes for HMS.
//
//#if !defined(base_Time_h)
//#define base_Time_h

#include <ctime>

namespace arl {
  namespace hms {

    /**
     * Get the current time.
     *
     * @return Current time.
     */
    struct timespec getTime();

    /**
     * Convert timespec to double representation in seconds
     *
     * @param time Time to convert.
     * @return Time in seconds using double representation.
     */
    double timespecToDouble(const struct timespec & time);

    /**
     * Subtract two timespecs.
     *
     * @param a First timespec argument.
     * @param b Second timespect argument.
     *
     * @return a - b.
     */
    struct timespec timeSubtract(const struct timespec & a,
				 const struct timespec & b);
    
  }
}

//#endif // base_Time_h
