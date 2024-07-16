//
// File:      AdaptiveSampling.cc
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "AdaptiveSampling.h"
#include "NullGenerator.h"

#include <exception/InvalidArgument.h>
#include <exception/OSError.h>

#if defined(HAVE_ALGORITHM)
#include <algorithm>
#else
#error algorithm header file not available
#endif // HAVE_ALGORITHM

#if defined(HAVE_CERRNO)
#include <cerrno>
#else
#error cerrno header file not available
#endif // HAVE_CERRNO

#if defined(HAVE_CSTRING)
#include <cstring>
#else
#error cstring header file not available
#endif // HAVE_CSTRING

#if defined(HAVE_FSTREAM)
#include <fstream>
#else
#error fstream header file not available
#endif // HAVE_FSTREAM

#if defined(HAVE_MAP)
#include <map>
#else
#error map header file not available
#endif // HAVE_MAP

#if defined(HAVE_NUMERIC)
#include <numeric>
#else
#error numeric header file not available
#endif // HAVE_NUMERIC

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

//
//
//

namespace arl {
  namespace hms {

    //
    // thread data for algorithm warehouse receive end
    //
    struct AdaptiveSampling::AlgorithmWarehouseReceiveThreadData {
      pthread_mutex_t                 * d_algorithmReceiveLock;
      std::deque<ModelPackagePointer> * d_algorithmReceivePile; 
      ModelPackageWarehousePointer      d_algorithmWarehouse;
      InterpolationDatabasePointer      d_interpolationDatabase;
      bool                            * d_isConnected;

      // Data for tracking statistics
      int                             * d_numberAdditionalPoints;
      int                             * d_numberCompletedSpeculativeEvaluations;
      int                             * d_numberEvaluationRequests;
      int                             * d_numberInsertedSpeculativeEvaluations;
      std::vector<int>                * d_numberSuccessfulInterpolations;
    };

    //
    // thread data for adaptive sampling thread
    //
    struct AdaptiveSampling::AdaptiveSamplingThreadData {
      ModelPackageWarehousePointer      d_algorithmWarehouse;
      pthread_mutex_t                 * d_algorithmReceiveLock;
      std::deque<ModelPackagePointer> * d_algorithmReceivePile; 
      const int                       * d_evaluationLimit;
      std::deque<ModelPackagePointer> * d_evaluationPile;
      pthread_mutex_t                 * d_evaluationPileLock;
      InterpolationDatabasePointer      d_interpolationDatabase;
      bool                            * d_isConnected;
      int                             * d_numberEvaluations;
      pthread_mutex_t                 * d_numberEvaluationsLock;
      int                             * d_numberSpeculativeEvaluations;
      pthread_mutex_t                 * d_numberSpeculativeEvaluationsLock;
      const int                       * d_speculativeEvaluationLimit;
      SpeculativeGeneratorPointer       d_speculativeGenerator;

      // Data for tracking statistics
      int                             * d_numberSuccessfulInterpolations;
    };
    
    //
    // thread data for evaluation thread
    //
    struct AdaptiveSampling::EvaluationThreadData {
      AdaptiveSamplingSchedulerPointer   d_adaptiveSamplingScheduler;
      std::vector<CommunicatorPointer> * d_brokerCommunicators;
      std::deque<ModelPackagePointer>  * d_evaluationPile;
      pthread_mutex_t                  * d_evaluationPileLock;
      bool                             * d_isConnected;
    };

    //
    // thread data for processing evaluated model packages
    //
    struct AdaptiveSampling::ValueProcessingThreadData {
      AdaptiveSamplingSchedulerPointer   d_adaptiveSamplingScheduler;
      ModelPackageWarehousePointer       d_algorithmWarehouse;
      std::vector<CommunicatorPointer> * d_brokerCommunicators;
      std::deque<ModelPackagePointer>  * d_evaluationPile;
      pthread_mutex_t                  * d_evaluationPileLock;
      InterpolationDatabasePointer       d_interpolationDatabase;
      bool                             * d_isConnected;
      int                              * d_numberEvaluations;
      pthread_mutex_t                  * d_numberEvaluationsLock;
      int                              * d_numberSpeculativeEvaluations;
      pthread_mutex_t                  * d_numberSpeculativeEvaluationsLock;
      SpeculativeGeneratorPointer        d_speculativeGenerator;

      // Data for tracking statistics
      int                              * d_numberAdditionalPoints;
      int                              * d_numberCompletedSpeculativeEvaluations;
      int                              * d_numberInsertedSpeculativeEvaluations;
    };
      
    //
    // local datatypes, data, and functions
    //

    namespace {

      const std::deque<ModelPackagePointer>::size_type 
      adaptiveSamplingThreadChunkSize = 50;

      void 
      processInterpolationResult(const std::pair<bool, ValuePointer> & interpolationResult,
				 ModelPackagePointer & modelPackage,
				 ModelPackageWarehousePointer & d_algorithmWarehouse,
				 const int d_evaluationLimit,
				 std::deque<ModelPackagePointer> & d_evaluationPile,
				 pthread_mutex_t                 & d_evaluationPileLock,
				 std::vector<ModelPackagePointer> & heldEvaluations,
				 int & d_numberEvaluations,
				 pthread_mutex_t & d_numberEvaluationsLock,
				 int & d_numberSuccessfulInterpolations,
				 SpeculativeGeneratorPointer & d_speculativeGenerator)
      {

	std::stringstream endInterpolateMessage;
	endInterpolateMessage << "End Interpolate Success = " 
			      << interpolationResult.first;
	modelPackage->logTimestamp(endInterpolateMessage.str());
	   
	//
	// check result of interpolation
	//
	if(interpolationResult.first == true) {
	  
	  // Track statistics
	  ++d_numberSuccessfulInterpolations;
          
	  //
	  // successful interpolation, get value
	  //
	  const ValuePointer & value = interpolationResult.second;
          
	  //
	  // insert value in model package
	  //
	  modelPackage->setValue(value);
          
	  //
	  // update speculative generator with completed evaluation
	  //
	  d_speculativeGenerator->update(modelPackage);
	  
	  //
	  // send model package back to algorithm
	  //
	  std::vector<ModelPackagePointer> modelPackageToSend(1,
							      modelPackage);
	  d_algorithmWarehouse->ship(modelPackageToSend);
	  
	}
	else {
	  
	  //
	  // unsuccessful interpolation, add model to evaluation pile
	  //
	  if(d_numberEvaluations < d_evaluationLimit) {
	    
	    //
	    // lock evaluation pile
	    //
	    pthread_mutex_lock(&d_evaluationPileLock);
	    
	    //
	    // add model to evaluation pile
	    //
	    d_evaluationPile.push_back(modelPackage);
	    
	    //
	    // unlock evaluation pile
	    //
	    pthread_mutex_unlock(&d_evaluationPileLock);
            
	    //
	    // lock number evaluations
	    //
	    pthread_mutex_lock(&d_numberEvaluationsLock);
	    
	    //
	    // increment number evaluations
	    //
	    ++d_numberEvaluations;
	    
	    //
	    // unlock number evaluations
	    //
	    pthread_mutex_unlock(&d_numberEvaluationsLock);
	    
	  } 
	  else {
	    
	    //
	    // hit the evaluation limit. keep this model package
	    // now and retry interpolation
	    //
	    
	    //
	    // get rid of previous failed interpolation timestamps
	    //
	    modelPackage->popTimestamp();
	    modelPackage->popTimestamp();
	    
	    //
	    // add to held evaluations
	    //
	    heldEvaluations.push_back(modelPackage);
	    
	  }
	  
	}
	
	//
	//
	//
	return;

      }
      
