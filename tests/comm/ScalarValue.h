//
// File:      ScalarValue.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_ScalarValue_h)
#define base_ScalarValue_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/Value.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing scalar value.
     */
    class ScalarValue : public Value {

      //
      // types
      //
    public:

      //
      // methods
      //
    public:

      /**
       * Constructor.
       */
      ScalarValue();

      /**
       * Constructor.
       *
       * @param value Value to store
       */
      ScalarValue(double value);

      /**
       * Destructor.
       */
      virtual ~ScalarValue();

      /**
       * Get value.
       */
      double getValue() const;

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

    private:
      //
      // copy constructor/assignment operator
      //
      ScalarValue(const ScalarValue &); // not implemented
      ScalarValue & operator=(const ScalarValue &); // not implemented

      //
      // data
      //
    private:

      double d_value;

    };

  }
}

#include "ScalarValue_t.cc"

#endif // base_ScalarValue_h
