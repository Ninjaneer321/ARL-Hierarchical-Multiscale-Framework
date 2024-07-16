//
// File:      OutOfRangeError.h
// Package    exception
//
// Out of range exception.
//
#if !defined(exception_OutOfRangeError_h)
#define exception_OutOfRangeError_h

#include "LogicError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief OutOfRangeError encapsulates a length error condition.
     */
    class OutOfRangeError : public LogicError {

    public:
      /*!
       * @brief Construct OutOfRangeError object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit OutOfRangeError( const std::string & what_arg ) throw();

      /*!
       * @brief Destructor.
       */
      virtual ~OutOfRangeError() throw();

    };

  }
}

#endif // exception_OutOfRangeError_h
