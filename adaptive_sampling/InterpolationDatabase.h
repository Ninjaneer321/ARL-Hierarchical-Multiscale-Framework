//
// File:      InterpolationDatabase.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_InterpolationDatabase_h)
#define adaptive_sampling_InterpolationDatabase_h

#include <base/Argument.h>
#include <base/Value.h>

#include <utils/HMSMacros.h>

#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class for HMS Interpolation Databases.
     */
    class InterpolationDatabase {

      //
      // types
      //
    public:

      //
      // methods
      //
    public:

      /**
       * Destructor.
       */
      virtual ~InterpolationDatabase() = 0;

      /**
       * Compute interpolated Value given Argument.
       *
       * @param argument Argument.
       * @param hint Hint for database.
       *
       * @return Pair containing: bool, whether interpolation was
       * successful and Value.
       */
      virtual std::pair<bool, ValuePointer>
      interpolate(const ArgumentPointer & argument,
                  int                   & hint) = 0;

      /**
       * Compute interpolated Value given Argument (batched version)
       *
       * @param arguments Arguments
       * @param hints Hints for database
       *
       * @return Vector of pairs containing: bool, whether interpolation was
       * successful and Value.
       */
      virtual std::vector<std::pair<bool, ValuePointer> >
      interpolate(const std::vector<ArgumentPointer> & arguments,
		  std::vector<int>                   & hints);
	
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
      virtual bool
      insert(const ArgumentPointer & argument,
             const ValuePointer    & value,
             int                   & hint) = 0;
      
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
      virtual std::vector<ArgumentPointer>
      generateSampleArguments(const ArgumentPointer & argument) const = 0;
        
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
      virtual bool
      addNewModel(const std::vector<ArgumentPointer> & arguments,
                  const std::vector<ValuePointer>    & values,
                  int                                & hint) = 0;

      /**
       * Output database
       */
      virtual void outputDatabase() const;

      /**
       * Print DB stats
       * 
       * @param outputStream Stream to be used for output.
       */
      virtual void printStatistics(std::ostream & outputStream) const;

    protected:
      
      /**
       * Constructor.
       */
      InterpolationDatabase();

    private:
      //
      // copy constructor/assignment operator
      //
      InterpolationDatabase(const InterpolationDatabase &); // not implemented
      InterpolationDatabase & operator=(const InterpolationDatabase &); // not implemented

      //
      // data
      //
    private:

    };

    HMS_SHARED_PTR(InterpolationDatabase);

  }
}

#endif // adaptive_sampling_InterpolationDatabase_h
