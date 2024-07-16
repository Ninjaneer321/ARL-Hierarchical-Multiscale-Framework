//
// File:      StressValue.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_StressValue_h)
#define base_StressValue_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/Value.h>
#include <utils/HMSMacros.h>

#if defined(HAVE_VECTOR)
#include <vector>
#else
#error vector header file not available
#endif // HAVE_VECTOR

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing scalar value.
     */
    class StressValue : public Value {

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
      StressValue();

      /**
       * Constructor.
       *
       * @param stress Stress to store
       */
      StressValue(const std::vector<double> & stress);

      /**
       * Destructor.
       */
      virtual ~StressValue();

      /**
       * Get stress.
       */
      const std::vector<double> & getStress() const;
  
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
      StressValue(const StressValue &); // not implemented
      StressValue & operator=(const StressValue &); // not implemented

      //
      // data
      //
    private:

      std::vector<double> d_stress;

    };

    HMS_SHARED_PTR(StressValue);

  }
}

#include "StressValue_t.cc"

#endif // base_StressValue_h
