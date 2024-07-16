//
// File:      AdaptiveSampling.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_AdaptiveSampling_h)
#define adaptive_sampling_AdaptiveSampling_h

#include "AdaptiveSamplingScheduler.h"
#include "InterpolationDatabase.h"
#include "SpeculativeGenerator.h"

#include <warehouse/ModelPackageWarehouse.h>
#include <utils/HMSMacros.h>

#include <deque>
#include <pthread.h>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Implements Adaptive Sampling.
     */
    class AdaptiveSampling {

    public:
      //
      // forward declarations
      // 
      struct AlgorithmWarehouseReceiveThreadData;
      struct AdaptiveSamplingThreadData;
      struct EvaluationThreadData;
      struct ValueProcessingThreadData;
  
      //
      // methods
      //
    public:

      /**
       * Constructor.
       *
       * @param interpolationDatabase Interpolation database.  
       * when interpolating without derivatives
       * @param scheduler Scheduler of model packages on brokers.
       * @param algorithmWarehouse Warehouse for receiving and sending
       * model packages from algorithm.
       * @param brokerCommunicators Communicators to brokers used to
       * evaluate model packages.
       * @param speculativeGenerator Generator of speculative evaluations.
       * @param numberInterpolationThreads Number of threads used for
       * interpolation.
       * @param evaluationLimit Maximum number of evaluations that
       * broker can hold
       */
      AdaptiveSampling(const InterpolationDatabasePointer & interpolationDatabase,
                       const AdaptiveSamplingSchedulerPointer & scheduler,
                       const ModelPackageWarehousePointer & algorithmWarehouse,
                       const std::vector<CommunicatorPointer> & brokerCommunicators,
		       const SpeculativeGeneratorPointer & speculativeGenerator,
                       int numberInterpolationThreads,
		       int evaluationLimit);

      /**
       * Destructor.
       */
      ~AdaptiveSampling();

    private:
      //
      // copy constructor/assignment operator
      //
      AdaptiveSampling(const AdaptiveSampling &); // not implemented
      AdaptiveSampling & operator=(const AdaptiveSampling &); // not implemented

      //
      // data
      //
    private:

      pthread_mutex_t                  d_algorithmReceiveLock;
      std::deque<ModelPackagePointer>  d_algorithmReceivePile;    
      ModelPackageWarehousePointer     d_algorithmWarehouse;
      std::vector<CommunicatorPointer> d_brokerCommunicators;
      const int                        d_evaluationLimit;
      std::deque<ModelPackagePointer>  d_evaluationPile;
      pthread_mutex_t                  d_evaluationPileLock;
      InterpolationDatabasePointer     d_interpolationDatabase;
      bool                             d_isConnected;
      int                              d_numberEvaluations;
      pthread_mutex_t                  d_numberEvaluationsLock;
      int                              d_numberSpeculativeEvaluations;
      pthread_mutex_t                  d_numberSpeculativeEvaluationsLock;
      AdaptiveSamplingSchedulerPointer d_scheduler;
      const int                        d_speculativeEvaluationLimit;
      SpeculativeGeneratorPointer      d_speculativeGenerator;

      std::vector<pthread_t>                d_threadIds;      
      AlgorithmWarehouseReceiveThreadData * d_algorithmWarehouseReceiveThreadData;
      std::vector<AdaptiveSamplingThreadData *> d_adaptiveSamplingThreadData;
      EvaluationThreadData                * d_evaluationThreadData;
      ValueProcessingThreadData           * d_valueProcessingThreadData;

      // Data for tracking statistics
      int              d_numberAdditionalPoints;
      int              d_numberCompletedSpeculativeEvaluations;
      int              d_numberEvaluationRequests;
      int              d_numberInsertedSpeculativeEvaluations;
      std::vector<int> d_numberSuccessfulInterpolations;

    };

    HMS_SHARED_PTR(AdaptiveSampling);

  }
}

#endif // adaptive_sampling_AdaptiveSampling_h
