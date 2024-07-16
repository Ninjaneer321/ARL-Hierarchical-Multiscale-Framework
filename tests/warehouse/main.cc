//
// Test warehouse
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#if defined(HAVE_MPI)
#include <mpi.h>
#include <comm/MPICommunicator.h>
#endif

#include "ScalarValue.h"

#include <comm/Communicator.h>
#include <comm/SocketCommunicator.h>

#include <base/ModelPackage.h>

#include <exception/ConnectionTerminationException.h>
#include <exception/IOError.h>
#include <exception/NetworkError.h>

#include <warehouse/Warehouse.h>

#if defined(ENABLE_LOGGING)
#if defined(HAVE_BOOST_LOG)
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#else
#error boost logging not available
#endif // HAVE_BOOST_LOG
#endif // ENABLE_LOGGING

#include <boost/serialization/shared_ptr.hpp>

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

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#else
#error unistd.h header file not available
#endif // HAVE_UNISTD_H

namespace {

  typedef enum CommunicatorType { SOCKETTCP, SOCKETUNIX, MPI } CommunicatorType;
  typedef enum CommunicatorMode { CLIENT0, CLIENT1, SERVER } CommunicatorMode;
  typedef arl::hms::Warehouse<arl::hms::ModelPackagePointer> ModelPackageWarehouse;


  //
  // print usage
  //
  inline void
  printUsage(const char * programName)
  {

    std::cerr << "usage: " << programName << " "
              << "-m mode -c comm" << std::endl;
    std::cerr << "\t-m mode: 1 - Client 1" << std::endl;
    std::cerr << "\t-m mode: 2 - Client 2" << std::endl;
    std::cerr << "\t-m mode: 3 - Server" << std::endl;
    std::cerr << "\t-c comm: 1 - TCPIP Socket" << std::endl;
    std::cerr << "\t-c comm: 2 - Unix Socket" << std::endl;
    std::cerr << "\t-c comm: 3 - MPI" << std::endl;

    //
    //
    //
    return;

  }

  //
  // process command line
  //
  void
  processCommandLine(CommunicatorType & communicatorType,
                     CommunicatorMode & communicatorMode,
                     int                ac,
                     char             * av[])
  {

    int c;
    bool errorFlag = false;

    while( (c=getopt(ac, av, "m:c:")) != -1 )
    switch(c){

    case 'm':
      {
        const int value = std::atoi(optarg);
        if(value == 1) {
          communicatorMode = CLIENT0;
        }
        else if(value == 2) {
          communicatorMode = CLIENT1;
        }
        else if(value == 3) {
          communicatorMode = SERVER;
        }
        else {
          errorFlag = true;
        }
        break;
      }
    case 'c':
      {
        const int value = std::atoi(optarg);
        if(value == 1) {
          communicatorType = SOCKETTCP;
        }
        else if(value == 2) {
          communicatorType = SOCKETUNIX;
        }
        else if(value == 3) {
          communicatorType = MPI;
        }
        else {
          errorFlag = true;
        }
        break;
      }
    case '?':
      errorFlag = true;
      break;

    }

    if(optind > ac) {
      errorFlag = true;
    }
    
    //
    // check errorFlag
    //
    if (errorFlag == true) {
      printUsage(av[0]);    
      std::exit(EXIT_FAILURE);
    }

    //
    //
    //
    return;

  }

}

