//
// Test ResourceList
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#include <base/ResourceList.h>
#include <base/ResourceType.h>

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
  // instantiate 10 CPU resources
  //
  arl::hms::ResourceList::Resources resources;
  resources.push_back("1");
  resources.push_back("2");
  resources.push_back("3");
  resources.push_back("4");
  resources.push_back("5");
  resources.push_back("6");
  resources.push_back("7");
  resources.push_back("8");
  resources.push_back("9");
  resources.push_back("10");

  //
  // instantiate ResourceList holding these resources
  //
  const arl::hms::ResourceListPointer 
    resourceList(new arl::hms::ResourceList(arl::hms::CPU,
                                            resources));

  //
  // output resourceList to stdout
  //
  std::cout << *resourceList << std::endl;

  //
  // checkout three resources
  //
  const arl::hms::ResourceListPointer threeResources = 
    resourceList->checkOut(3);
  
  //
  // output checkedout resources
  //
  std::cout << "Checked out: " << *threeResources << std::endl;
  
  //
  // output what was left
  // 
  std::cout << "Remaining resources: " << *resourceList << std::endl;
  
  //
  //
  //
  std::cout << "Checking in resources" << std::endl;

  //
  // check in resources
  //
  resourceList->checkIn(threeResources);

  //
  // output checkedout resources
  // 
  std::cout << "Remaining resources: " << *resourceList << std::endl;

  std::cout << *threeResources << std::endl;
  
  //
  //
  //
  return 0;

}
