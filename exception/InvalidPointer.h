//
// File:      InvalidPointer.h
// Package    exception
//
// Invalid pointer error exception.
//
#if !defined(exception_InvalidPointer_h)
#define exception_InvalidPointer_h

#include "MemoryError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief InvalidPointer encapsulates a length error condition.
     */
    class InvalidPointer : public MemoryError {

    public:
      /*!
       * @brief Construct InvalidPointer object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit InvalidPointer( const std::string & what_arg ) throw();

      /*!
       * @brief Destructor.
       */
      virtual ~InvalidPointer() throw();

    };

  }
}

#endif // exception_InvalidPointer_h
