//
// File:      DPDValue.cc
// Package    base
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "DPDValue.h"

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::DPDValue)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    DPDValue::DPDValue()
    {

      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    DPDValue::DPDValue(double temperature,
                       double pressure) :
      d_temperature(temperature),
      d_pressure(pressure)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    DPDValue::~DPDValue()
    {

      //
      //
      //
      return;

    }

    //
    // Get temperature.
    //
    double
    DPDValue::getTemperature() const
    {

      //
      //
      //
      return d_temperature;
      
    }

    //
    // Get pressure.
    //
    double
    DPDValue::getPressure() const
    {

      //
      //
      //
      return d_pressure;
      
    }

  }
}
