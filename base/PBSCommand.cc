//
// File:      PBSCommand.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PBSCommand.h"
#include "NumberProcessorsPerNode.h"

#include <exception/IOError.h>
#include <exception/OSError.h>
#include <utils/HMSPaths.h>

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

#if defined(HAVE_CSTDLIB)
#include <cstdlib>
#else
#error cstdlib not available
#endif // HAVE_CSTDLIB

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
    PBSCommand::PBSCommand(const ModelPackagePointer                              & modelPackage,
                           const std::string                                & directory,
			   const std::vector<StatusPredicatePointer> & statusPredicates,
                           const std::vector<EndPredicatePointer> & endPredicates) :
      Command(modelPackage,
              directory,
              directory + "/script.sh",
              directory + "/output",
              directory + "/error",
              statusPredicates,
              endPredicates),
      d_runningProcess(-1),
      d_pbsErrorPath(directory + "/pbs_error"),
      d_pbsOutputPath(directory + "/pbs_output")
    {

      //
      //
      //
      return;

    }
       
    //
    // Destructor.
    //
    PBSCommand::~PBSCommand()
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
    PBSCommand::execute()
    {
      
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
        if(chdir(PBSCommand::getDirectory().c_str()) == -1) {
          std::stringstream message;
          message << "Error chdir() child to new working directory: " 
                  << strerror(errno);
          throw OSError(message.str());
        }

        //
        // redirect stdout and stderr to file
        //
        if(freopen(PBSCommand::getStdOut().c_str(),
                   "w",
                   stdout) == NULL) {
          std::stringstream message;
          message << "Error in freopen() redirecting stdout file for child "
                  << "process: " << strerror(errno);
          throw IOError(message.str());
        }
        if(freopen(PBSCommand::getStdError().c_str(),
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
        const char * argv[] = {HMS_PBS_QSUB,
                               PBSCommand::getScript().c_str(),
                               NULL};

        if(execv(argv[0],
                 const_cast<char * const *>(argv)) != 0 ){
           std::stringstream message;
           message << "Error in execv() of child: " << strerror(errno);
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
      // wait for script submission to complete
      //
      int runningStatus;
      if(waitpid(d_runningProcess,
		 &runningStatus,
		 0) == -1) {
	std::stringstream message;
	message << "Error on waitpid of qsub: ";
	throw OSError(message.str());
      };
 
      if(!WIFEXITED(runningStatus) || WEXITSTATUS(runningStatus) != 0) {
	
	//
	// check return value of waitpid
	//
	std::stringstream message;
	message << "Error on waitpid of qsub: ";
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
    PBSCommand::generate() const
    {

      //
      // get some application specific environment variables
      //

      //
      // HMS_PBS_ACCOUNT_ID is the PBS allocation to charge hours to
      //
      char * accountId = getenv("HMS_PBS_ACCOUNT_ID");
      if(!accountId) {
	const std::string message("Cannot find $HMS_PBS_ACCOUNT_ID environment "
				  "variable");
	throw OSError(message);
      }

      char * gaussExe = getenv("HMS_GAUSSIAN_PATH");
      if(!gaussExe) {
	const std::string message("Cannot find $HMS_GAUSSIAN_PATH environment "
				  "variable");
	throw OSError(message);
      }
      std::string gaussianPath = gaussExe;
      
      //
      // get model data
      //
      const ModelPackagePointer & modelPackage = PBSCommand::getModelPackage();
      const ConstModelPointer & model = modelPackage->getModel();
      const std::vector<std::string> & modelArguments = model->getArguments();
      const BatchQueueSettingsPointer & settings = 
	modelPackage->getBatchQueueSettings();     

      //
      // ensure settings are present, otherwise we cannot submit to
      // the queue
      //
      if(!settings) {
	const std::string message("Cannot create PBSCommand as no "
				  "BatchQueueSettings are present in the "
				  "received ModelPackage");
	throw IOError(message);
      }
      
      //
      // generate model prefix from gaussian path
      //
      const size_t lastDir = gaussianPath.rfind("/");
      if(lastDir == std::string::npos) {
	const std::string message("Error determining GAUSS_EXEDIR");
	throw arl::hms::IOError(message);
      }    

      std::stringstream modelPrefixStream;
      modelPrefixStream << "setenv GAUSS_EXEDIR " << "\"" 
			<< gaussianPath.substr(0, lastDir)
			<< "\"";

      const std::string modelPrefix = modelPrefixStream.str();
      
      //
      // pull settings
      //
      const std::string & queue = settings->getQueue();
      const std::string & walltime = settings->getWalltime();
      const int & numberNodes = settings->getNumberNodes();
      const int numberCpus = 
	NumberProcessorsPerNode().getNumberProcessorsPerNode();
      
      //
      // open script file for executing model
      //
      std::ofstream script(PBSCommand::getScript().c_str());
      
      //
      // create name for job (15 character limit in PBS)
      //
      std::string jobName;
      if(modelArguments[0].size() < 15) {
	jobName = modelArguments[0];
      }
      jobName = modelArguments[0].substr(0, 15);

      //
      // write script
      //
      script << "#!/bin/csh" << std::endl;
      script << "#PBS -A " << accountId << std::endl;
      script << "#PBS -l walltime=" << walltime << std::endl;
      script << "#PBS -l select="<< numberNodes << ":ncpus=" << numberCpus <<":mpiprocs="
             << numberCpus << std::endl;
      script << "#PBS -q " << queue << std::endl;
      script << "#PBS -e " << d_pbsErrorPath << std::endl;
      script << "#PBS -o " << d_pbsOutputPath << std::endl;
      script << "#PBS -N " << jobName << std::endl;
      script << std::endl; 
      script << "cd $PBS_O_WORKDIR" << std::endl;
      script << std::endl;
      
      //
      // write model execution info to script
      //
      script << "set PBS_NUM_PROCS=`cat ${PBS_NODEFILE} | wc -l`" << std::endl;
      script << "sed -i \"s/nprocs=.*/nprocs=${PBS_NUM_PROCS}/g\" " << modelArguments[0] << std::endl;
      script << modelPrefix << std::endl;
      script << " " << gaussianPath << " ";
      std::vector<std::string>::const_iterator iter = modelArguments.begin();
      const std::vector<std::string>::const_iterator iterEnd = 
        modelArguments.end();
      for( ; iter != iterEnd; ++iter) {
        script << *iter << " ";
      }
      script << std::endl;
     
      //
      //
      //
      return;

    }

    //
    // Get status of running command.
    //
    CommandStatus 
    PBSCommand::getStatus()
    {

      //
      // check whether we have output from PBS (only written at completion);
      //
      std::ifstream file_to_check(d_pbsOutputPath.c_str());
      if(file_to_check.is_open())
         return COMPLETED;

      //
      //
      //
      return RUNNING;
 
    }
 
    //
    // Terminate executing command.
    //
    void 
    PBSCommand::terminate()
    {

      //
      // FIXME: needs to call pbs to kill running job
      //

      //
      // send kill signal to process
      // 
      if(kill(d_runningProcess,
              SIGTERM) == -1) {
        std::stringstream message;
        message << "Error in kill() terminating child process: " 
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
