//
// File:      MPICommand.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "MPICommand.h"

#include "TimeUtils.h"

#include <exception/IOError.h>
#include <exception/OSError.h>

#if defined(HAVE_CERRNO)
#include <cerrno>
#else
#error cerrno header file not available
#endif // HAVE_CERRNO

#if defined(HAVE_CSTDIO)
#include <cstdio>
#else
#error cstdio header file not available
#endif // HAVE_CSTDIO

#if defined(HAVE_CSTRING)
#include <cstring>
#else
#error cstring header file not available
#endif // HAVE_CSTRING

#if defined(HAVE_FSTREAM)
#include <fstream>
#else
#error fstream header file not available
#endif // HAVE_FSTREAM

#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iostream header file not available
#endif // HAVE_IOSTREAM

#if defined(HAVE_SIGNAL_H)
#include <signal.h>
#else
#error signal.h header file not available
#endif // HAVE_SIGNAL_H

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

#if defined(HAVE_SYS_WAIT_H)
#include <sys/wait.h>
#else
#error sys/wait.h header file not available
#endif // HAVE_SYS_WAIT_H

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#else
#error unistd.h header file not available
#endif // HAVE_UNISTD_H

//
//
//

extern char ** environ;

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    MPICommand::MPICommand(const ModelPackagePointer & modelPackage,
			   const std::string & directory,
			   const std::vector<StatusPredicatePointer> & statusPredicates,
                           const std::vector<EndPredicatePointer> & endPredicates,
                           const ConstMachinefileGeneratorPointer & machinefileGenerator) :
      Command(modelPackage,
	      directory,
	      directory + "/script.sh",
              directory + "/output.out",
              directory + "/error.err",
              statusPredicates,
              endPredicates),
      d_machinefileGenerator(machinefileGenerator),
      d_runningProcess(-1)
    {

      const std::string mpiEnvPrefix = "PMI_";
      const std::string impiEnvPrefix = "I_MPI";     
 
      //
      // create environment for exec'd children (since we are forking
      // from an MPI process, we need to remove MPI environmental
      // variables in some cases)
      //
      char ** environmentVariable = environ;
      while(*environmentVariable != NULL) {
	size_t size = strlen(*environmentVariable);
	if(size > mpiEnvPrefix.size()) {
	  if(strncmp(*environmentVariable, 
		     mpiEnvPrefix.c_str(), 
		     mpiEnvPrefix.size()) != 0 &&
             strncmp(*environmentVariable,
                     impiEnvPrefix.c_str(),
                     impiEnvPrefix.size()) != 0) { 
            d_environment.push_back(*environmentVariable);
	  }
	}
	++environmentVariable;
      }  
      d_environment.push_back(NULL);

      //
      //
      //
      return;

    }
       
    //
    // Destructor.
    //
    MPICommand::~MPICommand()
    {

      //
      //
      //
      return;

    }

    //
    // Execute the command.
    //
    void 
    MPICommand::execute()
    {
      
      //
      // start the timer
      //
      Command::setStartTime(getTime());
      
      //
      // fork child process
      //
      d_runningProcess = vfork();

      //
      // if we are the child exec our script
      //
      if(d_runningProcess == 0) {
        
        //
        // change current working directory
        //
        if(chdir(MPICommand::getDirectory().c_str()) == -1) {
          std::stringstream message;
          message << "Error chdir() child to new working directory: " 
                  << strerror(errno);
          throw OSError(message.str());
        }

        //
        // redirect stdout and stderr to file
        //
        if(freopen(MPICommand::getStdOut().c_str(),
                   "w",
                   stdout) == NULL) {
          std::stringstream message;
          message << "Error in freopen() redirecting stdout file for child "
                  << "process: " << strerror(errno);
          throw IOError(message.str());
        }
        if(freopen(MPICommand::getStdError().c_str(),
                   "w",
                   stderr) == NULL) {
          std::stringstream message;
          message << "Error in freopen() redirecting stderr file for child "
                  << "process: " << strerror(errno);
          throw IOError(message.str());
        }
        
        //
        // exec the script
        //
        const char * argv[] = {"/bin/sh",
                               MPICommand::getScript().c_str(),
                               NULL};

        if(execve(argv[0],
                  const_cast<char * const *>(argv),
                  &(d_environment[0])) == -1) {
          std::stringstream message;
          message << "Error in execve() of child: " << strerror(errno);
          throw OSError(message.str());
        }
        
      }
      else if(d_runningProcess == -1) {
        
        //
        // error forking, throw an exception.
        //
        std::stringstream message;
        message << "Error during fork() of child process: " << strerror(errno);
        throw OSError(message.str());
        
      }
      
      //
      //
      //
      return;

    }

    //
    // Generate a shell script in directory to execute Model.
    //
    void 
    MPICommand::generate() const
    {

      //
      // get model data
      //
      const ConstModelPointer & model = MPICommand::getModel();
      const std::string & modelPrefix = model->getPrefix();
      const std::string & modelExecutable = model->getExecutable();
      const std::vector<std::string> & modelArguments = model->getArguments();
      
      //
      // open script file for executing model
      //
      std::ofstream script(MPICommand::getScript().c_str(), std::ios::out);
      
      //
      // write script preamble
      //
      script << "#! /bin/sh" << std::endl;
      
      //
      // write model data
      //
      script << "exec " << modelPrefix << " " << modelExecutable << " ";
      std::vector<std::string>::const_iterator iter = modelArguments.begin();
      const std::vector<std::string>::const_iterator iterEnd = 
        modelArguments.end();
      for( ; iter != iterEnd; ++iter) {
        script << *iter << " ";
      }
     
      //
      // close script
      //
      script.close();

      //
      //
      //
      return;

    }

    //
    // Get status of running command.
    //
    CommandStatus 
    MPICommand::getStatus()
    {

      //
      // check if pid is valid
      //
      if(d_runningProcess != -1) {

	//
	// perform non-blocking query of child status
	//
	int exitStatus;
	pid_t status;
	if((status = waitpid(d_runningProcess,
			     &exitStatus,
			     WNOHANG)) == -1) {
	  std::stringstream message;
	  message << "Error in waitpid() querying status of child process: " 
		  << strerror(errno);
	  throw OSError(message.str());
	}
	
	//
	// if process is still running check status predicates
	//
	if(status == 0) {
	  bool runStatus = true;
	  std::vector<StatusPredicatePointer>::const_iterator iter = 
	    MPICommand::getStatusPredicates().begin();
	  const std::vector<StatusPredicatePointer>::const_iterator iterEnd =
	    MPICommand::getStatusPredicates().end();
	  for( ; iter != iterEnd; ++iter) {
	    runStatus = runStatus && 
	      (*iter)->evaluate(*this);
	  }
	  
	  if(runStatus == true) {
	    return RUNNING;
	  }
	  else {
	    //
	    // ensure that process is actually terminated since we
	    // detected run completion via status predicates
	    //
	    MPICommand::terminate();
	  }
	  
	}

      }

      //
      // End timer
      //
      Command::setEndTime(getTime());

      //
      // set runningProcess to -1 since it is finished
      //
      d_runningProcess = -1;

      //
      // process is finished, check end predicates
      //
      bool endStatus = true;
      std::vector<EndPredicatePointer>::const_iterator iter = 
        MPICommand::getEndPredicates().begin();
      const std::vector<EndPredicatePointer>::const_iterator iterEnd =
        MPICommand::getEndPredicates().end();
      for( ; iter != iterEnd; ++iter) {
        endStatus = endStatus && 
          (*iter)->evaluate(*this);
      }
   
      //
      //
      //
      return endStatus == true ? COMPLETED: ERROR;

    }

    //
    // Set resources command will use for execution.
    //
    void 
    MPICommand::setResources(const ResourceListPointer & resources)
    {

      //
      // store resources in base class
      //
      Command::setResources(resources);

      //
      // generate machinefile
      //
      const std::string 
        machinefilePath(MPICommand::getDirectory() + "/machinefile");

      d_machinefileGenerator->generate(machinefilePath,
                                       resources);

      //
      //
      //
      return;

    }
 
    //
    // Terminate executing command.
    //
    void 
    MPICommand::terminate()
    {

      //
      // send kill signal to process
      // 
      if(kill(d_runningProcess,
              SIGKILL) == -1) {
        std::stringstream message;
        message << "Error in kill() terminating child process: " 
                << strerror(errno);
        throw OSError(message.str());
      }

      //
      // collect killed child
      //
      int exitStatus;
      int status;
      if((status = waitpid(d_runningProcess,
                           &exitStatus,
                           0)) == -1) {
        std::stringstream message;
        message << "Error in waitpid() querying status of child process: " 
                << strerror(errno);
        throw OSError(message.str());
      }

      //
      // set running process pid to -1
      //
      d_runningProcess = -1;

      //
      //
      //
      return;
      
    }

  }
}
