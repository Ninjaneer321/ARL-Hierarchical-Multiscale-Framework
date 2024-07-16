//
// File:      AdaptiveSamplingPerformanceScheduler.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_AdaptiveSamplingPerformanceScheduler_h)
#define adaptive_sampling_AdaptiveSamplingPerformanceScheduler_h

#include "AdaptiveSamplingScheduler.h"
#include "InterpolationDatabaseInputFilter.h"
#include <comm/Communicator.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Performance-based scheduler of model packages for evaluation on
     * brokers.
     */
    class AdaptiveSamplingPerformanceScheduler : 
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
       * @param numberBrokers Number of brokers to schedule model
       * packages on.
       * @param numberBins Number of bins used for time prediction
       * @param maxPointsPerBin Maximum number of points in a bin
       * before fitting is stopped
       * @param minArgument Value of the minimum expected argument for
       * the simulation.
       * @param maxValue Value of the maximum expected argument for
       * the simulation.
       * @param inputFilter Interpolation database input filter
       * converting arguments to double.
       */
      AdaptiveSamplingPerformanceScheduler(int numberBrokers,
                                           int numberBins,
                                           int maxPointsPerBin,
                                           double minArgument,
                                           double maxArgument,
                                           const InterpolationDatabaseInputFilterPointer & inputFilter);
        
      /**
       * Destructor.
       */
      virtual ~AdaptiveSamplingPerformanceScheduler();
        
      /**
       * Obtain BrokerIds to evaluate model packages on.
       *
       * @param modelPackages ModelPackages to schedule.
       *
       * @return BrokerIds to evaluate model package on.
       */
      virtual std::vector<BrokerId>
      schedule(const std::vector<ModelPackagePointer> & modelPackages);
        
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

    private:
      //
      // copy constructor/assignment operator
      //
      AdaptiveSamplingPerformanceScheduler(const AdaptiveSamplingPerformanceScheduler &); // not implemented
      AdaptiveSamplingPerformanceScheduler & operator=(const AdaptiveSamplingPerformanceScheduler &); // not implemented

      //
      // data
      //
    private:

      std::vector<double>                     d_accumulatedTime;
      std::vector<std::vector<double> >       d_arguments;
      const double                            d_binDelta;
      InterpolationDatabaseInputFilterPointer d_inputFilter;
      const double                            d_maxArgument;
      const std::vector<double>::size_type    d_maxPointsPerBin;
      const double                            d_minArgument;
      const int                               d_numberBrokers;
      int                                     d_roundRobinIndex;
      std::vector<std::vector<double> >       d_times;
      std::vector<double>                     d_timeFit;

    };

  }
}

#endif // adaptive_sampling_AdaptiveSamplingPerformanceScheduler_h
