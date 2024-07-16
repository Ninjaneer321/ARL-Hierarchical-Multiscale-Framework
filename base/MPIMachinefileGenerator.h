//
// File:      MPIMachinefileGenerator.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_MPIMachinefileGenerator_h)
#define base_MPIMachinefileGenerator_h

#include "MachinefileGenerator.h"

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Generates a MPI machinefile given a resourceList.
     */
    class MPIMachinefileGenerator : public MachinefileGenerator {

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
      MPIMachinefileGenerator();

      /**
       * Destructor.
       */
      virtual ~MPIMachinefileGenerator();

      /**
       * Generate a machinefile from a resourceList.
       *
       * @param machinefilePath Path to machinefile to generate.
       * @param resourceList List of resources used to populate machinefile.
       */
      virtual void generate(const std::string              & machinefilePath,
                            const ConstResourceListPointer & resourceList) const;
  
    private:
      //
      // copy constructor/assignment operator
      //
      MPIMachinefileGenerator(const MPIMachinefileGenerator &); // not implemented
      MPIMachinefileGenerator & operator=(const MPIMachinefileGenerator &); // not implemented

      //
      // data
      //
    private:
  
    };

  }
}

#endif // base_MPIMachinefileGenerator_h
