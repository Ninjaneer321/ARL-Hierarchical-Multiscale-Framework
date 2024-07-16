//
// File:      CleanupPolicyFixed.cc
// Package    broker
//
// HMS Brokers.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "CleanupPolicyFixed.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    CleanupPolicyFixed::CleanupPolicyFixed(int numberToKeep) :
      d_numberToKeep(numberToKeep)
    {
      
      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    CleanupPolicyFixed::~CleanupPolicyFixed()
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
    CleanupPolicyFixed::apply(const std::vector<CommandPointer> & completedCommands)
    {

      for(std::vector<CommandPointer>::const_iterator iter =
	    completedCommands.begin(); iter != completedCommands.end();
	  ++iter) {
	const CommandPointer & commandPointer = *iter;
	const std::string & directory = commandPointer->getDirectory();
	d_completedDirectories.push_back(directory);
      }

      const int numberToRemove = d_completedDirectories.size() - d_numberToKeep;
      if(numberToRemove > 0) {
	for(int i=0; i<numberToRemove; ++i) {
	  this->removeDirectory(d_completedDirectories[i]);
	}
	d_completedDirectories.erase(d_completedDirectories.begin(),
				     d_completedDirectories.begin() + numberToRemove);
      }
      
      //
      //
      //
      return;
      
    }
    
  }
}

