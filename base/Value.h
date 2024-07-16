//
// File:      Value.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_Value_h)
#define base_Value_h

#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class for function value.
     */
    class Value {

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
      virtual ~Value() = 0;

      /**
       * Serialize value.
       *
       * @param archive Archive to store serialization.
       * @param version Version of serialization.
       */
      template <class Archive>
      void serialize(Archive & archive, const unsigned int version);
  
      //
      // methods
      // 
    protected:

      /**
       * Constructor.
       */
      Value();

    private:
      //
      // copy constructor/assignment operator
      //
      Value(const Value &); // not implemented
      Value & operator=(const Value &); // not implemented

      //
      // data
      //
    private:

    };

    HMS_SHARED_PTR(Value);

  }
}

#include "Value_t.cc"

#endif // base_Value_h
