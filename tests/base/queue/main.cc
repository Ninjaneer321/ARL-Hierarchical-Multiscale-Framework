//
// Test Queue
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#include <base/Queue.h>

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
  // instantiate queue of ints
  //
  arl::hms::Queue<int> queue;

  //
  // insert some ints
  //
  queue.add(1);
  queue.add(2);
  queue.add(3);

  //
  // output queue
  //
  std::cout << "Initial queue: " << queue << std::endl;

  //
  // remove second value
  //
  arl::hms::Queue<int>::iterator iter = queue.begin();
  ++iter;
  int removed = *iter;
  queue.pop(iter);
  std::cout << "Erased: " << removed << std::endl;
  std::cout << "Queue contents: " << queue << std::endl;

  //
  //
  //
  return 0;

}
