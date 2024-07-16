//
// File:      SleepInputFilter.cc
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "SleepInputFilter.h"

#include "SleepArgument.h"

HMS_SERIALIZATION_EXPORT(arl::hms::SleepInputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    SleepInputFilter::SleepInputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    SleepInputFilter::~SleepInputFilter()
    {
      
      //
      //
      //
      return;

    }

    //
    // Apply filter to Argument returning vector of doubles.
    //
    std::vector<double>
    SleepInputFilter::apply(const ArgumentPointer & argument) const
    {

      //
      // downcast to SleepArgument
      //
      const SleepArgumentPointer sleepArgument = 
        boost::dynamic_pointer_cast<SleepArgument>(argument);

      //
      // get sleep time
      // 
      const double sleepTime = sleepArgument->getSleepTime();
      
      //
      //
      //
      return std::vector<double>(1, sleepTime);
      
    }

    //
    // Apply filter inverse to vector of doubles, returning Argument
    // representation.
    //
    ArgumentPointer
    SleepInputFilter::applyInverse(const std::vector<double> & value,
				   const ArgumentPointer & /* argument */) const
    {

      //
      // firewall
      //
      assert(value.size() == 1);
      
      //
      // instantiate return value
      //
      arl::hms::ArgumentPointer 
        returnValue(new arl::hms::SleepArgument(value[0]));

      //
      //
      //
      return returnValue;

    }
    
    //
    // Get dimension of filtered argument.
    //
    int
    SleepInputFilter::getDimension() const
    {
      
      //
      //
      //
      return 1;
      
    }
     
  }
}
