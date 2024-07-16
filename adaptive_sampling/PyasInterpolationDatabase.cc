//
// File:      PyasInterpolationDatabase.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm using the python-base pyas package
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "PyasInterpolationDatabase.h"

#include <exception/NotImplemented.h>
#include <exception/OSError.h>

#include <iostream>

#if defined(HAVE_CSTRING)
#include <cstring>
#else
#error cstring header file not available
#endif // HAVE_CSTRING

#include <sstream>


//
//
//

namespace arl {
  namespace hms {

    namespace {
      // Hack because import_array() wants to return a value which a constructor
      // cannot do
      void
      import_numpy_array() {
#if defined(HAVE_PKG_pyas)
	_import_array();
#endif
      }
    }
        
    //
    // Constructor.
    //
    PyasInterpolationDatabase::PyasInterpolationDatabase(const InterpolationDatabaseInputFilterPointer & inputFilter,
			      const InterpolationDatabaseOutputFilterPointer & outputFilter,
			      std::vector<double> & deltaH,
			      const std::string   & configFileName) :
      d_inputFilter(inputFilter),
      d_outputFilter(outputFilter),
      d_deltaH(deltaH),
      d_configFileName(configFileName)
#if defined(HAVE_PKG_pyas)
      ,
      d_pyasDatabase(NULL),
      d_thetaArray(NULL),
      d_threadState(NULL)
#endif
    {

#if defined(HAVE_PKG_pyas)

      //
      // Initializes the Python interpreter
      //
      Py_Initialize();      
      //import_array();
      import_numpy_array();

      //
      // Load the module object
      //
      PyObject * pModuleName = PyUnicode_FromString("pyas.database");

      PyObject * pModule = PyImport_Import(pModuleName);
      if (pModule == NULL) {
	PyErr_Print();
	const std::string message("Failed to import pyas.database module");
	throw OSError(message);	
      }
      Py_XDECREF(pModuleName);

      //
      // Load the module's dict
      //
      PyObject * pDict = PyModule_GetDict(pModule);
      if (pDict == NULL) {
	PyErr_Print();
	const std::string message("Failed to get dict for the "
				  "pyas.database module");
	throw OSError(message);	
      }
      Py_XDECREF(pModule);

      //
      // Build the name of a callable class
      //
      // Fixme: Eventually we should be able to select which Database
      //        class to instantiate
      //
      PyObject * pClass = PyDict_GetItemString(pDict, "AdaptiveDatabaseINI");
      if (pClass == NULL || !PyCallable_Check(pClass)) {
	PyErr_Print();
	const std::string message("Failed to get the pyas.database class");
	throw OSError(message);	
      }
      Py_XDECREF(pDict);

      //
      // Creates an instance of the class
      //
      PyObject * pClassArg;
      pClassArg = Py_BuildValue("(s)",
				  d_configFileName.c_str());

      d_pyasDatabase = PyObject_CallObject(pClass, pClassArg);

      Py_XDECREF(pClass);
      Py_XDECREF(pClassArg);

      if (d_pyasDatabase == NULL) {
	PyErr_Print();
	const std::string message("Failed to instantiate the database object");
	throw OSError(message);	
      }

      //
      // Release the GIL so other threads can take it
      //
      d_threadState = PyEval_SaveThread();

      //
      // Create the thread lock so that only one python call occurs at a time
      // 
      if (pthread_mutex_init(&d_lock,NULL) != 0) {
	std::stringstream message;
        message << "Failure to create pthread mutex in Pyas " 
	  "interpolation database:" << strerror(errno);
        throw OSError(message.str());
      }

#else

      //
      // Throw exception 
      //
      const std::string message("HMS not compiled with pyas package "
				"to generate surrogate model");
      throw NotImplemented(message);

#endif // HAVE_PKG_pyas

      //
      //
      //
      return;      
    }
  
    //
    // Destructor.
    //
    PyasInterpolationDatabase::~PyasInterpolationDatabase()
    {

#if defined(HAVE_PKG_pyas)

      // Write the database to an output file
      outputDatabase();

      // deallocate python objects
      Py_XDECREF(d_pyasDatabase);
      Py_XDECREF(d_thetaArray);

      PyEval_RestoreThread(d_threadState);

      // Close python interpreter
      Py_Finalize();

      pthread_mutex_destroy(&d_lock);

#endif // HAVE_PKG_pyas

      return;

    }

