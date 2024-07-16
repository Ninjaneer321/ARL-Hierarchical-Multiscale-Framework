//
// File:      MPIResourceListGenerator.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "MPIResourceListGenerator.h"

#include <exception/IOError.h>

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

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    MPIResourceListGenerator::MPIResourceListGenerator(const std::string & machinefilePath) :
      d_machinefilePath(machinefilePath)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    MPIResourceListGenerator::~MPIResourceListGenerator()
    {
      
      //
      //
      //
      return;

    }

    //
    // Generate resource of type resourceType.
    //
    ResourceListPointer 
    MPIResourceListGenerator::generate(const ResourceType & resourceType) const
    {

      //
      // instantiate container of resources
      //
      ResourceList::Resources resources;
      
      //
      // read machinefile
      //
      std::ifstream machinefile; 
      machinefile.open(d_machinefilePath.c_str());
      if(!machinefile) {
        std::stringstream message;
        message << "Error opening " << d_machinefilePath
                << " when generating ResourceList";
        throw IOError(message.str());
      }

      //
      // parse machinefile filling resources
      //
      std::string line;
      while(std::getline(machinefile, line)) {
        
        //
        // ignore empty lines and comments
        //
        if(line.size() > 0 && strncmp(&(line[0]), "#", 1) != 0) {
          
          //
          // parse line to determine if slots= syntax is present
          //
          std::stringstream lineStream(line);

          //
          // host is always first token
          //
          std::string host;
          lineStream >> host;
          
          //
          // search for hosts= token
          //
          std::string token;
          int numSlots = 1;
          while(lineStream >> token) {
            if(token.size() > 6 && token.substr(0, 6).compare("slots=") == 0) {
              numSlots = std::atoi(token.substr(6).c_str());
              break;
            }
          }

          //
          // add to resources 
          //
          for(int i=0; i<numSlots; ++i) {
            resources.push_back(host);
          }
        }
      }
      
      //
      // instantiate resource list
      //
      ResourceListPointer resourceList(new ResourceList(resourceType,
                                                        resources));
      
      //
      //
      //
      return resourceList;

    }

  }
}