      //
      // adaptive sampling algorithm receive thread
      //
      void *
      algorithmWarehouseReceiveThread(void * argument)
      {

        //
        // cast argument to AlgorithmWarehouseReceiveThreadData
        //
        struct AdaptiveSampling::AlgorithmWarehouseReceiveThreadData * data = 
          static_cast<struct AdaptiveSampling::AlgorithmWarehouseReceiveThreadData *>(argument);
        
        //
        // pull data from AlgorithmWarehouseReceiveThreadData
        //
        ModelPackageWarehousePointer & d_algorithmWarehouse = 
          data->d_algorithmWarehouse;
        pthread_mutex_t & d_algorithmReceiveLock = 
          *(data->d_algorithmReceiveLock);
        std::deque<ModelPackagePointer> & d_algorithmReceivePile = 
          *(data->d_algorithmReceivePile); 
        bool & d_isConnected = *(data->d_isConnected);
        InterpolationDatabasePointer & d_interpolationDatabase = 
          data->d_interpolationDatabase;

	// Statistics tracking
        int & d_numberAdditionalPoints = *(data->d_numberAdditionalPoints);
	int & d_numberCompletedSpeculativeEvaluations = 
	  *(data->d_numberCompletedSpeculativeEvaluations);
        int & d_numberEvaluationRequests = *(data->d_numberEvaluationRequests);
	int & d_numberInsertedSpeculativeEvaluations = 
	  *(data->d_numberInsertedSpeculativeEvaluations);
        std::vector<int> & d_numberSuccessfulInterpolations = 
          *(data->d_numberSuccessfulInterpolations);
        
        //
        // instantiate model package id counter;
        //
        ModelPackage::IdType modelPackageIdCounter = 0;

        /*
	  ModelPackage::IdType outputCounter = 0;
        */

        try {
          
          //
          // loop indefinitely
          //
          while(true) {
            
            //
            // receive model packages from algorithm
            //
            std::vector<ModelPackagePointer> newModelPackages = 
              d_algorithmWarehouse->receive();
            
            d_numberEvaluationRequests += newModelPackages.size();
            
            //
            // set ids on received model packages
            //
            for(std::vector<ModelPackagePointer>::iterator iter = 
                  newModelPackages.begin(); iter != newModelPackages.end();
                ++iter) {
              
              //
              // get handle to model package
              //
              ModelPackagePointer & modelPackage = *iter;

              std::stringstream idMessage;
              idMessage << "Id: " << modelPackageIdCounter;
              modelPackage->logTimestamp(idMessage.str());
      
              //
              // set id on model package
              //
              modelPackage->setId(modelPackageIdCounter);
              
              //
              // increment counter
              //
              ++modelPackageIdCounter;
	      /*
		++outputCounter;
		if(outputCounter == 10000000) {
		outputCounter = 0;
		d_interpolationDatabase->outputDatabase();
		}
	      */
	                 
            }
           
            if(newModelPackages.size() > 0) {
 
              //
              // lock d_algorithmReceivePile
              //
              pthread_mutex_lock(&d_algorithmReceiveLock);
            
              //
              // add received model packages to d_algorithmReceivePile
              //
              d_algorithmReceivePile.insert(d_algorithmReceivePile.end(),
                                            newModelPackages.begin(),
                                            newModelPackages.end());
            
              //
              // unlock d_algorithmReceivePile
              //
              pthread_mutex_unlock(&d_algorithmReceiveLock);

            }            

          }
          
        }
        catch (arl::hms::ConnectionTerminationException & exception) {

          //
          // done receiving from algorithm, output statistics and exit
          //
     
          //
          // compute total number of successful interpolations
          //
          const int totalNumberSuccessfulInterpolations = 
            std::accumulate(d_numberSuccessfulInterpolations.begin(),
                            d_numberSuccessfulInterpolations.end(),
                            0.0);

          std::ofstream statFile("as_stat");
          statFile << "Number Additional Points: " << d_numberAdditionalPoints 
                   << std::endl;
          statFile << "Number Evaluation Requests: " 
                   << d_numberEvaluationRequests << std::endl;
          statFile << "Number Successful Interpolations: " 
                   <<  totalNumberSuccessfulInterpolations << std::endl;
	  statFile << "Number Speculative Evaluations: "
		   << d_numberCompletedSpeculativeEvaluations << std::endl;
	  statFile << "Number Speculative Evaluations Inserted into Surrogate "
	    "DB: " << d_numberInsertedSpeculativeEvaluations << std::endl;
	  d_interpolationDatabase->printStatistics(statFile);
          statFile.close();
          
          //
          // set is connected flag and teardown
          //
          d_isConnected = false;
          return NULL;

        }
  
        //
        //
        //
        return NULL;
      
      }
      
