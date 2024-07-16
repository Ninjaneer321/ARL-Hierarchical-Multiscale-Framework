//
// Test InterpolationDatabase
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#if defined(HAVE_MPI)
#include <mpi.h>
#endif // HAVE_MPI

// only compile test if we have asf package linked
#if defined(HAVE_PKG_asf)

#include "DPDArgument.h"
#include "DPDInputFilter.h"
#include "DPDOutputFilter.h"
#include "DPDValue.h"

#include <adaptive_sampling/KrigingInterpolationDatabase.h>
#include <adaptive_sampling/NullInterpolationDatabase.h>
#include <base/ModelPackage.h>
#include <base/ResourceType.h>
#include <broker/BrokerLauncher.h>
#include <exception/IOError.h>
#include <exception/OSError.h>

#include <mtreedb/MTreeObject.h>

#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#else
#error pthread.h header file not available
#endif // HAVE_PTHREAD_H

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

#if defined(HAVE_SYS_WAIT_H)
#include <sys/wait.h>
#else
#error sys/wait.h header file not available
#endif // HAVE_SYS_WAIT_H

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#else
#error unistd.h header file not available
#endif // HAVE_UNISTD_H

//
// local functions
//

namespace {

  //const std::string pointValueFilePath = "adaptive_sampling_data_3_31_15.txt";
  const std::string pointValueFilePath = "data.txt";
  const std::string executable = "interp.py";


  struct WorkerThreadData {
    pthread_t                           thread;
    int                                 numberPoints;
    int                                 threadId;
    int                                 numberThreads;
    std::vector<std::vector<double> > * points;
    std::vector<std::vector<double> > * values;
    arl::hms::InterpolationDatabase   * interpolationDatabase;
    int                                 numberSuccessfulInterpolations;
    double                              totalError;
    int                                 numberExtraEvals;
    arl::hms::CommunicatorPointer       brokerCommunicator;
  };

