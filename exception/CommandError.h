//
// File:      CommandError.h
// Package    exception
//
// Runtime error exception.
//
#if !defined(exception_CommandError_h)
#define exception_CommandError_h

#include "RuntimeError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief CommandError encapsulates a runtime error condition.
     */
    class CommandError : public RuntimeError {

    public:

      /*!
       * @brief Construct CommandError object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit CommandError( const std::string & what_arg ) throw();

      /*!
       * @brief Destructor.
       */
      virtual ~CommandError() throw();

    };

  }
}

#endif // exception_CommandError_h
