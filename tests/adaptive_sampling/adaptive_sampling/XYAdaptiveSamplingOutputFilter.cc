//
// File:      XYAdaptiveSamplingOutputFilter.cc
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "XYAdaptiveSamplingOutputFilter.h"

#include "XYValue.h"

HMS_SERIALIZATION_EXPORT(arl::hms::XYAdaptiveSamplingOutputFilter)

//
//
//

namespace arl {
  namespace hms {
    
    //
    // Constructor.
    //
    XYAdaptiveSamplingOutputFilter::XYAdaptiveSamplingOutputFilter()
    {
      
      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    XYAdaptiveSamplingOutputFilter::~XYAdaptiveSamplingOutputFilter()
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
    XYAdaptiveSamplingOutputFilter::apply(const std::vector<double> & value,
					  const ArgumentPointer &) const
    {

      //
      // firewalls
      //
      assert(value.size() == 1);
      
      //
      // instantiate XYValue
      //
      XYValuePointer returnValue(new XYValue(value[0]));

      //
      //
      //
      return returnValue;

    }

    //
    // Apply filter inverse to Value, returning vector representation.
    //
    std::vector<double>
    XYAdaptiveSamplingOutputFilter::applyInverse(const ValuePointer & value) const
    {

      //
      // downcast to XYValue
      //
      const XYValuePointer xyValue = 
        boost::dynamic_pointer_cast<XYValue>(value);

      //
      // instantiate return value
      //
      const std::vector<double> returnValue(1,
                                            xyValue->getValue());
      
      //
      //
      //
      return returnValue;

    }    
     
    //
    // Get dimension of filtered value.
    //
    int
    XYAdaptiveSamplingOutputFilter::getDimension() const
    {

      //
      //
      //
      return 1;

    }

  }
}
