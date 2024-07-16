//
// File:      NullInterpolationDatabase.cc
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "NullInterpolationDatabase.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    NullInterpolationDatabase::NullInterpolationDatabase()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    NullInterpolationDatabase::~NullInterpolationDatabase()
    {

      //
      //
      //
      return;

    }

    //
    // Compute interpolated Value given Argument.
    //
    std::pair<bool, ValuePointer>
    NullInterpolationDatabase::interpolate(const ArgumentPointer & /* argument */,
                                           int                   & /* hint */)
    {

      //
      // build false return value
      //
      
      const std::pair<bool, ValuePointer> returnValue(false,
                                                      ValuePointer());

      //
      //
      //
      return returnValue;

    }

    //
    // Insert (Argument, Value) pair into interpolation database.
    //
    bool
    NullInterpolationDatabase::insert(const ArgumentPointer & /* argument */,
                                      const ValuePointer    & /* value */,
                                      int                   & /* hint */)
    {

      //
      // insert nothing, return success
      //
      return true;

    }

    //
    // Generate a set of new arguments, centered around an argument,
    // that can be evaluated in order to bootstrap a new interpolation
    // model.
    //
    std::vector<ArgumentPointer>
    NullInterpolationDatabase::generateSampleArguments(const ArgumentPointer & /* argument */) const
    {

      //
      // generate nothing, return empty set of arguments
      //
      return std::vector<ArgumentPointer>();
      
    } 

    //
    // Insert new model containing a set of argument, value pairs into
    // interpolation database (necessary to bootstrap new kriging
    // models).
    //
    bool
    NullInterpolationDatabase::addNewModel(const std::vector<ArgumentPointer> & /* arguments */,
                                           const std::vector<ValuePointer>    & /* values */,
                                           int & /* hint */)
    {

      //
      //
      //
      return true;

    }

  }
}