      //
      // adaptive sampling interpolation thread 
      // 
      void *
      adaptiveSamplingThread(void * argument)
      {

        //
        // cast argument to AdaptiveSamplingThreadData
        //
        struct AdaptiveSampling::AdaptiveSamplingThreadData * data = 
          static_cast<struct AdaptiveSampling::AdaptiveSamplingThreadData *>(argument);
        
        //
        // pull data from AdaptiveSamplingThreadData
        //
        ModelPackageWarehousePointer & d_algorithmWarehouse = 
          data->d_algorithmWarehouse;
        pthread_mutex_t & d_algorithmReceiveLock = 
          *(data->d_algorithmReceiveLock);
        std::deque<ModelPackagePointer> & d_algorithmReceivePile = 
          *(data->d_algorithmReceivePile); 
	const int & d_evaluationLimit = *(data->d_evaluationLimit);
        std::deque<ModelPackagePointer> & d_evaluationPile = 
          *(data->d_evaluationPile);
        pthread_mutex_t & d_evaluationPileLock = 
          *(data->d_evaluationPileLock);
        InterpolationDatabasePointer & d_interpolationDatabase = 
          data->d_interpolationDatabase;
        bool & d_isConnected = *(data->d_isConnected);
	int & d_numberEvaluations = *(data->d_numberEvaluations);
	pthread_mutex_t & d_numberEvaluationsLock = 
	  *(data->d_numberEvaluationsLock);
	int & d_numberSpeculativeEvaluations = 
	  *(data->d_numberSpeculativeEvaluations);
	pthread_mutex_t & d_numberSpeculativeEvaluationsLock = 
	  *(data->d_numberSpeculativeEvaluationsLock);
	const int & d_speculativeEvaluationLimit = 
	  *(data->d_speculativeEvaluationLimit);
	SpeculativeGeneratorPointer & d_speculativeGenerator = 
	  data->d_speculativeGenerator;

	// statistics tracking
        int & d_numberSuccessfulInterpolations = 
          *(data->d_numberSuccessfulInterpolations);


	//
	// whether to interpolate one at a time or in batches
	//
	const bool batchInterpolate = false;

	//
	// vector for failed evaluations that we hold on to
	//
	std::vector<ModelPackagePointer> heldEvaluations;

	//
	// storage for model package template (a sample model package
	// used as a basis for generation of speculative evaluations)
	//
	ModelPackagePointer modelPackageTemplate;

        //
        // loop while connected to algorithm
        //
        while (d_isConnected) {
        
          //
          // lock d_algorithmReceivePile
          //
          pthread_mutex_lock(&d_algorithmReceiveLock);
	  
          //
          // get size of d_algorithmReceivePile 
          //
          const std::deque<ModelPackagePointer>::size_type receivePileSize = 
            d_algorithmReceivePile.size();

	  //
          // determine number of model packages to extract
          //
	  const std::deque<ModelPackagePointer>::size_type 
	    numberModelPackages = batchInterpolate ? 
	    receivePileSize : std::min(adaptiveSamplingThreadChunkSize,
				       receivePileSize);

          //
          // get iterators to beginning and end of extracted model packages
          //
          const std::deque<ModelPackagePointer>::iterator 
            algorithmReceivePileBegin = d_algorithmReceivePile.begin();
          std::deque<ModelPackagePointer>::iterator 
            algorithmReceivePileEnd = algorithmReceivePileBegin;
          std::advance(algorithmReceivePileEnd,
                       numberModelPackages);

          //
          // copy numberModelPackages from d_algorithmReceivePile
          //
          std::vector<ModelPackagePointer> 
            modelPackages(algorithmReceivePileBegin,
                          algorithmReceivePileEnd);
	  modelPackages.insert(modelPackages.end(),
			       heldEvaluations.begin(),
			       heldEvaluations.end());
	  heldEvaluations.clear();
                         
          //
          // remove numberModelPackages from d_algorithmReceivePile 
          //
          d_algorithmReceivePile.erase(algorithmReceivePileBegin,
                                       algorithmReceivePileEnd);

          //
          // unlock d_algorithmReceivePile
          //
          pthread_mutex_unlock(&d_algorithmReceiveLock);

	  //
	  // assign first received package to the template model package
	  //
	  if(!modelPackageTemplate && modelPackages.size() > 0) {
	    modelPackageTemplate = modelPackages[0];
	  }

	  if(batchInterpolate) {
	  
	    //
	    // package vectors for batch interpolate
	    //
	    std::vector<ArgumentPointer> arguments(modelPackages.size());
	    std::vector<int> hints(modelPackages.size()); 
	    for(std::vector<ModelPackagePointer>::size_type i=0; 
		i<modelPackages.size(); ++i) {
	      ModelPackagePointer & modelPackage = modelPackages[i];
	      modelPackage->logTimestamp("Start Interpolate");
	      arguments[i] = modelPackage->getArgument();
	      hints[i] = modelPackage->getHint();
	    }

	    //
	    // interpolate
	    //
	    std::vector<std::pair<bool, ValuePointer> > interpolationResults =
	      d_interpolationDatabase->interpolate(arguments,
						   hints);

	    assert(interpolationResults.size() == modelPackages.size());

	    //
	    // process interpolation results
	    //
	    for(std::vector<ModelPackagePointer>::size_type i=0; 
		i<modelPackages.size(); ++i) {

	      //
	      // save hint in model package
	      //
	      modelPackages[i]->setHint(hints[i]);
	      
	      processInterpolationResult(interpolationResults[i],
					 modelPackages[i],
					 d_algorithmWarehouse,
					 d_evaluationLimit,
					 d_evaluationPile,
					 d_evaluationPileLock,
					 heldEvaluations,
					 d_numberEvaluations,
					 d_numberEvaluationsLock,
					 d_numberSuccessfulInterpolations,
					 d_speculativeGenerator);	      

	    }
	    
	  }
	  else {

	    //
	    // iterate over model packages
	    //
	    std::vector<ModelPackagePointer>::iterator iter =
	      modelPackages.begin();
	    const std::vector<ModelPackagePointer>::iterator iterEnd =
	      modelPackages.end();
	    
	    for( ; iter != iterEnd; ++iter) {
	      
	      //
	      // get handle to model package
	      //
	      ModelPackagePointer & modelPackage = *iter;
	      
	      //
	      // get handle to argument in model package
	      //
	      const ArgumentPointer & argument = modelPackage->getArgument();
	      
	      //
	      // get hint from model package
	      //
	      int hint = modelPackage->getHint();
	      
	      //
	      // log end of input filter execution
	      //
	      modelPackage->logTimestamp("Start Interpolate");
	      
	      //
	      // attempt to interpolate model package
	      //
	      const std::pair<bool, ValuePointer> interpolationResult =
		d_interpolationDatabase->interpolate(argument,
						     hint);
	      	      
	      //
	      // save hint in model package
	      //
	      modelPackage->setHint(hint);
	      
	      processInterpolationResult(interpolationResult,
					 modelPackage,
					 d_algorithmWarehouse,
					 d_evaluationLimit,
					 d_evaluationPile,
					 d_evaluationPileLock,
					 heldEvaluations,
					 d_numberEvaluations,
					 d_numberEvaluationsLock,
					 d_numberSuccessfulInterpolations,
					 d_speculativeGenerator);
	      
	    }

	  }

	  //
	  // speculative evaluations
	  //
	  if(modelPackageTemplate) {

	    //
	    // generate speculative evaluations
	    //
	    std::vector<ModelPackagePointer> speculativeEvaluations =
	      d_speculativeGenerator->generate(modelPackageTemplate,
					       d_speculativeEvaluationLimit - d_numberSpeculativeEvaluations);
	    
	    //
	    // attempt to interpolate speculative evaluations
	    //
	    std::vector<ModelPackagePointer> failedSpeculativeEvaluations;
	    for(std::vector<ModelPackagePointer>::const_iterator iter = 
		  speculativeEvaluations.begin(); 
		iter != speculativeEvaluations.end(); ++iter) {

	      const ModelPackagePointer & modelPackage = *iter;
	      modelPackage->setPriority(SPECULATIVE);
	      
	      //
	      // get handle to argument in model package
	      //
	      const ArgumentPointer & argument = modelPackage->getArgument();
	      
	      //
	      // get hint from model package
	      //
	      int hint = modelPackage->getHint();
	      
	      //
	      // attempt to interpolate model package
	      //
	      const std::pair<bool, ValuePointer> interpolationResult =
		d_interpolationDatabase->interpolate(argument,
						     hint);
	      
	      //
	      // save hint in model package
	      //
	      modelPackage->setHint(hint);
	      
	      //
	      // check result of interpolation
	      //
	      if(interpolationResult.first == false) {
		failedSpeculativeEvaluations.push_back(modelPackage);
	      }
	      
	    }

	    if(failedSpeculativeEvaluations.size() > 0) {

	      //
	      // send evaluations to lower scale
	      //
	      pthread_mutex_lock(&d_evaluationPileLock);
	      d_evaluationPile.insert(d_evaluationPile.end(),
				      failedSpeculativeEvaluations.begin(),
				      failedSpeculativeEvaluations.end());
	      pthread_mutex_unlock(&d_evaluationPileLock);
	      
	      //
	      // adjust size of number of speculative evaluations
	      //
	      pthread_mutex_lock(&d_numberSpeculativeEvaluationsLock);
	      d_numberSpeculativeEvaluations += 
		failedSpeculativeEvaluations.size();
	      pthread_mutex_unlock(&d_numberSpeculativeEvaluationsLock);

	    }

	  }
	  
        }

        //
        //
        //
        return NULL;
	
      }

