//
// File:      AdaptiveSamplingLoadScheduler.cc
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "AdaptiveSamplingLoadScheduler.h"

#include <exception/IOError.h>
#include <scheduler/LoadQuery.h>

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
    // local functions
    //
    namespace {

      //
      // sort load
      //
      // First criteria: most free resources 
      // Second criteria: fewest queued jobs
      //
      /*
      bool 
      loadSorter(const std::pair<LoadQueryPointer, 
                                 AdaptiveSamplingScheduler::BrokerId> & i,
                 const std::pair<LoadQueryPointer, 
                                 AdaptiveSamplingScheduler::BrokerId> & j) {
        
        const unsigned int iFreeResources = i.first->getNumberFreeResources();
        const unsigned int jFreeResources = j.first->getNumberFreeResources();

        if(iFreeResources != jFreeResources) {
          return iFreeResources > jFreeResources;
        }

        const unsigned int iQueued = i.first->getNumberQueuedJobsAllUsers();
        const unsigned int jQueued = j.first->getNumberQueuedJobsAllUsers();
        
        //
        //
        //
        return iQueued < jQueued;
        
      }
      */
      
      //
      // sort load
      //
      // First criteria: highest ratio of free resources
      //
      bool 
      loadSorter(const std::pair<LoadQueryPointer, 
		                 AdaptiveSamplingScheduler::BrokerId> & i,
                 const std::pair<LoadQueryPointer, 
		                 AdaptiveSamplingScheduler::BrokerId> & j) {
                       
        const unsigned int iFreeResources = i.first->getNumberFreeResources();
        const unsigned int jFreeResources = j.first->getNumberFreeResources();

        const unsigned int iTotalResources = i.first->getNumberTotalResources();
        const unsigned int jTotalResources = j.first->getNumberTotalResources();

        const double iRelativeFreeResources = 
	  ((double)iFreeResources)/iTotalResources;
        const double jRelativeFreeResources = 
	  ((double)jFreeResources)/jTotalResources;

        if(iRelativeFreeResources != jRelativeFreeResources) {
          return iRelativeFreeResources > jRelativeFreeResources;
        }

        const unsigned int iQueued = i.first->getNumberQueuedJobsAllUsers();
        const unsigned int jQueued = j.first->getNumberQueuedJobsAllUsers();

        const double iRelativeQueued = ((double)iQueued/iTotalResources);
        const double jRelativeQueued = ((double)jQueued/jTotalResources);

        //
        //
        //
        return iRelativeQueued < jRelativeQueued; 

      }

      //
      // print header
      //
      void
      printHeader(std::ofstream                          & d_loadLog,
                  const std::vector<CommunicatorPointer> & brokers)
      {

        //
        // header for load log
        //
        d_loadLog << "#\t\t";
        for(std::vector<CommunicatorPointer>::size_type i=0; 
            i<brokers.size(); ++i) {
          d_loadLog << "Broker " << i << ":\t\t\t";
        }
        d_loadLog << std::endl;
        
        d_loadLog << "# Time (s)\t";
        for(std::vector<CommunicatorPointer>::size_type i=0; 
            i<brokers.size(); ++i) {
          d_loadLog << "Free\t" << "Used\t" << "My Queued\t" << "All Queued\t" 
		    << "Total\t";
        }
        
        d_loadLog << std::endl;

        //
        //
        //
        return;

      }

    }

    //
    // Constructor.
    //
    AdaptiveSamplingLoadScheduler::AdaptiveSamplingLoadScheduler(const std::vector<CommunicatorPointer> & brokers) :
      d_brokers(brokers),
      d_timeBetweenLogs(-1)
    {

      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    AdaptiveSamplingLoadScheduler::AdaptiveSamplingLoadScheduler(const std::vector<CommunicatorPointer> & brokers,
                                                                 const std::string & loadLogPath,
                                                                 int                 timeBetweenLogs) :
      d_brokers(brokers), 
      d_loadLogPath(loadLogPath),
      d_timeBetweenLogs(timeBetweenLogs)
    {

      //
      //
      //
      return;

    }
    
    //
    // Destructor.
    //
    AdaptiveSamplingLoadScheduler::~AdaptiveSamplingLoadScheduler()
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
    AdaptiveSamplingLoadScheduler::schedule(const std::vector<ModelPackagePointer> & modelPackages)
    {

      //
      // instantiate returnValue
      //
      std::vector<BrokerId> returnValue;
      returnValue.reserve(modelPackages.size());

      //
      // check if we must do a load query
      //
      bool logLoad = false;
      double appTime;

      if(d_timeBetweenLogs >= 0) {

        if(!d_loadLog.is_open()) { 

          //
          // initial call
          //
          d_loadLog.open(d_loadLogPath.c_str());
          
          if(!d_loadLog) {
            std::stringstream message;
            message << "Cannot open file to write load log to: " 
		    << d_loadLogPath;
            throw IOError(message.str());
          }

          appTime = 0.0;
          logLoad = true;
          d_initialTime = time(NULL);
          d_lastLogTime = time(NULL);
          printHeader(d_loadLog,
                      d_brokers);
          
        }
        else {        
          const time_t currentTime = time(NULL);
          const double diffTime = difftime(currentTime, d_lastLogTime);
          appTime = difftime(currentTime, d_initialTime);
          logLoad = diffTime >= d_timeBetweenLogs;
          if(logLoad) {
            d_lastLogTime = currentTime;
          }      
        }

      }

      //
      // only do the work if we have more than one model package to
      // schedule or we need to log current load of brokers
      //
      if(!modelPackages.empty() || logLoad) {

        //
        // send load queries to each broker
        //
        for(std::vector<CommunicatorPointer>::const_iterator iter = 
              d_brokers.begin(); iter != d_brokers.end(); ++iter) {
          
          LoadQueryPointer loadQuery(new LoadQuery);
          (*iter)->send(loadQuery);
          
        }
        
        //
        // receive responses back
        //
        std::vector<std::pair<LoadQueryPointer, BrokerId> > 
          responses(d_brokers.size());
        
        std::vector<CommunicatorPointer>::size_type numberResponses = 0;
        while(numberResponses != d_brokers.size()) {
          
          for(std::vector<CommunicatorPointer>::size_type i = 0; 
              i < d_brokers.size(); ++i) {
            
            const CommunicatorPointer & communicator = d_brokers[i];
            
            std::vector<LoadQueryPointer> loadQuery = 
              communicator->receive<LoadQueryPointer>();
            
            if(loadQuery.size() != 0) {
              responses[i] = std::make_pair(loadQuery.back(), i);
              ++numberResponses;
            }
            
          }
          
        }
        
        //
        // if logging load - write log query results to file
        //
        if(logLoad) {

          d_loadLog << appTime << "\t\t";
          for(std::vector<std::pair<LoadQueryPointer, 
                BrokerId> >::const_iterator  iter = responses.begin(); 
              iter != responses.end(); ++iter) {
            const LoadQueryPointer & loadQuery = iter->first;
            const int numberFreeResources = loadQuery->getNumberFreeResources();
            const int numberQueuedJobs = loadQuery->getNumberQueuedJobs();
            const int numberQueuedJobsAllUsers = 
	      loadQuery->getNumberQueuedJobsAllUsers();
            const int numberUsedResources = loadQuery->getNumberUsedResources();
            const int numberTotalResources = 
	      loadQuery->getNumberTotalResources();
            d_loadLog << numberFreeResources << "\t" 
                      << numberUsedResources << "\t"
                      << numberQueuedJobs << "\t\t"
                      << numberQueuedJobsAllUsers << "\t"
                      << numberTotalResources << "\t";
          }
          d_loadLog << std::endl;
          d_loadLog.flush();
        }

        //
        // iterate over all model packages scheduling execution
        //
        for(std::vector<ModelPackagePointer>::size_type i=0; 
            i<modelPackages.size(); ++i) {
          
          //
          // sort load (most free resources, and then fewest queued)
          //
          std::sort(responses.begin(),
                    responses.end(),
                    loadSorter);
          
          //
          // getting winning response
          //
          std::pair<LoadQueryPointer, BrokerId> & winningResponse = 
            responses[0];
          LoadQueryPointer & winningLoad = winningResponse.first;
          int numberFreeResources = 
            winningLoad->getNumberFreeResources();
          int numberQueuedJobs = winningLoad->getNumberQueuedJobsAllUsers();
          
          //
          // pick first broker to assign job to
          //
          returnValue.push_back(winningResponse.second);
          
          //
          // adjust data in winning response
          //
          const ModelPackagePointer & modelPackage = modelPackages[i];
          const ModelPointer & model = modelPackage->getModel();
          const int resourceAmount = model->getResourceAmount();
          
          ++numberQueuedJobs;
          numberFreeResources -= resourceAmount;
          
          winningLoad->setNumberQueuedJobsAllUsers(numberQueuedJobs);
          if(numberFreeResources > 0) {
            winningLoad->setNumberFreeResources(numberFreeResources);
          }
          else {
            winningLoad->setNumberFreeResources(0);
          }
          
        }

      }

      //
      //
      //
      return returnValue;

    }
      
  }
}
