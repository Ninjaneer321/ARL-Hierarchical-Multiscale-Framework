//
// File:      XYAdaptiveSamplingInputFilter.cc
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "XYAdaptiveSamplingInputFilter.h"

#include "XYArgument.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    XYAdaptiveSamplingInputFilter::XYAdaptiveSamplingInputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    XYAdaptiveSamplingInputFilter::~XYAdaptiveSamplingInputFilter()
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
    XYAdaptiveSamplingInputFilter::apply(const ArgumentPointer & argument) const
    {

      //
      // downcast to XYArgument
      //
      const XYArgumentPointer xyArgument = 
        boost::dynamic_pointer_cast<XYArgument>(argument);

      //
      // instantiate return value
      //
      std::vector<double> returnValue(2);
      
      //
      // fill return value
      //
      returnValue[0] = xyArgument->getX();
      returnValue[1] = xyArgument->getY();
      
      //
      //
      //
      return returnValue;
      
    }

    //
    // Apply filter inverse to vector of doubles, returning Argument
    // representation.
    ArgumentPointer
    XYAdaptiveSamplingInputFilter::applyInverse(const std::vector<double> & value,
						const ArgumentPointer &) const
    {
      
      //
      // firewall
      //
      assert(value.size() == 2);
      
      //
      // instantiate XYArgument
      //
      XYArgumentPointer argument(new XYArgument(value[0],
                                                value[1]));

      //
      //
      //
      return argument;

    }
    
    //
    // Get dimension of filtered argument.
    //
    int
    XYAdaptiveSamplingInputFilter::getDimension() const
    {
      
      //
      //
      //
      return 2;
      
    }
     
  }
}
