//
// File:      RangeError.h
// Package    exception
//
// Range error exception.
//
#if !defined(exception_RangeError_h)
#define exception_RangeError_h

#include "RuntimeError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief RangeError encapsulates a runtime error condition.
     */
    class RangeError : public RuntimeError {

    public:
      /*!
       * @brief Construct RangeError object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit RangeError( const std::string & what_arg ) throw();

      /*!
       * @brief Destructor.
       */
      virtual ~RangeError() throw();

    };

  }
}

#endif // exception_RangeError_h