      //
      // thread to process model packages on evaluation pile,
      // scheduling model packages on brokers
      //
      void *
      evaluationThread(void * argument)
      {
        
        //
        // cast argument to EvaluationThreadData
        //
        struct AdaptiveSampling::EvaluationThreadData * data = 
          static_cast<struct AdaptiveSampling::EvaluationThreadData *>(argument);
        
        //
        // pull data from EvaluationThreadData
        //
        AdaptiveSamplingSchedulerPointer & d_adaptiveSamplingScheduler = 
          data->d_adaptiveSamplingScheduler;
        std::vector<CommunicatorPointer> & d_brokerCommunicators = 
          *(data->d_brokerCommunicators);
        std::deque<ModelPackagePointer> & d_evaluationPile = 
          *(data->d_evaluationPile);
        pthread_mutex_t & d_evaluationPileLock = 
          *(data->d_evaluationPileLock);
        bool & d_isConnected = *(data->d_isConnected);
       
        //
        // loop while connected to algorithm
        //
        while (d_isConnected) {

          //
          // lock evaluation pile
          //
          pthread_mutex_lock(&d_evaluationPileLock);

          //
          // pull model packages off evaluation pile
          //
          std::vector<ModelPackagePointer>
            modelPackages(d_evaluationPile.begin(),
                          d_evaluationPile.end());
          d_evaluationPile.clear();
          
          //
          // unlock evaluation pile and continue
          //
          pthread_mutex_unlock(&d_evaluationPileLock);
            
          //
          // schedule model packages
          //
          const std::vector<AdaptiveSamplingScheduler::BrokerId> brokerIds =
            d_adaptiveSamplingScheduler->schedule(modelPackages);

          //
          // send model packages to assigned brokers
          //
          for(std::vector<AdaptiveSamplingScheduler::BrokerId>::size_type i=0;
              i<modelPackages.size(); ++i) {

            //
            // get assigned broker id
            //
            const AdaptiveSamplingScheduler::BrokerId brokerId = brokerIds[i];
          
            //
            // get handle to model package to evaluate
            //
            const ModelPackagePointer & modelPackage = modelPackages[i];
          
            //
            // get communicator for broker
            //
            CommunicatorPointer & communicator = d_brokerCommunicators[brokerId];
          
            //
            // send model package to broker
            //
            communicator->send(modelPackage);

          }

        }

        //
        //
        //
        return NULL;
        
      }

