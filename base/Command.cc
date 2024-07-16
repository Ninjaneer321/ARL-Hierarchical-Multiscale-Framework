//
// File:      Command.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "Command.h"

#include "TimeUtils.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    Command::Command(const ModelPackagePointer & modelPackage,
                     const std::string & directory,
                     const std::string & script,
                     const std::string & stdOut,
                     const std::string & stdError,
                     const std::vector<StatusPredicatePointer> & statusPredicates,
                     const std::vector<EndPredicatePointer> & endPredicates) :
      d_modelPackage(modelPackage),
      d_directory(directory),
      d_script(script),
      d_stdOut(stdOut),
      d_stdError(stdError),
      d_statusPredicates(statusPredicates),
      d_endPredicates(endPredicates)
    {

      d_startTime.tv_sec = 0;
      d_startTime.tv_nsec = 0;
      d_endTime.tv_sec = 0;
      d_endTime.tv_nsec = 0;
      
      //
      //
      //
      return;

    }
    
    //
    // Destructor.
    //
    Command::~Command()
    {

      //
      //
      //
      return;
      
    }
    
    //
    // Obtain directory that command is executed in.
    //
    const std::string & 
    Command::getDirectory() const
    {

      //
      //
      //
      return d_directory;
      
    }

    //
    // Obtain end predicate used by this command.
    //
    const std::vector<EndPredicatePointer> &
    Command::getEndPredicates() const
    {
      
      //
      //
      //
      return d_endPredicates;
      
    }

    //
    // Get end time.
    //
    struct timespec
    Command::getEndTime() const
    {

      //
      //
      //
      return d_endTime;
      
    }
       
    //
    // Obtain model that this command executes.
    //
    const ModelPointer &
    Command::getModel() const
    {

      //
      //
      //
      return d_modelPackage->getModel();

    }

    //
    // Obtain model package associated with this command.
    //
    const ModelPackagePointer & 
    Command::getModelPackage() const
    {
      
      //
      //
      //
      return d_modelPackage;

    }

    //
    // Obtain priority for evaluation.
    //
    const Priority & 
    Command::getPriority() const
    {

      //
      //
      //
      return d_modelPackage->getPriority();

    }

    //
    // Obtain resources assigned to this command.
    //
    const ResourceListPointer &
    Command::getResources()
    {

      //
      //
      //
      return d_resources;

    }

    //
    // Obtain amount of resources required for command to be run.
    //
    int 
    Command::getResourceAmount() const
    {

      //
      //
      //
      return d_modelPackage->getModel()->getResourceAmount();

    }

    //
    // Obtain type of resource required for command to be run.
    //
    const ResourceType & 
    Command::getResourceType() const
    {

      //
      //
      //
      return d_modelPackage->getModel()->getResourceType();

    }

    //
    // Obtain path to script this command executes.
    //
    const std::string & 
    Command::getScript() const
    {

      //
      //
      //
      return d_script;

    }

    //
    // Get start time.
    //
    struct timespec
    Command::getStartTime() const
    {

      return d_startTime;
      
    }
    
    //
    // Obtain status predicates used by this command.
    //
    const std::vector<StatusPredicatePointer> & 
    Command::getStatusPredicates() const
    {

      //
      //
      //
      return d_statusPredicates;

    }

    //
    // Obtain path to standard error file.
    //
    const std::string & 
    Command::getStdError() const
    {

      //
      //
      //
      return d_stdError;

    }
      
    //
    // Obtain path to standard out file.
    //
    const std::string & 
    Command::getStdOut() const
    {

      //
      //
      //
      return d_stdOut;

    }

    //
    // Get elapsed wall clock time in seconds for command evaluation.
    //
    struct timespec
    Command::getWallClockTime() const
    {

      //
      // subtract start time from end time
      //
      struct timespec elapsedTime = timeSubtract(d_endTime,
						 d_startTime);

      //
      //
      //
      return elapsedTime;
      
    }
    
    //
    // Print self.
    //
    void 
    Command::print(std::ostream & outputStream) const
    {

      //
      // output model
      //
      outputStream << "Model: " << *(d_modelPackage->getModel()) << std::endl;
      
      //
      // output directory
      //
      outputStream << "Directory: " << d_directory << std::endl;
      
      //
      // output script
      //
      outputStream << "Script: " << d_script << std::endl;
      
      //
      // output stdout
      //
      outputStream << "StdOut: " << d_stdOut << std::endl;
      
      //
      // output stderr
      //
      outputStream << "StdErr: " << d_stdError << std::endl;
     
      //
      // output status predicates
      //
      outputStream << "Status predicates: [ ";
      for(std::vector<StatusPredicatePointer>::const_iterator iter = 
            d_statusPredicates.begin(); iter != d_statusPredicates.end(); 
          ++iter) {
        outputStream << *iter << " ";
      }
      outputStream << "]" << std::endl;
      
      //
      // output end predicate 
      //
      outputStream << "End predicates: [ ";
      for(std::vector<EndPredicatePointer>::const_iterator iter = 
            d_endPredicates.begin(); iter != d_endPredicates.end(); 
          ++iter) {
        outputStream << *iter << " ";
      }
      outputStream << "]" << std::endl;
      
      //
      // output resources
      //
      if(d_resources) {
	outputStream << "Resources: " << *d_resources;
      }

      //
      // output priority
      //
      outputStream << "Priority: " << getPriority();
    
      //
      //
      //
      return;

    }

    //
    // Set end wall clock time of command evaluation
    //
    void
    Command::setEndTime(const struct timespec & time)
    {

      //
      //
      //
      d_endTime = time;

      //
      //
      //
      return;
      
    }
      
    //
    // Set start wall clock time of command evaluation
    //
    void
    Command::setStartTime(const struct timespec & time)
    {

      //
      //
      //
      d_startTime = time;

      //
      //
      //
      return;
      
    }
      
    //
    // Set resources command will use for execution.
    //
    void 
    Command::setResources(const ResourceListPointer & resources)
    {

      //
      // store resourceList
      // 
      d_resources = resources;
      
      //
      //
      //
      return;

    }

    //
    // output operator.
    //
    std::ostream & operator<<(std::ostream  & outputStream,
                              const Command & command)
    {

      //
      // print command (polymorphic)
      //
      command.print(outputStream);
      
      //
      //
      //
      return outputStream;

    }
   
  }
}
