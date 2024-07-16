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

#include <comm/Communicator.h>
#include <comm/SocketCommunicator.h>

#include <adaptive_sampling/AdaptiveSamplingLauncher.h>
#include <adaptive_sampling/AdaptiveSamplingLoadScheduler.h>

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

int main(int     ac,
         char ** av)
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

  arl::hms::AdaptiveSamplingLauncher launcher;
  
  const std::string configFile(hmsConf);
  
  std::vector<arl::hms::CommunicatorPointer> communicators = 
    launcher.launch(configFile,
                    rank,
                    size,
                    arl::hms::InterpolationDatabaseInputFilterPointer(),
                    arl::hms::InterpolationDatabaseOutputFilterPointer()); 

  assert(communicators.size() == 1);

  int numberModelPackages = 10;

  std::vector<arl::hms::ModelPackagePointer> modelPackages;

  //
  // create 10 model packages
  //
  for(int i=0 ; i < numberModelPackages; ++i) {

    std::stringstream iStr;
    iStr << i;

    //
    // instantiate model
    //
    arl::hms::ModelPointer 
      model(new arl::hms::Model("",
                                "sleep",
                                std::vector<std::string>(1, "10"),
                                arl::hms::CPU,
                                1));


    arl::hms::OutputFilterPointer outputFilter;
    arl::hms::InputFilterPointer inputFilter;
    arl::hms::ArgumentPointer argument;

    //
    // instantiate model package
    //
    arl::hms::ModelPackagePointer modelPackage = 
      arl::hms::ModelPackagePointer(new arl::hms::ModelPackage(model,
                                                               inputFilter,
                                                               outputFilter,
                                                               argument));


    communicators[0]->send(modelPackage);
    
  }

  /*
  //
  // schedule and execute model packages
  //
  arl::hms::AdaptiveSamplingLoadScheduler scheduler(communicators);
  std::vector<arl::hms::AdaptiveSamplingScheduler::BrokerId> brokerIds = 
    scheduler.schedule(modelPackages);
  for(std::vector<arl::hms::AdaptiveSamplingScheduler::BrokerId>::size_type i=0;
      i<brokerIds.size(); ++i) {
    const arl::hms::AdaptiveSamplingScheduler::BrokerId brokerId = brokerIds[i];
    std::cout << brokerId << std::endl;
    arl::hms::CommunicatorPointer & communicator = communicators[brokerId];
    communicator->send(modelPackages[i]);
  }
  */

  int numberReceivedPackages = 0;
  while(numberReceivedPackages != numberModelPackages) {
    
    for(std::vector<arl::hms::CommunicatorPointer>::const_iterator iter = 
          communicators.begin(); iter != communicators.end(); ++iter) {
      const arl::hms::CommunicatorPointer & communicator = *iter;
    
      std::vector<arl::hms::ModelPackagePointer> modelPackages = 
        communicator->receive<arl::hms::ModelPackagePointer>();
      numberReceivedPackages += modelPackages.size();
    
    }

  }
 
  std::cout << "Done" << std::endl;
  
  //
  //
  //
  return 0; 

}
