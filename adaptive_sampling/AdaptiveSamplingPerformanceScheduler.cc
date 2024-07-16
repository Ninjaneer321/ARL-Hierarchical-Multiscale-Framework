//
// File:      AdaptiveSamplingPerformanceScheduler.cc
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "AdaptiveSamplingPerformanceScheduler.h"

#include <exception/IOError.h>

#if defined(HAVE_CMATH)
#include <cmath>
#else
#error cmath header file not available
#endif // HAVE_CMATH

#if defined(HAVE_NUMERIC)
#include <numeric>
#else
#error numeric header file not available
#endif // HAVE_NUMERIC

//
//
//

namespace arl {
  namespace hms {

    namespace {

      //
      // compute l2 norm of argument
      //
      double
      norm(const std::vector<double> & argumentValue)
      {

        //
        // compute norm of argument
        //
        double norm = 0.0;
        for(std::vector<double>::size_type i=0; i<argumentValue.size(); ++i) {
          norm += argumentValue[i] * argumentValue[i];
        }
        norm = std::sqrt(norm);

        //
        //
        //
        return norm;

      }

      //
      // compute bin corresponding to an argument
      //
      int
      computeBin(double norm,
                 double d_minArgument,
                 double d_binDelta,
                 int    maxBin)
      {

        //
        // find bin to put argument in
        //
        int bin = (norm - d_minArgument) / d_binDelta + 1;
        if(bin < 0) {
          bin = 0;
        }
        else if(bin >= maxBin) {
          bin = maxBin - 1;
        }
        
        //
        //
        //
        return bin;

      }

    }

    //
    // Constructor.
    //
    AdaptiveSamplingPerformanceScheduler::AdaptiveSamplingPerformanceScheduler(int numberBrokers,
                                                                               int numberBins,
                                                                               int maxPointsPerBin,
                                                                               double minArgument,
                                                                               double maxArgument,
                                                                               const InterpolationDatabaseInputFilterPointer & inputFilter) :
      d_accumulatedTime(numberBrokers, 0.0),
      d_arguments(numberBins + 2),
      d_binDelta((maxArgument - minArgument) / numberBins), 
      d_inputFilter(inputFilter),
      d_maxArgument(maxArgument),
      d_maxPointsPerBin(maxPointsPerBin),
      d_minArgument(minArgument),
      d_numberBrokers(numberBrokers),
      d_roundRobinIndex(0),
      d_times(numberBins + 2),
      d_timeFit(numberBins + 2, 0.0)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    AdaptiveSamplingPerformanceScheduler::~AdaptiveSamplingPerformanceScheduler()
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
    AdaptiveSamplingPerformanceScheduler::schedule(const std::vector<ModelPackagePointer> & modelPackages)
    {

      //
      // instantiate returnValue
      //
      std::vector<AdaptiveSamplingScheduler::BrokerId> returnValue;
      
      //
      // iterate over model packaged pulling execution time
      //
      for(std::vector<ModelPackagePointer>::const_iterator iter = 
            modelPackages.begin(); iter != modelPackages.end(); ++iter) {
        
        const ModelPackagePointer & modelPackage = *iter;
        
        //
        // get argument data
        //
        const ArgumentPointer & argument = modelPackage->getArgument();
        const std::vector<double> argumentValue = 
          d_inputFilter->apply(argument);
        
        //
        // compute norm of argument
        //
        const double normValue = norm(argumentValue);
        
        //
        // get bin corresponding to argument
        //
        const int bin = computeBin(normValue,
                                   d_minArgument,
                                   d_binDelta,
                                   d_arguments.size());
                
        //
        // compute prediction
        //
        const double predictedTime = d_timeFit[bin];

        //
        // no predictions available, then do round robin
        //
        if(predictedTime == 0.0) {

          //std::cerr << "No Predictions: " << d_roundRobinIndex << std::endl;
          returnValue.push_back(d_roundRobinIndex++);
          if(d_roundRobinIndex >= d_numberBrokers) {
            d_roundRobinIndex = 0;
          }

        }
        else {

          //
          // find broker with minimum accumulated predicted time (has
          // the least work) and assign model package to that
          // broker.
          //
          std::vector<double>::iterator minIter =
            std::min_element(d_accumulatedTime.begin(),
                             d_accumulatedTime.end());        
          //std::cerr << "Predicted: " << predictedTime << " Assigned: " 
          //          << *minIter << " " << minIter - d_accumulatedTime.begin() 
          //          << std::endl;
          returnValue.push_back(minIter - d_accumulatedTime.begin());
          *(minIter) += predictedTime;

        }

      }

      //
      //
      //
      return returnValue;

    }
    
    //
    // Update scheduler with information from completed model
    // packages. In the case where the scheduler updates its
    // scheduling decisions based on the execution time of model
    // packages, it can update its scheduling model in this function.
    //
    void
    AdaptiveSamplingPerformanceScheduler::update(const std::vector<ModelPackagePointer> & modelPackages)
    {

      //
      // iterate over model packaged pulling execution time
      //
      for(std::vector<ModelPackagePointer>::const_iterator iter = 
            modelPackages.begin(); iter != modelPackages.end(); ++iter) {

        const ModelPackagePointer & modelPackage = *iter;
        
        //
        // get argument data
        //
        const ArgumentPointer & argument = modelPackage->getArgument();
        const std::vector<double> argumentValue = 
          d_inputFilter->apply(argument);
        const double wallClockTime = modelPackage->getWallClockTime();

        //
        // compute norm of argument
        //
        const double normValue = norm(argumentValue);

        //
        // get bin to put argument
        //
        const int bin = computeBin(normValue,
                                   d_minArgument,
                                   d_binDelta,
                                   d_arguments.size());
                                   
        //
        // update local data
        //
        if(d_arguments[bin].size() < d_maxPointsPerBin) {
          d_arguments[bin].push_back(normValue);
          d_times[bin].push_back(wallClockTime);

          //
          // refit time: todo: make this not happen every time a point
          // is added / allow for different regressions
          // 
          d_timeFit[bin] = std::accumulate(d_times[bin].begin(),
                                           d_times[bin].end(),
                                           0.0) / d_times[bin].size();
          
        }

      }

      //
      //
      //
      return;

    }
         
  }
}
