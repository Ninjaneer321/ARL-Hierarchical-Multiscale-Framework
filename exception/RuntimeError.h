//
// File:      RuntimeError.h
// Package    exception
//
// Runtime error exception.
//
#if !defined(exception_RuntimeError_h)
#define exception_RuntimeError_h

#include "Exception.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief RuntimeError encapsulates a runtime error condition.
     */
    class RuntimeError : public Exception {

    public:
      /*!
       * @brief Destructor.
       */
      virtual ~RuntimeError() throw() = 0;

    protected:
      /*!
       * @brief Construct RuntimeError object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit RuntimeError( const std::string & what_arg ) throw();

    };

  }
}

#endif // exception_RuntimeError_h
