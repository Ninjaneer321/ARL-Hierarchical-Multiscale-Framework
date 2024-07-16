//
// File:      PBSCommand.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_PBSCommand_h)
#define base_PBSCommand_h

#include "Command.h"
#include "MachinefileGenerator.h"

#include <string>
//#include <string.h>
#include <vector>

//
//
//
namespace arl {
  namespace hms {

    /**
     * @brief PBSCommand - generates a shell script to execute an
     * MPI-based model.
     */
    class PBSCommand : public Command {

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
       */
      PBSCommand(const ModelPackagePointer                 & modelPackage,
                 const std::string                         & directory,
                 const std::vector<StatusPredicatePointer> & statusPredicates,
                 const std::vector<EndPredicatePointer>    & endPredicates);
       
      /**
       * Destructor.
       */
      virtual ~PBSCommand();

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
      PBSCommand(const PBSCommand &); // not implemented
      PBSCommand & operator=(const PBSCommand &); // not implemented

      //
      // data
      //
    private:
      
      pid_t                                  d_runningProcess;
      std::vector<char *>                    d_environment;
      const std::string                      d_pbsErrorPath;
      const std::string                      d_pbsOutputPath;

    };

  }
}

#endif // base_PBSCommand_h
