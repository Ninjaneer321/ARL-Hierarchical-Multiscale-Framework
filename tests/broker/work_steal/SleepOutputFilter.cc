//
// Basic Monitor Test.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "SleepOutputFilter.h"

#include <exception/IOError.h>

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::SleepOutputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    SleepOutputFilter::SleepOutputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    SleepOutputFilter::~SleepOutputFilter()
    {

      //
      //
      //
      return;
    }

    //
    // Apply SleepOutputFilter
    //
    ValuePointer 
    SleepOutputFilter::apply(const std::string & /* directory */, 
			     const std::string & /* stdOut */,
			     const ArgumentPointer & /* argument */) const
    {
   
      //
      // instantiate return value
      //
      const arl::hms::ValuePointer value;

      //
      //
      //
      return value;

    }

  }
}
