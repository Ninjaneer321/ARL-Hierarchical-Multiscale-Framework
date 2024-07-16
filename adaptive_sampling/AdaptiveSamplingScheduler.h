//
// File:      AdaptiveSamplingScheduler.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_AdaptiveSamplingScheduler_h)
#define adaptive_sampling_AdaptiveSamplingScheduler_h

#include <base/ModelPackage.h>

#include <utils/HMSMacros.h>

#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Schedules model packages for evaluation on a set of
     * Brokers.
     */
    class AdaptiveSamplingScheduler {

      //
      // types
      //
    public:
      
      typedef int BrokerId;

      //
      // methods
      //
    public:

      /**
       * Destructor.
       */
      virtual ~AdaptiveSamplingScheduler() = 0;

      /**
       * Obtain BrokerIds to evaluate model packages on.
       *
       * @param modelPackages ModelPackages to schedule.
       *
       * @return BrokerIds to evaluate model package on.
       */
      virtual std::vector<BrokerId>
      schedule(const std::vector<ModelPackagePointer> & modelPackages) = 0;

      /**
       * Update scheduler with information from completed model
       * packages. In the case where the scheduler updates its
       * scheduling decisions based on the execution time of model
       * packages, it can update its scheduling model in this
       * function.
       *
       * @param modelPackages Completed ModelPackages.
       */
      virtual void
      update(const std::vector<ModelPackagePointer> & modelPackages);
      
    protected:

      /**
       * Constructor.
       */
      AdaptiveSamplingScheduler();
    
    private:
      //
      // copy constructor/assignment operator
      //
      AdaptiveSamplingScheduler(const AdaptiveSamplingScheduler &); // not implemented
      AdaptiveSamplingScheduler & operator=(const AdaptiveSamplingScheduler &); // not implemented

      //
      // data
      //
    private:

    };

    HMS_SHARED_PTR(AdaptiveSamplingScheduler);

  }
}

#endif // adaptive_sampling_AdaptiveSamplingScheduler_h
