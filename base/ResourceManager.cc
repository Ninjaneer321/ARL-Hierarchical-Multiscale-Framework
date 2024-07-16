//
// File:      ResourceManager.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "ResourceManager.h"

#include <exception/DomainError.h>

#if defined(ENABLE_LOGGING)
#if defined(HAVE_BOOST_LOG)
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#else
#error boost logging not available
#endif // HAVE_BOOST_LOG
#endif // ENABLE_LOGGING

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    ResourceManager::ResourceManager()
    {
      
      //
      //
      //
      return;
      
    }

    //
    // Destructor.
    //
    ResourceManager::~ResourceManager()
    {

      //
      //
      //
      return;

    }

    //
    // Add a resourceList to the resource manager.
    //
    void 
    ResourceManager::add(const ResourceListPointer & resourceList)
    {

#if defined(ENABLE_LOGGING)
      BOOST_LOG_TRIVIAL(info) << "Adding resources to resource manager " 
                              << *resourceList;
#endif // ENABLE_LOGGING          

      //
      // add resourceList to resources
      //
      d_resourceLists[resourceList->getType()] = resourceList;

      //
      //
      //
      return;
      
    }

    //
    // Check-In (return) resources to manager.
    //
    void 
    ResourceManager::checkIn(const ResourceListPointer & resourceList)
    {

      //
      // get resource list to check in to by type
      //
      const std::map<ResourceType, ResourceListPointer>::iterator iter = 
        d_resourceLists.find(resourceList->getType());

      //
      // check whether resource type has been found
      //
      if(iter == d_resourceLists.end()) {
        
        //
        // type not found, throw an exception
        //
        const std::string message("Resource type not managed by resource "
                                  "manager.");
        throw DomainError(message);

      }

      //
      // check in resources
      //
      iter->second->checkIn(resourceList);

      //
      //
      //
      return;

    }

    //
    // Check-out (obtain) resources from manager. If there is an
    // insufficient amount of resources, return an empty list.
    //
    ResourceListPointer 
    ResourceManager::checkOut(const ResourceType & type,
                              int                  amount)
    {

      //
      // get resource list to check out from by type
      //
      const std::map<ResourceType, ResourceListPointer>::iterator iter = 
        d_resourceLists.find(type);

      //
      // check whether resource type has been found
      //
      if(iter == d_resourceLists.end()) {
        
        //
        // type not found, throw an exception
        //
        const std::string message("Resource type not managed by resource "
                                  "manager.");
        throw DomainError(message);

      }
      
      //
      // check out resources
      //
      ResourceListPointer resources = iter->second->checkOut(amount);

      //
      //
      //
      return resources;

    }
 
    //
    // Check whether any resources are available for use.
    //
    bool 
    ResourceManager::hasResourcesAvailable() const
    {

      //
      // iterate over resource lists checking whether any resources
      // are available.
      //
      for(std::map<ResourceType, ResourceListPointer>::const_iterator iter = 
            d_resourceLists.begin(); iter != d_resourceLists.end(); ++iter) {
      
        //
        // get handle to resourceList
        //
        const ResourceListPointer & resourceList = iter->second;

        //
        // get handle to resources
        //
        const ResourceList::Resources & resources = 
          resourceList->getResources();
        
        //
        // check whether we have resources in this list
        //
        if(!resources.empty()) {
          
          //
          //
          //
          return true;

        }

      }

      //
      // iterated over all resource and found none available, return false
      //
      return false;

    }

    //
    // Get number of free resources.
    //
    unsigned int 
    ResourceManager::getNumberFreeResources() const
    {

      //
      // instantiate returnValue
      //
      unsigned int returnValue = 0;

      //
      // iterate over resource lists checking whether any resources
      // are available.
      //
      for(std::map<ResourceType, ResourceListPointer>::const_iterator iter = 
            d_resourceLists.begin(); iter != d_resourceLists.end(); ++iter) {
      
        //
        // get handle to resourceList
        //
        const ResourceListPointer & resourceList = iter->second;

        //
        // get handle to resources
        //
        const ResourceList::Resources & resources = 
          resourceList->getResources();
        
        //
        // accumulate in returnValue
        //
        returnValue += resources.size();

      }

      //
      //
      //
      return returnValue;

    }
   
    //
    // Output operator
    //
    std::ostream & 
    operator<<(std::ostream          & outputStream,
               const ResourceManager & resourceManager)
    {

      //
      // iterate over each list held by this manager and output
      //
      std::map<ResourceType, ResourceListPointer>::const_iterator iter = 
        resourceManager.d_resourceLists.begin();
      const std::map<ResourceType, ResourceListPointer>::const_iterator 
        iterEnd = resourceManager.d_resourceLists.end();        
      
      for( ; iter != iterEnd; ++iter) {
      
        //
        // output each resourceList
        //
        outputStream << *(iter->second) << std::endl;

      }
      
      //
      //
      //
      return outputStream;

    }
 
  }

}      

