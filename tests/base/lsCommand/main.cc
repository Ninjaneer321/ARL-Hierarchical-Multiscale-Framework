//
// Test Command using ls
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#if defined(HAVE_MPI)
#include <mpi.h>
#endif // HAVE_MPI

#include <base/Model.h>
#include <base/MPICommand.h>
#include <base/Priority.h>

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
  // instantiate ls Model running on a single cpu
  //
  const arl::hms::ModelPointer 
    model(new arl::hms::Model("mpirun -np 2",
                              "sleep",
                              std::vector<std::string>(1, "10"),
                              arl::hms::CPU,
                              1));
  
  const arl::hms::ModelPackagePointer
    modelPackage(new arl::hms::ModelPackage(model,
					    arl::hms::InputFilterPointer(),
					    arl::hms::OutputFilterPointer(),
					    arl::hms::ArgumentPointer()));

  //
  // instantiate command to run ls model
  //
  arl::hms::MPICommand command(modelPackage,
                               "",
                               std::vector<arl::hms::StatusPredicatePointer>(),
                               std::vector<arl::hms::EndPredicatePointer>(),
                               arl::hms::MachinefileGeneratorPointer());

  //
  // generate command script
  //
  command.generate();

  //
  // execute script
  //
  command.execute();

  //
  // sleep
  //
  sleep(10);

  //
  // kill
  //
  command.terminate();

  //
  // sleep
  //
  sleep(10);
  
  //
  // loop until command is finished
  //
  while(command.getStatus() == arl::hms::RUNNING);

#if defined(HAVE_MPI)

  MPI_Finalize();

#endif // HAVE_MPI

  //
  //
  //
  return 0;

}
