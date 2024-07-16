//
// File:      Command.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_Command_h)
#define base_Command_h

#include "CommandStatus.h"
#include "Model.h"
#include "ModelPackage.h"
#include "Priority.h"
#include "ResourceList.h"
#include "ResourceType.h"
#include "predicate/EndPredicate.h"
#include "predicate/StatusPredicate.h"

#include <utils/HMSMacros.h>

#include <ctime>
#include <string>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class implementing Command - generates a shell
     * script to execute Model.
     */
    class Command {

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
      virtual ~Command() = 0;

      /**
       * Execute the command.
       */
      virtual void execute() = 0;

      /**
       * Generate a shell script in directory to execute Model.
       */
      virtual void generate() const = 0;

      /**
       * Obtain directory that command is executed in.
       *
       * @return string containing the directory path where the
       * command is executed.
       */
      const std::string & getDirectory() const;

      /**
       * Get end time.
       */
      struct timespec getEndTime() const;
      
      /**
       * Obtain model package associated with this command.
       *
       * @return ModelPackage.
       */
      const ModelPackagePointer & getModelPackage() const;

      /**
       * Obtain priority for evaluation.
       *
       * @return Priority
       */
      const Priority & getPriority() const;

      /**
       * Obtain amount of resources required for command to be run.
       *
       * @return amount of resources required for command to be run.
       */
      int getResourceAmount() const;

      /**
       * Obtain type of resource required for command to be run.
       *
       * @return type of resource required for command to be run.
       */
      const ResourceType & getResourceType() const;

      /**
       * Obtain resources assigned to this command.
       */
      const ResourceListPointer & getResources();

      /**
       * Get start time.
       */
      struct timespec getStartTime() const;
      
      /**
       * Get status of running command.
       *
       * @return status of running command.
       */
      virtual CommandStatus getStatus() = 0;

      /**
       * Obtain path to standard error file.
       *
       * @return string containing the path to standard error file.
       */
      const std::string & getStdError() const;
      
      /**
       * Obtain path to standard out file.
       *
       * @return string containing the path to standard out file.
       */
      const std::string & getStdOut() const;
      
      /**
       * Get elapsed wall clock time for command evaluation.
       *
       * @return Wall clock time elapsed for command evaluation.
       */
      struct timespec getWallClockTime() const;

      /**
       * Set resources command will use for execution.
       *
       * @param resources List of resources command will use for execution.
       */
      virtual void setResources(const ResourceListPointer & resources);
      
      /**
       * Terminate executing command.
       */
      virtual void terminate() = 0;

      //
      // methods
      // 
    protected:

      /**
       * Constructor.
       *
       * @param modelPackage ModelPackage associated with this command.
       * @param directory Path of directory to use for Model execution.
       * @param script Path of script to be executed.
       * @param stdOut Path to standard out.
       * @param stdError Path to standard error.
       * @param statusPredicates List of status predicates - used to
       * determine whether command is running.
       * @param endPredicate List of end predicates - used to
       * determine whether command completed successfully.
       */
      Command(const ModelPackagePointer                 & modelPackage, 
              const std::string                         & directory,
              const std::string                         & script,
              const std::string                         & stdOut,
              const std::string                         & stdError,
              const std::vector<StatusPredicatePointer> & statusPredicates,
              const std::vector<EndPredicatePointer>    & endPredicates);

      /**
       * Obtain end predicate used by this command.
       *
       * @return EndPredicate used by this command.
       */
      const std::vector<EndPredicatePointer> & getEndPredicates() const;

      /**
       * Obtain model that this command executes.
       *
       * @return Model the command executes.
       */
      const ModelPointer & getModel() const;

      /**
       * Obtain path to script this command executes.
       *
       * @return Path to script the command executes.
       */
      const std::string & getScript() const;

      /**
       * Obtain status predicates used by this command.
       *
       * @return Vector of StatusPredicates used by this command.
       */
      const std::vector<StatusPredicatePointer> & getStatusPredicates() const;

      /**
       * Print self.
       *
       * @param stream Ostream to print to.
       */
      virtual void print(std::ostream & outputStream) const;

      /**
       * Set end wall clock time of command evaluation
       */
      void setEndTime(const struct timespec & time);
      
      /**
       * Set start wall clock time of command evaluation
       */
      void setStartTime(const struct timespec & time);
      
    private:
      //
      // copy constructor/assignment operator
      //
      Command(const Command &); // not implemented
      Command & operator=(const Command &); // not implemented
      
      //
      // output operator
      //
      friend std::ostream & operator<<(std::ostream  & outputStream,
                                       const Command & command);

      //
      // data
      //
    private:

      const ModelPackagePointer                 d_modelPackage;
      std::string                               d_directory;
      std::string                               d_script;
      std::string                               d_stdOut;
      std::string                               d_stdError;
      const std::vector<StatusPredicatePointer> d_statusPredicates;
      const std::vector<EndPredicatePointer>    d_endPredicates;
      ResourceListPointer                       d_resources;
      struct timespec                           d_startTime;
      struct timespec                           d_endTime;
      
    };

    HMS_SHARED_PTR(Command);

    /**
     * Output operator.
     *
     * @param outputStream Output stream to use for output.
     * @param resourceList Command object.
     *
     * @return Updated output stream.
     */
    std::ostream & operator<<(std::ostream  & outputStream,
                              const Command & command);


  }
}

#endif // base_Command_h
