//
// File:      KrigingInterpolationDatabase.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_KrigingInterpolationDatabase_h)
#define adaptive_sampling_KrigingInterpolationDatabase_h

#include "InterpolationDatabase.h"
#include "InterpolationDatabaseInputFilter.h"
#include "InterpolationDatabaseOutputFilter.h"

#include <utils/HMSMacros.h>

#include <pthread.h>

//
// forward declarations
//
#if defined(HAVE_PKG_asf)
namespace MPTCOUPLER {
  namespace krigcpl {
    class KrigingInterpolationDataBase;
  }
}
#endif // HAVE_PKG_asf

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief HMS Kriging Interpolation Database.
     */
    class KrigingInterpolationDatabase : public InterpolationDatabase {

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
       * @param tolerance Tolerance for interpolation.
       * @param maxKrigingModelSize Maximum number of point/value
       * pairs in a single kriging model.
       * @param maxNumberSearchModels Maximum number of kriging models
       * to be pulled out of the database during the search for the
       * best kriging model.
       * @param meanErrorFactor The value of the coefficient
       * multiplying the mean square error.
       * @param maxQueryPointModelDistance The maximum distance
       * between the query point and the model for which interpolation
       * is still attempted.
       * @param agingThreshold Time threshold for object aging.
       * @param theta Hyperparameter for correlation function
       * @param mtreeDirectoryName Name of the directory to use for
       * storage of disk MTree data.
       */
      KrigingInterpolationDatabase(const InterpolationDatabaseInputFilterPointer & inputFilter,
                                   const InterpolationDatabaseOutputFilterPointer & outputFilter,
                                   const std::vector<double> & deltaH,
                                   double tolerance,
                                   int maxKrigingModelSize,
                                   int maxNumberSearchModels,
                                   double meanErrorFactor,
                                   double maxQueryPointModelDistance,
                                   int agingThreshold,
				   const double theta,
                                   const std::string & mtreeDirectoryName);

      /**
       * Constructor.
       *
       * @param inputFilter Input filter for database.
       * @param outputFilter Output filter for database.
       * @param deltaH Used to generate new points for evaluation
       * around sample point in order to build a full kriging model
       * @param tolerance Tolerance for interpolation.
       * @param maxKrigingModelSize Maximum number of point/value
       * pairs in a single kriging model.
       * @param maxNumberSearchModels Maximum number of kriging models
       * to be pulled out of the database during the search for the
       * best kriging model.
       * @param meanErrorFactor The value of the coefficient
       * multiplying the mean square error.
       * @param maxQueryPointModelDistance The maximum distance
       * between the query point and the model for which interpolation
       * is still attempted.
       * @param agingThreshold Time threshold for object aging.
       * @param theta Hyperparameter for correlation function
       * @param mtreeDirectoryName Name of the directory to use for
       * storage of disk MTree data.
       * @param fileName File name to be used for seeding the database.
       */
      KrigingInterpolationDatabase(const InterpolationDatabaseInputFilterPointer & inputFilter,
                                   const InterpolationDatabaseOutputFilterPointer & outputFilter,
                                   const std::vector<double> & deltaH,
                                   double tolerance,
                                   int maxKrigingModelSize,
                                   int maxNumberSearchModels,
                                   double meanErrorFactor,
                                   double maxQueryPointModelDistance,
                                   int agingThreshold,
				   const double theta,
                                   const std::string & mtreeDirectoryName,
				   const std::string & fileName);

      /**
       * Destructor.
       */
      ~KrigingInterpolationDatabase();

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
      KrigingInterpolationDatabase(const KrigingInterpolationDatabase &); // not implemented
      KrigingInterpolationDatabase & operator=(const KrigingInterpolationDatabase &); // not implemented

      //
      // data
      //
    private:

#if defined(HAVE_PKG_asf)
      std::vector<double> d_deltaH;
      const InterpolationDatabaseInputFilterPointer d_inputFilter;
      mutable MPTCOUPLER::krigcpl::KrigingInterpolationDataBase * d_interpolationDatabase;
      const InterpolationDatabaseOutputFilterPointer d_outputFilter;
      pthread_rwlock_t * d_readWriteLock;      
      pthread_rwlockattr_t * d_readWriteLockAttribute;

      //
      // for reinstantiation of database
      // 
      double d_tolerance;
      int d_maxKrigingModelSize;
      int d_maxNumberSearchModels;
      double d_meanErrorFactor;
      double d_maxQueryPointModelDistance;
      int d_agingThreshold;
      double d_theta;
      std::string d_mtreeDirectoryName;
      std::string d_fileName;
#endif // HAVE_PKG_asf

    };

    HMS_SHARED_PTR(KrigingInterpolationDatabase);

  }
}

#endif // adaptive_sampling_KrigingInterpolationDatabase_h
