//
// File:      AdaptiveSamplingRoundRobinScheduler.cc
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "AdaptiveSamplingRoundRobinScheduler.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    AdaptiveSamplingRoundRobinScheduler::AdaptiveSamplingRoundRobinScheduler(int numberBrokers) :
      d_currentBrokerId(0),
      d_numberBrokers(numberBrokers)
    {

      //
      //
      //
      return;

    }
    
    //
    // Destructor.
    //
    AdaptiveSamplingRoundRobinScheduler::~AdaptiveSamplingRoundRobinScheduler()
    {

      //
      //
      //
      return;

    }
    
    //
    // Obtain BrokerIds to evaluate model packages on.
    //
    std::vector<AdaptiveSamplingScheduler::BrokerId>
    AdaptiveSamplingRoundRobinScheduler::schedule(const std::vector<ModelPackagePointer> & modelPackages)
    {

      //
      // instantiate returnValue
      //
      std::vector<BrokerId> returnValue;
      returnValue.reserve(modelPackages.size());

      //
      // iterate over all model packages scheduling execution
      //
      for(std::vector<ModelPackagePointer>::size_type i=0; 
          i<modelPackages.size(); ++i) {

        //
        // add to returnValue
        //
        returnValue.push_back(d_currentBrokerId);
        
        //
        // increment current broker id
        //
        ++d_currentBrokerId;

        //
        // check if we have wrapped around
        //
        if(d_currentBrokerId == d_numberBrokers) {
          d_currentBrokerId = 0;
        }
        
      }

      //
      //
      //
      return returnValue;

    }
      
  }
}
