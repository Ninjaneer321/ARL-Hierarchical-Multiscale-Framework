//
// File:      XYOutputFilter.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "XYOutputFilter.h"

#include "XYValue.h" 

#include <exception/IOError.h>

#if defined(HAVE_FSTREAM)
#include <fstream>
#else
#error fstream header file not available
#endif // HAVE_FSTREAM

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

HMS_SERIALIZATION_EXPORT(arl::hms::XYOutputFilter)

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    XYOutputFilter::XYOutputFilter()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    XYOutputFilter::~XYOutputFilter()
    {

      //
      //
      //
      return;

    }
   
    //
    // Apply XYOutputFilter
    //
    ValuePointer 
    XYOutputFilter::apply(const std::string & directory, 
                          const std::string &,
			  const ArgumentPointer &) const
    {

      //
      // build path to output file
      //
      std::stringstream outputFilePath;
      outputFilePath << directory << "/" << "output";
      
      //
      // open output file
      //
      std::ifstream file(outputFilePath.str().c_str());

      //
      // check file
      //
      if(!file) {
	const std::string message("Cannot open output file");
	throw IOError(message);
      }
      
      //
      // read value from output file
      //
      double value;
      file >> value;

      //
      // create XYValue containing value
      //
      XYValuePointer toReturn(new XYValue(value));
      
      //
      //
      //
      return toReturn;

    }
   
  }
}
