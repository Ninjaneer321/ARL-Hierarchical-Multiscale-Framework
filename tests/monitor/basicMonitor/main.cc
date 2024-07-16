//
// Test BasicMonitor
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#if defined(HAVE_MPI)
#include <mpi.h>
#endif // HAVE_MPI

#include "PicalcEndPredicate.h"
#include "PicalcOutputFilter.h"
#include "PiValue.h"

#include <base/MPICommand.h>
#include <base/MPIMachinefileGenerator.h>
#include <base/OutputFilter.h>
#include <base/ResourceList.h>
#include <base/ResourceManager.h>
#include <base/StateDB.h>
#include <base/Value.h>
#include <base/predicate/EndPredicate.h>
#include <base/predicate/StatusPredicate.h>
#include <exception/IOError.h>
#include <monitor/BasicMonitor.h>
#include <scheduler/BasicScheduler.h>

#if defined(ENABLE_LOGGING)
#if defined(HAVE_BOOST_LOG)
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#else
#error boost logging not available
#endif // HAVE_BOOST_LOG
#endif // ENABLE_LOGGING

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

#if defined(HAVE_SYS_STAT_H)
#include <sys/stat.h>
#else
#error sys/stat.h header file not available
#endif // HAVE_SYS_STAT_H

//
//
//
int
main(int    ac,
     char * av[])
{

#if defined(HAVE_MPI)

  MPI_Init(&ac,
           &av);

#endif // HAVE_MPI

  //
  // setup logging
  //

#if defined(ENABLE_LOGGING)

  boost::log::add_common_attributes();
  boost::log::core::get()->
    add_global_attribute("Scope",
                         boost::log::attributes::named_scope());

  std::string formatString("[%TimeStamp%] - %Scope% - %Message%");

  boost::log::add_console_log(std::cout,
                              boost::log::keywords::format = formatString);

#endif // ENABLE_LOGGING

  //
  // create a list of CPU resources
  //
  arl::hms::ResourceListPointer 
    processors(new arl::hms::ResourceList(arl::hms::CPU,
                                          std::vector<std::string>(4, 
                                                                   "localhost")));

  //
  // create resource manager and add some resources
  //
  arl::hms::ResourceManager resourceManager;
  resourceManager.add(processors);

  //
  // create StateDB
  //
  arl::hms::StateDB stateDB;

  //
  // create Scheduler
  //
  arl::hms::BasicScheduler scheduler(false);

  //
  // create Monitor
  //
  arl::hms::BasicMonitor monitor;

  //
  // create picalc model
  //
  arl::hms::ModelPointer 
    model(new arl::hms::Model("mpirun -machinefile machinefile -np 2",
                              "cxxpi",
                              std::vector<std::string>(),
                              arl::hms::CPU,
                              2));

  const arl::hms::ModelPackagePointer
    modelPackage(new arl::hms::ModelPackage(model,
					    arl::hms::InputFilterPointer(),
					    arl::hms::OutputFilterPointer(),
					    arl::hms::ArgumentPointer()));

  //
  // create and schedule several commands
  //
  for(int i=0; i<4; ++i) {
   
    //
    // initialize predicate
    //
    arl::hms::EndPredicatePointer
      endPredicate(new arl::hms::PicalcEndPredicate);
  
    //
    // initialize MPI machinefile generator
    //
    arl::hms::MachinefileGeneratorPointer 
      machinefileGenerator(new arl::hms::MPIMachinefileGenerator);
    
    //
    // create directory for command
    //
    std::stringstream directory;
    directory << "/tmp/" << i;
    if(mkdir(directory.str().c_str(),
             S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == -1) {
      std::stringstream message;
      message << "Error mkdir directory: " << directory.str();
      throw arl::hms::IOError(message.str());
    }

    arl::hms::CommandPointer
      command(new arl::hms::MPICommand(modelPackage,
                                       directory.str(),
                                       std::vector<arl::hms::StatusPredicatePointer>(),            
                                       std::vector<arl::hms::EndPredicatePointer>(1, endPredicate),
                                       machinefileGenerator));

    //
    // generate command
    //
    command->generate();

    //
    // schedule job for execution
    //
    stateDB.getQueued().add(command);

  }
  
  arl::hms::StateDB::Queue & queued = stateDB.getQueued();
  arl::hms::StateDB::Queue & running = stateDB.getRunning();
  arl::hms::StateDB::Queue & completed = stateDB.getCompleted();

  //
  // loop scheduling and monitoring jobs
  //
  while(!queued.empty() || !running.empty()) {
    
    //
    // try to schedule jobs
    //
    scheduler.processQueue(resourceManager,
                           stateDB);
    
    //
    // monitor jobs
    //
    monitor.processQueue(resourceManager,
                         stateDB);

    //
    // apply output filter on completed jobs
    //
    arl::hms::StateDB::Queue::iterator iter = completed.begin();
    const arl::hms::StateDB::Queue::const_iterator iterEnd = completed.end();
    for( ; iter != iterEnd; ) {
      
      //
      // get handle to command
      //
      const arl::hms::CommandPointer & command = *iter;

      //
      // apply output filter
      //
      arl::hms::PicalcOutputFilter outputFilter;
      arl::hms::ValuePointer value = 
        outputFilter.apply(command->getDirectory(),
                           command->getStdOut(),
			   command->getModelPackage()->getArgument());


      //
      // downcast value to appropriate type
      //
      arl::hms::PiValuePointer piValue = 
        boost::dynamic_pointer_cast<arl::hms::PiValue>(value);

      //
      // output value of pi
      //
      std::cout << "Pi calculated to be: " << piValue->getValue() << std::endl;

      //
      // remove command from completed queue
      //
      const arl::hms::StateDB::Queue::iterator removeIter = iter++;
      completed.pop(removeIter);
      
    }
    
  }

  //
  //
  //
  return 0;

}
