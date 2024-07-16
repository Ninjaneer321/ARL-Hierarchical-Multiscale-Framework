//
// File:      Priority.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_Priority_h)
#define base_Priority_h

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * Priority.
     */
    typedef enum Priority {
      SPECULATIVE = 0,
      LOWEST,
      LOW,
      NORMAL,
      HIGH,
      HIGHEST
    } Priority;
  
  }
}

#endif // base_Priority_h
