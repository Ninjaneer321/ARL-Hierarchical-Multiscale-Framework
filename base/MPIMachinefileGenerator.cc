//
// File:      MachinefileGenerator.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "MPIMachinefileGenerator.h"
#include "ResourceList.h"

#include <exception/IOError.h>

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

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    MPIMachinefileGenerator::MPIMachinefileGenerator()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    MPIMachinefileGenerator::~MPIMachinefileGenerator()
    {

      //
      //
      //
      return;

    }

    //
    // Generate a machinefile from a resourceList.
    //
    void 
    MPIMachinefileGenerator::generate(const std::string & machinefilePath,
                                        const ConstResourceListPointer & resourceList) const
    {

      //
      // open file for writing
      //
      std::ofstream outputFile;
      outputFile.open(machinefilePath.c_str(), std::ios::out);

      //
      // check whether file was successfully opened
      //
      if(outputFile.is_open() == false) {
        const std::string message("Failed to open machinefile for writing");
        throw IOError(message);
      }
      
      //
      // get resources from resourceList
      //
      const ResourceList::Resources & resources = resourceList->getResources();
      
      //
      // iterate over resources, writing to file
      //
      ResourceList::Resources::const_iterator iter = resources.begin();
      const ResourceList::Resources::const_iterator iterEnd = resources.end();
      for( ; iter != iterEnd; ++iter) {
        outputFile << *iter << std::endl;
      }
      
      //
      // close machinefile
      //
      outputFile.close();
      
      //
      //
      //
      return;

    }

  }
}