//
//
//
int
main(int    ac,
     char * av[])
{

#if defined(HAVE_MPI)

  MPI_Init(&ac,
           &av);

#endif // HAVE_MPI

  //
  // process command line arguments
  //
  CommunicatorType communicatorType;
  CommunicatorMode communicatorMode;
  processCommandLine(communicatorType,
                     communicatorMode,
                     ac,
                     av);


  //
  // convert SERVER / CLIENT into is serverFlag
  //
  const bool isServer = communicatorMode == SERVER;

#if defined(HAVE_MPI)

  //
  // get rank
  //
  int taskId;
  if(MPI_Comm_rank(MPI_COMM_WORLD,
                   &taskId) != MPI_SUCCESS) {
    const std::string message("Error getting rank");
    throw arl::hms::NetworkError(message);
  };

#endif // HAVE_MPI

  //
  // initialize logging
  //

#if defined(ENABLE_LOGGING)

  boost::log::add_common_attributes();
  boost::log::core::get()->
    add_global_attribute("Scope",
                         boost::log::attributes::named_scope());

  std::string formatString("[%TimeStamp%] - %Scope% - %Message%");

#if defined(HAVE_MPI)

  //
  // get rank
  //
  //int taskId;
  if(MPI_Comm_rank(MPI_COMM_WORLD,
                   &taskId) != MPI_SUCCESS) {
    const std::string message("Error getting rank");
    throw arl::hms::NetworkError(message);
  };

  std::stringstream format;
  format << "r@" << taskId << ": " << formatString; 

  formatString = format.str();

#endif // HAVE_MPI

  std::stringstream fileName;
  fileName << "Communicator_";

  switch(communicatorMode) {
    
  case SERVER:
    fileName << "server.log";
    break;

  case CLIENT0:
    fileName << "client_0.log";
    break;
  
  case CLIENT1:
    fileName << "client_1.log";
    break;

  }

  boost::log::add_file_log(boost::log::keywords::file_name = fileName.str(),
                           boost::log::keywords::format = formatString);

#endif // ENABLE_LOGGING

  //
  // server model package warehouse
  //
  ModelPackageWarehouse * modelPackageWarehouse = NULL;

  //
  // client communicator
  //
  arl::hms::Communicator * communicator = NULL;

  //
  // initialize addresses for socket communication
  //
  std::string socket0Address;
  std::string socket1Address;
  arl::hms::SocketCommunicator::SocketType socketType;  

  switch (communicatorType) {
  case SOCKETTCP: {
    socket0Address = "0.0.0.0:9000";
    socket1Address = "0.0.0.0:9001";
    socketType = arl::hms::SocketCommunicator::TCPIP;
    break;
  }
  case SOCKETUNIX: {
    socket0Address = "/tmp/socket0";
    socket1Address = "/tmp/socket1";
    socketType = arl::hms::SocketCommunicator::UNIX;
    break;
  }
  default:
    break;
  }
  
  //
  // initialize communicators
  //
  if(communicatorType == SOCKETTCP || communicatorType == SOCKETUNIX) {
  
    switch(communicatorMode) {

    case SERVER: {
      
      //
      // instantiate two communicators to clients
      //
      arl::hms::CommunicatorPointer 
        communicator0(new arl::hms::SocketCommunicator(socket0Address,
                                                       socketType,
                                                       true));
      arl::hms::CommunicatorPointer
        communicator1(new arl::hms::SocketCommunicator(socket1Address,
                                                       socketType,
                                                       true));
      
      //
      // instantiate model package warehouse
      //
      std::vector<arl::hms::CommunicatorPointer> communicators;
      communicators.push_back(communicator0);
      communicators.push_back(communicator1);
      modelPackageWarehouse = new ModelPackageWarehouse(communicators);
      break;

    }
    case CLIENT0: {
        
      //
      // instantiate socket communicator
      //
      communicator = 
        new arl::hms::SocketCommunicator(socket0Address,
                                         socketType,
                                         false);
      break;
    
    }
    case CLIENT1: {
      
      //
      // instantiate socket communicator
      //
      communicator = 
        new arl::hms::SocketCommunicator(socket1Address,
                                         socketType,
                                         false);
      break;

    }
    }

  }
  else {
    
#if defined(HAVE_MPI)

    MPI_Comm intercommunicator;
    
    if(isServer == false) {
      
      //
      // launch server from client
      //
      const char * arguments[5] = {"-m", "3", "-c", "3", NULL};
      int errorcodes;
      
      if(MPI_Comm_spawn(av[0],
                        const_cast<char **>(arguments),
                        1,
                        MPI_INFO_NULL,
                        0,
                        MPI_COMM_WORLD,
                        &intercommunicator,
                        &errorcodes) != MPI_SUCCESS) {
        const std::string message("Error spawning server from client");
        throw arl::hms::NetworkError(message);
      }

      //
      // instantiate MPICommunicator
      //
      communicator = new arl::hms::MPICommunicator(intercommunicator,
                                                   false,
                                                   0);

    } 
    else {

      //
      // find out how many clients are connecting
      //

      //
      // get parent communicator of the intercommunicator
      //
      MPI_Comm parentCommunicator;
      if(MPI_Comm_get_parent(&parentCommunicator) != MPI_SUCCESS) {
        const std::string message("Failure getting parent communicator");
        throw arl::hms::NetworkError(message);
      }

      //
      // check if parent communicator is NULL
      //
      if(parentCommunicator == MPI_COMM_NULL) {
        const std::string message("Parent communicator is NULL - this MPI "
                                  "process was not spawned");
        throw arl::hms::NetworkError(message);
      }
      
      //
      // determine number clients
      //
      int numberClients;
      if(MPI_Comm_remote_size(parentCommunicator,
                              &numberClients) != MPI_SUCCESS) {
        const std::string message("Failure getting remote size of "
                                  "intercommunicator");
        throw arl::hms::NetworkError(message);
      }

      //
      // instantiate communicators to clients
      //
      std::vector<arl::hms::CommunicatorPointer> communicators;
      for(int i=0; i<numberClients; ++i) {
        arl::hms::CommunicatorPointer 
          communicator(new arl::hms::MPICommunicator(parentCommunicator,
                                                     true,
                                                     i));
        communicators.push_back(communicator);
      }
      
      //
      // instantiate model package warehouse
      //
      modelPackageWarehouse = new ModelPackageWarehouse(communicators);

    }
    
#else
    
    //
    // MPI not available, throw IOError
    //
    const std::string message("MPI not compiled in - MPI communicators not "
                              "available");
    throw arl::hms::IOError(message);
    
#endif // HAVE_MPI    

  }

  //
  // send model packages from client to server
  //
  if(isServer) {
    
    //
    // continuously loop receiving model packages from client
    //
    while (true) {

      std::vector<arl::hms::ModelPackagePointer> modelPackages;
      try {
        modelPackages = modelPackageWarehouse->receive();
      }
      catch (arl::hms::ConnectionTerminationException & exception) {
#if defined(ENABLE_LOGGING)
        BOOST_LOG_TRIVIAL(info) << exception.what();
#endif // ENABLE_LOGGING
        break;
      }
      catch (arl::hms::NetworkError & exception) {
#if defined(ENABLE_LOGGING)
        BOOST_LOG_TRIVIAL(fatal) << exception.what();
#endif // ENABLE_LOGGING
        break;
      }

      //
      // iterate over received model package
      //
      std::vector<arl::hms::ModelPackagePointer>::const_iterator iter = 
        modelPackages.begin();
      const std::vector<arl::hms::ModelPackagePointer>::const_iterator 
        iterEnd = modelPackages.end();

      for( ; iter != iterEnd; ++iter) {
        
        //
        // obtain handle to model package
        //
        const arl::hms::ModelPackagePointer & modelPackage = *iter; 

        //
        // obtain handle to model
        //
        const arl::hms::ConstModelPointer & model = modelPackage->getModel();

        //
        // output model
        //
        std::cout << "Received: " << *model << std::endl;

        //
        // create value and attach to model package
        //
        const double val = std::atof(model->getPrefix().c_str());
        arl::hms::ValuePointer value(new arl::hms::ScalarValue(val));
        modelPackage->setValue(value);
 
      }
    
      //
      // send model packages back to client
      // 
      modelPackageWarehouse->ship(modelPackages);
         
    }
   
  }
  else {

    //
    // send 10 model packages to server
    //
    for(int i=0; i<10; ++i) {
      
      //
      // convert i to string
      //
      std::stringstream iString;
      iString << i;
      
      //
      // instantiate model
      //
      arl::hms::ModelPointer 
        model(new arl::hms::Model(iString.str(),
                                  "ls",
                                  std::vector<std::string>(1, "-l"),
                                  arl::hms::CPU,
                                  1));
      
      //
      // instantiate model package
      //
      arl::hms::ModelPackagePointer 
        modelPackage(new arl::hms::ModelPackage(model, 
                                                arl::hms::InputFilterPointer(),
                                                arl::hms::OutputFilterPointer(),
                                                arl::hms::ArgumentPointer()));

      //
      // send model package
      //
      communicator->send(modelPackage);

    }

    //
    // receive values back
    //
    int counter = 0;
    while(counter < 10) {

      //
      // receive model packages
      //
      std::vector<arl::hms::ModelPackagePointer> modelPackages = 
        communicator->receive<arl::hms::ModelPackagePointer>();
      
      //
      // increment counter
      //
      counter += modelPackages.size();
      
      //
      // iterate over received model package
      //
      std::vector<arl::hms::ModelPackagePointer>::const_iterator iter = 
        modelPackages.begin();
      const std::vector<arl::hms::ModelPackagePointer>::const_iterator 
        iterEnd = modelPackages.end();
      
      for( ; iter != iterEnd; ++iter) {
        
        //
        // obtain handle to model package
        //
        const arl::hms::ModelPackagePointer & modelPackage = *iter; 

        //
        // obtain handle to value
        //
        const arl::hms::ValuePointer value = modelPackage->getValue(); 

        //
        // obtain handle to value
        //
        const boost::shared_ptr<arl::hms::ScalarValue> scalarValue = 
          boost::dynamic_pointer_cast<arl::hms::ScalarValue>(value);
        
        //
        // output value
        //
        std::cout << "Received: " << scalarValue->getValue() << std::endl;

      }

    }
             
  }
  
  //
  // cleanup
  //
  delete communicator;
  delete modelPackageWarehouse;

#if defined(HAVE_MPI)
  
  MPI_Finalize();

#endif // HAVE_MPI

  //
  //
  //
  return 0;

}
