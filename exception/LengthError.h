//
// File:      LengthError.h
// Package    exception
//
// Length error exception.
//
#if !defined(exception_LengthError_h)
#define exception_LengthError_h

#include "LogicError.h"

//
//
//
namespace arl {
  namespace hms {

  /*!
   * @brief LengthError encapsulates a length error condition.
   */
  class LengthError : public LogicError {

  public:
    /*!
     * @brief Construct LengthError object given a message string.
     *
     * @param what_arg String containing the message associated with 
     *                 the exception.
     */
    explicit LengthError( const std::string & what_arg ) throw();

    /*!
     * @brief Destructor.
     */
    virtual ~LengthError() throw();

  };

  }
}

#endif // exception_LengthError_h
