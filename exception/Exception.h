//
// File:      Exception.h
// Package    exception
//
// Base class for exceptions.
//
#if !defined(exception_Exception_h)
#define exception_Exception_h

#include <exception>
#include <string>

//
//
//
namespace arl {
  namespace hms {

    /*!
     * @brief Exception is a base class of the exception hierarchy.
     */
    class Exception : public std::exception {
    
    public:
      
      Exception() throw();

      /*!
       * @brief Construct Exception object given a message string.
       *
       * @param what_arg String containing the message associated with the
       *                 exception. In the multi-processor context a string
       *                 charaterizing task is prepended.
       */
      explicit Exception( const std::string & what_arg ) throw();
      
      /*!
       * @brief Destructor.
       */
      virtual ~Exception() throw();

      /*!
       * @brief Provide a message associated with the Exception object.
       *
       * @return Const pointer to a char array containing the message.
       */
      virtual const char * what() const throw();

      /**
       * Serialize Exception.
       *
       * @param archive Archive to store serialization.
       * @param version Version of serialization.
       */
      template <class Archive>
      void serialize(Archive & archive, const unsigned int version);

      //
      // data
      //
    private:
      std::string d_message;

    };

  }
}

#include "Exception_t.cc"

#endif // exception_Exception_h
