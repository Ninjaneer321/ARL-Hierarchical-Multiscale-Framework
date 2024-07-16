//
// File:      NullPointer.h
// Package    exception
//
// Null pointer error exception.
//
#if !defined(exception_NullPointer_h)
#define exception_NullPointer_h

#include "MemoryError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief NullPointer encapsulates a length error condition.
     */
    class NullPointer : public MemoryError {

    public:
      /*!
       * @brief Construct NullPointer object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit NullPointer( const std::string & what_arg ) throw();

      /*!
       * @brief Destructor.
       */
      virtual ~NullPointer() throw();

    };

  }
}

#endif // exception_NullPointer_h
