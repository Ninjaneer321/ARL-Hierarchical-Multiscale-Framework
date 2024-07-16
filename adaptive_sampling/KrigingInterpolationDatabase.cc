//
// File:      KrigingInterpolationDatabase.cc
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "KrigingInterpolationDatabase.h"

#include <exception/NotImplemented.h>
#include <exception/OSError.h>

#if defined(HAVE_PKG_asf)
#include <mtreedb/MTreeObject.h>
#include <kriging/GaussianCorrelationModel.h>
#include <kriging/LinearRegressionModel.h>
#include <kriging/ConstantRegressionModel.h>
#include <kriging/MultivariateKrigingModelFactory.h>
//#include <kriging/MultivariateKrigingModel.h>
#include <kriging_mtreedb/KrigingInterpolationDataBase.h>
#endif // HAVE_PKG_asf

#if defined(HAVE_CERRNO)
#include <cerrno>
#else
#error cerrno header file not available
#endif // HAVE_CERRNO

#if defined(HAVE_CSTRING)
#include <cstring>
#else
#error cstring header file not available
#endif // HAVE_CSTRING

//
//
//

namespace arl {
  namespace hms {

    namespace {

      //
      // FIXME: set theta value and useHint
      //
      //      const double theta = 1e6;
      const bool useHint = false;
      
    }
    
#if defined(HAVE_PKG_asf)
    
