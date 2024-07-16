//
// File:      AdaptiveSamplingLoadScheduler.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_AdaptiveSamplingLoadScheduler_h)
#define adaptive_sampling_AdaptiveSamplingLoadScheduler_h

#include "AdaptiveSamplingScheduler.h"

#include <comm/Communicator.h>

#include <ctime>
#include <fstream>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Load-based scheduler of model packages for evaluation on
     * brokers.
     */
    class AdaptiveSamplingLoadScheduler : 
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
       */
      AdaptiveSamplingLoadScheduler(const std::vector<CommunicatorPointer> & brokers);

      /**
       * Constructor.
       *
       * @param loadLogPath Path to log load to disk.
       * @param timeBetweenLogs Time between logging load to disk.
       */
      AdaptiveSamplingLoadScheduler(const std::vector<CommunicatorPointer> & brokers,
                                    const std::string & loadLogPath,
                                    int timeBetweenLogs);
   
      /**
       * Destructor.
       */
      virtual ~AdaptiveSamplingLoadScheduler();

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
      AdaptiveSamplingLoadScheduler(const AdaptiveSamplingLoadScheduler &); // not implemented
      AdaptiveSamplingLoadScheduler & operator=(const AdaptiveSamplingLoadScheduler &); // not implemented

      //
      // data
      //
    private:

      std::vector<CommunicatorPointer> d_brokers; 
      time_t                           d_initialTime;
      std::ofstream                    d_loadLog;
      std::string                      d_loadLogPath;
      time_t                           d_lastLogTime;
      int                              d_timeBetweenLogs;

    };

  }
}

#endif // adaptive_sampling_AdaptiveSamplingLoadScheduler_h
