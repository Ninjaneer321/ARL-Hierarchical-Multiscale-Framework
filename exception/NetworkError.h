//
// File:      NetworkError.h
// Package    exception
//
// Network error exception.
//
#if !defined(exception_NetworkError_h)
#define exception_NetworkError_h

#include "IOError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief NetworkError encapsulates a runtime error condition.
     */
    class NetworkError : public IOError {

    public:
      /*!
       * @brief Construct NetworkError object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit NetworkError( const std::string & what_arg ) throw();

      /*!
       * @brief Destructor.
       */
      virtual ~NetworkError() throw();

    };
  }
}

#endif // exception_NetworkError_h
