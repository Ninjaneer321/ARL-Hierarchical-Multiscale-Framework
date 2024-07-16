//
// File:      BrokerFactory.cc
// Package    broker
//
// HMS Brokers.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#if defined(HAVE_MPI)
#include <mpi.h>
#include <comm/MPICommunicator.h>
#endif // HAVE_MPI

#include "BrokerFactory.h"

#include "BasicBroker.h"

#include "CleanupPolicyCustom.h"
#include "CleanupPolicyFixed.h"
#include "CleanupPolicyNone.h"

#include <base/CommandType.h>
#include <base/LocalhostResourceListGenerator.h>
#include <base/MPIResourceListGenerator.h>
#include <base/predicate/TimeoutStatusPredicate.h>

#include <comm/SocketCommunicator.h>

#include <exception/OSError.h>

#include <monitor/BasicMonitor.h>

#include <scheduler/BasicScheduler.h>
#include <scheduler/BatchQueueScheduler.h>

#if defined(ENABLE_LOGGING)
#if defined(HAVE_BOOST_LOG)
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#else
#error boost logging not available
#endif // HAVE_BOOST_LOG
#endif // ENABLE_LOGGING

#if defined(HAVE_BOOST)
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#else
#error boost not available
#endif // HAVE_BOOST

#if defined(HAVE_CSTDLIB)
#include <cstdlib>
#else
#error cstdlib not available
#endif // HAVE_CSTDLIB

#if defined(HAVE_DLFCN_H)
#include <dlfcn.h>
#else
#error dlfcn.h header file not available
#endif // HAVE_DLFCN_H

#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iostream header file not available
#endif // HAVE_IOSTREAM

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_IOSTREAM

//
//
//

namespace arl {
  namespace hms {

    //
    // local datatypes, data, and functions
    //
    namespace {

      //
      // split a string by whitespace
      //
      std::vector<std::string>
      splitString(const std::string & string) 
      {
       
        //
        // instantiate returnValue
        //
        std::vector<std::string> returnValue;

        //
        // build stream and use stream to split strings
        //
        std::istringstream stringStream(string);
        std::string splitString;
        while(stringStream >> splitString) {
          returnValue.push_back(splitString);
        }

        //
        //
        //
        return returnValue;

      }

      //
      // get processor setting
      //
      const std::string
      getConfigValue(const boost::property_tree::ptree & propertyTree,
		     const std::string & configKey,
		     unsigned int processorId,
		     unsigned int processorSize)
      {
	//
	// get value associated with key
	//
	const std::string configValue =
	  propertyTree.get<std::string>(configKey);

	//
	// split value
	//
	const std::vector<std::string> value =
	  splitString(configValue);

        //
        // if size is zero - setting is empty
        //
        if(value.size() == 0) {
          return "";
        }

	//
	// if size is one - setting is same for all processors
	//
	if(value.size() == 1) {
	  return value[0];
	}

	//
	// otherwise return particular value for that processor
	//
	assert(processorId < value.size());

	if(value.size() != processorSize) {
	  std::stringstream message;
	  message << "Each broker needs a " << configKey 
		  << " in configuration file.";
	  throw IOError(message.str());
	}

	//
	//
	//
	return value[processorId];
	
      }  

    }

    //
    // Constructor.
    //
    BrokerFactory::BrokerFactory()
    {
      
      //
      //
      //
      return;

    }
 
    //
    // Destructor.
    //
    BrokerFactory::~BrokerFactory()
    {
      
      //
      //
      //
      return;

    }

