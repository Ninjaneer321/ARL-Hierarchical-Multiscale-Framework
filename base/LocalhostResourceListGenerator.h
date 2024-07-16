//
// File:      LocalhostResourceListGenerator.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_LocalhostResourceListGenerator_h)
#define base_LocalhostResourceListGenerator_h

#include "ResourceListGenerator.h"

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Resource list generator for localhost.
     */
    class LocalhostResourceListGenerator : public ResourceListGenerator {

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
      LocalhostResourceListGenerator();

      /**
       * Destructor.
       */
      virtual ~LocalhostResourceListGenerator();

      /**
       * Generate resource of type resourceType.
       *
       * @param resourceType Type of generated resource
       */
      virtual ResourceListPointer 
      generate(const ResourceType & resourceType) const;

    private:
      //
      // copy constructor/assignment operator
      //
      LocalhostResourceListGenerator(const LocalhostResourceListGenerator &); // not implemented
      LocalhostResourceListGenerator & operator=(const LocalhostResourceListGenerator &); // not implemented

      //
      // data
      //
    private:

    };

  }
}

#endif // base_LocalhostResourceListGenerator_h
