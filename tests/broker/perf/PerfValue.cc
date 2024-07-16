//
// Basic Monitor Test
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PerfValue.h"

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::PerfValue)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    PerfValue::PerfValue()
    {

      //
      //
      // 
      return;

    }

    //
    // Constructor.
    //
    PerfValue::PerfValue(double value) :
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
    PerfValue::~PerfValue()
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
    PerfValue::getValue() const
    {

      //
      //
      //
      return d_value;
      
    }

  }
}
