//
// File:      KDEGenerator.cc
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "KDEGenerator.h"

#include <exception/OSError.h>

#if defined(HAVE_ALGORITHM)
#include <algorithm>
#else
#error algorithm header file not available
#endif // HAVE_ALGORITHM

#if defined(HAVE_CSTDLIB)
#include <cstdlib>
#else
#error cstdlib header file not available
#endif // HAVE_CSTDLIB

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

#include <Python.h>

//
//
//

namespace arl {
  namespace hms {

    struct KDEGenerator::GenerateThreadData {
      bool                  * d_generatingKDE;
      pthread_cond_t        * d_generateThreadCondition;
      KDEGenerator::KDEData * d_kdeData;
      pthread_mutex_t       * d_kdeLock;
    };

    //
    // PIMPL
    //
    class KDEGenerator::KDEData {
      
    public:
      
      KDEData(int cycleSize) :
	d_kdeA(NULL),
	d_kdeB(NULL),
	d_kdeD(NULL),
	d_kdeF(NULL),
	d_scaler(NULL)
      {

        //
        // initialize python interpreter
        //
        Py_Initialize();

	//
	// since we are calling python from multiple threads we need
	// to initialize the GIL
	//
	PyEval_InitThreads();
        PyEval_ReleaseLock();

        //
        // import spec2d_anni module
        //
        d_spaModule = PyImport_ImportModule("spec2d_anni");
        
        if(d_spaModule == NULL) {
          PyErr_Print();
          const std::string message("Cannot import spec2d_anni python module "
				    "for speculative generation");
          throw OSError(message);
        }                 

	//
	// initialize list of points
	//
	d_currentCycle = PyList_New(cycleSize);
	d_previousCycle = PyList_New(cycleSize);
    
        //
        //
        //
        return;
        
      }        
      
      ~KDEData() 
      {

	//
	// cleanup
	//
	PyGILState_STATE gstate;
	gstate = PyGILState_Ensure();
	Py_DECREF(d_currentCycle);
	Py_XDECREF(d_kdeA);
	Py_XDECREF(d_kdeB);
	Py_XDECREF(d_kdeD);
	Py_XDECREF(d_kdeF);
	Py_XDECREF(d_scaler);
	Py_DECREF(d_previousCycle);
	Py_DECREF(d_spaModule);
	PyGILState_Release(gstate);

	//
	//
	//
	return;
           
      }

      std::vector<std::vector<double> > d_currentCycleVector;
      std::vector<std::vector<double> > d_previousCycleVector;

      PyObject * d_currentCycle;
      PyObject * d_kdeA;
      PyObject * d_kdeB;
      PyObject * d_kdeD;
      PyObject * d_kdeF;
      PyObject * d_previousCycle;     
      PyObject * d_scaler;
      PyObject * d_spaModule;
      
    };

    namespace {

      //
      // Build python tuple from vector v
      //
      PyObject *
      buildTuple(const std::vector<double> & v) 
      {
	
	PyObject * point = PyTuple_New(v.size());
	for(std::vector<double>::size_type i=0; i<v.size(); ++i) {
	  PyObject * value =  PyFloat_FromDouble(v[i]);
	  PyTuple_SetItem(point,
			  i,
			  value);
	}

	//
	//
	//
	return point;
	
      }

