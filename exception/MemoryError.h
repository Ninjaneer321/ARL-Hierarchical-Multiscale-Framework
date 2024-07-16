//
// File:      MemoryError.h
// Package    exception
//
// Memory error exception.
//
#if !defined(exception_MemoryError_h)
#define exception_MemoryError_h

#include "RuntimeError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief MemoryError encapsulates a runtime error condition.
     */
    class MemoryError : public RuntimeError {

    public:
      /*!
       * @brief Destructor.
       */
      virtual ~MemoryError() throw() = 0;

    protected:
      /*!
       * @brief Construct MemoryError object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit MemoryError( const std::string & what_arg ) throw();

    };

  }
}

#endif // exception_MemoryError_h
