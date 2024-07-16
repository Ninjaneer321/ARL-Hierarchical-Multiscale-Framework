//
// File:      XYInputFilter.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "XYInputFilter.h"

#include "XYArgument.h"

#if defined(HAVE_FSTREAM)
#include <fstream>
#else
#error fstream header file not available
#endif // HAVE_FSTREAM

#if defined(HAVE_IOMANIP)
#include <iomanip>
#else
#error iomanip header file not available
#endif // HAVE_IOMANIP

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

HMS_SERIALIZATION_EXPORT(arl::hms::XYInputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    XYInputFilter::XYInputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    XYInputFilter::~XYInputFilter()
    {

      //
      //
      //
      return;

    }
    
    //
    // Apply XYInputFilter
    //
    void 
    XYInputFilter::apply(const ArgumentPointer & argument, 
                         const std::string     & directory) const
    {

      //
      // cast argument to XYArgument
      //
      XYArgumentPointer xyArgument = 
        boost::dynamic_pointer_cast<XYArgument>(argument);
      
      //
      // get x and y value;
      //
      const double x = xyArgument->getX();
      const double y = xyArgument->getY();

      //
      // create filePath for xy input
      //
      std::stringstream filePath;
      filePath << directory << "/" << "input";

      //
      // open file and write x, y value
      //
      std::ofstream file(filePath.str().c_str());
      file << std::setprecision(16);
      file << x << std::endl;
      file << y << std::endl;

      //
      //
      //
      return;

    }
   
  }
}
