//
// File:      DPDValue.h
// Package    base
//
#if !defined(tests_adaptive_sampling_interpolation_dpd_DPDValue_h)
#define tests_adaptive_sampling_interpolation_dpd_DPDValue_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/Value.h>
#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing scalar value.
     */
    class DPDValue : public Value {

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
      DPDValue();

      /**
       * Constructor.
       */
      DPDValue(double temperature,
               double pressure);

      /**
       * Destructor.
       */
      virtual ~DPDValue();

      /**
       * Get temperature.
       */
      double getTemperature() const;

      /**
       * Get pressure
       */
      double getPressure() const;
  
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
      DPDValue(const DPDValue &); // not implemented
      DPDValue & operator=(const DPDValue &); // not implemented

      //
      // data
      //
    private:

      double d_temperature;
      double d_pressure;

    };

    HMS_SHARED_PTR(DPDValue);

  }
}

#include "DPDValue_t.cc"

#endif // base_DPDValue_h
