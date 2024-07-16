//
// File:      MachinefileGenerator.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_MachinefileGenerator_h)
#define base_MachinefileGenerator_h

#include "ResourceList.h"

#include <utils/HMSMacros.h>

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class implementing MachinefileGenerator - generates
     * a machinefile in a particular format given a resourceList.
     */
    class MachinefileGenerator {

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
      virtual ~MachinefileGenerator() = 0;

      /**
       * Generate a machinefile from a resourceList.
       *
       * @param machinefilePath Path to machinefile to generate.
       * @param resourceList List of resources used to populate machinefile.
       */
      virtual void generate(const std::string              & machinefilePath,
                            const ConstResourceListPointer & resourceList) const = 0;
  
      //
      // methods
      // 
    protected:

      /**
       * Constructor.
       */
      MachinefileGenerator();

    private:
      //
      // copy constructor/assignment operator
      //
      MachinefileGenerator(const MachinefileGenerator &); // not implemented
      MachinefileGenerator & operator=(const MachinefileGenerator &); // not implemented

      //
      // data
      //
    private:
  
    };

    HMS_SHARED_PTR(MachinefileGenerator);

  }
}

#endif // base_MachinefileGenerator_h
