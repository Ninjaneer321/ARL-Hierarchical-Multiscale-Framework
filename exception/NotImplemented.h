//
// File:      NotImplemented.h
// Package    exception
//
// Not implemented error exception
//
#if !defined(exception_NotImplemented_h)
#define exception_NotImplemented_h

#include "RuntimeError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief NotImplemented encapsulates a runtime error condition.
     */
    class NotImplemented : public RuntimeError {

    public:
      /*!
       * @brief Construct NotImplemented object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit NotImplemented( const std::string & what_arg ) throw();

      /*!
       * @brief Destructor.
       */
      virtual ~NotImplemented() throw();

    };

  }
}

#endif // exception_NotImplemented_h
