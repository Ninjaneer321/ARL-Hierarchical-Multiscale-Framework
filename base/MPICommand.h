//
// File:      MPICommand.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_MPICommand_h)
#define base_MPICommand_h

#include "Command.h"
#include "MachinefileGenerator.h"

#include <string>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief MPICommand - generates a shell scrip to execute an
     * MPI-based model.
     */
    class MPICommand : public Command {

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
       * @param modelPackage ModelPackage associated with the command.
       * @param directory Path of directory to use for Model execution.
       * @param statusPredicates List of status predicates - used to
       * determine whether command is running.  
       * @param endPredicates List of end predicates - used to
       * determine whether command completed successfully.
       * @param machinefileGenerator Generator to build machinefiles
       * in a particular format.
       */
      MPICommand(const ModelPackagePointer                 & modelPackage,
		 const std::string                         & directory,
		 const std::vector<StatusPredicatePointer> & statusPredicates,
                 const std::vector<EndPredicatePointer>    & endPredicates,
                 const ConstMachinefileGeneratorPointer    & machinefileGenerator);
       
      /**
       * Destructor.
       */
      virtual ~MPICommand();

      /**
       * Execute the command.
       */
      virtual void execute();

      /**
       * Generate a shell script in directory to execute Model.
       */
      virtual void generate() const;

      /**
       * Get status of running command.
       *
       * @return status of running command.
       */
      virtual CommandStatus getStatus();

      /**
       * Set resources command will use for execution.
       *
       * @param resources List of resources command will use for execution.
       */
      virtual void setResources(const ResourceListPointer & resources);
      
      /**
       * Terminate executing command.
       */
      virtual void terminate();

      //
      // methods
      // 
    protected:

    private:
      //
      // copy constructor/assignment operator
      //
      MPICommand(const MPICommand &); // not implemented
      MPICommand & operator=(const MPICommand &); // not implemented

      //
      // data
      //
    private:

      const ConstMachinefileGeneratorPointer d_machinefileGenerator;
      pid_t                                  d_runningProcess;
      std::vector<char *>                    d_environment;

    };

  }
}

#endif // base_MPICommand_h
