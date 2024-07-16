//
// File:      ConfigReader.cc
// Package    comm
//
// Package for handling communication between individual components.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#if defined(HAVE_MPI)
#include <mpi.h>
#endif // HAVE_MPI

#include "ConfigReader.h"

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

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    ConfigReader::ConfigReader()
    {
      
      //
      //
      //
      return;

    }
 
    //
    // Destructor.
    //
    ConfigReader::~ConfigReader()
    {
      
      //
      //
      //
      return;

    }

    //
    // Read config file from parent.
    //
    std::string 
    ConfigReader::read() const
    {
      
      //
      // read configFile from stdin 
      //
      std::stringstream configFile;
      
#if defined(HAVE_MPI)
      
      //
      // check for parent communicator - if it exists, we launched using MPI
      // and can receive config file on that communicator
      //
      MPI_Comm parent;
      MPI_Comm_get_parent(&parent);
      
      if(parent != MPI_COMM_NULL) {

        //
        // read config file from parent communicator
        //
        MPI_Status status;
        MPI_Probe(0,
                  0,
                  parent,
                  &status);
        int count;
        MPI_Get_count(&status,
                      MPI_CHAR,
                      &count);
        char * recvBuffer = new char[count];
        MPI_Recv(recvBuffer,
                 count,
                 MPI_CHAR,
                 0,
                 0,
                 parent,
                 MPI_STATUS_IGNORE);
        configFile << recvBuffer;
        delete [] recvBuffer;
        
      }
      else {
        
#endif // HAVE_MPI
        
        //
        // read config file from stdin
        //
        std::string line;
        while(std::getline(std::cin, line)) {
          configFile << line << std::endl;
        }
        
#if defined(HAVE_MPI)
      }
#endif // HAVE_MPI

      //
      //
      //
      return configFile.str();
      
    }

  }
   
}
