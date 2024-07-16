//
// File:      BatchQueueScheduler.cc
// Package    scheduler
//
// HMS Job Schedulers.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "BatchQueueScheduler.h"

#include <base/StateDB.h>
#include <base/ResourceManager.h>
#include <exception/IOError.h>
#include <exception/OSError.h>
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

#if defined(HAVE_CERRNO)
#include <cerrno>
#else
#error cerrno header file not available
#endif // HAVE_CERRNO

#if defined(HAVE_CSTDIO)
#include <cstdio>
#else
#error cstdio header file not available
#endif // HAVE_CSTDIO

#if defined(HAVE_CSTDLIB)
#include <cstdlib>
#else
#error cstdlib header file not available
#endif // HAVE_CSTDLIB

#if defined(HAVE_CSTRING)
#include <cstring>
#else
#error cstring header file not available
#endif // HAVE_CSTRING

#if defined(HAVE_PWD_H)
#include <pwd.h>
#else
#error pwd.h header file not available
#endif // HAVE_PWD_H

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

#if defined(HAVE_SYS_TYPES_H)
#include <sys/types.h>
#else
#error sys/types.h header file not available
#endif // HAVE_SYS_TYPES_H

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#else
#error unistd.h header file not available
#endif // HAVE_UNISTD_H

//
//
//

namespace arl {
  namespace hms {


    //
    // Constructor
    //
    BatchQueueScheduler::BatchQueueScheduler()
    {

      const uid_t uid = getuid();

      struct passwd * pass = getpwuid(uid);
      if(pass == NULL) {
	const std::string message("Error in getpwuid() retrieving user name");
	throw OSError(message);
      }
      d_userId = pass->pw_name;

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    BatchQueueScheduler::~BatchQueueScheduler()
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
    BatchQueueScheduler::processQueue(ResourceManager & resourceManager,
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
      // iterate over queued commands
      //
      StateDB::Queue::iterator iter = queued.begin();
      const StateDB::Queue::const_iterator iterEnd = queued.end();
      
      for( ; iter != iterEnd; ) {
        
        //
        // get handle to command
        //
        const CommandPointer & command = *iter;
       

#if defined(ENABLE_LOGGING)
	BOOST_LOG_TRIVIAL(debug) << "Executing command: " 
				 << *command;
#endif // ENABLE_LOGGING          
          
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
	// increment to next command
	//
	++iter; 

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
    BatchQueueScheduler::getNumberFreeResources(const ResourceManager & resourceManager) const
    {

      //
      // get number of free resources
      //
      char buffer[128]; 
      FILE * pipe = popen("pbsnodes -a | grep free | wc -l", "r");
      if (pipe) {
	while(!feof(pipe)) {
	  if(fgets(buffer, 128, pipe) != NULL){}
	}
	pclose(pipe);
      }
      else {
	std::stringstream message;
	message << "Failure querying pbsnodes: " << strerror(errno);
	throw IOError(message.str());
      }

      //
      //
      //
      return atoi(buffer);

    }

    //
    // Get number of queued jobs for all users.
    //
    unsigned int 
    BatchQueueScheduler::getNumberQueuedJobsAllUsers(const StateDB & stateDB) const
    {

      char queuedBuffer[128];
      FILE * pb = popen("qstat -i | wc -l","r");
      if (pb) {
	while(!feof(pb)) {
	  if(fgets(queuedBuffer, 128, pb) != NULL) {}
	}
	pclose(pb);
	queuedBuffer[strlen(queuedBuffer)-1] = '\0';
      }
      else {
	std::stringstream message;
	message << "Failure querying qstat: " << strerror(errno);
	throw IOError(message.str());
      }


      //
      //
      //
      return atoi(queuedBuffer);

    }

    //
    // Get number of queued jobs.
    //
    unsigned int 
    BatchQueueScheduler::getNumberQueuedJobs(const StateDB & stateDB) const
    {

      char queuedBuffer[128];
  
      std::stringstream pbsStringStream;
      pbsStringStream << "qstat -a | grep "<< d_userId << " | grep Q | wc -l";

      const std::string pbsString = pbsStringStream.str();

      FILE * pb = popen(pbsString.c_str(), "r");

      if (pb) {
	while(!feof(pb)){
	  if(fgets(queuedBuffer, 128, pb) != NULL) {}
	}
	pclose(pb);
	queuedBuffer[strlen(queuedBuffer)-1] = '\0';
      }
      else {
	std::stringstream message;
	message << "Failure querying qstat: " << strerror(errno);
	throw IOError(message.str());
      }

      //
      //
      //
      return atoi(queuedBuffer);

    }


    //
    // Get number of used resources.
    //
    unsigned int 
    BatchQueueScheduler::getNumberUsedResources(const StateDB & stateDB) const
    {

      unsigned int numNodes = 0;
      
      //
      // get running jobs from PBS
      //
      char buffer[128];
     
      std::stringstream pbsStringStream;
      pbsStringStream << "qstat -r | grep " << d_userId;

      const std::string pbsString = pbsStringStream.str();

      FILE * pipe = popen(pbsString.c_str(), "r");

      if (pipe) {
	  while(!feof(pipe)) {
	      if(fgets(buffer, 128, pipe) != NULL) {
		std::stringstream jobDataArray(buffer);
		
		std::string token;

		for(int i = 0; i <=5; ++i) {
		  jobDataArray >> token;
		}
		numNodes += atoi(token.c_str());
	      }
	  }
	  pclose(pipe);
      }
      else {
	std::stringstream message;
	message << "Failure querying qstat: " << strerror(errno);
	throw IOError(message.str());
      }

      //
      //
      //
      return numNodes;

    }

    unsigned int
    BatchQueueScheduler::getNumberTotalResources(const StateDB & stateDB) const
    {

      char buffer[128];
      FILE * pipe = popen("pbsnodes -a | grep available.ncpus | awk -F '=' '{sum=sum+$2} END {print sum}'","r");
      int totalNodes = 0;
      if (pipe) {
	while(!feof(pipe)) {
	  if(fgets(buffer, 128, pipe) != NULL) {}
	}
	pclose(pipe);
	totalNodes = atoi(buffer);
      }
      else {
	std::stringstream message;
	message << "Failure querying pbsnodes: " << strerror(errno);
	throw IOError(message.str());
      }

      //
      //
      //
      return totalNodes;
    }

  }
}
