//
// File:      BasicBroker.cc
// Package    broker
//
// HMS Brokers.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "BasicBroker.h"

#include <base/TimeUtils.h>

#include <exception/DomainError.h>
#include <exception/OSError.h>

#include <scheduler/LoadQuery.h>

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

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

#if defined(HAVE_SYS_STAT_H)
#include <sys/stat.h>
#else
#error sys/stat.h header file not available
#endif // HAVE_SYS_STAT_H

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
    // local types, data, and functions
    //
    namespace {

      //
      // utility to properly create broker directories to store
      // running commands
      //
      // directoryPath Path of directory to create
      // allowExisting Whether to allow the reuse of an existing directory
      //
      void
      createBrokerDirectory(const std::string & directoryPath,
			    bool allowExisting)
      {

        const int err = mkdir(directoryPath.c_str(),
			      S_IRWXU);
	if(err == -1) {
	  
	  //
	  // check whether the error is because the directory already
	  // exists - if we allow the use of existing directories,
	  // return without error
	  //
	  if(errno == EEXIST && allowExisting) {
	    
	    //
	    //
	    //
	    return;

	  }
	  std::stringstream message;
          message << "Error in mkdir(): " << strerror(errno) 
                  << " for directory: " << directoryPath;
          throw IOError(message.str()); 

	}

	//
	//
	//
	return;

      }

      //
      // comparator for CommandPointers, ordering by priority
      //
      bool compareCommandPriority(const CommandPointer & first,
                                  const CommandPointer & second) 
      {
        
        //
        // get priorities
        //
        const Priority & firstPriority = first->getPriority();
        const Priority & secondPriority = second->getPriority();
        
        //
        // order by higher priority
        //
        return firstPriority > secondPriority;

      }

      //
      // Generate commands from model packages
      //
      std::vector<CommandPointer>
      commandsFromModelPackages(const std::vector<ModelPackagePointer> & modelPackages,
                                const CommandFactoryPointer & commandFactory,
                                const std::string & d_outputDirectory,
                                long unsigned int & d_commandCounter) 
      {

#if defined(ENABLE_LOGGING)
        BOOST_LOG_FUNCTION();

        if(modelPackages.size() > 0) {
          BOOST_LOG_TRIVIAL(debug) << "Creating commands from " 
                                   << modelPackages.size() << " model packages";
        }
#endif // ENABLE_LOGGING
        
        //
        // instantiate return value
        //
        std::vector<CommandPointer> commands;

        //
        // iterate over model packages generate a command for each
        //
        std::vector<ModelPackagePointer>::const_iterator iter = 
          modelPackages.begin();
        const std::vector<ModelPackagePointer>::const_iterator iterEnd = 
          modelPackages.end();
        
        for( ; iter != iterEnd; ++iter) {
        
          //
          // get handle to model package
          //
          const ModelPackagePointer & modelPackage = *iter;

	  //
	  // log start of input filter execution
	  //
	  std::stringstream timestampMessage;
          timestampMessage << "Start input filter " << d_commandCounter;
	  modelPackage->logTimestamp(timestampMessage.str());
	            
          //
          // build directory for the command using commandCounter
          //
          std::stringstream commandDirectory;
          commandDirectory << d_outputDirectory << "/" << d_commandCounter;
          
          //
          // create directory for command to be run in
          //
          if(mkdir(commandDirectory.str().c_str(),
                   S_IRWXU) == -1) {
            std::stringstream message;
            message << "Error in mkdir(): " << strerror(errno) 
                    << " for directory: " << commandDirectory.str();
            throw IOError(message.str());
          }

          //
          // initialize command
          //
          const CommandPointer command = 
            commandFactory->build(modelPackage,
                                  commandDirectory.str());

          //
          // append command to list of commands
          //
          commands.push_back(command);

          //
          // generate command
          //
          command->generate();
          
          //
          // execute input filter
          //
          const ArgumentPointer & argument = modelPackage->getArgument();
          const InputFilterPointer & inputFilter = 
            modelPackage->getInputFilter();
          if(inputFilter) {
            inputFilter->apply(argument,
                               commandDirectory.str());
          }

	  //
	  // log end of input filter execution
	  //
	  modelPackage->logTimestamp("End input filter");
	  
          //
          // increment command counter
          //
          ++d_commandCounter;
          
        }

        //
        //
        //
        return commands;
        
      }

      //
      // schedule commands
      //
      void scheduleCommands(const std::vector<CommandPointer> & commands,
                            StateDB                           & d_stateDB)
      {

#if defined(ENABLE_LOGGING)
        BOOST_LOG_FUNCTION();

        if(commands.size() > 0) {
          BOOST_LOG_TRIVIAL(debug) << "Scheduling "  << commands.size() 
                                   << " commands";
        }
#endif // ENABLE_LOGGING
        
        std::vector<CommandPointer>::const_iterator iter = commands.begin();
        const std::vector<CommandPointer>::const_iterator iterEnd = 
          commands.end();
        
        for( ; iter != iterEnd; ++iter) {
          
          //
          // get handle to command
          //
          const CommandPointer & command = *iter;

          //
          // log scheduled command
          //
          command->getModelPackage()->logTimestamp("Scheduled command");
          
          //
          // schedule command
          //
          d_stateDB.getQueued().add(command);

        }

        //
        // sort queued items by priority 
        //
        if(commands.size() > 0) {
          d_stateDB.getQueued().sort(compareCommandPriority);
        }

        //
        //
        //
        return;

      }

      //
      // get completed commands (monitor running commands)
      //
      std::vector<CommandPointer>
      getCompletedCommands(const MonitorPointer         & d_monitor,
                           const ResourceManagerPointer & d_resourceManager,
                           StateDB                      & d_stateDB)
      {

        //
        // get handle to completed queue
        //
        StateDB::Queue & completed = d_stateDB.getCompleted();

        //
        // clear previous entries from completed queue 
        //
        completed.clear();
     
        //
        // process monitor queue
        //
        // FIXME: d_resourceManager needs to be handled consistently
        // 
        d_monitor->processQueue(*d_resourceManager,
                                d_stateDB);

        //
        // get iterator to new end of completed commands
        //
        const StateDB::Queue::const_iterator iterBegin = completed.begin();
        const StateDB::Queue::const_iterator iterEnd = completed.end();
        
        //
        // return vector of completed commands
        //
        const std::vector<CommandPointer> completedCommands(iterBegin,
                                                            iterEnd);

#if defined(ENABLE_LOGGING)
        BOOST_LOG_FUNCTION();

        if(completedCommands.size() > 0) {
          BOOST_LOG_TRIVIAL(debug) << "Completed "  << completedCommands.size() 
                                   << " commands";
        }
#endif // ENABLE_LOGGING

        //
        // return completed commands
        //
        return completedCommands;

      }

      //
      // apply output filters and store values in model packages.
      //
      std::vector<ModelPackagePointer>
      applyOutputFilters(const std::vector<CommandPointer> & completedCommands,
                         std::vector<ModelPackagePointer> & failedModelPackages)
      {

#if defined(ENABLE_LOGGING)
        BOOST_LOG_FUNCTION();

        if(completedCommands.size() > 0) {
          BOOST_LOG_TRIVIAL(debug) << "Applying output filter to "  
                                   << completedCommands.size() << " commands";
        }
#endif // ENABLE_LOGGING
        
        //
        // instantiate return value
        //
        std::vector<ModelPackagePointer> completedModelPackages;

        //
        // iterate over completed commands
        //
        std::vector<CommandPointer>::const_iterator completedCommandIter = 
          completedCommands.begin();
        const std::vector<CommandPointer>::const_iterator 
          completedCommandIterEnd = completedCommands.end();

        for( ; completedCommandIter != completedCommandIterEnd; 
             ++completedCommandIter) {
        
          //
          // get handle to command
          //
          const CommandPointer & command = *completedCommandIter;

          //
          // get model package from command
          //
	  const ModelPackagePointer & modelPackage = 
	    command->getModelPackage();

	  //
	  // increment number of evaluations
	  //
	  double numberEvaluations = modelPackage->getNumberEvaluations();
	  ++numberEvaluations;
	  modelPackage->setNumberEvaluations(numberEvaluations);
	  
	  //
	  // log start of output filter
	  //
	  modelPackage->logTimestamp("Start output filter");
	  
          //
          // get output filter from model package
          // 
          const OutputFilterPointer & outputFilter = 
            modelPackage->getOutputFilter();

          //
          // get output filter from model package
          // 
          const ArgumentPointer & argument = 
            modelPackage->getArgument();
          
          //
          // get directory where command was executed
          //
          const std::string & directory = command->getDirectory();

          //
          // get stdout of command
          //
          const std::string & stdOut = command->getStdOut();

          //
          // apply output filter to obtain value
          //
          ValuePointer value;
	  if(outputFilter) {
	    try {
	      
	      value = outputFilter->apply(directory,
					  stdOut,
					  argument);
	      
	    }
	    catch(const IOError & exception) {
	      
	      //
	      // if this is a speculative evaluation, it could have
	      // failed for a variety of reasons (most likely not a
	      // reasonable input), so just ignore that it did not
	      // finish (it is not mission critical). still want to
	      // return something above, so do so with empty value and
	      // let above handle it
	      //
	      if(modelPackage->getPriority() == SPECULATIVE) {
		value = arl::hms::ValuePointer();
	      }
	      else {

		//
		// FIXME: avoid this hardcoded value 
		// 
		// throw the exception if we have tried and failed 500 times
		// to evaluate this model.
		//
		if(numberEvaluations > 5) {
		  throw exception;
		}
		
		//
		// add to set of failed evaluations
		//
		failedModelPackages.push_back(modelPackage);
		
		//
		//
		//
		continue;

	      }
	      
	    }
	    
          }

          //
          // transfer timing info to model package
          //
	  const struct timespec wallClockTime = command->getWallClockTime();
          modelPackage->setWallClockTime(timespecToDouble(wallClockTime));

          //
          // insert value into model package
          //
          modelPackage->setValue(value);

	  //
	  // log end of output filter
	  //
	  modelPackage->logTimestamp("End output filter");

          //
          // add model package to returnValue
          //
          completedModelPackages.push_back(modelPackage);
	  
        }

        //
        // return completed model packages
        //
        return completedModelPackages;
        
      }
 
    }

    //
    // Constructor.
    //
    BasicBroker::BasicBroker(const std::vector<CommunicatorPointer> & clientCommunicators,
                             const SchedulerPointer       & scheduler,
                             const MonitorPointer         & monitor,
			     const CleanupPolicyPointer   & cleanupPolicy,
                             const CommandFactoryPointer  & commandFactory,
                             const ResourceManagerPointer & resourceManager,
                             const std::string            & outputDirectory,
			     bool                           handleLoadQuery) :
      d_modelPackageWarehouse(new ModelPackageWarehouse(clientCommunicators)),
      d_clientCommunicators(clientCommunicators),
      d_scheduler(scheduler),
      d_monitor(monitor),
      d_cleanupPolicy(cleanupPolicy),
      d_commandFactory(commandFactory),
      d_resourceManager(resourceManager),
      d_outputDirectory(outputDirectory),
      d_commandCounter(0),
      d_handleLoadQuery(handleLoadQuery)
    {

      //
      // get max hostname size
      //
      const long hostnameSize = sysconf(_SC_HOST_NAME_MAX) + 1;

      //
      // get hostname where broker is executing
      //
      std::vector<char> hostname(hostnameSize);
      if(gethostname(&(hostname[0]),
                     hostnameSize) == -1) {
        const std::string message("Error in gethostname()");
        throw OSError(message);
      }
      
      //
      // get pid of broker
      //
      const pid_t pid = getpid();
      
      //
      // modify output directory as /hostname/pid to ensure unique
      // directories for brokers
      //
      std::stringstream newOutputDirectory;

      newOutputDirectory << outputDirectory;
      createBrokerDirectory(newOutputDirectory.str(), true);

      newOutputDirectory << "/" << &(hostname[0]);
      createBrokerDirectory(newOutputDirectory.str(), true);

      //
      // here we ensure the last directory does not exist so that the
      // broker starts with a fresh running directory
      //
      newOutputDirectory << "/" << pid;
      createBrokerDirectory(newOutputDirectory.str(), false);

      d_outputDirectory = newOutputDirectory.str();

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    BasicBroker::~BasicBroker()
    {
      
      //
      //
      //
      return;

    }
  
    //
    // Execute cycle of receiving model packages, creating commands,
    // scheduling them to run, monitoring their execution, and
    // sending values back.
    //
    void 
    BasicBroker::run()
    {
     
/* 
      std::stringstream brokerLogName;
      brokerLogName << d_outputDirectory << "/broker.log";
      std::ofstream brokerLog(brokerLogName.str().c_str(),
			      std::ofstream::out);
      time_t start = time(NULL);
      time_t last = start;
*/

      try {

	//
	// execute continuously
	//
	while (true) {
	  
	  //
	  // instantiate storage for model packages
	  //
	  std::vector<ModelPackagePointer> modelPackages;
	  
	  //
	  // receive model packages
	  //
	  try {
          
	    //
	    // default mode - receiving model packages
	    //
	    modelPackages = d_modelPackageWarehouse->receive();

	  }
	  catch (arl::hms::ConnectionTerminationException & exception) {
          
	    //
	    // done communicating, can exit
	    //
	    return;

	  }

	  //
	  // convert model packages to commands
	  //
	  std::vector<CommandPointer> commands = 
	    commandsFromModelPackages(modelPackages,
				      d_commandFactory,
				      d_outputDirectory,
				      d_commandCounter);

	  //
	  // schedule commands
	  //
	  scheduleCommands(commands,
			   d_stateDB);
        
	  //
	  // process scheduler queue
	  //
	  // FIXME: d_resourceManager needs to
	  // be handled consistently
	  //
	  d_scheduler->processQueue(*d_resourceManager,
				    d_stateDB);

/*
	  //
	  // log running commands
	  //
	  time_t now = time(NULL);
	  if(now > last) {
	    last = now;
	    int regular = 0;
	    int speculative = 0;
	    StateDB::Queue & running = d_stateDB.getRunning();
	    for(StateDB::Queue::const_iterator iter = running.begin();
		iter != running.end(); ++iter) {
	      if((*iter)->getPriority() == SPECULATIVE) {
		++speculative;
	      }
	      else {
		++regular;
	      }
	    }

	    const int freeResources = 
	      d_resourceManager->getNumberFreeResources();
	    brokerLog << now-start << " " << regular << " " 
		      << speculative << " " << freeResources << std::endl;

	  }
*/

	  //
	  // get completed commands
	  //
	  const std::vector<CommandPointer> completedCommands = 
	    getCompletedCommands(d_monitor,
				 d_resourceManager,
				 d_stateDB);

	  //
	  // immediately schedule new commands if some completed
	  // 
	  if(completedCommands.size() > 0) {
	    d_scheduler->processQueue(*d_resourceManager,
				      d_stateDB);
	  }        
                          
	  //
	  // apply output filters and get completed model packages
	  //
	  std::vector<ModelPackagePointer> failedModelPackages;
	  const std::vector<ModelPackagePointer> completedModelPackages =
	    applyOutputFilters(completedCommands,
			       failedModelPackages);

	  //
	  // apply cleanup policy to completed commands
	  //
	  d_cleanupPolicy->apply(completedCommands);
	 	  
	  //
	  // reschedule failed model packages
	  // 
	  if(failedModelPackages.size() > 0) {
	  
	    //
	    // convert model packages to commands
	    //
	    std::vector<CommandPointer> restartCommands = 
	      commandsFromModelPackages(failedModelPackages,
					d_commandFactory,
					d_outputDirectory,
					d_commandCounter);
	  
	    //
	    // schedule commands
	    //
	    scheduleCommands(restartCommands,
			     d_stateDB);
	  
	    //
	    // process scheduler queue
	    //
	    // FIXME: d_resourceManager needs to
	    // be handled consistently
	    //
	    d_scheduler->processQueue(*d_resourceManager,
				      d_stateDB);
	  
	  }

	  //
	  // ship completed model packages
	  //
	  d_modelPackageWarehouse->ship(completedModelPackages);

	  if(d_handleLoadQuery) {
	  
	    //
	    // try receiving load query requests
	    //
	    for(std::vector<CommunicatorPointer>::const_iterator iter = 
		  d_clientCommunicators.begin(); 
		iter != d_clientCommunicators.end(); ++iter) {
	    
	      const CommunicatorPointer & communicator = *iter;
	    
	      std::vector<LoadQueryPointer> loadQueries;
	    
	      try {
	      
		loadQueries = communicator->receive<LoadQueryPointer>();
	      
	      }
	      catch (arl::hms::ConnectionTerminationException & exception) {
	      
		//
		// done communicating, can exit
		//
		return;
	      
	      }
	    
	      //
	      // execute loadQueries and return to sender.
	      // 
	      for(std::vector<LoadQueryPointer>::const_iterator queryIter = 
		    loadQueries.begin(); queryIter != loadQueries.end(); 
		  ++queryIter) {
	      
		const LoadQueryPointer & loadQuery = *queryIter;
		loadQuery->execute(d_scheduler,
				   *d_resourceManager,
				   d_stateDB);
		communicator->send(loadQuery);
	      
	      }
	    
	    }

	  }

	}

      }
      catch(const Exception & exception) {
	
	//
	// try to communicate exceptions back to the client in order
	// to produce nicer error messages - previously broker would
	// die, producing "ConnectionTerminationException" in client,
	// without a useful error message.
	//

	//
	// adjust message to ensure that it is clear this originated
	// in broker
	//
	std::stringstream newExceptionMessage;
	newExceptionMessage << "Error encountered in Broker:" << std::endl;
	newExceptionMessage << exception.what();
        std::cerr << exception.what() << std::endl;

	Exception newException(newExceptionMessage.str());

	for(std::vector<CommunicatorPointer>::const_iterator iter = 
	      d_clientCommunicators.begin(); 
	    iter != d_clientCommunicators.end(); ++iter) {
	  const CommunicatorPointer & communicator = *iter;
	  communicator->send(newException);
	}
	
	//
	// we are done, throw exception up to kill broker
	//
	throw;

      }
    
      //
      //
      //
      return;

    }

  }
}
