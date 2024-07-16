//
// File:      DPDInputFilter.cc
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "DPDInputFilter.h"

#include "DPDArgument.h"

HMS_SERIALIZATION_EXPORT(arl::hms::DPDInputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    DPDInputFilter::DPDInputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    DPDInputFilter::~DPDInputFilter()
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
    DPDInputFilter::apply(const ArgumentPointer & argument) const
    {

      //
      // downcast to DPDArgument
      //
      const DPDArgumentPointer dpdArgument = 
        boost::dynamic_pointer_cast<DPDArgument>(argument);

      //
      // get densities
      // 
      const double energyDensity = dpdArgument->getEnergyDensity();
      const double massDensity = dpdArgument->getMassDensity();

      //
      // scale densities
      //
      std::vector<double> scaledDPD(2);
      scaledDPD[0] = energyDensity / 2e-3;
      scaledDPD[1] = (massDensity - 1.9) / 2e-1;

      //
      //
      //
      return scaledDPD;
      
    }

    //
    // Apply filter inverse to vector of doubles, returning Argument
    // representation.
    //
    ArgumentPointer
    DPDInputFilter::applyInverse(const std::vector<double> & value,
				 const ArgumentPointer &) const
    {

      //
      // firewall
      //
      assert(value.size() == 2);
      
      //
      // instantiate return value
      //
      arl::hms::ArgumentPointer 
        returnValue(new arl::hms::DPDArgument(value[0] * 2e-3,
                                              value[1] * 2e-1 + 1.9));

      //
      //
      //
      return returnValue;

    }
    
    //
    // Get dimension of filtered argument.
    //
    int
    DPDInputFilter::getDimension() const
    {
      
      //
      //
      //
      return 2;
      
    }
     
  }
}