      //
      // thread for building kdes from data
      //
      void *
      generateThread(void * argument) 
      {

        //
        // cast argument to GenerateThreadData
        //
        struct KDEGenerator::GenerateThreadData * data = 
          static_cast<struct KDEGenerator::GenerateThreadData *>(argument);
        
        //
        // pull data from GenerateThreadData
        //
	bool & d_generatingKDE = *(data->d_generatingKDE);
	pthread_cond_t & d_generateThreadCondition =
	  *(data->d_generateThreadCondition);
	KDEGenerator::KDEData * d_kdeData = data->d_kdeData;
        pthread_mutex_t & d_kdeLock = *(data->d_kdeLock);

	
	//
	// loop indefinitely
	//
	while(true) {
	
	  //
	  // wait until we have new data
	  //
	  pthread_mutex_lock(&d_kdeLock);
	  while(!d_generatingKDE) {
	    pthread_cond_wait(&d_generateThreadCondition, 
			      &d_kdeLock);
	  }

	  PyGILState_STATE gstate;
	  gstate = PyGILState_Ensure();

	  /*
	  std::cerr << "Calling generate kdes: " << std::endl;
	  std::cerr << "Previous Cycle: " << std::endl;
	  PyObject_Print(d_kdeData->d_previousCycle, stderr, 0);
	  std::cerr << "Current Cycle: " << std::endl;
	  PyObject_Print(d_kdeData->d_currentCycle, stderr, 0);    	    
	  */	  

	  //
	  // generate new kdes
	  //
	  PyObject * createString = PyString_FromString("create_all_kdes");
	  PyObject * createResult =
	    PyObject_CallMethodObjArgs(d_kdeData->d_spaModule,
				       createString,
				       d_kdeData->d_previousCycle,
				       d_kdeData->d_currentCycle,
				       NULL);
	  
	  if(createResult == NULL) {

	    PyErr_Print();
	    std::cerr << "Previous Cycle: " << std::endl;
	    PyObject_Print(d_kdeData->d_previousCycle, stderr, 0);
	    std::cerr << "Current Cycle: " << std::endl;
	    PyObject_Print(d_kdeData->d_currentCycle, stderr, 0);    	    

	    std::stringstream message;
	    message << "Error calling create_all_kdes on kde speculative "
		    << "generator ";

	    /*
	    PyObject *ptype, *pvalue, *ptraceback;
	    PyErr_Fetch(&ptype, &pvalue, &ptraceback);

	    if(ptype) {
	      message << PyString_AsString(ptype) << " ";
	    }
	    
	    if(pvalue) {
	      message << PyString_AsString(pvalue) << " ";
	    }

	    if(ptraceback) {
	      message << PyString_AsString(ptraceback) << " ";
	    }
	    */

	    throw OSError(message.str());
	    
	  }
	  
	  //
	  // assert that we got a sequence back of size 5
	  //
	  assert(PySequence_Check(createResult));
	  assert(PySequence_Size(createResult) == 5); 
	  
	  //
	  // cleanup old kdes
	  //
	  Py_XDECREF(d_kdeData->d_kdeA);
	  Py_XDECREF(d_kdeData->d_kdeB);
	  Py_XDECREF(d_kdeData->d_kdeD);
	  Py_XDECREF(d_kdeData->d_kdeF);
	  Py_XDECREF(d_kdeData->d_scaler);
	  
	  d_kdeData->d_kdeA = PySequence_GetItem(createResult, 0);
	  d_kdeData->d_kdeB = PySequence_GetItem(createResult, 1);
	  d_kdeData->d_kdeD = PySequence_GetItem(createResult, 2);
	  d_kdeData->d_kdeF = PySequence_GetItem(createResult, 3);
	  d_kdeData->d_scaler = PySequence_GetItem(createResult, 4);
	  	  
	  //
	  // cleanup
	  //
	  Py_DECREF(createResult);
	  Py_DECREF(createString);
	  
	  PyGILState_Release(gstate);

	  //
	  // mark that we are done generating the KDE
	  //
	  d_generatingKDE = false;
	  pthread_mutex_unlock(&d_kdeLock);
	
	}

      }

    }

