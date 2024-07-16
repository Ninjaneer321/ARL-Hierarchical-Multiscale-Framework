//
// Basic Monitor Test
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PiValue.h"

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::PiValue)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    PiValue::PiValue()
    {

      //
      //
      // 
      return;

    }

    //
    // Constructor.
    //
    PiValue::PiValue(double value) :
      d_value(value)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    PiValue::~PiValue()
    {

      //
      //
      //
      return;

    }

    //
    // Get value.
    //
    double 
    PiValue::getValue() const
    {

      //
      //
      //
      return d_value;
      
    }

  }
}
