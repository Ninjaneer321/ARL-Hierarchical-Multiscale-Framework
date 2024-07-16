//
// File:      CommandStatus.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_CommandStatus_h)
#define base_CommandStatus_h

//
//
//

namespace arl {
  namespace hms {

    /**
     * CommandStatus.
     */
    typedef enum CommandStatus {
      RUNNING = 0,
      COMPLETED = 1,
      ERROR = -1
    } CommandStatus;

  }
}

#endif // base_CommandStatus_h
