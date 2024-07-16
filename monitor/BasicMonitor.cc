//
// File:      BasicMonitor.cc
// Package    monitor
//
// HMS Job Monitors.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "BasicMonitor.h"

#include <base/StateDB.h>
#include <base/ResourceManager.h>
#include <exception/CommandError.h>

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
    BasicMonitor::BasicMonitor()
    {
      
      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    BasicMonitor::~BasicMonitor()
    {

      //
      //
      //
      return;

    }
  
    //
    // Process queue of running commands.
    //
    void 
    BasicMonitor::processQueue(ResourceManager & resourceManager,
                               StateDB         & stateDB) const
    {
      
#if defined(ENABLE_LOGGING)
      BOOST_LOG_FUNCTION();
#endif // ENABLE_LOGGING

      //
      // get running and completed queues
      //
      StateDB::Queue & running = stateDB.getRunning();
      StateDB::Queue & completed = stateDB.getCompleted();

      //
      // 
      //

      //
      // iterate over running commands
      //

      StateDB::Queue::iterator iter = running.begin();
      const StateDB::Queue::const_iterator iterEnd = running.end();
      
      for( ; iter != iterEnd; ) {
        
        //
        // get handle to command
        //
        const CommandPointer & command = *iter;
        
        //
        // get status of running command
        //
        const CommandStatus status = command->getStatus();

        //
        // process status
        //
        switch(status) {
        
        case RUNNING:

          //
          // iterate to next command
          //
          ++iter;

          break;
          
        case COMPLETED: {
#if defined(ENABLE_LOGGING)
          BOOST_LOG_TRIVIAL(debug) << "Completed job";
#endif // ENABLE_LOGGING          

	  //
	  // log execution completion
	  //
	  command->getModelPackage()->logTimestamp("End execution");
	  
          //
          // add command to completed queue 
          //
          completed.add(command);
          
          //
          // copy iter to removeIter and then increment to next command
          //
          const StateDB::Queue::iterator removeIter = iter++;
          
          //
          // remove command from running queue
          //
          running.pop(removeIter);
          
          //
          // get resources from command
          //
          const ResourceListPointer & resources = command->getResources();
          
          //
          // check resources into manager (if we have resources to return)
          //
	  if(resources) {
	    resourceManager.checkIn(resources);
	  }
          
          //
          //
          //
          break;
        }
        case ERROR: {
          
          //
          // error in command, throw an exception
          //
          std::stringstream message;
          message << "Error in completed command: " << command;
          
          //
          //
          //
          throw CommandError(message.str());

          //
          //
          //
          break;
          
        }
        default: {
        
          //
          // unknown CommandStatus
          //
          std::stringstream message;
          message << "Invalid CommandStatus: " << status 
                  << " returned from command: " << command; 
          
          //
          //
          //
          throw CommandError(message.str());

          //
          //
          //
          break;
          
        }          
          
        }
        
      }
      
      //
      //
      //
      return;

    }

  }
}
