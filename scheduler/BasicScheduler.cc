//
// File:      BasicScheduler.cc
// Package    scheduler
//
// HMS Job Schedulers.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "BasicScheduler.h"

#include <base/StateDB.h>
#include <base/ResourceManager.h>
#include <exception/RangeError.h>

#if defined(ENABLE_LOGGING)
#if defined(HAVE_BOOST_LOG)
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#else
#error boost logging not available
#endif // HAVE_BOOST_LOG
#endif // ENABLE_LOGGING

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
    // Constructor.
    //
    BasicScheduler::BasicScheduler(bool isSpeculative) :
      d_isSpeculative(isSpeculative)
    {
      
      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    BasicScheduler::~BasicScheduler()
    {

      //
      //
      //
      return;

    }
  
    //
    // Process queue of queued commands.
    //
    void 
    BasicScheduler::processQueue(ResourceManager & resourceManager,
                                 StateDB         & stateDB) const
    {
      
#if defined(ENABLE_LOGGING)
      BOOST_LOG_FUNCTION();
#endif // ENABLE_LOGGING

      //
      // get queued and running queues
      //
      StateDB::Queue & queued = stateDB.getQueued();
      StateDB::Queue & running = stateDB.getRunning();

      //
      // check whether resources are available - if not bail out now
      // rather than attempting to schedule.
      //

      //
      // in non speculative scheme - bail out if no resources are available
      //
      if(!d_isSpeculative) {

	if(!resourceManager.hasResourcesAvailable()) {

	  //
	  //
	  //
	  return;

	}

      }
      else {

	//
	// in speculative scheme - attempt to cancel speculative jobs
	// to be able to replace them with "real" jobs
	//

	if(resourceManager.getNumberFreeResources() < 64) {

	  //
	  // if there are no resources available and queued jobs exist,
	  // we may have to cancel some running speculative evaluations
	  //

	  //
	  // iterate over queued jobs and cancel speculative evaluations
	  // one-to-one. we are implicitly assuming, for ease of
	  // implementation, that both evaluations execute over the same
	  // number of resources (may need to change in the future)
	  //
	  // other improvements could be to cancel speculative
	  // evaluations in order of execution for the least time
	  //
       
	  StateDB::Queue::iterator queuedIter = queued.begin();
	  const StateDB::Queue::const_iterator queuedIterEnd = queued.end();
	  StateDB::Queue::iterator runningIter = running.begin();
	  const StateDB::Queue::const_iterator runningIterEnd = running.end();

	  for( ; queuedIter != queuedIterEnd; ) {
	  
	    const CommandPointer & queuedCommand = *queuedIter;

	    //
	    // if the queued command is speculative, we are done
	    //
	    if(queuedCommand->getPriority() == SPECULATIVE) {
	      break;
	    }

	    //
	    // the queued command is non-speculative, cancel the next
	    // speculative running job
	    //
	    for( ; runningIter != runningIterEnd; ) {

	      const CommandPointer & runningCommand = *runningIter;

	      if(runningCommand->getPriority() == SPECULATIVE) {

		//
		// terminate and place back in queue
		//
		runningCommand->terminate();
		const StateDB::Queue::iterator removeIter = runningIter++;
		queued.add(runningCommand);

		//
		// check back in resources dedicated to this evaluation
		//
		const ResourceListPointer & resources = 
		  runningCommand->getResources();
		if(resources) {
		  resourceManager.checkIn(resources);
		}

		running.pop(removeIter);

		break;

	      }
	      else {

		++runningIter;

	      }
	    
	    }

	    //
	    // if we have reached the end of running jobs, then
	    // everything running is non-speculative and we are done
	    //
	    if(runningIter == runningIterEnd) {
	      break;
	    }
	  
	    ++queuedIter;

	  }
	   
	  //
	  //
	  //
	  return;

	}

      }
      
      //
      // iterate over queued commands
      //

      StateDB::Queue::iterator iter = queued.begin();
      const StateDB::Queue::const_iterator iterEnd = queued.end();
      
      for( ; iter != iterEnd; ) {
        
        //
        // get handle to command
        //
        const CommandPointer & command = *iter;
        
        //
        // get resource amount and type from command
        //
        const int resourceAmount = command->getResourceAmount();
        const ResourceType & resourceType = command->getResourceType();
      
        //
        // check if resourceAmount is logical
        //
        if(resourceAmount < 1) {
          
          //
          // throw an exception
          //
          std::stringstream message;
          message << "Invalid resource amount: " << resourceAmount 
                  << " requested by command: " << command;

          //
          //
          //
          throw RangeError(message.str());

        }

        //
        // try to check out resources from resource manager.
        //
        const ResourceListPointer acquiredResources = 
          resourceManager.checkOut(resourceType,
                                   resourceAmount);
        

        //
        // get list of underlying resources
        //
        const ResourceList::Resources & resources = 
          acquiredResources->getResources();
        
        //
        // check if we successfully acquired resources
        //
        if(resources.size() > 0) {
          
#if defined(ENABLE_LOGGING)
          BOOST_LOG_TRIVIAL(debug) << "Checked out resources: " 
                                   << *acquiredResources;
#endif // ENABLE_LOGGING          
          
          //
          // set resources on command
          //
          command->setResources(acquiredResources);
          
#if defined(ENABLE_LOGGING)
          BOOST_LOG_TRIVIAL(debug) << "Executing command: " 
                                   << *command;
#endif // ENABLE_LOGGING          
          command->getModelPackage()->logTimestamp("Start execution");
          //
          // execute command
          //
          command->execute();

          //
          // add command to running queue
          //
          running.add(command);

          //
          // copy iter to removeIter and then increment to next command
          //
          const StateDB::Queue::iterator removeIter = iter++;
          
          //
          // remove command from queued queue
          //
          queued.pop(removeIter);
          
          //
          // check whether resources are still available, if
          // exhausted, bail out now
          //
          if(!resourceManager.hasResourcesAvailable()) {
          
            //
            //
            //
            return;
            
          }

        }
        else {
          
          //
          // increment to next command
          //
          ++iter;
          
        }
        
      }
      
      //
      //
      //
      return;

    }
    
    //
    // Get number of free resources.
    //
    unsigned int 
    BasicScheduler::getNumberFreeResources(const ResourceManager & resourceManager) const
    {

      //
      //
      //
      return resourceManager.getNumberFreeResources();

    }

    //
    // Get number of queued jobs.
    //
    unsigned int 
    BasicScheduler::getNumberQueuedJobs(const StateDB & stateDB) const
    {

      //
      //
      //
      return stateDB.getQueued().size();

    }

    //
    // Get number of queued jobs (for all users).
    //
    unsigned int 
    BasicScheduler::getNumberQueuedJobsAllUsers(const StateDB & stateDB) const
    {

      //
      //
      //
      return BasicScheduler::getNumberQueuedJobs(stateDB);
      
    }

    //
    // Get number of used resources.
    //
    unsigned int 
    BasicScheduler::getNumberUsedResources(const StateDB & stateDB) const
    {
      
      unsigned int numberUsedResources = 0;

      //
      // get running jobs from stateDB
      //
      const StateDB::Queue & running = stateDB.getRunning();
      for(StateDB::Queue::const_iterator iter = running.begin(); 
          iter != running.end(); ++iter) {
        const CommandPointer & command = *iter;
        const int resourceAmount = command->getResourceAmount();
        numberUsedResources += resourceAmount;
      }

      //
      //
      //
      return numberUsedResources;

    }

    unsigned int
    BasicScheduler::getNumberTotalResources(const StateDB & stateDB) const
    {

      //
      // FIXME: need to obtain this (likely from resource manager)
      //
      return 0; 

    }

  }
}
