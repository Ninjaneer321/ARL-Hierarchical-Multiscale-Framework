//
// File:      CleanupPolicy.cc
// Package    broker
//
// HMS Brokers.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "CleanupPolicy.h"

#if defined(HAVE_BOOST_FILESYSTEM)
#include <boost/filesystem.hpp>
#else
#error boost filesystem not available
#endif // HAVE_BOOST_FILESYSTEM

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    CleanupPolicy::CleanupPolicy()
    {
      
      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    CleanupPolicy::~CleanupPolicy()
    {
      
      //
      //
      //
      return;

    }

    //
    // Helper function to remove a directory.
    //
    void
    CleanupPolicy::removeDirectory(const std::string & directory) const
    {

      //
      // Remove contents of the directory
      //
      boost::filesystem::remove_all(directory);

      //
      //
      //
      return;
      
    }
    
  }
}

