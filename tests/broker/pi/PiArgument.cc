//
// Basic Monitor Test
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PiArgument.h"

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::PiArgument)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    PiArgument::PiArgument()
    {

      //
      //
      // 
      return;

    }

    //
    // Constructor.
    //
    PiArgument::PiArgument(double argument) :
      d_argument(argument)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    PiArgument::~PiArgument()
    {

      //
      //
      //
      return;

    }

    //
    // Get argument.
    //
    double 
    PiArgument::getArgument() const
    {

      //
      //
      //
      return d_argument;
      
    }

  }
}
