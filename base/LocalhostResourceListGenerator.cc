//
// File:      LocalhostResourceListGenerator.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "LocalhostResourceListGenerator.h"

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#else
#error unistd.h header file not available
#endif // HAVE_UNISTD_H

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    LocalhostResourceListGenerator::LocalhostResourceListGenerator()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    LocalhostResourceListGenerator::~LocalhostResourceListGenerator()
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
    LocalhostResourceListGenerator::generate(const ResourceType & resourceType) const
    {

      //
      // get hostname of local system
      //
      char hostname[1024];
      hostname[1023] = '\0';
      gethostname(hostname, 1023);
          
      //
      // get number of processors of local system
      //
      long numberProcessors = sysconf(_SC_NPROCESSORS_ONLN);
      
      //
      // instantiate container of resources
      //
      ResourceList::Resources resources(numberProcessors,
                                        hostname);
                  
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
