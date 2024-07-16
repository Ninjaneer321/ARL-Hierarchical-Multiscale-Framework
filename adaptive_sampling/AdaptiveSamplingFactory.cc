//
// File:      AdaptiveSamplingFactory.cc
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#if defined(HAVE_MPI)
#include <comm/MPICommunicator.h>
#endif // HAVE_MPI

#include "AdaptiveSamplingFactory.h"
#include "AdaptiveSamplingLoadScheduler.h"
#include "AdaptiveSamplingPerformanceScheduler.h"
#include "AdaptiveSamplingRoundRobinScheduler.h"
#include "InterpolationDatabase.h"
#include "InterpolationDatabaseInputFilter.h"
#include "InterpolationDatabaseOutputFilter.h"
//#include "KDEGenerator.h"
#include "MonteCarloGenerator.h"
#include "NullInterpolationDatabase.h"
#include "NullGenerator.h"
#include "KrigingInterpolationDatabase.h"
#include "PyasInterpolationDatabase.h"
//#include "RandomForestInterpolationDatabase.h"
#include "SpeculativeGenerator.h"
//#include "SparseKrigingInterpolationDatabase.h"

#include <broker/BrokerLauncher.h>
#include <comm/SocketCommunicator.h>

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
    AdaptiveSamplingFactory::AdaptiveSamplingFactory()
    {
      
      //
      //
      //
      return;

    }
 
    //
    // Destructor.
    //
    AdaptiveSamplingFactory::~AdaptiveSamplingFactory()
    {
      
      //
      //
      //
      return;

    }

    //
    // Build an adaptive sampling module given a configuration file.
    //
    AdaptiveSamplingPointer 
    AdaptiveSamplingFactory::build(const std::string      & configFile,
                                   unsigned int             adaptiveSamplingId,  
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
      const std::string hostsString =
        propertyTree.get<std::string>("AdaptiveSampling.Hosts");
      const std::string communicatorType = 
        propertyTree.get<std::string>("Communicator.Type");
      const int evaluationLimit =
	propertyTree.get<int>("AdaptiveSampling.EvaluationLimit",
			      std::numeric_limits<int>::max());	

      //
      // split hosts string
      //
      const std::vector<std::string> hosts = splitString(hostsString);

      const std::vector<std::string>::size_type adaptiveSamplingSize = hosts.size();

      if (adaptiveSamplingSize <= adaptiveSamplingId) {
        const std::string message("Error launching adaptive sampling "
                                  "id is larger than number of brokers in "
                                  "configuration file."); 
        throw IOError(message);                    
      }

      const std::string libraryPath =
        getConfigValue(propertyTree,
                       "AdaptiveSampling.Library",
                       adaptiveSamplingId,
                       adaptiveSamplingSize);
      const std::string numberInterpolationThreadsStr =
        getConfigValue(propertyTree,
                       "AdaptiveSampling.NumberInterpolationThreads",
                       adaptiveSamplingId,
                       adaptiveSamplingSize);
      int numberInterpolationThreads = 
        std::atoi(numberInterpolationThreadsStr.c_str());
      const std::string schedulerType =
        getConfigValue(propertyTree,
                       "AdaptiveSampling.Scheduler",
                       adaptiveSamplingId,
                       adaptiveSamplingSize);
      const std::string adaptiveSamplingType =
        getConfigValue(propertyTree,
                       "AdaptiveSampling.Type",
                       adaptiveSamplingId,
                       adaptiveSamplingSize);
      
      //
      // dlopen any necessary libraries (for deserialization of objects)
      //
      if(libraryPath.compare("") != 0) {
        if(dlopen(libraryPath.c_str(),
                  RTLD_LAZY | RTLD_GLOBAL) == NULL) {
          std::stringstream message;
          message << "Error loading library in adaptive sampling " 
                  << libraryPath << " " << dlerror();
          throw IOError(message.str());
        }
      }
  
      //
      // instantiate communicators to algorithm
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
  
        //
        // determine number clients via MPI
        //
        int numberClients;
        if(MPI_Comm_remote_size(intercommunicator,
                                &numberClients) != MPI_SUCCESS) {
          const std::string message("Failure getting remote size of "
                                    "intercommunicator");
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
        const std::string message("Invalid communicator type in "
                                  "configuration file: " + communicatorType);
        throw IOError(message);
      }

      std::cerr << "Number communicators: " << communicators.size() << std::endl; 

      //
      // Redirect stdout to stderr
      //
      dup2(2, 1);

      //
      // receive input and output filters from algorithm
      //
      InterpolationDatabaseInputFilterPointer inputFilter;
      InterpolationDatabaseOutputFilterPointer outputFilter;
      for(std::vector<CommunicatorPointer>::const_iterator iter = 
            communicators.begin(); iter != communicators.end(); ++iter) {
        
        //
        // get handle to communicator
        //
        const CommunicatorPointer & communicator = *iter;

        //
        // receive input and output filters
        //
        std::vector<InterpolationDatabaseInputFilterPointer> inputFilters;
        while(inputFilters.size() == 0) {

          //
          // receive input filter
          //
          inputFilters = 
            communicator->receive<InterpolationDatabaseInputFilterPointer>();

        }

        //
        // send ack
        //
        communicator->send(1);

        std::vector<InterpolationDatabaseOutputFilterPointer> outputFilters;
        while(outputFilters.size() == 0) {

          //
          // receive output filters      
          //
          outputFilters = 
            communicator->receive<InterpolationDatabaseOutputFilterPointer>();

        }

        //
        // send ack
        //
        communicator->send(1);
        
        inputFilter = inputFilters[0];
        outputFilter = outputFilters[0];

      }
     
      //
      // build speculative generator
      //
      const std::string speculative =
	propertyTree.get<std::string>("AdaptiveSampling.Speculative",
				      "");	
      SpeculativeGeneratorPointer speculativeGenerator;
      if(speculative.size() != 0) {
	if(speculative.compare("KDE") == 0) {
/*
	  const int cycleSize =
	    propertyTree.get<int>("AdaptiveSampling.KDECycleSize");
	  speculativeGenerator =
	    SpeculativeGeneratorPointer(new KDEGenerator(inputFilter,
							 cycleSize));
*/
	}
	else if(speculative.compare("MonteCarlo") == 0) {
	  speculativeGenerator = 
	    SpeculativeGeneratorPointer(new MonteCarloGenerator(inputFilter));
	}
	else if(speculative.compare("Null") == 0) {
	  speculativeGenerator = 
	    SpeculativeGeneratorPointer(new NullGenerator);
	}
	else {
	  //
	  // unknown speculative generator, throw an exception
	  //
	  const std::string message("Invalid speculative generator in "
				    "configuration file: " + 
				    speculative);
	  throw IOError(message);
	    
	}
      }
      else {
	speculativeGenerator = 
	  SpeculativeGeneratorPointer(new NullGenerator);
      }

      //
      // instantiate interpolation database
      //
      InterpolationDatabasePointer interpolationDatabase;
      if(adaptiveSamplingType.compare("Null") == 0) {
        interpolationDatabase = 
          InterpolationDatabasePointer(new NullInterpolationDatabase);
      }
      else if(adaptiveSamplingType.compare("Kriging") == 0) {
        
        const double theta =
          propertyTree.get<double>("AdaptiveSampling.Theta");
        const int agingThreshold =
          propertyTree.get<int>("AdaptiveSampling.AgingThreshold");
        const std::string deltaHString =
          propertyTree.get<std::string>("AdaptiveSampling.DeltaH");
        const int maxKrigingModelSize = 
          propertyTree.get<int>("AdaptiveSampling.MaxKrigingModelSize");
        const int maxNumberSearchModels = 
          propertyTree.get<int>("AdaptiveSampling.MaxNumberSearchModels");
        const double maxQueryPointModelDistance = 
          propertyTree.get<double>("AdaptiveSampling.MaxQueryPointModelDistance");
        const double meanErrorFactor =
          propertyTree.get<double>("AdaptiveSampling.MeanErrorFactor");
        const std::string mtreeDirectoryName =
          propertyTree.get<std::string>("AdaptiveSampling.MtreeDirectoryName");
        const double tolerance = 
          propertyTree.get<double>("AdaptiveSampling.Tolerance");
	const std::string database =
	  propertyTree.get<std::string>("AdaptiveSampling.Database",
					"");

        //
        // split deltaH string to get deltaH values
        //
        std::vector<double> deltaH;
        std::istringstream deltaHStream(deltaHString);
        while(!deltaHStream.eof()) {
          double deltaHValue;
          deltaHStream >> deltaHValue;
          deltaH.push_back(deltaHValue);
        } 

	if(database.size() == 0) {
	
          interpolationDatabase = 
	    InterpolationDatabasePointer
	    (new KrigingInterpolationDatabase(inputFilter,
					      outputFilter,
					      deltaH,
					      tolerance,
					      maxKrigingModelSize,
					      maxNumberSearchModels,
					      meanErrorFactor,
					      maxQueryPointModelDistance,
					      agingThreshold,
					      theta,
					      mtreeDirectoryName));
	}
	else {

	  //
	  // read in existing database
	  //
	  interpolationDatabase = 
	    InterpolationDatabasePointer
	    (new KrigingInterpolationDatabase(inputFilter,
					      outputFilter,
					      deltaH,
					      tolerance,
					      maxKrigingModelSize,
					      maxNumberSearchModels,
					      meanErrorFactor,
					      maxQueryPointModelDistance,
					      agingThreshold,
					      theta,
					      mtreeDirectoryName,
					      database));
	  
	}
    
      }
/*
      else if(adaptiveSamplingType.compare("SparseKriging") == 0) {
        const double rho =
          propertyTree.get<double>("AdaptiveSampling.Rho");
	interpolationDatabase = 
	  InterpolationDatabasePointer(new SparseKrigingInterpolationDatabase(inputFilter,
									      outputFilter,
									      rho));

      }
*/
      else if(adaptiveSamplingType.compare("Pyas") == 0) {


        const std::string deltaHString =
          propertyTree.get<std::string>("AdaptiveSampling.DeltaH");

        //
        // split deltaH string to get deltaH values
        //
        std::vector<double> deltaH;
        std::istringstream deltaHStream(deltaHString);
        while(!deltaHStream.eof()) {
          double deltaHValue;
          deltaHStream >> deltaHValue;
          deltaH.push_back(deltaHValue);
        } 
	
	interpolationDatabase = 
	  InterpolationDatabasePointer
	  (new PyasInterpolationDatabase(inputFilter,
					 outputFilter,
					 deltaH,
					 configFile));
      }
      /*
      else if(adaptiveSamplingType.compare("RandomForest") == 0) {
       
        const std::string trainedModelPath = 
          propertyTree.get<std::string>("AdaptiveSampling.RandomForestTrainedModel");
        const int minimumTrainingSupport = 
          propertyTree.get<int>("AdaptiveSampling.RandomForestMinimumTrainingSupport");
        
        //
        // can only use one interpolation thread for random forest -
        // set that hwere, but warn if different setting in file
        //
        if(numberInterpolationThreads != 1) {
          std::cerr << "Number of interpolator threads for random forest "
            "must be 1 - overriding setting in input file." << std::endl;
          numberInterpolationThreads = 1;
        }

        interpolationDatabase = 
          InterpolationDatabasePointer
          (new RandomForestInterpolationDatabase(inputFilter,
                                                 outputFilter,
                                                 trainedModelPath,
                                                 minimumTrainingSupport));
      }
      */
      else {
        //
        // unknown adaptive sampling type, throw an exception
        //
        const std::string message("Invalid adaptive sampling type in "
                                  "configuration file: " + 
                                  adaptiveSamplingType);
        throw IOError(message);
      }
     
      //
      // instantiate model package warehouse containing communicators
      // to algorithm
      //
      ModelPackageWarehousePointer 
        algorithmWarehouse(new ModelPackageWarehouse(communicators));

      //
      // launch broker
      //	
      configFileStream.clear();	
      configFileStream.seekg(0) ;
      const std::vector<CommunicatorPointer> brokerCommunicators = 
        BrokerLauncher().launch(configFileStream,
                                adaptiveSamplingId,
                                hosts.size());
              
      //
      // build AdaptiveSamplingScheduler
      //
      AdaptiveSamplingSchedulerPointer scheduler;
      if(schedulerType.compare("RoundRobin") == 0) {
        scheduler = 
          AdaptiveSamplingSchedulerPointer(new AdaptiveSamplingRoundRobinScheduler(brokerCommunicators.size()));
      }
      else if(schedulerType.compare("Performance") == 0) {
        const int numberBins = 3;
        const int maxPointsPerBin = 1;
        const double minArgument = 0;
        const double maxArgument = 10;
        scheduler = AdaptiveSamplingSchedulerPointer(
          new AdaptiveSamplingPerformanceScheduler(brokerCommunicators.size(),
                                                   numberBins,
                                                   maxPointsPerBin,
                                                   minArgument,
                                                   maxArgument,
                                                   inputFilter));
      }
      else if(schedulerType.compare("Load") == 0) {

        //
        // see if logging is turned on
        //
        try {

          const std::string loadLog =
            getConfigValue(propertyTree,
                           "AdaptiveSampling.LoadLog",
                           adaptiveSamplingId,
                           adaptiveSamplingSize);

          const std::string loadLogDelta =
            getConfigValue(propertyTree,
                           "AdaptiveSampling.LoadLogDelta",
                           adaptiveSamplingId,
                           adaptiveSamplingSize);


          scheduler = 
            AdaptiveSamplingSchedulerPointer(new AdaptiveSamplingLoadScheduler(brokerCommunicators,
                                                                               loadLog,
                                                                               std::atoi(loadLogDelta.c_str())));
        
        }
        catch(...) {
          
          scheduler = 
            AdaptiveSamplingSchedulerPointer(new AdaptiveSamplingLoadScheduler(brokerCommunicators));
          
        }

      }
      else {
        //
        // unknown adaptive sampling scheduler, throw an exception
        //
        const std::string message("Invalid adaptive sampling scheduler in "
                                  "configuration file: " + schedulerType);
        throw IOError(message);
      }
      
      //
      // instantiate adaptive sampling
      //
      AdaptiveSamplingPointer
        adaptiveSampling(new AdaptiveSampling(interpolationDatabase,
                                              scheduler,
                                              algorithmWarehouse,
                                              brokerCommunicators, 
					      speculativeGenerator,
                                              numberInterpolationThreads,
					      evaluationLimit));
                         
      //
      //
      //
      return adaptiveSampling;

    }
   
  }
}