    //
    // Compute interpolated Value given Argument.
    //
    std::pair<bool, ValuePointer>
    PyasInterpolationDatabase::interpolate(const ArgumentPointer & argument,
                                              int                   & /*hint*/)
    {

      //
      // instantiate dummy returnValue
      //
      std::pair<bool, ValuePointer> returnValue(false, ValuePointer());

#if defined(HAVE_PKG_pyas)

      pthread_mutex_lock(&d_lock);

      //
      // Get lock to python interpreter
      //
      PyGILState_STATE gState;
      gState =  PyGILState_Ensure();

      // apply input filter to argument
      const std::vector<double> databasePoint = d_inputFilter->apply(argument);
      
      // convert the input point to a python array
      const unsigned int point_dim = databasePoint.size();
      npy_intp dims[2];
      dims[0] = 1;
      dims[1] = point_dim;
      
      PyArrayObject * inArray= 
	(PyArrayObject *)PyArray_SimpleNewFromData(2,dims,
						   NPY_DOUBLE,
						   (void *)&(databasePoint[0]));

      // Compute the interpolated value at the point
      PyObject * retVal = PyObject_CallMethod(d_pyasDatabase,
					       "interpolate",
					       "(O)",inArray);

      if (retVal == NULL) {
	PyErr_Print();
	const std::string message("Failed in python call to interpolate()");
	throw OSError(message);	
      }

      // Parse the return value
      int interpSuccess;
      PyArrayObject * interpValueArray;
      int parseSuccess = PyArg_ParseTuple(retVal,"iO",
					  &interpSuccess,
					  &interpValueArray);
      if (!parseSuccess) {
	PyErr_Print();
	const std::string message("Failed to parse results from python "
				  "call to interpolate()");
	throw OSError(message);	
      }

      if (interpSuccess) {
      
	// Store the interpolated value array into an STL vector
	int filter_value_dim = d_outputFilter->getDimension();
	unsigned int numOutDims = PyArray_NDIM(interpValueArray);
	npy_intp * outDims = PyArray_DIMS(interpValueArray);
	unsigned int value_dim = outDims[numOutDims-1];
	
	// Make sure that the returned array is either 1D, or 1XN since we only
	// asked to interpolate a single point
	assert(numOutDims == 1 || outDims[0] == 1);
	assert(value_dim == filter_value_dim);
	
	double * outCArray = (double *)PyArray_DATA(interpValueArray);
	std::vector<double> databaseValue(outCArray, outCArray + value_dim);
	
	//
	// apply output filter
	//
	const ValuePointer value = d_outputFilter->apply(databaseValue,
							 argument);

	//
	// fill returnValue
	//
	returnValue.first = true;
	returnValue.second = value;
      }

      Py_XDECREF(inArray);
      Py_XDECREF(retVal);
      // We do NOT need to call Py_XDECREF on interpValueArray because it is
      // a borrowed reference
      //Py_XDECREF(interpValueArray);

      PyGILState_Release(gState);

      pthread_mutex_unlock(&d_lock);

#endif //HAVE_PKG_pyas

      //
      //
      //
      return returnValue;

    }

    //
    // Insert (Argument, Value) pair into interpolation database.
    //
    bool
    PyasInterpolationDatabase::insert(const ArgumentPointer & argument,
                                         const ValuePointer    & value,
                                         int                   & /*hint*/)
    {
      int insertSuccess = 0;
      
#if defined(HAVE_PKG_pyas)

      pthread_mutex_lock(&d_lock);

      //
      // Get lock to python interpreter
      //
      PyGILState_STATE gState;
      gState =  PyGILState_Ensure();

      //
      // apply input filter to argument
      //
      const std::vector<double> databasePoint = d_inputFilter->apply(argument);

      //
      // apply inverse output filter to value
      //
      const std::vector<double> databaseValue =
        d_outputFilter->applyInverse(value);

      // convert the input point to a python array
      npy_intp pointDims[2];
      pointDims[0] = 1;
      pointDims[1] = databasePoint.size();

      PyArrayObject * pointArray= 
	(PyArrayObject *)PyArray_SimpleNewFromData(2,pointDims,
						   NPY_DOUBLE,
						   (void *)&(databasePoint[0]));
      // convert the value to a python array
      npy_intp valueDims[2];
      valueDims[0] = 1;
      valueDims[1] = databaseValue.size();
      
      PyArrayObject * valueArray= 
	(PyArrayObject *)PyArray_SimpleNewFromData(2,valueDims,
						   NPY_DOUBLE,
						   (void *)&(databaseValue[0]));


      std::cout << "INSERTING: [" << databasePoint[0] << ", " 
		<< databasePoint[1] << ", " << databasePoint[2] << "] , [" 
		<< databaseValue[0] << ", " << databaseValue[1] << ", " 
		<< databaseValue[2] << "]" << std::endl;

      PyObject * retVal = PyObject_CallMethod(d_pyasDatabase,
					      "insert_point","(OO)",
					      pointArray,valueArray);
      if (retVal == NULL) {
	PyErr_Print();
	const std::string message("Failed to call insert_point()");
	throw OSError(message);	
      }

      insertSuccess = PyLong_AsLong(retVal);
      //int parseSuccess = PyArg_ParseTuple(retVal,"i",&insertSuccess);
      //if (!parseSuccess) {
      // 	PyErr_Print();
      // 	const std::string message("Failed to parse results from python "
      // 				  "call to insert_point()");
      // 	throw OSError(message);	
      // }

      Py_XDECREF(pointArray);
      Py_XDECREF(valueArray);
      Py_XDECREF(retVal);

      PyGILState_Release(gState);

      pthread_mutex_unlock(&d_lock);

#endif // HAVE_PKG_pyas
      
      //
      //
      //
      return insertSuccess;
    }

