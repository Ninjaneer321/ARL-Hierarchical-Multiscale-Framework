//
// File:      ResourceManager.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_ResourceManager_h)
#define base_ResourceManager_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "ResourceList.h"
#include "ResourceType.h"

#include <utils/HMSMacros.h>

#if defined(HAVE_IOSFWD)
#include <iosfwd>
#else
#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iosfwd or iostream header file not available
#endif // HAVE_IOSTREAM
#endif // HAVE_IOSFWD

#include <map>
#include <string>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing a list of resource list. Each
     * resource list contains resources of a particular type.
     */
    class ResourceManager {

      //
      // types
      //
    public:

      //
      // methods
      //
    public:
      
      /**
       * Constructor.
       */
      ResourceManager();

      /**
       * Destructor.
       */
      ~ResourceManager();

      /**
       * Add a resourceList to the resource manager.
       *
       * @param resourceList A list of resources to add to the manager.
       */
      void add(const ResourceListPointer & resourceList); 

      /**
       * Check-In (return) resources to manager.
       *
       * @param resourceList List of resources to return.
       */
      void checkIn(const ResourceListPointer & resourceList);

      /**
       * Check-out (obtain) resources from manager. If there is an
       * insufficient amount of resources, return an empty list.
       *
       * @param type Type of resource to obtain.
       * @param amount Amount of resources to obtain.
       *
       * @return Checked out resources.
       */
      ResourceListPointer checkOut(const ResourceType & type,
                                   int                  amount);

      /**
       * Check whether any resources are available for use.
       *
       * @return bool indicating whether resources are available for use.
       */
      bool hasResourcesAvailable() const;

      /**
       * Get number of free resources.
       *
       * @return number of free resources available for use.
       */
      unsigned int getNumberFreeResources() const;

      //
      // methods
      // 
    protected:

    private:

      //
      // output operator
      //
      friend std::ostream & operator<<(std::ostream          & outputStream,
                                       const ResourceManager & resourceManager);
      
      //
      // copy constructor/assignment operator
      //
      ResourceManager(const ResourceManager &); // not implemented
      ResourceManager & operator=(const ResourceManager &); // not implemented
      
      //
      // data
      //
    private:
      
      std::map<ResourceType, ResourceListPointer> d_resourceLists;
      
    };

    HMS_SHARED_PTR(ResourceManager);

    /**
     * Output operator.
     *
     * @param outputStream Output stream to use for output.
     * @param resourceManager ResourceManager object.
     *
     * @return Updated output stream.
     */
    std::ostream &
    operator<<(std::ostream          & outputStream,
               const ResourceManager & resourceManager);

  }
}

#endif // base_ResourceManager_h