  //
  //
  //
  void *
  workerThread(void * argument)
  {
   
    //
    // cast argument to WorkerThreadData
    //
    struct WorkerThreadData * 
      data = static_cast<struct WorkerThreadData *>(argument);
      
    //
    // pull data 
    //
    const int numberPoints = data->numberPoints;
    const int threadId = data->threadId;
    const int numberThreads = data->numberThreads;
    const std::vector<std::vector<double> > & points = *(data->points);
    const std::vector<std::vector<double> > & values = *(data->values);
    arl::hms::InterpolationDatabase & interpolationDatabase = 
      *(data->interpolationDatabase);
    int & numberSuccessfulInterpolations = data->numberSuccessfulInterpolations;
    double & totalError = data->totalError;
    arl::hms::CommunicatorPointer & brokerCommunicator = 
      data->brokerCommunicator;
    
    //
    // compute share of work
    //
    const int threadDataSize = numberPoints / numberThreads;
    const int iStart = threadId * threadDataSize;
    const int iEnd = (threadId + 1) * threadDataSize;

    //
    // instantiate database hint
    //
    int hint = MPTCOUPLER::mtreedb::MTreeObject::getUndefinedId();

    //
    // iterate over points, attempting to interpolate
    //
    for(int i=iStart; i<iEnd; ++i) {
    
      //
      // get point
      //
      const std::vector<double> & point = points[i];
  
      //
      // get actual value
      //
      const std::vector<double> & exactValue = values[i];

      //
      // instantiate DPDArgument from value
      //
      arl::hms::DPDArgumentPointer 
        dpdArgument(new arl::hms::DPDArgument(point[0],
                                              point[1]));
      
      //
      // try to interpolate
      //
      const std::pair<bool, arl::hms::ValuePointer> interpolationResult = 
        interpolationDatabase.interpolate(dpdArgument,
                                          hint);
      
      //
      // test whether interpolation was a success
      //
      const bool interpolationSuccess = interpolationResult.first;
      if(interpolationSuccess == true) {
        
        //
        //
        //
        std::cout << "Interpolated Successfully" << std::endl;
        ++numberSuccessfulInterpolations;

        //
        // get interpolated value
        //
        const arl::hms::DPDValuePointer interpolatedValue = 
	  boost:: dynamic_pointer_cast<arl::hms::DPDValue>(interpolationResult.second);
        
        //
        // get interpolated temperature and pressure
        //
        const double interpolatedTemperature = 
          interpolatedValue->getTemperature();
        const double interpolatedPressure = 
          interpolatedValue->getPressure();
        
        //
        // get exact temperature and pressure
        //
        const double exactTemperature = exactValue[0];
        const double exactPressure = exactValue[1];

        const double temperatureError = 
          (exactTemperature - interpolatedTemperature) / exactTemperature;
        const double pressureError = 
          (exactPressure - interpolatedPressure) / exactPressure;

        totalError += (temperatureError + pressureError) / 2.0;

        std::cout << "Temperature: " << temperatureError << std::endl;
        std::cout << "Pressure: " << pressureError << std::endl;
        
        
        
      }
      else {
                
        //
        // instantiate DPDValue
        //
        arl::hms::DPDValuePointer 
          dpdValue(new arl::hms::DPDValue(exactValue[0],
                                          exactValue[1]));
        
        //
        // insert (argument, value) into database
        //
        const bool insertSuccess = 
          interpolationDatabase.insert(dpdArgument,
                                       dpdValue,
                                       hint);

        //
        // need to insert new set of points to database
        //
        if(insertSuccess == false) {

          //
          // generate new arguments
          //
          const std::vector<arl::hms::ArgumentPointer> newArguments =
            interpolationDatabase.generateSampleArguments(dpdArgument);

          data->numberExtraEvals += newArguments.size();

          int pid = fork();
          if(pid == 0) {

            std::vector<std::string> energyDensities;
            std::vector<std::string> massDensities;

            for(std::vector<double>::size_type i=0; i<newArguments.size(); 
                ++i) {
           
              //
              // get argument handle
              //
              arl::hms::DPDArgumentPointer newArgument = 
                boost:: dynamic_pointer_cast<arl::hms::DPDArgument>(newArguments[i]);

              std::stringstream energyDensity;
              energyDensity << std::setprecision(18) 
                            << newArgument->getEnergyDensity();            
              energyDensities.push_back(energyDensity.str());
              
              std::stringstream massDensity;
              massDensity << std::setprecision(18) 
                          << newArgument->getMassDensity();
              massDensities.push_back(massDensity.str());
              
              /*
              std::cout << energyDensity.str() << std::endl;
              std::cout << massDensity.str() << std::endl;
              */

            }
            
            std::vector<std::string> arguments;
            arguments.push_back(executable);
            arguments.push_back("energy_refit.txt");
            arguments.push_back("pressure_refit.txt");
            for(std::vector<double>::size_type i=0; 
                i<energyDensities.size(); ++i ) {
              arguments.push_back(energyDensities[i].c_str());
              arguments.push_back(massDensities[i].c_str()); 
            }
              
            std::vector<const char *> argumentsChar;
            for(std::vector<std::string>::const_iterator iter = 
                  arguments.begin(); iter != arguments.end(); ++iter) {
              std::cout << iter->c_str() << std::endl;
              argumentsChar.push_back(iter->c_str());
            }
            argumentsChar.push_back(NULL);
            
            if(execv(argumentsChar[0],
                     const_cast<char * const *>(&(argumentsChar[0]))) != 0) {
              const std::string message("Error in exec");
              throw arl::hms::OSError(message);
            };
            
          }
          
          int status;
          if(waitpid(pid,
                     &status,
                     0) == -1) {
            const std::string message("Error in waitpid");
            throw arl::hms::OSError(message);
          }
 
          std::vector<arl::hms::ArgumentPointer> arguments;
          std::vector<arl::hms::ValuePointer> values;

          arguments.push_back(dpdArgument);
          for(std::vector<double>::size_type i=0; i<newArguments.size(); 
              ++i) {
            arguments.push_back(newArguments[i]);
          }

          values.push_back(dpdValue);
         
          double temperature;
          double pressure;

          std::ifstream buffer("output");

          while(buffer >> temperature >> pressure) {
            std::cout << temperature << " " << pressure << std::endl;
            arl::hms::DPDValuePointer 
              value(new arl::hms::DPDValue(temperature,
                                           pressure));
            values.push_back(value);
          }

          //
          // add new model containing all sampled points and values
          //
          const bool addModelSuccess = 
            interpolationDatabase.addNewModel(arguments,
                                              values,
                                              hint);

          if(addModelSuccess == false) {
          
            //
            //
            //
            std::cout << "Error adding new kriging model from sampled points" 
                      << std::endl;
            exit(0);

          }
          
        }
        
      }
      
    }
    
    //
    //
    //
    return NULL;
  }
  

}

#endif // HAVE_PKG_asf

