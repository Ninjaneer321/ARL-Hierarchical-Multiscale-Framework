//
// Basic Monitor Test.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PicalcOutputFilter.h"
#include "PiValue.h"

#include <exception/IOError.h>

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

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::PicalcOutputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    PicalcOutputFilter::PicalcOutputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    PicalcOutputFilter::~PicalcOutputFilter()
    {

      //
      //
      //
      return;
    }

    //
    // Apply PicalcOutputFilter
    //
    ValuePointer 
    PicalcOutputFilter::apply(const std::string &, 
                              const std::string & stdOut,
			      const ArgumentPointer &) const
    {

      //
      // open stdOut
      //
      std::ifstream file(stdOut.c_str());

      if(!file) {
        const std::string message("Error opening file " + stdOut);
        throw arl::hms::IOError(message);
      }

      //
      // read lines
      //
      std::string line;
      std::string secondToLastLine;
      std::string lastLine;
      while(std::getline(file,
                         line)) {
        secondToLastLine = lastLine;
        lastLine = line;
      }

      if(secondToLastLine.size() < 20) {
        const std::string message("Error");
        throw IOError(message);
      }  

      //
      // extract value of pi from second to last line 
      //
      const std::string pi = secondToLastLine.substr(20, 28);
    
      //
      // convert pi string to double
      //
      const double piValue = std::atof(pi.c_str());

      std::cout << "Returning " << piValue << std::endl;
    
      //
      // instantiate return value
      //
      const arl::hms::ValuePointer value(new arl::hms::PiValue(piValue));

      //
      //
      //
      return value;

    }

  }
}
