//
// Basic Monitor Test
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PiValue.h"

//
//
//

namespace arl {
  namespace hms {

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
