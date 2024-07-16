//
// File:      StartBroker.cc
// Package    broker
//
// HMS Brokers.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#if defined(HAVE_MPI)
#include <mpi.h>
#endif // HAVE_MPI

#include "Broker.h"
#include "BrokerFactory.h"

#include <base/Logger.h>
#include <comm/ConfigReader.h>
#include <exception/OSError.h>

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

#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iostream header file not available
#endif // HAVE_IOSTREAM

#if defined(HAVE_STRING)
#include <string>
#else
#error string header file not available
#endif // HAVE_STRING

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

#if defined(HAVE_VECTOR)
#include <vector>
#else
#error vector header file not available
#endif // HAVE_VECTOR

//
// local functions
//
namespace {
  
  //
  // print usage
  //
  inline void
  printUsage(const char * programName)
  {
    
    std::cerr << "usage: " << programName << " [broker id] "
              << "[list of client ranks broker communicates with]" 
              << std::endl;
    
    //
    //
    //
    return;
    
  }
  
  //
  // process command line
  //
  void
  processCommandLine(int              & brokerId,
                     std::vector<int> & communicatorRanks,
                     int                ac,
                     char             * av[])
  {

    int c;
    bool errorFlag = false;
    
    while( (c=getopt(ac, av, "")) != -1 )
      switch(c){
        
      case '?':
        errorFlag = true;
        break;
        
      }
    
    if(optind > ac) {
      errorFlag = true;
    }
    
    if(ac == 3) {
      brokerId = std::atoi(av[1]);
      char * token = strtok(av[2], ",");
      while(token) {
        communicatorRanks.push_back(std::atoi(token));
        token = strtok(NULL, ",");
      }
    }
    else {
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

int main(int    ac,
         char * av[])
{

#if defined(HAVE_MPI)
  
  MPI_Init(&ac,
           &av);
   
#endif // HAVE_MPI

  //
  // process command line arguments
  //
  int brokerId;
  std::vector<int> communicatorRanks;
  processCommandLine(brokerId,
                     communicatorRanks,
                     ac,
                     av);

  /*
  pid_t process_id = 0; 
  
  //
  // create the child process
  //
  process_id = fork();

  //
  // check if fork failed
  //
  if (process_id < 0) {
    const std::string message("Fork to create child process failed");
    throw arl::hms::OSError(message);
  } 

  if (process_id > 0) {

#if defined(HAVE_MPI)    
    MPI_Finalize();
#endif // HAVE_MPI

    //
    // kill parent process
    //
    exit(0);
 
  }
*/

  //
  // read config file
  //
  arl::hms::ConfigReader reader;
  const std::string configFile = reader.read();

  //
  // initialize logging
  //
#if defined(ENABLE_LOGGING)
  std::stringstream configFileStream;
  configFileStream << configFile;
  arl::hms::Logger logger(configFileStream);
#endif // ENABLE_LOGGING

  //
  // Instantiate Broker from config file 
  //
  arl::hms::BrokerPointer broker = 
    arl::hms::BrokerFactory().build(configFile,
                                    brokerId,
                                    communicatorRanks);

  //
  // Redirect stdout and stderr to /dev/null
  //
  freopen("/dev/null", 
          "a", 
          stdout);
  /*
  freopen("/dev/null", 
          "a", 
          stderr);
  */
  
  //sleep(10);
      
  //
  // begin broker
  //
  broker->run();

  //
  //
  //
  return 0;

}
