//
// Test Queue
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#include <base/Model.h>
#include <base/MPICommand.h>
#include <base/StateDB.h>

#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iostream header file not available.
#endif // HAVE_IOSTREAM

//
//
//
int
main(int    ac,
     char * av[])
{

  //
  // instantiate stateDB
  //
  arl::hms::StateDB stateDB;

  //
  // instantiate model
  //
  const arl::hms::ModelPointer
    model(new arl::hms::Model("",
                              "/bin/ls",
                              std::vector<std::string>(1, "-l"),
                              arl::hms::CPU,
                              1));

  const arl::hms::ModelPackagePointer
    modelPackage(new arl::hms::ModelPackage(model,
					    arl::hms::InputFilterPointer(),
					    arl::hms::OutputFilterPointer(),
					    arl::hms::ArgumentPointer()));

  //
  // instantiate some commands
  //
  arl::hms::CommandPointer 
    command1(new arl::hms::MPICommand(modelPackage,
                                      "/tmp/",
                                      std::vector<arl::hms::StatusPredicatePointer>(),
                                      std::vector<arl::hms::EndPredicatePointer>(),
                                      arl::hms::MachinefileGeneratorPointer()));
  arl::hms::CommandPointer 
    command2(new arl::hms::MPICommand(modelPackage,
                                      "/tmp/foo/",
                                      std::vector<arl::hms::StatusPredicatePointer>(),
                                      std::vector<arl::hms::EndPredicatePointer>(),
                                      arl::hms::MachinefileGeneratorPointer()));

  //
  // add some queued jobs
  //
  arl::hms::StateDB::Queue & queued = stateDB.getQueued();
  queued.add(command1);
  queued.add(command2);

  //
  // output stateDB
  //
  std::cout << "Queued some commands: " << std::endl << stateDB << std::endl;
  
  //
  // move first job to running
  //
  arl::hms::StateDB::Queue & running = stateDB.getRunning();
  arl::hms::CommandPointer firstJob = *queued.begin();
  queued.pop(queued.begin());
  running.add(firstJob);

  //
  // output stateDB
  //
  std::cout << "Ran the first command: " << std::endl << stateDB << std::endl;

  //
  // move running job to completed
  //
  arl::hms::StateDB::Queue & completed = stateDB.getCompleted();
  firstJob = *(running.begin());
  running.pop(running.begin());
  completed.add(firstJob);

  //
  // output stateDB
  //
  std::cout << "Completed the first command: " << std::endl << stateDB 
            << std::endl;

  //
  //
  //
  return 0;

}
