//
// File:      InvalidArgument.h
// Package    exception
//
// Invalid argument exception.
//
#if !defined(exception_InvalidArgument_h)
#define exception_InvalidArgument_h

#include "LogicError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief InvalidArgument encapsulates an invalid argument condition.
     */
    class InvalidArgument : public LogicError {

    public:
      /*!
       * @brief Construct InvalidArgument object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit InvalidArgument( const std::string & what_arg ) throw();

      /*!
       * @brief Destructor.
       */
      virtual ~InvalidArgument() throw();

    };

  }
}

#endif // exception_InvalidArgument_h
