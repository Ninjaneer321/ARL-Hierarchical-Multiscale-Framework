//
// File:      OSError.h
// Package    exception
//
// Network error exception.
//
#if !defined(exception_OSError_h)
#define exception_OSError_h

#include "RuntimeError.h"

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief OSError encapsulates a runtime error condition.
     */
    class OSError : public RuntimeError {

    public:
      /*!
       * @brief Construct OSError object given a message string.
       *
       * @param what_arg String containing the message associated with 
       *                 the exception.
       */
      explicit OSError( const std::string & what_arg ) throw();

      /*!
       * @brief Destructor.
       */
      virtual ~OSError() throw();

    };
  }
}

#endif // exception_OSError_h
