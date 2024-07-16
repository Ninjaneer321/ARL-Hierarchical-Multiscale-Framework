//
// File:      NullInterpolationDatabase.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_NullInterpolationDatabase_h)
#define adaptive_sampling_NullInterpolationDatabase_h

#include "InterpolationDatabase.h"

#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief HMS Null Interpolation Database. Always fails to interpolate.
     */
    class NullInterpolationDatabase : public InterpolationDatabase {

      //
      // types
      //
    public:

      //
      // methods
      //
    public:

      /**
       * Constructor.
       */
      NullInterpolationDatabase();

      /**
       * Destructor.
       */
      ~NullInterpolationDatabase();

      /**
       * Compute interpolated Value given Argument.
       *
       * @param argument Argument.
       * @param hint Hint for database.
       *
       * @return Pair containing: bool, whether interpolation was
       * successful and Value.
       */
      std::pair<bool, ValuePointer>
      interpolate(const ArgumentPointer & argument,
                  int                   & hint);

      /**
       * Insert (Argument, Value) pair into interpolation database.
       *
       * @param argument Argument.
       * @param value Value.
       * @param hint Hint for database.
       *
       * @return Bool whether insertion was successful, if not,
       * insufficient points exist in the database and a new model
       * needs to be bootstrapped.
       */
      bool
      insert(const ArgumentPointer & argument,
             const ValuePointer    & value,
             int                   & hint);
      
      /**
       * Generate a set of new Arguments, centered around an argument,
       * that can be evaluated in order to bootstrap a new
       * interpolation model.
       *
       * @param argument Argument to sample around.
       *
       * @return Vector of new Arguments that can be used to boostrap
       * a new interpolation model.
       */
      std::vector<ArgumentPointer>
      generateSampleArguments(const ArgumentPointer & argument) const;
        
      /**
       * Insert new model containing a set of argument, value pairs
       * into interpolation database (necessary to bootstrap new
       * kriging models).
       *
       * @param arguments Vector of arguments to add.
       * @param values Vector of values to add.
       * @param hint Hint for database.
       *
       * @return Bool whether creation of new kriging model was
       * successful.
       */
      bool
      addNewModel(const std::vector<ArgumentPointer> & arguments,
                  const std::vector<ValuePointer>    & values,
                  int                                & hint);

    private:
      //
      // copy constructor/assignment operator
      //
      NullInterpolationDatabase(const NullInterpolationDatabase &); // not implemented
      NullInterpolationDatabase & operator=(const NullInterpolationDatabase &); // not implemented

      //
      // data
      //
    private:

    };

    HMS_SHARED_PTR(NullInterpolationDatabase);

  }
}

#endif // adaptive_sampling_NullInterpolationDatabase_h
