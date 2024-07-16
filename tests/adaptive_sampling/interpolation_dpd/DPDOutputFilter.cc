//
// File:      DPDOutputFilter.cc
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "DPDOutputFilter.h"

#include "DPDValue.h"

HMS_SERIALIZATION_EXPORT(arl::hms::DPDOutputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    DPDOutputFilter::DPDOutputFilter()
    {
      
      //
      //
      //
      return;

    }
    
    //
    // Destructor.
    //
    DPDOutputFilter::~DPDOutputFilter()
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
    DPDOutputFilter::apply(const std::vector<double> & value,
			   const ArgumentPointer &) const
    {

      //
      // firewalls
      //
      assert(value.size() == 2);
      
      //
      // instantiate DPDValue
      //
      DPDValuePointer returnValue(new DPDValue((value[0] * 50.0) + 325,
                                               value[1] * 2e-2));

      //
      //
      //
      return returnValue;

    }

    //
    // Apply filter inverse to Value, returning vector representation.
    //
    std::vector<double>
    DPDOutputFilter::applyInverse(const ValuePointer & value) const
    {

      //
      // downcast to DPDValue
      //
      const DPDValuePointer dpdValue = 
        boost::dynamic_pointer_cast<DPDValue>(value);

      //
      // get temperature and pressure
      //
      const double temperature = dpdValue->getTemperature();
      const double pressure = dpdValue->getPressure();

      //
      // scale stress
      //
      std::vector<double> scaledDPD(2);
      scaledDPD[0] = (temperature - 325) / 50.0;
      scaledDPD[1] = pressure / 2e-2;

      //
      //
      //
      return scaledDPD;

    }    
     
    //
    // Get dimension of filtered value.
    //
    int
    DPDOutputFilter::getDimension() const
    {

      //
      //
      //
      return 2;

    }

  }
}
