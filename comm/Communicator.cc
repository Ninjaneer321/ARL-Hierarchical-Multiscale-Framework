//
// File:      Communicator.cc
// Package    comm
//
// Package for handling communication between individual components.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "Communicator.h"

#include <exception/OSError.h>

#if defined(HAVE_CERRNO)
#include <cerrno>
#else
#error cerrno header file not available
#endif // HAVE_CERRNO

#if defined(HAVE_CSTRING)
#include <cstring>
#else
#error cstring header file not available
#endif // HAVE_CSTRING

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    Communicator::Communicator()
    {

      if(pthread_mutex_init(&d_failedDeserializationLock,
                            NULL) != 0) {
        std::stringstream message;
        message << "Failure on pthread_mutex_init() of failed deserialization "
          "lock: " << strerror(errno);
        throw OSError(message.str());
      }
      
      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    Communicator::~Communicator()
    {
      
      pthread_mutex_destroy(&d_failedDeserializationLock);

      //
      //
      //
      return;
      
    }
    
  }
}
