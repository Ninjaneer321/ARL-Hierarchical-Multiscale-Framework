//
// File:      AdaptiveSamplingRoundRobinScheduler.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_AdaptiveSamplingRoundRobinScheduler_h)
#define adaptive_sampling_AdaptiveSamplingRoundRobinScheduler_h

#include "AdaptiveSamplingScheduler.h"

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Round robin scheduler of model packages for evaluation
     * on brokers.
     */
    class AdaptiveSamplingRoundRobinScheduler : 
      public AdaptiveSamplingScheduler {

      //
      // types
      //
    public:
      
      //
      // methods
      //
    public:

      /**
       * Constructor.
       *
       * @param numberBrokers Number of brokers to schedule model packages on.
       */
      AdaptiveSamplingRoundRobinScheduler(int numberBrokers);
    
      /**
       * Destructor.
       */
      virtual ~AdaptiveSamplingRoundRobinScheduler();

      /**
       * Obtain BrokerIds to evaluate model packages on.
       *
       * @param modelPackages ModelPackages to schedule.
       *
       * @return BrokerIds to evaluate model package on.
       */
      virtual std::vector<BrokerId>
      schedule(const std::vector<ModelPackagePointer> & modelPackages);
      
    private:
      //
      // copy constructor/assignment operator
      //
      AdaptiveSamplingRoundRobinScheduler(const AdaptiveSamplingRoundRobinScheduler &); // not implemented
      AdaptiveSamplingRoundRobinScheduler & operator=(const AdaptiveSamplingRoundRobinScheduler &); // not implemented

      //
      // data
      //
    private:

      BrokerId d_currentBrokerId;
      const int d_numberBrokers;

    };

  }
}

#endif // adaptive_sampling_AdaptiveSamplingRoundRobinScheduler_h