      //
      // thread to receive results from brokers, adding results to
      // interpolation database and sending result back to algorithm.
      //
      void *
      valueProcessingThread(void * argument)
      {
     
        //
        // cast argument to ValueProcessingThreadData
        //
        struct AdaptiveSampling::ValueProcessingThreadData * data = 
          static_cast<struct AdaptiveSampling::ValueProcessingThreadData *>(argument);
      
        //
        // pull data from ValueProcessingThreadData
        //
        AdaptiveSamplingSchedulerPointer & d_adaptiveSamplingScheduler =
          data->d_adaptiveSamplingScheduler; 
        ModelPackageWarehousePointer & d_algorithmWarehouse = 
          data->d_algorithmWarehouse;
        std::vector<CommunicatorPointer> & d_brokerCommunicators = 
          *(data->d_brokerCommunicators);
        std::deque<ModelPackagePointer> & d_evaluationPile = 
          *(data->d_evaluationPile);
        pthread_mutex_t & d_evaluationPileLock = *(data->d_evaluationPileLock);
        InterpolationDatabasePointer & d_interpolationDatabase = 
          data->d_interpolationDatabase;
        bool & d_isConnected = *(data->d_isConnected);
	int & d_numberEvaluations = *(data->d_numberEvaluations);
	pthread_mutex_t & d_numberEvaluationsLock = 
	  *(data->d_numberEvaluationsLock);
	int & d_numberSpeculativeEvaluations = 
	  *(data->d_numberSpeculativeEvaluations);
	pthread_mutex_t & d_numberSpeculativeEvaluationsLock = 
	  *(data->d_numberSpeculativeEvaluationsLock);
	SpeculativeGeneratorPointer & d_speculativeGenerator = 
	  data->d_speculativeGenerator;

	// statistics tracking
        int & d_numberAdditionalPoints = *(data->d_numberAdditionalPoints);
	int & d_numberCompletedSpeculativeEvaluations = 
	  *(data->d_numberCompletedSpeculativeEvaluations);
	int & d_numberInsertedSpeculativeEvaluations = 
	  *(data->d_numberInsertedSpeculativeEvaluations);

        //
        // instantiate map of model package ids to model packages -
        // used when we need to generate additional points before
        // adding to the interpolation database
        //
        typedef std::map<ModelPackage::IdType, 
	  std::pair<unsigned int, 
	  std::vector<ModelPackagePointer> > >
          AdditionalEvaluationMap;  
        AdditionalEvaluationMap additionalEvaluationMap;
      
	//
	// storage for all completed speculative evaluations 
	//
	std::vector<ModelPackagePointer> speculativeEvaluations;

        //
        // loop while connected to algorithm
        //
        while (d_isConnected) {
      
          //
          // iterate over broker warehouses
          //
          std::vector<CommunicatorPointer>::iterator iter = 
            d_brokerCommunicators.begin(); 
          const std::vector<CommunicatorPointer>::iterator iterEnd = 
            d_brokerCommunicators.end();

          for( ; iter != iterEnd; ++iter) {
          
            //
            // get handle to communicator
            //
            CommunicatorPointer & communicator = *iter;
            
            //
            // receive model packages from communicator
            // 
            const std::vector<ModelPackagePointer> modelPackages = 
              communicator->receive<ModelPackagePointer>();

            //
            // update scheduler with info from completed model packages
            //
            d_adaptiveSamplingScheduler->update(modelPackages);

            //
            // iterate over model packages
            // 
            std::vector<ModelPackagePointer>::const_iterator modelIter = 
              modelPackages.begin();
            const std::vector<ModelPackagePointer>::const_iterator modelIterEnd
              = modelPackages.end();

	    int numberCompletedEvaluations = 0;
	    int numberCompletedSpeculativeEvaluations = 0;

            for( ; modelIter != modelIterEnd; ++modelIter) {
            
              //
              // get handle to model package
              //
              const ModelPackagePointer & modelPackage = *modelIter;
            
	      //
	      // if the result is from a speculative evaluation, treat
	      // it differently than the rest
	      //
	      if(modelPackage->getPriority() == SPECULATIVE) {

		//
		//
		//
		++numberCompletedSpeculativeEvaluations;
		speculativeEvaluations.push_back(modelPackage);
		continue;

	      }
	      else {

		++numberCompletedEvaluations;

	      }
  
              //
              // get model package id
              //
              const ModelPackage::IdType & modelPackageId = 
                modelPackage->getId();

              //
              // get hint from model package
              //
              int hint = modelPackage->getHint();
              
              //
              // check whether model package is in map to determine
              // how to process it
              //
              AdditionalEvaluationMap::iterator mapIter = 
                additionalEvaluationMap.find(modelPackage->getId());

              //
              // if not in map, continue trying to insert point into
              // database
              //
              if(mapIter == additionalEvaluationMap.end()) {

		//
		// update speculative generator with new data
		//
		d_speculativeGenerator->update(modelPackage);

                //
                // get handle to model package argument
                //
                const ArgumentPointer & argument = modelPackage->getArgument();
                
                //
                // get handle to model package value
                //
                const ValuePointer & value = modelPackage->getValue();
		
		//
		// log end of input filter execution
		//
		modelPackage->logTimestamp("Add to Database");
                                
                //
                // insert into interpolation database
                //
                const bool insertSuccess = 
                  d_interpolationDatabase->insert(argument,
                                                  value,
                                                  hint);

		//
		// log end of input filter execution
		//
		modelPackage->logTimestamp("End Add to Database");
                
                if(insertSuccess == false) {
              
                  //
                  // first check whether this point could be interpolated from 
                  // the database now that new data has been received 
                  //
                  int fooHint = -1;
                  const std::pair<bool, ValuePointer> interpolationResult =
                    d_interpolationDatabase->interpolate(argument,
                                                         fooHint);

                  if(interpolationResult.first == false) {
    
                    //
                    // could not insert (argument, value) pair, need to
                    // generate additional points in order to build a new
                    // interpolation model.
                    //
		    modelPackage->logTimestamp("Start Additional Evaluations");

                    const std::vector<ArgumentPointer> newArguments = 
                      d_interpolationDatabase->generateSampleArguments(argument);
		                      
                    d_numberAdditionalPoints += newArguments.size();

                    //
                    // create new model packages from arguments
                    //
                    std::vector<ModelPackagePointer> newModelPackages;
                    for(std::vector<ArgumentPointer>::const_iterator 
                          argumentIter = newArguments.begin(); 
                        argumentIter != newArguments.end(); 
                        ++argumentIter) {
                    
                      //
                      // get handle to model, inputFilter, outputFilter,
                      // and argument
                      //
                      const ModelPointer & model = modelPackage->getModel();
                      const InputFilterPointer & inputFilter = 
                        modelPackage->getInputFilter();
                      const OutputFilterPointer & outputFilter =
                        modelPackage->getOutputFilter();
                      const ArgumentPointer & argument = *argumentIter; 

                      //
                      // create model package from new argument
                      //
                      ModelPackagePointer 
                        newModelPackage(new ModelPackage(model,
                                                         inputFilter,
                                                         outputFilter,
                                                         argument));
                    
                      //
                      // set highest priority for this model package
                      //
                      newModelPackage->setPriority(HIGHEST);
                    
                      //
                      // set id and hint of new model package to
                      // original model package --- this enables us to
                      // map back to the original model package in order
                      // build a new interpolation model.
                      //
                      newModelPackage->setId(modelPackageId);
                      newModelPackage->setHint(hint);
                    
                      //
                      // add to list of new model packages
                      //
                      newModelPackages.push_back(newModelPackage);
                    
                    }

                    //
                    // add current modelPackage to additional evaluation map
                    //
                    additionalEvaluationMap[modelPackageId] =
                      std::make_pair(newModelPackages.size() + 1,
                                     std::vector<ModelPackagePointer>(1,
                                                                      modelPackage));

		    pthread_mutex_lock(&d_numberEvaluationsLock);
		    d_numberEvaluations += newModelPackages.size();
		    pthread_mutex_unlock(&d_numberEvaluationsLock);
                 
                    //
                    // lock evaluation pile
                    //
                    pthread_mutex_lock(&d_evaluationPileLock);
                  
                    //
                    // add model packages to beginning of evaluation pile
                    //
                    d_evaluationPile.insert(d_evaluationPile.begin(),
                                            newModelPackages.begin(),
                                            newModelPackages.end());
                  
                    //
                    // send new model packages for evaluation
                    //
                    pthread_mutex_unlock(&d_evaluationPileLock);
                  
                  }

                }
                
                //
                // update hint in model package
                //
                modelPackage->setHint(hint);
                
                //
                // send model package back to algorithm
                //
                std::vector<ModelPackagePointer> 
                  modelPackagesToSend(1,
                                      modelPackage);
                d_algorithmWarehouse->ship(modelPackagesToSend);
                
              }
              else {
              
                //
                // model package corresponds to an additional
                // evaluation in order to build a full kriging model.
                //
                AdditionalEvaluationMap::mapped_type & mapValues = 
                  mapIter->second;
                
                //
                // get handle to number of additional evaluations
                //
                const unsigned int numberAdditionalEvaluations = 
                  mapValues.first;
                
                //
                // get handle to model packages
                //
                std::vector<ModelPackagePointer> & 
                  additionalModelPackages = 
                  mapValues.second;

                //
                // add received model package to model packages
                //
                additionalModelPackages.push_back(modelPackage);

                //
                // check size of additionalModelPackages to see if we
                // can insert into database
                //
                if(additionalModelPackages.size() == 
                   numberAdditionalEvaluations) {

		  //
		  // log end of input filter execution
		  //
		  std::stringstream additionalEvalLog;
		  additionalEvalLog << "End Additional Evaluations";
		  modelPackage->logTimestamp(additionalEvalLog.str());

                  //
                  // iterate over all model packages, pulling
                  // arguments and values for insertion into database
                  //
                  std::vector<ModelPackagePointer>::const_iterator 
                    additionalIter = additionalModelPackages.begin();
                  const std::vector<ModelPackagePointer>::const_iterator 
                    additionalIterEnd = additionalModelPackages.end();

                  std::vector<ArgumentPointer> arguments;
                  std::vector<ValuePointer> values;

                  for( ; additionalIter != additionalIterEnd; 
                       ++additionalIter) {
                    
                    //
                    // get handle to additional model package
                    //
                    const ModelPackagePointer & additionalModelPackage = 
                      *additionalIter;
                  
                    //
                    // add argument and value to arguments and values
                    //
                    arguments.push_back(additionalModelPackage->getArgument());
                    values.push_back(additionalModelPackage->getValue());
                    
                  }
                  
                  //
                  // insert into database
                  //
                  const bool insertSuccess = 
                    d_interpolationDatabase->addNewModel(arguments,
                                                         values,
                                                         hint);
                  
                  if(insertSuccess == false) {
                    
                    //
                    // fatal error for now
                    //
                    const std::string message("Error adding new model to "
                                              "interpolation database. Try "
                                              "adjusting deltaH value.");
                    throw InvalidArgument(message);
                    
                  }

                  //
                  // erase from map since we are done with this
                  // additional evaluation
                  //
                  additionalEvaluationMap.erase(mapIter);

                }
                
              }

            }

	    //
	    // decrement number of outstanding evaluations
	    //
	    pthread_mutex_lock(&d_numberEvaluationsLock);
	    d_numberEvaluations -= numberCompletedEvaluations;
	    pthread_mutex_unlock(&d_numberEvaluationsLock);
	    pthread_mutex_lock(&d_numberSpeculativeEvaluationsLock);
	    d_numberSpeculativeEvaluations -= 
	      numberCompletedSpeculativeEvaluations;
	    pthread_mutex_unlock(&d_numberSpeculativeEvaluationsLock);

	    d_numberCompletedSpeculativeEvaluations += 
	      numberCompletedSpeculativeEvaluations;

	    //
	    // handle speculative results
	    //
            std::vector<ModelPackagePointer>::const_iterator 
	      speculativeModelIter = speculativeEvaluations.begin();
            const std::vector<ModelPackagePointer>::const_iterator 
	      speculativeModelIterEnd = speculativeEvaluations.end();

	    std::vector<ModelPackagePointer> failedSpeculative;
            for( ; speculativeModelIter != speculativeModelIterEnd; 
		 ++speculativeModelIter) {
            
              //
              // get handle to model package
              //
              const ModelPackagePointer & modelPackage = *speculativeModelIter;

	      //
	      // get hint from model package
	      //
	      int hint = modelPackage->getHint();

	      //
	      // get handle to model package argument
	      //
	      const ArgumentPointer & argument = modelPackage->getArgument();
              
	      //
	      // get handle to model package value
	      //
	      const ValuePointer & value = modelPackage->getValue();
	      
	      //
	      //
	      //
	      if(value) {
	
		//
		// first check whether this point could be interpolated from 
		// the database now that new data has been received 
		//
		int fooHint = -1;
		const std::pair<bool, ValuePointer> interpolationResult =
		  d_interpolationDatabase->interpolate(argument,
						       fooHint);
		
		//
		// if interpolation fails, we should add to database
		//
		if(interpolationResult.first == false) {
		  
		  //
		  // insert into interpolation database
		  //
		  const bool insertSuccess = 
		    d_interpolationDatabase->insert(argument,
						    value,
						    hint);
		  
		  if(insertSuccess) {
		    ++d_numberInsertedSpeculativeEvaluations;
		  }

		  //
		  // if we can't insert, add to failed list and we will
		  // try again later - don't want to generate stencil
		  // for speculative evaluation at this time
		  //
		  if(insertSuccess == false) {
		    failedSpeculative.push_back(modelPackage);
		  }
		}
	      }
	      else {
		--d_numberCompletedSpeculativeEvaluations;
	      }
	    }
	    
	    speculativeEvaluations = failedSpeculative;
	    
	  }

	}
        
        //
        //
        //
        return NULL;

      }
            
    }

