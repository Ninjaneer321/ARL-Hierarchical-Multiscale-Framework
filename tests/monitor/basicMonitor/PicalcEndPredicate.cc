//
// Test Basic Monitor
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PicalcEndPredicate.h"

#include <base/Command.h>
#include <exception/IOError.h>

#if defined(HAVE_FSTREAM)
#include <fstream>
#else
#error fstream header file not available
#endif // HAVE_FSTREAM

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    PicalcEndPredicate::PicalcEndPredicate()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    PicalcEndPredicate::~PicalcEndPredicate()
    {

      //
      //
      //
      return;
      
    }
    
    //
    // Evaluate predicate.
    //
    bool 
    PicalcEndPredicate::evaluate(const Command & command)
    {

      //
      // open stdout
      //
      const std::string & stdOut = command.getStdOut();
      std::ifstream file(stdOut.c_str());
      
      if(!file) {
        const std::string message("Error opening file " + stdOut);
        throw arl::hms::IOError(message);
      }
      
      //
      // read lines
      //
      std::string line;
      std::string lastLine;
      std::string secondToLastLine;
      while(std::getline(file,
                         line)) {
        secondToLastLine = lastLine;
        lastLine = line;
      }
      
      //
      // check is 'pi is' is found
      //
      if(secondToLastLine.find("pi is") != std::string::npos) {
        
        //
        // completed successfully
        //
        return true;
        
      }
      
      //
      // did not find pi, return error
      //
      return false;

    }
  
  }
}