//
//
//
int
main(int ac,
     char * av[])
{

#if defined(HAVE_PKG_asf)

#if defined(HAVE_MPI)
  MPI_Init(&ac,
           &av);
#endif // HAVE_MPI

  //
  // set tolerance
  //
  const double tolerance = 5.0e-5;

  //
  // instantiate DPDInputFilter and DPDOutputFilter
  //
  arl::hms::DPDInputFilterPointer 
    dpdInputFilter(new arl::hms::DPDInputFilter);
  arl::hms::DPDOutputFilterPointer 
    dpdOutputFilter(new arl::hms::DPDOutputFilter);
  
  //
  // instantiate interpolation database
  //
  std::vector<double> deltaHs(2);
  deltaHs[0] = 1e-2;
  deltaHs[1] = 1e-2;
  
  const int maxKrigingModelSize = 50;
  const int maxNumberSearchModels = 10;
  const double meanErrorFactor = 1.0;
  const double maxQueryPointModelDistance = 100000.0;
  const int agingThreshold = 600000000;
  const double theta = 1.0; 
  const std::string mtreeDirectoryName(".");
  
  arl::hms::KrigingInterpolationDatabase 
    interpolationDatabase(dpdInputFilter,
                          dpdOutputFilter,
                          deltaHs,
                          tolerance,
                          maxKrigingModelSize,
                          maxNumberSearchModels,
                          meanErrorFactor,
                          maxQueryPointModelDistance,
                          agingThreshold,
                          theta,
                          mtreeDirectoryName);
  /*
  arl::hms::NullInterpolationDatabase interpolationDatabase;  
  */

  /*
  //
  // launch broker to evaluate powell model at unknown points
  //
  std::vector<arl::hms::CommunicatorPointer> brokerCommunicators = 
    arl::hms::BrokerLauncher().launch(brokerConfigPath,
                                      0,
                                      1);
    
  //
  // check that we only launched one broker
  // 
  if(brokerCommunicators.size() != 1) {
    const std::string message("Error launching broker.");
    throw arl::hms::IOError(message);
  }

  arl::hms::CommunicatorPointer & brokerCommunicator = brokerCommunicators[0];
  */  

  //
  // read points and values
  //
  std::string numberPointsString;
  std::ifstream pointValueFile(pointValueFilePath.c_str());

  std::vector<std::vector<double> > points;
  std::vector<std::vector<double> > values;
  std::vector<double> point(2);
  std::vector<double> value(2);
  
  double energyDensity;
  double massDensity;
  double temperature;
  double pressure;
  while(pointValueFile >> point[0] >> point[1] >> value[0] >> value[1]) {
    points.push_back(point);
    values.push_back(value);
  }
  
  int numberWorkers = 1;

  std::vector<WorkerThreadData> workerThreadData(numberWorkers);
  
  for(int iWorker=0; iWorker<numberWorkers; ++iWorker) {

    //
    // initialize thread data
    //
    WorkerThreadData & data = workerThreadData[iWorker];
    data.numberPoints = points.size();
    data.threadId = iWorker;
    data.numberThreads = numberWorkers;
    data.points = &points;
    data.values = &values;
    data.interpolationDatabase = &interpolationDatabase;
    data.numberSuccessfulInterpolations = 0;
    data.numberExtraEvals = 0;
    data.totalError = 0.0;
    //data.brokerCommunicator = brokerCommunicator;

    //
    // initialize thread attributes to detached
    //
    pthread_attr_t threadAttribute;
    
    if(pthread_attr_init(&threadAttribute) != 0) {
      std::stringstream message;
      message << "Error on pthread_attr_init() of threadAttribute: " 
              << strerror(errno);
      throw arl::hms::OSError(message.str());
    }
    
    if(pthread_attr_setscope(&threadAttribute, 
                             PTHREAD_SCOPE_SYSTEM) != 0) {
      std::stringstream message;
      message << "Error on pthread_attr_setscope() of threadAttribute: "
              << strerror(errno);
      throw arl::hms::OSError(message.str());
    }

    //
    // start receive thread
    //
    if(pthread_create(&data.thread, 
                      &threadAttribute, 
                      workerThread,
                      static_cast<void *>(&data)) != 0) {
      std::stringstream message;
      message << "Error on pthread_create() of receiveThread: "
              << strerror(errno);
      throw arl::hms::OSError(message.str());
    }
    
  }

  int numberExtraEvals = 0;
  int numberSuccessfulInterpolations = 0;  
  double totalError = 0.0;
  for(int iWorker = 0; iWorker < numberWorkers; ++iWorker) {
      WorkerThreadData & data = workerThreadData[iWorker];
      pthread_join(data.thread,
                   NULL);
      numberSuccessfulInterpolations += data.numberSuccessfulInterpolations;
      totalError += data.totalError;
      numberExtraEvals += data.numberExtraEvals;
  }

  std::cout << "Number Successful Interpolations: " 
            << numberSuccessfulInterpolations << " " 
            << static_cast<double>(numberSuccessfulInterpolations) / points.size() << std::endl;

  std::cout << "Avg error of interpolated values: " 
            << totalError / numberSuccessfulInterpolations << std::endl;
  
  std::cout << "Number extra evals: " << numberExtraEvals << std::endl;

#endif // HAVE_PKG_asf

  //
  //
  //
  return 0;

}
