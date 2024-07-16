//
// File:      ResourceListGenerator.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_ResourceListGenerator_h)
#define base_ResourceListGenerator_h

#include "ResourceList.h"
#include "ResourceType.h"

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class for resource list generator.
     */
    class ResourceListGenerator {

      //
      // types
      //
    public:

      //
      // methods
      //
    public:

      /**
       * Destructor.
       */
      virtual ~ResourceListGenerator() = 0;

      /**
       * Generate resource of type resourceType.
       *
       * @param resourceType Type of generated resource
       */
      virtual ResourceListPointer 
      generate(const ResourceType & resourceType) const = 0;
  
      //
      // methods
      // 
    protected:

      /**
       * Constructor.
       */
      ResourceListGenerator();

    private:
      //
      // copy constructor/assignment operator
      //
      ResourceListGenerator(const ResourceListGenerator &); // not implemented
      ResourceListGenerator & operator=(const ResourceListGenerator &); // not implemented

      //
      // data
      //
    private:

    };

  }
}

#endif // base_ResourceListGenerator_h
