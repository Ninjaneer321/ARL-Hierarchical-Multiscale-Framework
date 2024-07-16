//
// Basic Monitor Test
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PerfArgument.h"

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::PerfArgument)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    PerfArgument::PerfArgument()
    {

      //
      //
      // 
      return;

    }

    //
    // Constructor.
    //
    PerfArgument::PerfArgument(const std::vector<double> & argument) :
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
    PerfArgument::~PerfArgument()
    {

      //
      //
      //
      return;

    }

    //
    // Get argument.
    //
    const std::vector<double> &
    PerfArgument::getArgument() const
    {

      //
      //
      //
      return d_argument;
      
    }

  }
}
