//
// File:      PyasInterpolationDatabase.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm using the python-base pyas package
//
#if !defined(adaptive_sampling_PyasInterpolationDatabase_h)
#define adaptive_sampling_PyasInterpolationDatabase_h

#include "InterpolationDatabase.h"
#include "InterpolationDatabaseInputFilter.h"
#include "InterpolationDatabaseOutputFilter.h"

#include <utils/HMSMacros.h>

#include <pthread.h>

#if defined(HAVE_PKG_pyas)
#define PY_SSIZE_T_CLEAN
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>
#endif // HAVE_PKG_pyas

//
// forward declarations
//
//#if defined(HAVE_PKG_pyas)
//#ifndef PyObject_HEAD
//struct _object;
//typedef _object PyObject;
//#endif
//#endif // HAVE_PKG_pyas

namespace arl {
  namespace hms {

    /**
     * @brief HMS pyas Interpolation Database.
     */
    class PyasInterpolationDatabase : public InterpolationDatabase {

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
       *
       * @param inputFilter Input filter for database.
       * @param outputFilter Output filter for database.
       * @param deltaH Used to generate new points for evaluation
       * around sample point in order to build a full kriging model
       * between the query point and the model for which interpolation
       * is still attempted.
       * @param configFileName Configuration file with adaptive sampling options
       */
      PyasInterpolationDatabase(const InterpolationDatabaseInputFilterPointer & inputFilter,
				const InterpolationDatabaseOutputFilterPointer & outputFilter,
				std::vector<double> & deltaH,
				const std::string   & configFileName);

      /**
       * Destructor.
       */
      ~PyasInterpolationDatabase();

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

      /**
       * Print DB stats
       * 
       * @param outputStream Stream to be used for output.
       */
      virtual void printStatistics(std::ostream & outputStream) const;

      /**
       * Output database
       */
      virtual void outputDatabase() const;

    private:
      //
      // copy constructor/assignment operator
      //
      PyasInterpolationDatabase(const PyasInterpolationDatabase &); // not implemented
      PyasInterpolationDatabase & operator=(const PyasInterpolationDatabase &); // not implemented

      //
      // data
      //
    private:
      
      const InterpolationDatabaseInputFilterPointer d_inputFilter;
      const InterpolationDatabaseOutputFilterPointer d_outputFilter;

      const std::vector<double> d_deltaH;      
      const std::string d_configFileName;

#if defined(HAVE_PKG_pyas)
      PyObject * d_pyasDatabase;
      PyArrayObject * d_thetaArray;
      PyThreadState * d_threadState;
#endif // HAVE_PKG_pyas

      pthread_mutex_t d_lock;
      //      void * d_pyAsDatabase;
      //      void * d_thetaArray;
    };

    HMS_SHARED_PTR(PyasInterpolationDatabase);

  }
}

#endif // adaptive_sampling_PyasInterpolationDatabase_h