    //
    // Constructor.
    //
    KrigingInterpolationDatabase::KrigingInterpolationDatabase(const InterpolationDatabaseInputFilterPointer & inputFilter,
                                                               const InterpolationDatabaseOutputFilterPointer & outputFilter,
                                                               const std::vector<double> & deltaH,
                                                               double tolerance,
                                                               int maxKrigingModelSize,
                                                               int maxNumberSearchModels,
                                                               double meanErrorFactor,
                                                               double maxQueryPointModelDistance,
                                                               int agingThreshold,
							       const double theta,
                                                               const std::string & mtreeDirectoryName) :
      d_deltaH(deltaH),
      d_inputFilter(inputFilter),
      d_outputFilter(outputFilter),
      d_tolerance(tolerance),
      d_maxKrigingModelSize(maxKrigingModelSize),
      d_maxNumberSearchModels(maxNumberSearchModels),
      d_meanErrorFactor(meanErrorFactor),
      d_maxQueryPointModelDistance(maxQueryPointModelDistance),
      d_agingThreshold(agingThreshold),
      d_theta(theta),
      d_mtreeDirectoryName(mtreeDirectoryName),
      d_fileName("kriging_model_database_000000/krigcpl")
    {
      
      //
      // get dimensions of Argument and Value
      //
      const int argumentDimension = d_inputFilter->getDimension();
      const int valueDimension = d_outputFilter->getDimension();

      //MPTCOUPLER::krigalg::MultivariateKrigingModel::_modelCounter = 0;

      //
      // instantiate correlation and regression models
      //
      //MPTCOUPLER::krigalg::RegressionModelPointer regressionModel(new MPTCOUPLER::krigalg::ConstantRegressionModel);
      MPTCOUPLER::krigalg::RegressionModelPointer regressionModel(new MPTCOUPLER::krigalg::LinearRegressionModel);
     
      MPTCOUPLER::krigalg::CorrelationModelPointer
        correlationModel(new MPTCOUPLER::krigalg::GaussianCorrelationModel(std::vector<double>(1, theta)));

      MPTCOUPLER::krigalg::MultivariateKrigingModelFactoryPointer
        modelFactory(new MPTCOUPLER::krigalg::MultivariateKrigingModelFactory(regressionModel,
                                                                              correlationModel));

      //
      // instantiate interpolation database
      //
      d_interpolationDatabase =
        new MPTCOUPLER::krigcpl::KrigingInterpolationDataBase(argumentDimension,
                                                              valueDimension,
                                                              modelFactory,
                                                              maxKrigingModelSize,
                                                              maxNumberSearchModels,
                                                              useHint,
                                                              meanErrorFactor,
                                                              tolerance,
                                                              maxQueryPointModelDistance,
                                                              agingThreshold,
                                                              mtreeDirectoryName);

      //
      // allocate database lock attribute
      //
      d_readWriteLockAttribute = new pthread_rwlockattr_t;

      //
      // initialize database lock attribute
      //
#if !defined(__MACH__)
      pthread_rwlockattr_init(d_readWriteLockAttribute);
      pthread_rwlockattr_setkind_np(d_readWriteLockAttribute,
				    PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
#endif // ! __MACH__
      
      //
      // allocate database lock
      //
      d_readWriteLock = new pthread_rwlock_t;

      //
      // initialize database lock
      //
      if(pthread_rwlock_init(d_readWriteLock,
                             d_readWriteLockAttribute) != 0) {
        std::stringstream message;
        message << "Failure on pthread_rwlock_init() of interpolation database "
          "lock: " << strerror(errno);
        throw OSError(message.str());
      }
      
      //
      //
      //
      return;
      
    }
    
    //
    // Constructor.
    //
    KrigingInterpolationDatabase::KrigingInterpolationDatabase(const InterpolationDatabaseInputFilterPointer & inputFilter,
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
							       const std::string & fileName) :
      d_deltaH(deltaH),
      d_inputFilter(inputFilter),
      d_outputFilter(outputFilter),
      d_tolerance(tolerance),
      d_maxKrigingModelSize(maxKrigingModelSize),
      d_maxNumberSearchModels(maxNumberSearchModels),
      d_meanErrorFactor(meanErrorFactor),
      d_maxQueryPointModelDistance(maxQueryPointModelDistance),
      d_agingThreshold(agingThreshold),
      d_theta(theta),
      d_mtreeDirectoryName(mtreeDirectoryName),
      d_fileName(fileName)
    {
      
      //
      // get dimensions of Argument and Value
      //
      const int argumentDimension = d_inputFilter->getDimension();
      const int valueDimension = d_outputFilter->getDimension();

      //
      // instantiate correlation and regression models
      //
      //MPTCOUPLER::krigalg::RegressionModelPointer regressionModel(new MPTCOUPLER::krigalg::ConstantRegressionModel);
      MPTCOUPLER::krigalg::RegressionModelPointer regressionModel(new MPTCOUPLER::krigalg::LinearRegressionModel);

      MPTCOUPLER::krigalg::CorrelationModelPointer
        correlationModel(new MPTCOUPLER::krigalg::GaussianCorrelationModel(std::vector<double>(1, theta)));

      MPTCOUPLER::krigalg::MultivariateKrigingModelFactoryPointer
        modelFactory(new MPTCOUPLER::krigalg::MultivariateKrigingModelFactory(regressionModel,
                                                                              correlationModel));

      //
      // instantiate interpolation database
      //
      d_interpolationDatabase =
        new MPTCOUPLER::krigcpl::KrigingInterpolationDataBase(argumentDimension,
                                                              valueDimension,
                                                              modelFactory,
                                                              maxKrigingModelSize,
                                                              maxNumberSearchModels,
                                                              useHint,
                                                              meanErrorFactor,
                                                              tolerance,
                                                              maxQueryPointModelDistance,
                                                              agingThreshold,
                                                              mtreeDirectoryName,
							      fileName);

      //
      // allocate database lock attribute
      //
      d_readWriteLockAttribute = new pthread_rwlockattr_t;

      //
      // initialize database lock attribute
      //
#if !defined(__MACH__)
      pthread_rwlockattr_init(d_readWriteLockAttribute);
      pthread_rwlockattr_setkind_np(d_readWriteLockAttribute,
				    PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
#endif // ! __MACH__

      //
      // allocate database lock
      //
      d_readWriteLock = new pthread_rwlock_t;

      //
      // initialize database lock
      //
      if(pthread_rwlock_init(d_readWriteLock,
                             d_readWriteLockAttribute) != 0) {
        std::stringstream message;
        message << "Failure on pthread_rwlock_init() of interpolation database "
          "lock: " << strerror(errno);
        throw OSError(message.str());
      }

      //
      //
      //
      return;

    }

#else

    //
    // Constructor.
    //
    KrigingInterpolationDatabase::KrigingInterpolationDatabase(const InterpolationDatabaseInputFilterPointer & /* inputFilter */,
                                                               const InterpolationDatabaseOutputFilterPointer & /*outputFilter */,
                                                               const std::vector<double> & /* deltaH */,
                                                               double /* tolerance */,
                                                               int /* maxKrigingModelSize */,
                                                               int /* maxNumberSearchModels */,
                                                               double /* meanErrorFactor */,
                                                               double /* maxQueryPointModelDistance */,
                                                               int /* agingThreshold */,
							       const double /* theta */,
                                                               const std::string & /*mtreeDirectoryName */)
    {

      //
      // Throw exception 
      //
      const std::string message("HMS not compiled with asf package to generate Kriging surrogate model");
      throw NotImplemented(message);

      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    KrigingInterpolationDatabase::KrigingInterpolationDatabase(const InterpolationDatabaseInputFilterPointer & /* inputFilter */,
							       const InterpolationDatabaseOutputFilterPointer & /* outputFilter */,
							       const std::vector<double> & /* deltaH */,
							       double /* tolerance */,
							       int /* maxKrigingModelSize */,
							       int /* maxNumberSearchModels */,
							       double /* meanErrorFactor */,
							       double /* maxQueryPointModelDistance */,
							       int /* agingThreshold */,
							       const double /* theta */,
							       const std::string & /* mtreeDirectoryName */,
							       const std::string & /* fileName */) 
    {

      //
      // Throw exception 
      //
      const std::string message("HMS not compiled with asf package to generate Kriging surrogate model");
      throw NotImplemented(message);

      //
      //
      //
      return;

    }

#endif // HAVE_PKG_asf

    //
    // Destructor.
    //
    KrigingInterpolationDatabase::~KrigingInterpolationDatabase()
    {

#if defined(HAVE_PKG_asf)
      //
      // destroy database lock
      //
      pthread_rwlock_destroy(d_readWriteLock);

      //
      // destroy database attribute
      //
      pthread_rwlockattr_destroy(d_readWriteLockAttribute);

      //
      // deallocate database lock
      //
      delete d_readWriteLock;

      //
      // deallocate database lock attribute
      //
      delete d_readWriteLockAttribute;

      //
      // deallocate database
      //
      delete d_interpolationDatabase;

#endif // HAVE_PKG_asf

      //
      //
      //
      return;

    }

    //
    // Compute interpolated Value given Argument.
    //
    std::pair<bool, ValuePointer>
    KrigingInterpolationDatabase::interpolate(const ArgumentPointer & argument,
                                              int                   & hint)
    {

#if defined(HAVE_PKG_asf)

      //
      // apply input filter to argument
      //
      const std::vector<double> databasePoint = d_inputFilter->apply(argument);

      //
      // instantiate databaseValue
      //
      std::vector<double> databaseValue(d_outputFilter->getDimension());

      //
      // instantiate interpolateFlags
      //
      std::vector<bool> interpolateFlags(MPTCOUPLER::krigcpl::InterpolationDataBase::NUMBER_FLAGS);

      //
      // acquire read lock
      //
      if(pthread_rwlock_rdlock(d_readWriteLock) != 0) {
        std::stringstream message;
        message << "Failure on pthread_rwlock_rdlock() acquiring read lock to "
          "interpolation database:" << strerror(errno);
        throw OSError(message.str());
      }

      //
      // interpolate argument
      //
      const bool interpolationResult =
        d_interpolationDatabase->interpolate(&(databaseValue[0]),
                                             hint,
                                             &(databasePoint[0]),
                                             interpolateFlags);

      //
      // release read lock
      //
      if(pthread_rwlock_unlock(d_readWriteLock) != 0) {
        std::stringstream message;
        message << "Failure on pthread_rwlock_unlock() releasing read lock to "
          "interpolation database:" << strerror(errno);
        throw OSError(message.str());
      }

      //
      // apply output filter
      //
      const ValuePointer value = d_outputFilter->apply(databaseValue,
                                                       argument);

      //
      // instantiate returnValue
      //
      const std::pair<bool, ValuePointer> returnValue(interpolationResult,
                                                      value);
#else
      //
      // instantiate dummy returnValue
      //
      std::pair<bool, ValuePointer> returnValue(false, ValuePointer());

#endif //HAVE_PKG_asf

      //
      //
      //
      return returnValue;

    }

    //
    // Insert (Argument, Value) pair into interpolation database.
    //
    bool
    KrigingInterpolationDatabase::insert(const ArgumentPointer & argument,
                                         const ValuePointer    & value,
                                         int                   & hint)
    {

#if defined(HAVE_PKG_asf)

      //
      // apply input filter to argument
      //
      const std::vector<double> databasePoint = d_inputFilter->apply(argument);

      //
      // apply inverse output filter to value
      //
      const std::vector<double> databaseValue =
        d_outputFilter->applyInverse(value);

      //
      // instantiate interpolateFlags
      //
      std::vector<bool>
        interpolateFlags(MPTCOUPLER::krigcpl::InterpolationDataBase::NUMBER_FLAGS);

      //
      // acquire write lock
      //
      if(pthread_rwlock_wrlock(d_readWriteLock) != 0) {
        std::stringstream message;
        message << "Failure on pthread_rwlock_wrlock() acquiring write lock to "
          "interpolation database:" << strerror(errno);
        throw OSError(message.str());
      }

      //
      // insert (argument, value) into database
      //
      const bool insertSuccess =
        d_interpolationDatabase->insert(hint,
                                        &(databasePoint[0]),
                                        &(databaseValue[0]),
                                        NULL,
                                        interpolateFlags);
      //
      // release write lock
      //
      if(pthread_rwlock_unlock(d_readWriteLock) != 0) {
        std::stringstream message;
        message << "Failure on pthread_rwlock_unlock() releasing write lock to "
          "interpolation database:" << strerror(errno);
        throw OSError(message.str());
      }

      //
      //
      //
      return insertSuccess;

#else

      //
      //
      //
      return false;

#endif // HAVE_PKG_asf

    }

    //
    // Generate a set of new arguments, centered around an argument,
    // that can be evaluated in order to bootstrap a new interpolation
    // model.
    //
    std::vector<ArgumentPointer>
    KrigingInterpolationDatabase::generateSampleArguments(const ArgumentPointer & argument) const
    {

      //
      // instantiate return value
      //
      std::vector<ArgumentPointer> returnValue;

#if defined(HAVE_PKG_asf)
      
      //
      // apply input filter to argument
      //
      const std::vector<double> databasePoint = d_inputFilter->apply(argument);

      //
      // sanity check 
      //
      assert(databasePoint.size() == d_deltaH.size());

      //
      // sample around point using deltaH values.
      //
      for(std::vector<double>::size_type i=0; i<databasePoint.size(); ++i) {
        
        //
        // get deltaH in i dimension
        //
        const double deltaH = d_deltaH[i];
        
        //
        // generate minus deltaH point in i dimension
        //
        std::vector<double> minusHPoint = databasePoint;
        minusHPoint[i] -= deltaH;

        //
        // apply input filter to generate minus deltaH argument
        //
        ArgumentPointer minusHArgument = 
          d_inputFilter->applyInverse(minusHPoint,
                                      argument);
   
        //
        // add point to returnValue
        //
        returnValue.push_back(minusHArgument);

        //
        // generate plus deltaH point in i dimension
        //
        std::vector<double> plusHPoint = databasePoint;
        plusHPoint[i] += deltaH;

        //
        // apply input filter to generate plus deltaH argument
        //
        ArgumentPointer plusHArgument = 
          d_inputFilter->applyInverse(plusHPoint,
                                      argument);
   
        //
        // add point to returnValue
        //
        returnValue.push_back(plusHArgument);
        
      }

#endif // HAVE_PKG_asf

      //
      //
      //
      return returnValue;
      
    } 

    //
    // Insert new model containing a set of argument, value pairs into
    // interpolation database (necessary to bootstrap new kriging
    // models).
    //
    bool
    KrigingInterpolationDatabase::addNewModel(const std::vector<ArgumentPointer> & arguments,
                                              const std::vector<ValuePointer>    & values,
                                              int & hint)
    {
#if defined(HAVE_PKG_asf)
      //
      // instantiate list of points, values, and gradients
      //
      std::vector<std::vector<double> > databasePoints(arguments.size());
      std::vector<std::vector<double> > databaseValues(arguments.size());
      std::vector<std::vector<double> > databaseGradients(arguments.size());

      //
      // apply input filters to arguments
      //
      for(std::vector<ArgumentPointer>::size_type i=0; i<arguments.size();
          ++i) {

        const ArgumentPointer & argument = arguments[i];

        const std::vector<double> databasePoint =
          d_inputFilter->apply(argument);

        databasePoints[i] = databasePoint;

      }

      //
      // apply inverse output filters to values
      //
      for(std::vector<ValuePointer>::size_type i=0; i<values.size();
          ++i) {

        const ValuePointer & value = values[i];

        const std::vector<double> databaseValue =
          d_outputFilter->applyInverse(value);

        databaseValues[i] = databaseValue;

      }

      //
      // instantiate interpolateFlags
      //
      std::vector<bool>
        interpolateFlags(MPTCOUPLER::krigcpl::InterpolationDataBase::NUMBER_FLAGS);

      //
      // acquire write lock
      //
      if(pthread_rwlock_wrlock(d_readWriteLock) != 0) {
        std::stringstream message;
        message << "Failure on pthread_rwlock_wrlock() acquiring write lock to "
          "interpolation database:" << strerror(errno);
        throw OSError(message.str());
      }

      //
      // add new model containing points and values into database
      //
      const bool addNewModelSuccess =
        d_interpolationDatabase->addNewModel(hint,
                                             databasePoints,
                                             databaseValues,
                                             databaseGradients,
                                             interpolateFlags);

      //
      // release write lock
      //
      if(pthread_rwlock_unlock(d_readWriteLock) != 0) {
        std::stringstream message;
        message << "Failure on pthread_rwlock_unlock() releasing write lock to "
          "interpolation database:" << strerror(errno);
        throw OSError(message.str());
      }

      //
      //
      //
      return addNewModelSuccess;

#else

      //
      //
      //
      return false;

#endif // HAVE_PKG_asf

    }

    //
    // Print DB stats
    // 
    void 
    KrigingInterpolationDatabase::printStatistics(std::ostream & outputStream) const
    {

#if defined(HAVE_PKG_asf)
      //
      //
      //
      d_interpolationDatabase->printDBStats(outputStream);
#endif // HAVE_PKG_asf

      //
      //
      //
      return;

    }

    //
    // Output database
    //
    void 
    KrigingInterpolationDatabase::outputDatabase() const
    {

#if defined(HAVE_PKG_asf)
      //
      // acquire write lock
      //
      if(pthread_rwlock_wrlock(d_readWriteLock) != 0) {
        std::stringstream message;
        message << "Failure on pthread_rwlock_wrlock() acquiring write lock to "
          "interpolation database:" << strerror(errno);
        throw OSError(message.str());
      }

      //
      // this is really hacky!
      //
      delete d_interpolationDatabase;

      const int argumentDimension = d_inputFilter->getDimension();
      const int valueDimension = d_outputFilter->getDimension();

      //
      // instantiate correlation and regression models
      //
      //MPTCOUPLER::krigalg::RegressionModelPointer regressionModel(new MPTCOUPLER::krigalg::ConstantRegressionModel);
      MPTCOUPLER::krigalg::RegressionModelPointer regressionModel(new MPTCOUPLER::krigalg::LinearRegressionModel);

      MPTCOUPLER::krigalg::CorrelationModelPointer
        correlationModel(new MPTCOUPLER::krigalg::GaussianCorrelationModel(std::vector<double>(1, d_theta)));

      MPTCOUPLER::krigalg::MultivariateKrigingModelFactoryPointer
        modelFactory(new MPTCOUPLER::krigalg::MultivariateKrigingModelFactory(regressionModel,
                                                                              correlationModel));

      d_interpolationDatabase = 
	new MPTCOUPLER::krigcpl::KrigingInterpolationDataBase(argumentDimension,
                                                              valueDimension,
                                                              modelFactory,
                                                              d_maxKrigingModelSize,
                                                              d_maxNumberSearchModels,
                                                              useHint,
                                                              d_meanErrorFactor,
                                                              d_tolerance,
                                                              d_maxQueryPointModelDistance,
                                                              d_agingThreshold,
                                                              d_mtreeDirectoryName,
							      d_fileName);
     
      //
      // release write lock
      //
      if(pthread_rwlock_unlock(d_readWriteLock) != 0) {
        std::stringstream message;
        message << "Failure on pthread_rwlock_unlock() releasing write lock to "
          "interpolation database:" << strerror(errno);
        throw OSError(message.str());
      }

#endif // HAVE_PKG_asf
    }


  }
}
