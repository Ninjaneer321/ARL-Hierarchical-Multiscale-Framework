//
// Basic Monitor Test.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PowellInputFilter.h"
#include "StrainArgument.h"

#include <exception/IOError.h>
#include <exception/OSError.h>

#if defined(HAVE_CERRNO)
#include <cerrno>
#else
#error cerrno header file not available
#endif // HAVE_CERRNO

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

#if defined(HAVE_STRING_H)
#include <string.h>
#else
#error string.h header file not available
#endif // HAVE_STRING_H

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#else
#error unistd.h header file not available
#endif // HAVE_UNISTD_H

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(arl::hms::PowellInputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    PowellInputFilter::PowellInputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    PowellInputFilter::PowellInputFilter(const std::string & meshFileDirectory,
                                         const std::string & meshFileName) :
      d_meshFileDirectory(meshFileDirectory),
      d_meshFileName(meshFileName)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    PowellInputFilter::~PowellInputFilter()
    {

      //
      //
      //
      return;
    }

    //
    // Apply PowellInputFilter
    //
    void
    PowellInputFilter::apply(const ArgumentPointer & argument,
                             const std::string     & directory) const
    {

      //
      // downcast argument to strain argument
      //
      StrainArgumentPointer strainArgument = 
        boost::dynamic_pointer_cast<StrainArgument>(argument);
      
      //
      // get strain from argument
      //
      const std::vector<double> & strain = strainArgument->getStrain();

      //
      // construct macro_data.input path
      //
      std::stringstream inputFilePath;
      inputFilePath << directory << "/" << "macro_data.input";

      //
      // open macro_data.input for writing
      //
      std::ofstream outputFile(inputFilePath.str().c_str());

      //
      // write strain to file
      //
      outputFile << "*LOCALIZATION_PROBLEM" << std::endl;
      outputFile << "*MACRO_STRAIN" << std::endl;
      outputFile << "$--------+---------+---------+---------+---------+";
      outputFile << "---------+---------+---------+" << std::endl;
      outputFile << "$      Exx       Eyy       Ezz       Exy       Eyz";
      outputFile << "      Ezx" << std::endl;
      outputFile << strain[0] << "\t";
      outputFile << strain[1] << "\t";
      outputFile << strain[2] << "\t";
      outputFile << strain[3] << "\t";
      outputFile << strain[4] << "\t";
      outputFile << strain[5] << std::endl;

      //
      // generate full path to original powell mesh file
      //
      std::stringstream fullMeshFilePath;
      fullMeshFilePath << d_meshFileDirectory << "/" << d_meshFileName;

      //
      // generate full path to new powell mesh file symlink
      //
      std::stringstream newFullMeshFilePath;
      newFullMeshFilePath << directory << "/" << d_meshFileName;

      //
      // link mesh to directory
      //
      if(symlink(fullMeshFilePath.str().c_str(),
                 newFullMeshFilePath.str().c_str()) != 0) {
        std::stringstream message;
        message << "Error on symlink() of mesh file for Powell model: " 
                << strerror(errno);
        throw OSError(message.str());
      }

      //
      //
      //
      return;

    }

  }
}
