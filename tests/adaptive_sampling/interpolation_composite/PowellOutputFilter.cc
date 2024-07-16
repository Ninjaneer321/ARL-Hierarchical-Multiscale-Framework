//
// Basic Monitor Test.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PowellOutputFilter.h"
#include "StressValue.h"

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

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::PowellOutputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    PowellOutputFilter::PowellOutputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    PowellOutputFilter::~PowellOutputFilter()
    {

      //
      //
      //
      return;
    }

    //
    // Apply PowellOutputFilter
    //
    ValuePointer 
    PowellOutputFilter::apply(const std::string & directory, 
                              const std::string &,
			      const ArgumentPointer &) const
    {

      //
      // open micro_data.output
      //
      std::stringstream filePath;
      filePath << directory << "/" << "micro_data.output";
      std::ifstream file(filePath.str().c_str());

      if(!file) {
        const std::string message("Error opening file micro_data.output");
        throw arl::hms::IOError(message);
      }

      //
      // read lines to get to second piola kirchoff stress
      //
      std::string line;
      std::getline(file, line);
      std::getline(file, line);
      std::getline(file, line);
      std::getline(file, line);
      std::getline(file, line);
      std::getline(file, line);
      std::getline(file, line);
      std::getline(file, line);

      //
      // split line
      //
      std::vector<double> secondPiolaKirchhoffStress(6);
      std::istringstream is(line);
      is >> secondPiolaKirchhoffStress[0];
      is >> secondPiolaKirchhoffStress[1];
      is >> secondPiolaKirchhoffStress[2];
      is >> secondPiolaKirchhoffStress[3];
      is >> secondPiolaKirchhoffStress[4];
      is >> secondPiolaKirchhoffStress[5];
      
      //
      // instantiate return value
      //
      const arl::hms::ValuePointer 
        value(new arl::hms::StressValue(secondPiolaKirchhoffStress));

      std::cout << secondPiolaKirchhoffStress[0] << " " 
                << secondPiolaKirchhoffStress[1] << " "
                << secondPiolaKirchhoffStress[2] << " "
                << secondPiolaKirchhoffStress[3] << " "
                << secondPiolaKirchhoffStress[4] << " "
                << secondPiolaKirchhoffStress[5] << std::endl;

      //
      //
      //
      return value;

    }

  }
}
