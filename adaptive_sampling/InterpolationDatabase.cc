//
// File:      InterpolationDatabase.cc
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "InterpolationDatabase.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    InterpolationDatabase::InterpolationDatabase()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    InterpolationDatabase::~InterpolationDatabase()
    {

      //
      //
      //
      return;

    }

    //
    // Compute interpolated Value given Argument (batched version)
    //
    std::vector<std::pair<bool, ValuePointer> >
      InterpolationDatabase::interpolate(const std::vector<ArgumentPointer> & arguments,
					 std::vector<int>                   & hints)
    {

      //
      // get number of arguments to process
      //
      const std::vector<ArgumentPointer>::size_type numberArguments = 
	arguments.size();

      std::vector<std::pair<bool, ValuePointer> > 
	returnValue(arguments.size());
      for(std::vector<ArgumentPointer>::size_type i=0; i<numberArguments; 
	  ++i) {
	const ArgumentPointer & argument = arguments[i];
	int & hint = hints[i];
	returnValue[i] = interpolate(argument, hint);
      }

      //
      //
      //
      return returnValue;

    }

    //
    // Output database
    //
    void 
    InterpolationDatabase::outputDatabase() const
    {

      //
      //
      //
      return;

    }


    //
    // Print DB stats
    // 
    void 
    InterpolationDatabase::printStatistics(std::ostream &) const
    {

      //
      //
      //
      return;

    }

  }
}
