//
// File:      ConnectionTerminationException.h
// Package    exception
//
// Connection termination exception.
//
#if !defined(exception_ConnectionTerminationException_h)
#define exception_ConnectionTerminationException_h

#include "NetworkError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief ConnectionTerminationException encapsulates a runtime
     * error condition.
     */
    class ConnectionTerminationException : public NetworkError {

    public:
      /*!
       * @brief Construct ConnectionTerminationException object given
       * a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit 
      ConnectionTerminationException( const std::string & what_arg ) throw();

      /*!
       * @brief Destructor.
       */
      virtual ~ConnectionTerminationException() throw();

    };
  }
}

#endif // exception_ConnectionTerminationException_h