    //
    // Constructor.
    //
    KDEGenerator::KDEGenerator(const InterpolationDatabaseInputFilterPointer & inputFilter,
			       int cycleSize) :
      d_generatingKDE(false),
      d_cycleSize(cycleSize),
      d_inputFilter(inputFilter),
      d_kdeData(new KDEData(cycleSize))
    {

      if(pthread_cond_init(&d_generateThreadCondition,
                            NULL) != 0) {
        std::stringstream message;
        message << "Failure on pthread_cond_init() of KDE speculative "
	  "generator condition: " << strerror(errno);
        throw OSError(message.str());
      }

      if(pthread_mutex_init(&d_kdeLock,
                            NULL) != 0) {
        std::stringstream message;
        message << "Failure on pthread_mutex_init() of KDE speculative "
	  "generator lock: " << strerror(errno);
        throw OSError(message.str());
      }     

      d_generateThreadData = new GenerateThreadData;
      d_generateThreadData->d_generatingKDE = &d_generatingKDE;
      d_generateThreadData->d_generateThreadCondition = 
	&d_generateThreadCondition;
      d_generateThreadData->d_kdeData = d_kdeData;
      d_generateThreadData->d_kdeLock = &d_kdeLock;

      //
      // start generate thread
      //
      if(pthread_create(&d_threadId,
                        NULL,
                        generateThread,
                        static_cast<void *>(d_generateThreadData)) != 0) {
        std::stringstream message;
        message 
          << "Error on pthread_create of algorithmWarehouseReceiveThread: "
          << strerror(errno);
        throw OSError(message.str());
      }      

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    KDEGenerator::~KDEGenerator()
    {

      //
      // TODO: join this thread and cleanup properly
      // join thread
      //
      //pthread_join(d_threadId,
      //NULL);

      //
      //
      //
      pthread_cond_destroy(&d_generateThreadCondition);
      pthread_mutex_destroy(&d_kdeLock);

      //
      //
      //
      delete d_kdeData;
      delete d_generateThreadData;

      //
      //
      //
      return;

    }

    //
    // Generate evaluations
    // 
    std::vector<ModelPackagePointer>
    KDEGenerator::generate(const ModelPackagePointer & modelPackageTemplate,
			   unsigned int numberEvaluations)
    {
    
      //
      // instantiate return value
      //
      std::vector<ModelPackagePointer> returnValue;

      //
      // first check if we are in the middle of generating KDEs - if
      // so return immediately
      //
      if(d_generatingKDE) {
	return returnValue;
      }

      //
      // also check if we have kdes generated - if not then return
      // empty vector
      //
      if(d_kdeData->d_kdeA == NULL || numberEvaluations == 0) {
	return returnValue;
      }

      //
      // use trylock for efficiency sake - do not want to block here
      //
      if(pthread_mutex_trylock(&d_kdeLock) != 0) {
        return returnValue;
      }

      PyGILState_STATE gstate;
      gstate = PyGILState_Ensure();
     
      //
      // generate new model packages
      //
      PyObject * sampleString = PyString_FromString("sample_my_kdes");
      PyObject * numberDesiredPoints = PyInt_FromLong(numberEvaluations);

      /*
      std::cerr << "Calling sample my kdes: " << std::endl;
      std::cerr << "Number Points : " << std::endl;
      PyObject_Print(numberDesiredPoints, stderr, 0);
      std::cerr << std::endl << "KDE A : " << std::endl;
      PyObject_Print(d_kdeData->d_kdeA, stderr, 0);
      std::cerr << std::endl << "KDE B : " << std::endl;
      PyObject_Print(d_kdeData->d_kdeB, stderr, 0);
      std::cerr << std::endl << "KDE D : " << std::endl;
      PyObject_Print(d_kdeData->d_kdeD, stderr, 0);
      std::cerr << std::endl << "KDE F : " << std::endl;
      PyObject_Print(d_kdeData->d_kdeF, stderr, 0);
      std::cerr << std::endl << "Scaler : " << std::endl;
      PyObject_Print(d_kdeData->d_scaler, stderr, 0);
      */

      PyObject * sampleResult =
	PyObject_CallMethodObjArgs(d_kdeData->d_spaModule,
				   sampleString,
				   numberDesiredPoints,
				   d_kdeData->d_kdeA,
				   d_kdeData->d_kdeB,
				   d_kdeData->d_kdeD,
				   d_kdeData->d_kdeF,
				   d_kdeData->d_scaler,
				   NULL);

      if(sampleResult == NULL) {
	
	PyErr_Print();
	std::cerr << "Scaler: " << std::endl;
	PyObject_Print(d_kdeData->d_scaler, stderr, 0);

	std::stringstream message;
	message << "Error calling create_all_kdes on kde speculative "
		<< "generator ";
	
	throw OSError(message.str());

      }

      //
      // assert that we got a sequence back
      //
      assert(PySequence_Check(sampleResult));
      
      const Py_ssize_t numberPoints = PySequence_Size(sampleResult);
            
      for(Py_ssize_t i=0; i<numberPoints; ++i) {
	
	PyObject * newPoint = PySequence_GetItem(sampleResult, i);
	assert(PySequence_Check(newPoint));
	
	const Py_ssize_t pointDimension = PySequence_Size(newPoint);

	//
	// fill in newArgument
	//
	std::vector<double> newArgument(pointDimension);
	for(Py_ssize_t j=0; j<pointDimension; ++j) {
	  PyObject * value = PySequence_GetItem(newPoint, j);
	  newArgument[j] = PyFloat_AsDouble(value);
	  Py_DECREF(value);
	}

	ArgumentPointer generatedArgument = 
	  d_inputFilter->applyInverse(newArgument,
				      modelPackageTemplate->getArgument());
	ModelPackagePointer 
	  newModelPackage(new ModelPackage(modelPackageTemplate->getModel(),
					   modelPackageTemplate->getInputFilter(),
					   modelPackageTemplate->getOutputFilter(),
					   generatedArgument));
	returnValue.push_back(newModelPackage);	
		
	Py_DECREF(newPoint);

      }
      
      //
      // cleanup
      //
      Py_DECREF(sampleResult);
      Py_DECREF(sampleString);
      Py_DECREF(numberDesiredPoints);

      PyGILState_Release(gstate);
      pthread_mutex_unlock(&d_kdeLock);

      //
      //
      //
      return returnValue;


    }

    //
    // Update generator with new completed evaluations
    //
    void
    KDEGenerator::update(const ModelPackagePointer & modelPackage) 
    {

      //
      // apply input filter to get floating point representation
      //
      const ArgumentPointer & argumentPtr = modelPackage->getArgument();
      const std::vector<double> argument = d_inputFilter->apply(argumentPtr);

      //
      // add to kde data
      //
      d_kdeData->d_currentCycleVector.push_back(argument);

      if(d_kdeData->d_currentCycleVector.size() == d_cycleSize) {
	
	//
	// if we also have the previous cycle filled
	//
	if(d_kdeData->d_previousCycleVector.size() == d_cycleSize) {

	  //
	  // check if we are not currently generating KDE
	  //
	  if(d_generatingKDE == false) {

	    pthread_mutex_lock(&d_kdeLock);

            //
            // mark that we are generating kdes
            //
            d_generatingKDE = true;

	    //
	    // fill python lists in preparation of calling generate KDE
	    //
	    PyGILState_STATE gstate;
	    gstate = PyGILState_Ensure();
	    for(int i=0; i<d_cycleSize; ++i) {
	      std::vector<double> & previousArgument = 
		d_kdeData->d_previousCycleVector[i];
	      std::vector<double> & currentArgument = 
		d_kdeData->d_currentCycleVector[i];
	      PyObject * previousTuple = buildTuple(previousArgument);
	      PyObject * currentTuple = buildTuple(currentArgument);
	      PyList_SetItem(d_kdeData->d_previousCycle,
			     i,
			     previousTuple);
	      PyList_SetItem(d_kdeData->d_currentCycle,
			     i,
			     currentTuple);
	    }
	    PyGILState_Release(gstate);
		  
	    pthread_mutex_unlock(&d_kdeLock);

	    //
	    // unlock thread to kick off generation of kdes
	    //
	    pthread_cond_signal(&d_generateThreadCondition);
	    
	  }

	}

	//
	// make current cycle the previous cycle and clear current
	//
	d_kdeData->d_currentCycleVector.swap(d_kdeData->d_previousCycleVector);
	d_kdeData->d_currentCycleVector.clear();

      }
     
      //
      //
      //
      return;

    }

  }

}