    //
    // Constructor.
    //
    AdaptiveSampling::AdaptiveSampling(const InterpolationDatabasePointer & interpolationDatabase,
                                       const AdaptiveSamplingSchedulerPointer & scheduler,
                                       const ModelPackageWarehousePointer & algorithmWarehouse,
				       const std::vector<CommunicatorPointer> & brokerCommunicators,
				       const SpeculativeGeneratorPointer & speculativeGenerator,
                                       int numberInterpolationThreads,
				       int evaluationLimit) :
      d_algorithmWarehouse(algorithmWarehouse),
      d_brokerCommunicators(brokerCommunicators),
      d_evaluationLimit(evaluationLimit),
      d_interpolationDatabase(interpolationDatabase),
      d_isConnected(true),
      d_numberEvaluations(0),
      d_numberSpeculativeEvaluations(0),
      d_scheduler(scheduler),
      d_speculativeEvaluationLimit(evaluationLimit),
      d_speculativeGenerator(speculativeGenerator),
      d_numberAdditionalPoints(0),
      d_numberCompletedSpeculativeEvaluations(0),
      d_numberEvaluationRequests(0),
      d_numberInsertedSpeculativeEvaluations(0)
    {

      //
      // initialize mutex locks
      //
      if(pthread_mutex_init(&d_algorithmReceiveLock,
                            NULL) != 0) {
        std::stringstream message;
        message << "Failure on pthread_mutex_init() of algorithm receive "
          "lock: " << strerror(errno);
        throw OSError(message.str());
      }
    
      if(pthread_mutex_init(&d_evaluationPileLock,
                            NULL) != 0) {
        std::stringstream message;
        message << "Failure on pthread_mutex_init() of evaluation pile "
          "lock: " << strerror(errno);
        throw OSError(message.str());
      }

      if(pthread_mutex_init(&d_numberEvaluationsLock,
                            NULL) != 0) {
        std::stringstream message;
        message << "Failure on pthread_mutex_init() of number evaluations "
          "lock: " << strerror(errno);
        throw OSError(message.str());
      }

      if(pthread_mutex_init(&d_numberSpeculativeEvaluationsLock,
                            NULL) != 0) {
        std::stringstream message;
        message << "Failure on pthread_mutex_init() of number speculative "
	  "evaluations lock: " << strerror(errno);
        throw OSError(message.str());
      }

      //
      // instantiate algorithm receive thread data
      //
      d_algorithmWarehouseReceiveThreadData = 
        new AlgorithmWarehouseReceiveThreadData;
      d_algorithmWarehouseReceiveThreadData->d_algorithmReceiveLock =
        &d_algorithmReceiveLock;
      d_algorithmWarehouseReceiveThreadData->d_algorithmReceivePile =
        &d_algorithmReceivePile;
      d_algorithmWarehouseReceiveThreadData->d_algorithmWarehouse = 
        d_algorithmWarehouse;
      d_algorithmWarehouseReceiveThreadData->d_isConnected = &d_isConnected;
      d_algorithmWarehouseReceiveThreadData->d_interpolationDatabase = 
        d_interpolationDatabase;

      // statistics tracking
      d_algorithmWarehouseReceiveThreadData->d_numberAdditionalPoints = 
        &d_numberAdditionalPoints;
      d_algorithmWarehouseReceiveThreadData->d_numberCompletedSpeculativeEvaluations = &d_numberCompletedSpeculativeEvaluations;
      d_algorithmWarehouseReceiveThreadData->d_numberEvaluationRequests = 
        &d_numberEvaluationRequests;
      d_algorithmWarehouseReceiveThreadData->d_numberInsertedSpeculativeEvaluations = &d_numberInsertedSpeculativeEvaluations;
      d_algorithmWarehouseReceiveThreadData->d_numberSuccessfulInterpolations =
        &d_numberSuccessfulInterpolations;
      
      //
      // start receive thread
      //
      pthread_t threadId;
      if(pthread_create(&threadId,
                        NULL,
                        algorithmWarehouseReceiveThread,
                        static_cast<void *>(d_algorithmWarehouseReceiveThreadData)) != 0) {
        std::stringstream message;
        message 
          << "Error on pthread_create of algorithmWarehouseReceiveThread: "
          << strerror(errno);
        throw OSError(message.str());
      }      
      
      d_threadIds.push_back(threadId);

      d_adaptiveSamplingThreadData.resize(numberInterpolationThreads);

      d_numberSuccessfulInterpolations.resize(numberInterpolationThreads, 0);

      //
      // start interpolation threads
      //
      for(int i=0; i<numberInterpolationThreads; ++i) {

	//
	// instantiate adaptive sampling thread data
	//
	d_adaptiveSamplingThreadData[i] = new AdaptiveSamplingThreadData;
	d_adaptiveSamplingThreadData[i]->d_algorithmWarehouse = 
	  d_algorithmWarehouse;
	d_adaptiveSamplingThreadData[i]->d_algorithmReceiveLock = 
	  &d_algorithmReceiveLock;
	d_adaptiveSamplingThreadData[i]->d_algorithmReceivePile = 
	  &d_algorithmReceivePile;
	d_adaptiveSamplingThreadData[i]->d_evaluationLimit = 
	  &d_evaluationLimit;
	d_adaptiveSamplingThreadData[i]->d_evaluationPile = 
	  &d_evaluationPile;
	d_adaptiveSamplingThreadData[i]->d_evaluationPileLock = 
	  &d_evaluationPileLock;
	d_adaptiveSamplingThreadData[i]->d_interpolationDatabase = 
	  d_interpolationDatabase;   
	d_adaptiveSamplingThreadData[i]->d_isConnected = 
	  &d_isConnected;   
	d_adaptiveSamplingThreadData[i]->d_numberEvaluations = 
	  &d_numberEvaluations;
	d_adaptiveSamplingThreadData[i]->d_numberEvaluationsLock = 
	  &d_numberEvaluationsLock;
	d_adaptiveSamplingThreadData[i]->d_numberSpeculativeEvaluations = 
	  &d_numberSpeculativeEvaluations;
	d_adaptiveSamplingThreadData[i]->d_numberSpeculativeEvaluationsLock = 
	  &d_numberSpeculativeEvaluationsLock;
	d_adaptiveSamplingThreadData[i]->d_speculativeEvaluationLimit = 
	  &d_speculativeEvaluationLimit;
	if(i == 0) {
	  d_adaptiveSamplingThreadData[i]->d_speculativeGenerator = 
	    d_speculativeGenerator;
	}
	else {
	  //
	  // only run real speculative generator on the first thread
	  //
	  d_adaptiveSamplingThreadData[i]->d_speculativeGenerator = 
	    SpeculativeGeneratorPointer(new NullGenerator);
	}

        d_adaptiveSamplingThreadData[i]->d_numberSuccessfulInterpolations
          = &d_numberSuccessfulInterpolations[i];

        if(pthread_create(&threadId,
                          NULL,
                          adaptiveSamplingThread,
                          static_cast<void *>(d_adaptiveSamplingThreadData[i])) != 0) {
          std::stringstream message;
          message << "Error on pthread_create of adaptiveSamplingThread: "
                  << strerror(errno);
          throw OSError(message.str());
        }      
        
        d_threadIds.push_back(threadId);
        
      }
      
      //
      // instantiate evaluation thread
      //
      d_evaluationThreadData = new EvaluationThreadData;
      d_evaluationThreadData->d_adaptiveSamplingScheduler = scheduler;
      d_evaluationThreadData->d_brokerCommunicators = &d_brokerCommunicators;
      d_evaluationThreadData->d_evaluationPile = &d_evaluationPile;
      d_evaluationThreadData->d_evaluationPileLock = &d_evaluationPileLock;
      d_evaluationThreadData->d_isConnected = &d_isConnected;
      
      if(pthread_create(&threadId,
                        NULL,
                        evaluationThread,
                        static_cast<void *>(d_evaluationThreadData)) != 0) {
        std::stringstream message;
        message << "Error on pthread_create of evaluationThread: "
                << strerror(errno);
        throw OSError(message.str());
      }
      
      d_threadIds.push_back(threadId);
      
      //
      // instantiate value processing thread
      //
      d_valueProcessingThreadData = new ValueProcessingThreadData;
      d_valueProcessingThreadData->d_adaptiveSamplingScheduler = scheduler;
      d_valueProcessingThreadData->d_algorithmWarehouse = d_algorithmWarehouse;
      d_valueProcessingThreadData->d_evaluationPile = & d_evaluationPile;
      d_valueProcessingThreadData->d_evaluationPileLock = 
        &d_evaluationPileLock;
      d_valueProcessingThreadData->d_brokerCommunicators = 
        &d_brokerCommunicators;
      d_valueProcessingThreadData->d_interpolationDatabase = 
        d_interpolationDatabase;
      d_valueProcessingThreadData->d_isConnected = &d_isConnected;
      d_valueProcessingThreadData->d_numberEvaluations = &d_numberEvaluations;
      d_valueProcessingThreadData->d_numberEvaluationsLock = 
	&d_numberEvaluationsLock;
      d_valueProcessingThreadData->d_numberSpeculativeEvaluations = 
	&d_numberSpeculativeEvaluations;
      d_valueProcessingThreadData->d_numberSpeculativeEvaluationsLock = 
	&d_numberSpeculativeEvaluationsLock;
      d_valueProcessingThreadData->d_speculativeGenerator = 
	d_speculativeGenerator;

      // statistics tracking
      d_valueProcessingThreadData->d_numberAdditionalPoints = 
        &d_numberAdditionalPoints;
      d_valueProcessingThreadData->d_numberCompletedSpeculativeEvaluations = 
        &d_numberCompletedSpeculativeEvaluations;
      d_valueProcessingThreadData->d_numberInsertedSpeculativeEvaluations = 
        &d_numberInsertedSpeculativeEvaluations;

      if(pthread_create(&threadId,
                        NULL,
                        valueProcessingThread,
                        static_cast<void *>(d_valueProcessingThreadData)) != 0) {
        std::stringstream message;
        message << "Error on pthread_create of valueProcessingThread: "
                << strerror(errno);
        throw OSError(message.str());
      }
      
      d_threadIds.push_back(threadId);
      
      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    AdaptiveSampling::~AdaptiveSampling()
    {
      
      //
      // join threads
      //
      for(std::vector<pthread_t>::size_type i=0; i<d_threadIds.size(); ++i) {
        pthread_join(d_threadIds[i],
                     NULL);
      }

      //
      // destroy mutex locks
      //
      pthread_mutex_destroy(&d_evaluationPileLock);
      pthread_mutex_destroy(&d_algorithmReceiveLock);

      for(std::vector<AdaptiveSamplingThreadData *>::size_type i=0; 
          i<d_adaptiveSamplingThreadData.size(); ++i) {
	delete d_adaptiveSamplingThreadData[i];
      }
      
      //
      // delete data
      //
      delete d_algorithmWarehouseReceiveThreadData;
      delete d_evaluationThreadData;
      delete d_valueProcessingThreadData;
    
      //
      //
      //
      return;

    }

  }
}
