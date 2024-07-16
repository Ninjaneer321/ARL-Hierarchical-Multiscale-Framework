//
// Basic Monitor Test.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PerfInputFilter.h"
#include "PerfValue.h"
#include "PerfArgument.h"

#include <exception/IOError.h>

#if defined(HAVE_FSTREAM)
#include <fstream>
#else
#error fstream header file not available
#endif // HAVE_FSTREAM

#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iostream header file not available
#endif // HAVE_IOSTREAM

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::PerfInputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    PerfInputFilter::PerfInputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    PerfInputFilter::~PerfInputFilter()
    {

      //
      //
      //
      return;
    }

    //
    // Apply PerfInputFilter
    //
    void
    PerfInputFilter::apply(const ArgumentPointer & argument,
                           const std::string     & directory) const
    {

      //
      //
      //
      return;

    }

  }
}