    //
    // Build a broker given a configuration file.
    //
    BrokerPointer 
    BrokerFactory::build(const std::string      & configFile,
                         unsigned int             brokerProcessorId,
                         const std::vector<int> & communicatorRanks)
    {

#if defined(ENABLE_LOGGING)
      BOOST_LOG_FUNCTION();
#endif // ENABLE_LOGGING
      
      //
      // parse config file
      //
      boost::property_tree::ptree propertyTree;
      std::stringstream configFileStream(configFile);
      boost::property_tree::ini_parser::read_ini(configFileStream, 
                                                 propertyTree);
      
      //
      // read required configuration options
      //
      const std::string brokerHosts =
        propertyTree.get<std::string>("Broker.Hosts");
      const std::string communicatorType =
        propertyTree.get<std::string>("Communicator.Type");

      //
      // split broker hosts
      //
      const std::vector<std::string> brokerHostsVector =
        splitString(brokerHosts);

      const std::vector<std::string>::size_type brokerSize = brokerHostsVector.size();

      //
      // assert that brokerProcessorId is logical
      //
      if(brokerSize <= brokerProcessorId) {
        const std::string message("Error launching broker - broker processor "
                                  "id is larger than number of brokers in "
                                  "configuration file.");
        throw IOError(message);
      }

      //
      // get settings for this broker
      //
      std::string brokerCleanup = "None";
      try {
	brokerCleanup =
	  getConfigValue(propertyTree,
			 "Broker.Cleanup",
			 brokerProcessorId,
			 brokerSize);
      }
      catch(boost::property_tree::ptree_bad_path & e) {
	// not specified in input file, catch and use default value
      }
      const std::string libraryPath = 
	getConfigValue(propertyTree,
		       "Broker.Library",
		       brokerProcessorId,
		       brokerSize);
      const std::string outputDirectory = 
	getConfigValue(propertyTree,
		       "Broker.OutputDirectory",
		       brokerProcessorId,
		       brokerSize);
      const std::string resource = 
	getConfigValue(propertyTree,
		       "Broker.Resources",
		       brokerProcessorId,
		       brokerSize);
      const std::string resourceType = 
	getConfigValue(propertyTree,
		       "Broker.ResourceTypes",
		       brokerProcessorId,
		       brokerSize);
      const std::string resourceListType = 
	getConfigValue(propertyTree,
		       "Broker.ResourceListTypes",
		       brokerProcessorId,
		       brokerSize);
      const std::string schedulerType =
        getConfigValue(propertyTree,
                       "Broker.Scheduler",
                       brokerProcessorId,
                       brokerSize);
      const std::string monitorType =
        getConfigValue(propertyTree,
                       "Broker.Monitor",
                       brokerProcessorId,
                       brokerSize);
      const std::string brokerType =
        getConfigValue(propertyTree,
                       "Broker.Type",
                       brokerProcessorId,
                       brokerSize);
      std::string statusPredicates;
      try {
	statusPredicates =
	  getConfigValue(propertyTree,
			 "Broker.StatusPredicates",
			 brokerProcessorId,
			 brokerSize);
      }
      catch(boost::property_tree::ptree_bad_path & e) {
	// not specified in input file, catch and use default value
      }
      std::string endPredicates;
      try {
	endPredicates =
	  getConfigValue(propertyTree,
			 "Broker.EndPredicates",
			 brokerProcessorId,
			 brokerSize);
      }
      catch(boost::property_tree::ptree_bad_path & e) {
	// not specified in input file, catch and use default value
      }
      
      //
      // dlopen any necessary libraries (for deserialization of objects)
      //
      if(libraryPath.compare("") != 0) {
        if(dlopen(libraryPath.c_str(),
                  RTLD_LAZY | RTLD_GLOBAL) == NULL) {
          std::stringstream message;
          message << "Error loading library in broker " << libraryPath << " "
                  << dlerror();
          throw IOError(message.str());
        }
      }

      //
      // create cleanup policy
      //
      CleanupPolicyPointer cleanupPolicy;
      if(brokerCleanup.compare("None") == 0) {
	cleanupPolicy = CleanupPolicyPointer(new CleanupPolicyNone);
      }
      else if(brokerCleanup.compare("Fixed") == 0) {
	const std::string fixedSize = 
	  getConfigValue(propertyTree,
			 "Broker.CleanupFixedSize",
			 brokerProcessorId,
			 brokerSize);
	int fixedSizeInt;
	std::istringstream stream(fixedSize);
	stream >> fixedSizeInt;
	cleanupPolicy =
	  CleanupPolicyPointer(new CleanupPolicyFixed(fixedSizeInt));
      }
      else if(brokerCleanup.compare("Custom") == 0) {
	cleanupPolicy =
	  CleanupPolicyPointer(new CleanupPolicyCustom);
      }
      else {
	//
	// unknown broker cleanup policy, throw an exception
	//
	const std::string message("Invalid cleanup policy in broker "
				  "configuration file: " + brokerCleanup);
	throw IOError(message);
      }
      
      //
      // create resource manager and populate with resources
      //
      ResourceManagerPointer resourceManager(new ResourceManager);
           
      //
      // build ResourceType
      //
      ResourceType type;
      if(resourceType.compare("CPU") == 0) {
        type = CPU;
      }
      else {
        //
        // unknown resource type, throw an exception
        //
        const std::string message("Invalid resource type in broker "
                                  "configuration file: " + resourceType);
        throw IOError(message);
      }
      
      //
      // build ResourceList
      //
      ResourceListPointer resourceList;
      if(resourceListType.compare("Localhost") == 0) {
        LocalhostResourceListGenerator generator;
        resourceList = generator.generate(type);
      }
      else if(resourceListType.compare("List") == 0) {
        ResourceList::Resources newResources;
        std::istringstream resourceStream(resource);
        std::string token;
        while(std::getline(resourceStream, token, ',')) {
          newResources.push_back(token);
        }
        resourceList = ResourceListPointer(new ResourceList(type,
                                                            newResources));
      }
      else if(resourceListType.compare("MPI") == 0 || 
	      resourceListType.compare("Machinefile") == 0) {
        MPIResourceListGenerator generator(resource);
        resourceList = generator.generate(type);
      }
      else {
        //
        // unknown resource list type, throw an exception
        //
        const std::string message("Invalid resource list type in broker "
                                  "configuration file: " + resourceListType);
        throw IOError(message);
      }
      
      std::cout << *resourceList << std::endl;

      //
      // add resource to manager
      //
      resourceManager->add(resourceList); 
      
      //
      // instantiate communicators to client
      //
      std::vector<CommunicatorPointer> communicators;
      if(communicatorType.compare("UnixSocket") == 0) {
        const std::string socketAddress = 
          propertyTree.get<std::string>("Communicator.UnixSocketFileName");
        for(std::vector<int>::size_type i=0; i<communicatorRanks.size(); ++i) {
          CommunicatorPointer 
            communicator(new SocketCommunicator(socketAddress,
                                                SocketCommunicator::UNIX,
                                                true,
                                                std::cout));
          communicators.push_back(communicator);
        }
      }
      else if(communicatorType.compare("TCPIPSocket") == 0) {
        const std::string socketAddress("0.0.0.0:0");
        for(std::vector<int>::size_type i=0; i<communicatorRanks.size(); ++i) {
          CommunicatorPointer
            communicator(new SocketCommunicator(socketAddress,
                                                SocketCommunicator::TCPIP,
                                                true,
                                                std::cout));
          communicators.push_back(communicator);
        }
      }
      else if(communicatorType.compare("MPI") == 0) {
        
#if defined(HAVE_MPI)

        //
        // get parent communicator of the intercommunicator
        //
        MPI_Comm intercommunicator;
        if(MPI_Comm_get_parent(&intercommunicator) != MPI_SUCCESS) {
          const std::string message("Failure getting parent communicator");
          throw NetworkError(message);
        }

        //
        // check if parent communicator is NULL
        //
        if(intercommunicator == MPI_COMM_NULL) {
          const std::string message("Parent communicator is NULL - this MPI "
                                    "process was not spawned");
          throw NetworkError(message);
        }
  
        for(std::vector<int>::size_type i=0; i<communicatorRanks.size(); ++i) {
          
          const int intercommunicatorRank = communicatorRanks[i];
          
          CommunicatorPointer
            communicator(new MPICommunicator(intercommunicator,
                                             true,
                                             intercommunicatorRank));

#if defined(ENABLE_LOGGING)
          BOOST_LOG_TRIVIAL(info) << "Server initialized to communicate with "
                                  << "remote rank " << intercommunicatorRank;
#endif // ENABLE_LOGGING

          communicators.push_back(communicator);
        }

#else

        //
        // HMS not built with MPI
        //
        const std::string message("Cannot create MPI communicator because "
                                  "HMS was not compiled with MPI support - "
                                  "rebuild HMS or choose another communication "
                                  "mechanism");
        throw IOError(message);

#endif // HAVE_MPI

      }
      else {
        //
        // unknown communicator type, throw an exception
        //
        const std::string message("Invalid communicator type in broker "
                                  "configuration file: " + communicatorType);
        throw IOError(message);
      }
    
      //
      // instantiate scheduler
      //
      CommandType commandType;
      SchedulerPointer scheduler;
      if(schedulerType.compare("Basic") == 0) {

	//
	// determine if scheduler should handle speculative evaluations
	//
	bool isSpeculative = false;
	const std::string adaptiveSamplingSpeculative = 
	  propertyTree.get<std::string>("AdaptiveSampling.Speculative", "Null");
	if(adaptiveSamplingSpeculative.compare("Null") == 0) {
	  isSpeculative = false;
	}
	else {
	  isSpeculative = true;
	}
        scheduler = SchedulerPointer(new BasicScheduler(isSpeculative));
	commandType = MPI;
      }
      else if (schedulerType.compare("PBS") == 0) {
        scheduler = SchedulerPointer(new BatchQueueScheduler);
	commandType = PBS;
	
	//
	// get qc settings for this broker
	//
	const std::string accountId =
	  getConfigValue(propertyTree,
			 "Broker.AccountId",
			 brokerProcessorId,
			 brokerSize);
	const std::string gaussianPath =
	  getConfigValue(propertyTree,
			 "Broker.GaussianPath",
			 brokerProcessorId,
			 brokerSize);
	
	//
	// hack to handle machine specific info for PBS (need a better
	// way to do this)
	//
	if(setenv("HMS_PBS_ACCOUNT_ID",
		  accountId.c_str(),
		  1) != 0) {
	  const std::string message("Error setting $HMS_PBS_ACCOUNT_ID "
				    "environment variable.");
	  throw OSError(message);
	}

	if(setenv("HMS_GAUSSIAN_PATH",
		  gaussianPath.c_str(),
		  1) != 0) {
	  const std::string message("Error setting $HMS_GAUSSIAN_PATH "
				    "environment variable.");
	  throw OSError(message);
	}
	
      }
      else {
        //
        // unknown scheduler type, throw an exception
        //
        const std::string message("Invalid scheduler type in broker "
                                  "configuration file: " + schedulerType);
        throw IOError(message);
      }
    
      //
      // instantiate monitor
      //
      MonitorPointer monitor;
      if(monitorType.compare("Basic") == 0) {
        monitor = MonitorPointer(new BasicMonitor);
      }
      else {
        //
        // unknown monitor type, throw an exception
        //
        const std::string message("Invalid monitor type in broker "
                                  "configuration file: " + monitorType);
        throw IOError(message);
      }

      //
      // determine if broker should handle load query requests
      //
      bool handleLoadQuery = false;
      const std::string adaptiveSamplingScheduler = 
        propertyTree.get<std::string>("AdaptiveSampling.Scheduler", "RoundRobin");
      if(adaptiveSamplingScheduler.compare("Load") == 0) {
	handleLoadQuery = true;
      }

      //
      // build universal status and end predicates that are applied to
      // all evaluations
      //
      std::vector<StatusPredicatePointer> statusPredicatesVector;
      std::istringstream statusPredicatesStream(statusPredicates);
      std::string token;
      while(std::getline(statusPredicatesStream, token, ',')) {
	if(token.compare("Timeout") == 0) {
	  const std::string timeoutTime = 
	    getConfigValue(propertyTree,
			   "Broker.StatusPredicateTimeoutTime",
			   brokerProcessorId,
			   brokerSize);
	  const int timeout = std::atoi(timeoutTime.c_str());
	  StatusPredicatePointer
	    timeoutPredicate(new TimeoutStatusPredicate(timeout));
	  statusPredicatesVector.push_back(timeoutPredicate);
	}
	else {
	  //
	  // unknown status predicate type, throw an exception
	  //
	  const std::string message("Invalid status predicate in broker "
				    "configuration file: " + token);
	  throw IOError(message);
	}
      }

      std::vector<EndPredicatePointer> endPredicatesVector;
      std::istringstream endPredicatesStream(endPredicates);
      while(std::getline(endPredicatesStream, token, ',')) {
	//
	// currently no "universal" end predicates have been implemented, but
	// unknown end predicate type, throw an exception
	//
	const std::string message("Invalid end predicate in broker "
				  "configuration file: " + token);
	throw IOError(message);
      }
      
      CommandFactoryPointer
	commandFactory(new CommandFactory(commandType,
					  statusPredicatesVector,
					  endPredicatesVector));
      
      //
      // instantiate broker
      //
      BrokerPointer broker;
      if(brokerType.compare("Basic") == 0) {
        broker = 
          BrokerPointer(new BasicBroker(communicators,
                                        scheduler,
                                        monitor,
					cleanupPolicy,
                                        commandFactory,
					resourceManager,
                                        outputDirectory,
					handleLoadQuery));
      }
      else {
        //
        // unknown broker type, throw an exception
        //
        const std::string message("Invalid broker type in broker "
                                  "configuration file: " + brokerType);
        throw IOError(message);
      }
                         
      //
      //
      //
      return broker;

    }
   
  }
}
