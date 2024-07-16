//
// File:      CommandType.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_CommandType_h)
#define base_CommandType_h

//
//
//

namespace arl {
  namespace hms {

    /**
     * CommandType.
     */
    typedef enum CommandType {
      MPI = 0,
      PBS
    } CommandType;
  
  }
}

#endif // base_CommandType_h
