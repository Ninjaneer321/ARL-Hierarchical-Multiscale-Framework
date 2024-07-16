//
// Test broker
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#if defined(HAVE_MPI)
#include <mpi.h>
#include <comm/MPICommunicator.h>
#endif

#include "SleepInputFilter.h"
#include "SleepOutputFilter.h"

#include <comm/Communicator.h>
#include <comm/SocketCommunicator.h>

#include <base/ModelPackage.h>
#include <broker/BrokerLauncher.h>

#include <exception/ConnectionTerminationException.h>
#include <exception/IOError.h>
#include <exception/NetworkError.h>

#include <warehouse/Warehouse.h>

#if defined(ENABLE_LOGGING)
#if defined(HAVE_BOOST_LOG)
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#else
#error boost logging not available
#endif // HAVE_BOOST_LOG
#endif // ENABLE_LOGGING

#include <boost/serialization/shared_ptr.hpp>

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

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#else
#error unistd.h header file not available
#endif // HAVE_UNISTD_H

const std::string hmsConf("hmsConf.ini");

int main(int    ac,
         char * av[])
{

#if defined(HAVE_MPI)

  MPI_Init(&ac,
           &av);

  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD,
                &rank);
  MPI_Comm_size(MPI_COMM_WORLD,
                &size);

#else

  int rank = 0;
  int size = 1;

#endif // HAVE_MPI

  arl::hms::BrokerLauncher brokerLauncher;
  
  const std::string configFile(hmsConf);
  
  std::vector<arl::hms::CommunicatorPointer> communicators = 
    brokerLauncher.launch(configFile,
                          rank,
                          size); 

  if(communicators.size() != 3) {
    const std::string message("Error launching broker - must have 3 brokers "
                              "for this test");
    throw arl::hms::IOError(message);
  }

  const arl::hms::CommunicatorPointer & communicator0 = communicators[0];
  const arl::hms::CommunicatorPointer & communicator1 = communicators[1];
  const arl::hms::CommunicatorPointer & communicator2 = communicators[2];

  int i = 0;

  //
  // instantiate models
  //
  arl::hms::ModelPointer 
    model0(new arl::hms::Model("",
                               "sleep",
                               std::vector<std::string>(1, "1"),
                               arl::hms::CPU,
                               1));

  arl::hms::ModelPointer 
    model1(new arl::hms::Model("",
                               "sleep",
                               std::vector<std::string>(1, "10"),
                               arl::hms::CPU,
                               1));

  arl::hms::OutputFilterPointer 
    outputFilter(new arl::hms::SleepOutputFilter()) ;
  arl::hms::InputFilterPointer 
    inputFilter(new arl::hms::SleepInputFilter()) ;
  arl::hms::ArgumentPointer argument;

#if defined(HAVE_MPI)
  const double start = MPI_Wtime();
#endif // HAVE_MPI

  //
  // send model packages 
  //
  int numModelPackages = 0;
  for( ; i < 10; ++i ) {

    //
    // instantiate model packages
    //
    arl::hms::ModelPackagePointer modelPackage0 = 
      arl::hms::ModelPackagePointer(new arl::hms::ModelPackage(model0,
                                                               inputFilter,
                                                               outputFilter,
                                                               argument));
    
    arl::hms::ModelPackagePointer modelPackage1 = 
      arl::hms::ModelPackagePointer(new arl::hms::ModelPackage(model1,
                                                               inputFilter,
                                                               outputFilter,
                                                               argument));
      
    //
    // send model package
    //
    communicator0->send(modelPackage0); 
    communicator1->send(modelPackage1); 
    communicator2->send(modelPackage0);
    numModelPackages += 3;
    std::cout << "Model Package Sent" << std::endl;
    
  }

  //
  // receive values back 
  //
  int counter = 0; 
  while(counter < numModelPackages) {

    
    std::vector<arl::hms::ModelPackagePointer> modelPackages = 
      communicator0->receive<arl::hms::ModelPackagePointer>();

    if(modelPackages.size() > 0) {
      std::cout << "Received " << modelPackages.size() 
                << " model packages" << std::endl;
    }

    //
    // increment counter
    //
    counter += modelPackages.size();

    modelPackages =
      communicator1->receive<arl::hms::ModelPackagePointer>();

    if(modelPackages.size() > 0) {
      std::cout << "Received " << modelPackages.size() 
                << " model packages" << std::endl;
    }

    //
    // increment counter
    //
    counter += modelPackages.size();

    modelPackages =
      communicator2->receive<arl::hms::ModelPackagePointer>();

    if(modelPackages.size() > 0) {
      std::cout << "Received " << modelPackages.size() 
                << " model packages" << std::endl;
    }

    //
    // increment counter
    //
    counter += modelPackages.size();

  }

#if defined(HAVE_MPI)
  MPI_Barrier(MPI_COMM_WORLD);
  const double end = MPI_Wtime();
  std::cout << "Time: " << end - start << std::endl;
#endif // HAVE_MPI

  /*
#if defined(HAVE_MPI)

  MPI_Finalize();

#endif // HAVE_MPI
  */

  //
  //
  //
  return 0; 

}
