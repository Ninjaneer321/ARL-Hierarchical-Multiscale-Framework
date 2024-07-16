//
// File:      CleanupPolicyCustom.cc
// Package    broker
//
// HMS Brokers.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "CleanupPolicyCustom.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    CleanupPolicyCustom::CleanupPolicyCustom()
    {
      
      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    CleanupPolicyCustom::~CleanupPolicyCustom()
    {
      
      //
      //
      //
      return;

    }

    //
    // Apply policy to cleanup command directories.
    //
    void
    CleanupPolicyCustom::apply(const std::vector<CommandPointer> & completedCommands)
    {
      
      for(std::vector<CommandPointer>::const_iterator iter =
	    completedCommands.begin(); iter != completedCommands.end();
	  ++iter) {
	const CommandPointer & commandPointer = *iter;
	const ArgumentPointer & argument =
	  commandPointer->getModelPackage()->getArgument();
	if(argument->getToCleanup()) {
	  const std::string & directory = commandPointer->getDirectory();
	  this->removeDirectory(directory);
	}

      }
	
      //
      //
      //
      return;
	
    }
    
  }
}

