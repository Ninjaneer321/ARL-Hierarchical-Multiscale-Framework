//
// Basic Monitor Test.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PicalcInputFilter.h"
#include "PiValue.h"
#include "PiArgument.h"

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

HMS_SERIALIZATION_EXPORT(arl::hms::PicalcInputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    PicalcInputFilter::PicalcInputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    PicalcInputFilter::~PicalcInputFilter()
    {

      //
      //
      //
      return;
    }

    //
    // Apply PicalcInputFilter
    //
    void
    PicalcInputFilter::apply(const ArgumentPointer & argument,
                             const std::string     & directory) const
    {

      //
      //
      //
      return;

    }

  }
}
