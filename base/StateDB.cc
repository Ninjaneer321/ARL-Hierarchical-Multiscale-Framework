//
// File:      StateDB.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "StateDB.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    StateDB::StateDB()
    {

      //
      //
      //
      return;
      
    }

    //
    // Destructor.
    //
    StateDB::~StateDB()
    {

      //
      //
      //
      return;

    }
  
    //
    // Get queued commands.
    //
    StateDB::Queue & 
    StateDB::getQueued()
    {

      //
      //
      //
      return d_queued;

    }

    //
    // Get queued commands.
    //
    const StateDB::Queue & 
    StateDB::getQueued() const
    {

      //
      //
      //
      return d_queued;

    }

    //
    // Get running commands.
    //
    StateDB::Queue & 
    StateDB::getRunning()
    {

      //
      //
      //
      return d_running;

    }

    //
    // Get running commands.
    //
    const StateDB::Queue & 
    StateDB::getRunning() const
    {

      //
      //
      //
      return d_running;

    }

    //
    // Get completed commands.
    //
    StateDB::Queue & 
    StateDB::getCompleted()
    {

      //
      //
      //
      return d_completed;

    }

    //
    // Get completed commands.
    //
    const StateDB::Queue & 
    StateDB::getCompleted() const
    {

      //
      //
      //
      return d_completed;

    }

    //
    // Output operator.
    //
    std::ostream & operator<<(std::ostream  & outputStream,
                              const StateDB & stateDB)
    {

      //
      // output queued commands
      //
      outputStream << "Queued: " << std::endl << stateDB.getQueued() 
                   << std::endl;
      
      //
      // output running commands
      //
      outputStream << "Running: " << std::endl << stateDB.getRunning()
                   << std::endl;
      
      //
      // output completed commands
      //
      outputStream << "Completed: " << std::endl << stateDB.getCompleted() 
                   << std::endl;
      
      //
      //
      //
      return outputStream;

    }
    
  }
}

