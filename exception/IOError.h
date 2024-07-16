//
// File:      IOError.h
// Package    exception
//
// IO error exception.
//
#if !defined(exception_IOError_h)
#define exception_IOError_h

#include "RuntimeError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief IOError encapsulates a runtime error condition.
     */
    class IOError : public RuntimeError {

    public:
      /*!
       * @brief Construct IOError object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit IOError( const std::string & what_arg ) throw();

      /*!
       * @brief Destructor.
       */
      virtual ~IOError() throw();

    };
  }
}

#endif // exception_IOError_h
