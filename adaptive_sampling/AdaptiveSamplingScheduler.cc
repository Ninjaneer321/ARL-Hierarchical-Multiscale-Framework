//
// File:      AdaptiveSamplingScheduler.cc
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "AdaptiveSamplingScheduler.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    AdaptiveSamplingScheduler::AdaptiveSamplingScheduler()
    {
      
      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    AdaptiveSamplingScheduler::~AdaptiveSamplingScheduler()
    {

      //
      //
      //
      return;

    }

    //
    // Update scheduler with information from completed model
    // packages. In the case where the scheduler updates its
    // scheduling decisions based on the execution time of model
    // packages, it can update its scheduling model in this function.
    //
    void
    AdaptiveSamplingScheduler::update(const std::vector<ModelPackagePointer> &)
    {

      //
      //
      //
      return;

    }
       
  }
}
