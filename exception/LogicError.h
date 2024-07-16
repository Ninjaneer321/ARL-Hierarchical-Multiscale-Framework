//
// File:      LogicError.h
// Package    exception
//
// Logic error exception.
//
#if !defined(exception_LogicError_h)
#define exception_LogicError_h

#include "Exception.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief LogicError encapsulates a logic error condition.
     */
    class LogicError : public Exception {

    public:
      /*!
       * @brief Destructor.
       */
      virtual ~LogicError() throw() = 0;

    protected:
      /*!
       * @brief Construct LogicError object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit LogicError( const std::string & what_arg ) throw();

    };

  }
}

#endif // exception_LogicError_h
