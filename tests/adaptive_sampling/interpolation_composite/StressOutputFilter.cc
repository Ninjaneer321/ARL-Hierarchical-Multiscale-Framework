//
// File:      StressOutputFilter.cc
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "StressOutputFilter.h"

#include "StressValue.h"

HMS_SERIALIZATION_EXPORT(arl::hms::StressOutputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    StressOutputFilter::StressOutputFilter() :
      d_scale(1.0)
    {
      
      //
      //
      //
      return;

    }
    
    //
    // Constructor.
    //
    StressOutputFilter::StressOutputFilter(double /* youngsModulus */) :
      //d_scale(.1 * youngsModulus)
      d_scale(1.0)
    {
      
      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    StressOutputFilter::~StressOutputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Apply filter to vector of doubles returning Value.
    // 
    ValuePointer
    StressOutputFilter::apply(const std::vector<double> & value,
			      const ArgumentPointer &) const
    {

      //
      // firewalls
      //
      assert(value.size() == 6);
      
      //
      // scale stress
      //
      std::vector<double> scaledStress(value);
      for(std::vector<double>::size_type i=0; i<value.size(); ++i) {
        scaledStress[i] /= d_scale;
      }

      //
      // instantiate StressValue
      //
      StressValuePointer returnValue(new StressValue(scaledStress));

      //
      //
      //
      return returnValue;

    }

    //
    // Apply filter inverse to Value, returning vector representation.
    //
    std::vector<double>
    StressOutputFilter::applyInverse(const ValuePointer & value) const
    {

      //
      // downcast to StressValue
      //
      const StressValuePointer stressValue = 
        boost::dynamic_pointer_cast<StressValue>(value);

      //
      // get stress
      //
      const std::vector<double> & stress = stressValue->getStress();

      //
      // scale stress
      //
      std::vector<double> scaledStress(stress);
      for(std::vector<double>::size_type i=0; i<stress.size(); ++i) {
        scaledStress[i] *= d_scale;
      }

      //
      //
      //
      return scaledStress;

    }    
     
    //
    // Get dimension of filtered value.
    //
    int
    StressOutputFilter::getDimension() const
    {

      //
      //
      //
      return 6;

    }

  }
}