    //
    // Generate a set of new arguments, centered around an argument,
    // that can be evaluated in order to bootstrap a new interpolation
    // model.
    //
    std::vector<ArgumentPointer>
    PyasInterpolationDatabase::generateSampleArguments(const ArgumentPointer & argument) const
    {

      //
      // instantiate return value
      //
      std::vector<ArgumentPointer> returnValue;

#if defined(HAVE_PKG_pyas)
      
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

#endif // HAVE_PKG_pyas

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
    PyasInterpolationDatabase::addNewModel(const std::vector<ArgumentPointer> & arguments,
					   const std::vector<ValuePointer>    & values,
					   int & /*hint*/)
    {
      
      int addNewModelSuccess = 0;

#if defined(HAVE_PKG_pyas)

      pthread_mutex_lock(&d_lock);

      //
      // Get lock to python interpreter
      //
      PyGILState_STATE gState;
      gState =  PyGILState_Ensure();
      
      // instantiate arrays of points and values
      const int numPoints = arguments.size();
      const int pointDim = d_inputFilter->getDimension();
      const int valueDim = d_outputFilter->getDimension();
      double databasePoints[numPoints][pointDim];
      double databaseValues[numPoints][valueDim];

      // apply input filters to arguments
      for(std::vector<ArgumentPointer>::size_type i=0; i<arguments.size();
          ++i) {

        const ArgumentPointer & argument = arguments[i];
        const std::vector<double> databasePoint =
          d_inputFilter->apply(argument);

	for(std::vector<double>::size_type j=0; j<databasePoint.size();
	    ++j) {	  
	  databasePoints[i][j] = databasePoint[j];       
	}
      }

      // apply inverse output filters to values
      for(std::vector<ValuePointer>::size_type i=0; i<values.size();
          ++i) {

        const ValuePointer & value = values[i];
        const std::vector<double> databaseValue =
          d_outputFilter->applyInverse(value);

	for(std::vector<double>::size_type j=0; j<databaseValue.size();
	    ++j) {
	  databaseValues[i][j] = databaseValue[j];       
	}      
      }

      // Convert points and values to numpy arrays
      npy_intp pointDims[2];
      pointDims[0] = numPoints;
      pointDims[1] = pointDim;
      PyArrayObject * pointArray= 
	(PyArrayObject *)PyArray_SimpleNewFromData(2,pointDims,
						   NPY_DOUBLE,
						   databasePoints);

      npy_intp valueDims[2];
      valueDims[0] = numPoints;
      valueDims[1] = valueDim;
      PyArrayObject * valueArray= 
	(PyArrayObject *)PyArray_SimpleNewFromData(2,valueDims,
						   NPY_DOUBLE,
						   databaseValues);

      // Insert the new model into the python database
      PyObject * retVal = PyObject_CallMethod(d_pyasDatabase,
					       "insert_new_model",
					      "(OO)",pointArray,valueArray);

      if (retVal == NULL) {
	PyErr_Print();
	const std::string message("Failed to call insert_new_model()");
	throw OSError(message);	
      }

      addNewModelSuccess = PyLong_AsLong(retVal);
      // int parseSuccess = PyArg_ParseTuple(retVal,"i",&addNewModelSuccess);
      // if (!parseSuccess) {
      // 	PyErr_Print();
      // 	const std::string message("Failed to parse results from python "
      // 				  "call to insert_new_model()");
      // 	throw OSError(message);	
      // }

      Py_XDECREF(pointArray);
      Py_XDECREF(valueArray);
      Py_XDECREF(retVal);

      PyGILState_Release(gState);

      pthread_mutex_unlock(&d_lock);

#endif // HAVE_PKG_pyas

      //
      //
      //
      return addNewModelSuccess;
    }

    //
    // Print DB stats
    // 
    void 
    PyasInterpolationDatabase::printStatistics(std::ostream & outputStream) const
    {

#if defined(HAVE_PKG_pyas)

      //
      // Get lock to python interpreter
      //
      PyGILState_STATE gState;
      gState =  PyGILState_Ensure();

      //
      // Print summary of the python database
      //
      // Fixme: Eventually need to add tracking of statistics
      //
      PyObject_CallMethod(d_pyasDatabase,"print_summary","()");

      PyGILState_Release(gState);

#endif // HAVE_PKG_pyas

      return;

    }

    //
    // Output database
    //
    void 
    PyasInterpolationDatabase::outputDatabase() const
    {

#if defined(HAVE_PKG_pyas)

      //
      // Get lock to python interpreter
      //
      PyGILState_STATE gState;
      gState =  PyGILState_Ensure();

      //
      // Write the contents of the python database to a file
      //
      // Fixme: eventually need to select from multiple options for format
      // 
      PyObject_CallMethod(d_pyasDatabase,"write_asf_format","()");

      PyGILState_Release(gState);

#endif // HAVE_PKG_pyas

      return;
    }
  }
}
