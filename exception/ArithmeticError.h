//
// File:      ArithmeticError.h
// Package    exception
//
// Arithmetic error exception.
//
#if !defined(exception_ArithmeticError_h)
#define exception_ArithmeticError_h

#include "RuntimeError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief ArithmeticError encapsulates a runtime error condition.
     */
    class ArithmeticError : public RuntimeError {

    public:
      /*!
       * @brief Construct ArithmeticError object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit ArithmeticError( const std::string & what_arg ) throw();

      /*!
       * @brief Destructor.
       */
      virtual ~ArithmeticError() throw();

    };

  }
}

#endif // exception_ArithmeticError_h
