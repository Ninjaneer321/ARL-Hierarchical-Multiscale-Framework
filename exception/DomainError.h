//
// File:      DomainError.h
// Package    exception
//
// Domain error exception.
//
#if !defined(exception_DomainError_h)
#define exception_DomainError_h

#include "LogicError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief DomainError encapsulates a length error condition.
     */
    class DomainError : public LogicError {

    public:
      /*!
       * @brief Construct DomainError object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit DomainError( const std::string & what_arg ) throw();

      /*!
       * @brief Destructor.
       */
      virtual ~DomainError() throw();

    };

  }
}

#endif // exception_DomainError_h
