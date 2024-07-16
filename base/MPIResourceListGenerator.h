//
// File:      MPIResourceListGenerator.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_MPIResourceListGenerator_h)
#define base_MPIResourceListGenerator_h

#include "ResourceListGenerator.h"

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Resource list generator for MPI machinefiles.
     */
    class MPIResourceListGenerator : public ResourceListGenerator {

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
       *
       * @param machinefilePath Path to MPI machinefile used to
       * populate resource list.
       */
      MPIResourceListGenerator(const std::string & machinefilePath);

      /**
       * Destructor.
       */
      virtual ~MPIResourceListGenerator();

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
      MPIResourceListGenerator(const MPIResourceListGenerator &); // not implemented
      MPIResourceListGenerator & operator=(const MPIResourceListGenerator &); // not implemented

      //
      // data
      //
    private:

      const std::string d_machinefilePath;

    };

  }
}

#endif // base_MPIResourceListGenerator_h
