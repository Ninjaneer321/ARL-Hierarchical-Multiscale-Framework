//
// File:      ResourceList.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_ResourceList_h)
#define base_ResourceList_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

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

#include <string>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing a list (set) of various
     * resources. Resources on the list are characterized by having
     * the same property (i.e.  processor type, amount of memory,
     * etc.).
     */
    class ResourceList {

      //
      // types
      //
    public:

      typedef std::vector<std::string> Resources; 

      //
      // methods
      //
    public:
      
      /**
       * Constructor.
       *
       * resourceType Type of resource the list holds.
       * resources Available resources held by this list.
       */
      ResourceList(const ResourceType        & resourceType,
                   const Resources           & resources);

      /**
       * Destructor.
       */
      virtual ~ResourceList();

      /**
       * Check-In (return) resources to list.
       *
       * @param resourceList List of resources to return.
       */
      void checkIn(const boost::shared_ptr<ResourceList> & resourceList);

      /**
       * Check-out (obtain) resources from list. If there is an
       * insufficient amount of resources, return an empty list.
       *
       * @param amount Amount of resources to obtain.
       * @return Checked out resources.
       */
      boost::shared_ptr<ResourceList> checkOut(unsigned int amount);

      /**
       * Get resources held by this list.
       *
       * @return List of resources held by this list.
       */
      const Resources & getResources() const;

      /**
       * Get resource type held by this list.
       *
       * @return Type of resources held by this list.
       */
      const ResourceType & getType() const;
  
      //
      // methods
      // 
    protected:

    private:
      friend std::ostream & operator<<(std::ostream       & outputStream,
                                       const ResourceList & resourceList);


      //
      // copy constructor/assignment operator
      //
      ResourceList(const ResourceList &); // not implemented
      ResourceList & operator=(const ResourceList &); // not implemented

      //
      // data
      //
    private:
      
      ResourceType  d_resourceType;
      Resources     d_resources;
      unsigned int  d_totalAvailableResources;

    };

    /**
     * Output operator.
     *
     * @param outputStream Output stream to use for output.
     * @param resourceList ResourceList object.
     *
     * @return Updated output stream.
     */
    std::ostream &
    operator<<(std::ostream       & outputStream,
               const ResourceList & resourceList);

    HMS_SHARED_PTR(ResourceList);

  }
}

#endif // base_ResourceList_h
