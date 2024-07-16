//
// Basic Monitor Test.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PerfOutputFilter.h"
#include "PerfValue.h"

#include <exception/IOError.h>

#if defined(HAVE_FSTREAM)
#include <fstream>
#else
#error fstream header file not available
#endif // HAVE_FSTREAM

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::PerfOutputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    PerfOutputFilter::PerfOutputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    PerfOutputFilter::~PerfOutputFilter()
    {

      //
      //
      //
      return;
    }

    //
    // Apply PerfOutputFilter
    //
    ValuePointer 
    PerfOutputFilter::apply(const std::string & /* directory */, 
                            const std::string & /* stdOut */,
			    const ArgumentPointer & /* argument */) const
    {

      //
      // instantiate return value
      //
      const arl::hms::ValuePointer value(new arl::hms::PerfValue(0.0));

      //
      //
      //
      return value;

    }

  }
}
