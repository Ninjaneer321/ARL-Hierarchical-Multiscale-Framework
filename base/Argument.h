//
// File:      Argument.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_Argument_h)
#define base_Argument_h

#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class for function argument.
     */
    class Argument {

      //
      // types
      //
    public:

      //
      // methods
      //
    public:

      /**
       * Destructor.
       */
      virtual ~Argument() = 0;

      /**
       * Get flag whether broker should cleanup the execution
       * directory after model evaluation.
       */
      bool getToCleanup() const;
      
      /**
       * Serialize argument.
       *
       * @param archive Archive to store serialization.
       * @param version Version of serialization.
       */
      template <class Archive>
      void serialize(Archive & archive, const unsigned int version);

      /**
       * Set flag whether broker should cleanup the execution
       * directory after model evaluation.
       */
      void setToCleanup(bool toCleanup);
      
      //
      // methods
      // 
    protected:

      /**
       * Constructor.
       */
      Argument();

    private:
      //
      // copy constructor/assignment operator
      //
      Argument(const Argument &); // not implemented
      Argument & operator=(const Argument &); // not implemented

      //
      // data
      //
    private:
      bool d_toCleanupFlag;

    };

    HMS_SHARED_PTR(Argument);

  }
}

#include "Argument_t.cc"

#endif // base_Argument_h
