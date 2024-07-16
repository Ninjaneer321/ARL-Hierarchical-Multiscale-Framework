//
// File:      ResourceList.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "ResourceList.h"

#include <exception/InvalidArgument.h>

#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iostream header file not available
#endif

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
    ResourceList::ResourceList(const ResourceType & resourceType,
                               const Resources    & resources) :
      d_resourceType(resourceType),
      d_resources(resources),
      d_totalAvailableResources(resources.size())
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    ResourceList::~ResourceList()
    {

      //
      //
      //
      return;

    }

    //
    // Check-In (return) resources to list.
    //
    void 
    ResourceList::checkIn(const ResourceListPointer & resourceList)
    {

      //
      // ensure that resourceTypes are consistent
      //
      if(resourceList->d_resourceType != d_resourceType) {
        const std::string message("Checking in resources of a different type");
        throw InvalidArgument(message);
      }
      
      //
      // insert resources back into resource list.
      //
      d_resources.insert(d_resources.end(),
                         resourceList->d_resources.begin(),
                         resourceList->d_resources.end());

      //
      // clear resourceList to avoid resource duplication
      //
      resourceList->d_resources.clear();

      //
      //
      //
      return;

    }

    //
    // Check-out (obtain) resources from list. If there is an
    // insufficient amount of resources, return an empty list.
    //
    ResourceListPointer
    ResourceList::checkOut(unsigned int amount)
    {

      //
      // check if amount is greater than total possible amount
      //
      if(amount > d_totalAvailableResources) {
        std::stringstream messageStream;
        messageStream << "Requesting more resources than possibly available:" 
                      << " Requested: " << amount 
                      << " Total Available: " << d_totalAvailableResources;
        const std::string message(messageStream.str());
        throw InvalidArgument(message);
      }
      
      //
      // checkout resources if available
      //
      Resources checkedOutResources;
      if(amount <= d_resources.size()) {

        //
        // get iterators to range of resources
        // 
        Resources::iterator checkedOutBegin = d_resources.end() - amount;
        Resources::iterator checkedOutEnd = d_resources.end();

        //
        // copy resources into checkedOutResources
        //
        checkedOutResources.insert(checkedOutResources.begin(),
                                   checkedOutBegin,
                                   checkedOutEnd);

        //
        // remove checked-out resources from resources
        //
        d_resources.erase(checkedOutBegin,
                          checkedOutEnd);
      
      }

      //
      //
      //
      return ResourceListPointer(new ResourceList(d_resourceType,
                                                  checkedOutResources));

    }

    //
    // Get resources held by this list.
    //
    const ResourceList::Resources & 
    ResourceList::getResources() const
    {

      //
      //
      //
      return d_resources;

    }

    //
    // Get resource type held by this list.
    //
    const ResourceType & 
    ResourceList::getType() const
    {
      
      //
      //
      //
      return d_resourceType;

    }

    //
    // Output operator.
    //
    std::ostream &
    operator<<(std::ostream       & outputStream,
               const ResourceList & resourceList)
    {
      
      //
      // output resourceType
      //
      outputStream << resourceList.d_resourceType << ": [";

      //
      // output resources
      //
      ResourceList::Resources::const_iterator iter = 
        resourceList.d_resources.begin();
      const ResourceList::Resources::const_iterator iterEnd = 
        resourceList.d_resources.end();
      
      for( ; iter != iterEnd; ++iter) {
        
        outputStream << " " << *iter;
        
      }
      outputStream << " ]";
      
      //
      //
      //
      return outputStream;

    }
  
  }

}
