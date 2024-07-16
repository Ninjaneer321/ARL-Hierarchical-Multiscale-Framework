//
// File:      StrainInputFilter.cc
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "StrainInputFilter.h"

#include "StrainArgument.h"

HMS_SERIALIZATION_EXPORT(arl::hms::StrainInputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    StrainInputFilter::StrainInputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    StrainInputFilter::~StrainInputFilter()
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
    StrainInputFilter::apply(const ArgumentPointer & argument) const
    {

      //
      // downcast to StrainArgument
      //
      const StrainArgumentPointer strainArgument = 
        boost::dynamic_pointer_cast<StrainArgument>(argument);

      //
      // get strain
      // 
      const std::vector<double> & strain = strainArgument->getStrain();
      
      //
      // scale strain
      //
      std::vector<double> scaledStrain(strain);
      for(std::vector<double>::size_type i=0; i<scaledStrain.size(); ++i) {
        scaledStrain[i] /= 1.0;//1.0e-4;
      }

      //
      //
      //
      return scaledStrain;
      
    }

    //
    // Apply filter inverse to vector of doubles, returning Argument
    // representation.
    //
    ArgumentPointer
    StrainInputFilter::applyInverse(const std::vector<double> & value,
				    const ArgumentPointer &) const
    {

      //
      // firewall
      //
      assert(value.size() == 6);
      
      //
      // scale value
      //
      std::vector<double> scaledStrain(value);
      for(std::vector<double>::size_type i=0; i<scaledStrain.size(); ++i) {
        scaledStrain[i] *= 1.0;//1.0e-4;
      }

      //
      // instantiate return value
      //
      arl::hms::ArgumentPointer 
        returnValue(new arl::hms::StrainArgument(scaledStrain));

      //
      //
      //
      return returnValue;

    }
    
    //
    // Get dimension of filtered argument.
    //
    int
    StrainInputFilter::getDimension() const
    {
      
      //
      //
      //
      return 6;
      
    }
     
  }
}
