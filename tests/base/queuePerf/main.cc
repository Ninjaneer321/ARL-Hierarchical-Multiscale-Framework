//
// Test Queue
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#include <base/MPICommand.h>

#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iostream header file not available.
#endif // HAVE_IOSTREAM

#if defined(HAVE_LIST)
#include <list>
#else
#error list header file not available.
#endif // HAVE_LIST

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available.
#endif // HAVE_SSTREAM

int numberCommands = 1000000;
int numberPasses = 1000;

//
//
//
int
main(int    ac,
     char * av[])
{

  //
  // instantiate vector of commands
  //
  //typedef std::vector<arl::hms::CommandPointer> CommandQueue;
  //typedef std::vector<arl::hms::Command *> CommandQueue;
  typedef std::list<arl::hms::CommandPointer> CommandQueue;
  //typedef std::list<arl::hms::Command *> CommandQueue;
  
  CommandQueue commands;
  CommandQueue completedCommands;

  //
  // fill vector with commands
  //
  for(int i=0; i<numberCommands; ++i) {

    std::stringstream directory;
    directory << i;
    
    arl::hms::CommandPointer 
      command(new arl::hms::MPICommand(arl::hms::ModelPackagePointer(),
                                       directory.str(),
                                       std::vector<arl::hms::StatusPredicatePointer>(),
                                       std::vector<arl::hms::EndPredicatePointer>(),
                                       arl::hms::MachinefileGeneratorPointer()));
    
    /* 
    arl::hms::Command * command = 
      new arl::hms::MPICommand(arl::hms::ModelPointer(),
                               directory.str(),
                               std::vector<arl::hms::StatusPredicatePointer>(),
                               arl::hms::EndPredicatePointer(),
                               arl::hms::MachinefileGeneratorPointer());
    */

    commands.push_back(command);
    
  }

  clock_t start = clock();
   
  //int countFlag = 1;
  int countFlag = numberCommands - 100;

  //
  // remove every second element from queue
  //
  for(int pass = 0; pass < numberPasses; ++pass) {
    int count = 0;
    for(CommandQueue::iterator iter = commands.begin(); 
        iter != commands.end();) {
      
      if(count >= countFlag) {

        /*
        CommandQueue::iterator spliceIter = iter++;
        completedCommands.splice(completedCommands.end(),
                                 commands,
                                 spliceIter);
        */
        
        completedCommands.push_back(*iter);
        iter = commands.erase(iter); 
        
        //count = 0;
  
      }
      else {
        ++iter;
        ++count;
      }
 
    }

    countFlag -= 100;
    
  }

  /*
  int count = 0;
  for(int pass = 0; pass < numberPasses; ++pass) {
    
    for(CommandQueue::iterator iter = commands.begin(); 
        iter != commands.end(); ++iter) {
      
      ++count;
      
    }
    
  }
  */

  clock_t end = clock();
  std::cout << "Time: " << static_cast<double>(end - start) / CLOCKS_PER_SEC
            << std::endl;
  std::cout << "End queue size: " << commands.size() << std::endl;
  std::cout << "End completed size: " << completedCommands.size() << std::endl;

  //
  // output result
  //
  /*
  for(CommandQueue::iterator iter = commands.begin(); 
      iter != commands.end(); ++iter) {
    std::cout << (*iter)->getDirectory() << std::endl;
  }
  */

  //
  //
  //
  return 0;

}
