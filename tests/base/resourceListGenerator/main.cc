//
// Test ResourceListGenerator
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#include <base/MPIResourceListGenerator.h>
#include <base/ResourceList.h>
#include <base/ResourceType.h>

#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iostream header file not available.
#endif // HAVE_IOSTREAM

namespace {

  //
  // print usage
  //
  void printUsage(int ac,
                  char * av[])
  {

    //
    //
    //
    std::cerr << "usage: " << av[0] << " [machinefile]" << std::endl;

    //
    //
    //
    return;

  }

}

//
//
//
int
main(int    ac,
     char * av[])
{
  
  if(ac != 2) {
    printUsage(ac, av);
    exit(-1);
  }
  
  //
  // parse machinefile
  //
  arl::hms::MPIResourceListGenerator generator(av[1]);
  
  //
  // generate CPU resources
  //
  const arl::hms::ResourceListPointer resources = 
    generator.generate(arl::hms::CPU);

  //
  // output resources
  //
  std::cout << *resources << std::endl;

  //
  //
  //
  return 0;

}
