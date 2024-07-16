//
// Test InterpolationDatabase
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

// only compile test if we have asf package linked
#if defined(HAVE_PKG_asf)

#if defined(HAVE_MPI)
#include <mpi.h>
#endif // HAVE_MPI

#include "PowellInputFilter.h"
#include "PowellOutputFilter.h"
#include "StrainArgument.h"
#include "StrainInputFilter.h"
#include "StressOutputFilter.h"
#include "StressValue.h"

#include <adaptive_sampling/KrigingInterpolationDatabase.h>
#include <adaptive_sampling/NullInterpolationDatabase.h>
//#include <adaptive_sampling/RandomForestInterpolationDatabase.h>
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

//
// local functions
//

namespace {

  const std::string powellExecutablePath = "Powell_FEM";
  const std::string powellInputFile = "input_microscale.m";
  const std::string powellMeshFileDirectory = "";
  const std::string powellMeshFileName = "mesh_input_periodic_x.m";
  const double deltaH = 1.0e-5;
  const std::string brokerConfigPath = "hmsConf.ini";
  const double youngsModulus = 212.52;

  struct WorkerThreadData {
    pthread_t                           thread;
    int                                 numberPoints;
    int                                 threadId;
    int                                 numberThreads;
    std::vector<std::vector<double> > * strains;
    std::vector<std::vector<double> > * stresses;
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
    const std::vector<std::vector<double> > & strains = *(data->strains);
    const std::vector<std::vector<double> > & stresses = *(data->stresses);
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
      // get strain
      //
      const std::vector<double> & strain = strains[i];
  
      //
      // get actual value of stress
      //
      const std::vector<double> & exactStress = stresses[i];

      //
      // instantiate StrainArgument from strain
      //
      arl::hms::StrainArgumentPointer 
        strainArgument(new arl::hms::StrainArgument(strain));
      
      //
      // try to interpolate
      //
      const std::pair<bool, arl::hms::ValuePointer> interpolationResult = 
        interpolationDatabase.interpolate(strainArgument,
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
        const arl::hms::StressValuePointer interpolatedValue = 
	  boost:: dynamic_pointer_cast<arl::hms::StressValue>(interpolationResult.second);
        
        //
        // get stress
        //
        const std::vector<double> & interpolatedStress = 
          interpolatedValue->getStress();
        
        double meanSquareError = 0;
        for(std::vector<double>::size_type j=0; 
            j<interpolatedStress.size(); ++j) {
          meanSquareError += (interpolatedStress[j] - exactStress[j]) *
            (interpolatedStress[j] - exactStress[j]);
        }
      
        //
        // output error
        //
        std::cout << "Strain: ";
        for(std::vector<double>::size_type j=0; j<strain.size(); ++j) {
          std::cout << strain[j] << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Exact: ";
        for(std::vector<double>::size_type j=0; j<exactStress.size(); ++j) {
          std::cout << exactStress[j] << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Interpolated: ";
        for(std::vector<double>::size_type j=0; j<interpolatedStress.size();
            ++j) {
          std::cout << interpolatedStress[j] << " ";
        }
        std::cout << std::endl;
                
        std::cout << "Interpolation Error: " << meanSquareError << std::endl;

        if(meanSquareError > 10) {
          exit(0);
        }

        totalError += meanSquareError;
     
      }
      else {
                
        //
        // instantiate StressValue
        //
        arl::hms::StressValuePointer 
          stressValue(new arl::hms::StressValue(exactStress));
        
        //
        // insert (argument, value) into database
        //
        const bool insertSuccess = 
          interpolationDatabase.insert(strainArgument,
                                       stressValue,
                                       hint);

        //
        // need to insert new set of points to database
        //
        if(insertSuccess == false) {

          std::vector<std::string> powellArguments(1,
                                                   powellInputFile);

          //
          // create powell model
          //
          arl::hms::ModelPointer 
            model(new arl::hms::Model("",
                                      powellExecutablePath,
                                      powellArguments,
                                      arl::hms::CPU,
                                      1));
          
          //
          // create powell input and output filters
          //
          arl::hms::InputFilterPointer
            inputFilter(new arl::hms::PowellInputFilter(powellMeshFileDirectory,
                                                        powellMeshFileName));
          arl::hms::OutputFilterPointer
            outputFilter(new arl::hms::PowellOutputFilter);

          //
          // generate powell arguments
          //
          const std::vector<arl::hms::ArgumentPointer> newArguments =
            interpolationDatabase.generateSampleArguments(strainArgument);

          data->numberExtraEvals += newArguments.size();

          for(std::vector<double>::size_type i=0; i<newArguments.size(); ++i) {
           
            //
            // get argument handle
            //
            const arl::hms::ArgumentPointer & newArgument = newArguments[i];
 
            //
            // create model package
            //
            arl::hms::ModelPackagePointer
              newModelPackage(new arl::hms::ModelPackage(model,
                                                         inputFilter,
                                                         outputFilter,
                                                         newArgument));
   
            //
            // send model packages for evaluation
            //
            brokerCommunicator->send(newModelPackage);
                  
          }

          std::vector<arl::hms::ArgumentPointer> arguments;
          std::vector<arl::hms::ValuePointer> values;

          std::vector<arl::hms::ArgumentPointer>::size_type
            numberReceivedPoints = 0;
          while(numberReceivedPoints < newArguments.size()) {
            
            //
            // receive completed model packages
            //
            const std::vector<arl::hms::ModelPackagePointer> modelPackages = 
              brokerCommunicator->receive<arl::hms::ModelPackagePointer>();
            numberReceivedPoints += modelPackages.size();

            for(std::vector<arl::hms::ModelPackagePointer>::size_type i=0;
                i < modelPackages.size(); ++i) {
              
              const arl::hms::ModelPackagePointer & modelPackage =
                modelPackages[i];
             
              //
              // extract arguments and values into local storage
              //
              arguments.push_back(modelPackage->getArgument());
              values.push_back(modelPackage->getValue());

              const arl::hms::StressValuePointer interpolatedValue = 
                boost::dynamic_pointer_cast<arl::hms::StressValue>(modelPackage->getValue());
              const std::vector<double> & stress = interpolatedValue->getStress();
              std::cout << stress[0] << " " << stress[1] << " " << stress[2] 
                        << " " << stress[3] << " " << stress[4] << " " 
                        << stress[5] 
                        << std::endl;
 
            }

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
     char ** av)
{

#if defined(HAVE_MPI)
  MPI_Init(&ac,
           &av);
#endif // HAVE_MPI

#if defined(HAVE_PKG_asf)

  //
  // set tolerance
  //
  const double tolerance = 1.0e-6;

  //
  // instantiate StrainInputFilter and StressOutputFilter
  //
  arl::hms::StrainInputFilterPointer 
    strainInputFilter(new arl::hms::StrainInputFilter);
  arl::hms::StressOutputFilterPointer 
    stressOutputFilter(new arl::hms::StressOutputFilter(youngsModulus));
  
  //
  // instantiate interpolation database
  //
  const std::vector<double> deltaHs(6, deltaH);
  const double theta = 1.0;
  const int maxKrigingModelSize = 50;
  const int maxNumberSearchModels = 4;
  const double meanErrorFactor = 1.0;
  const double maxQueryPointModelDistance = 1000.0;
   const int agingThreshold = 600000000;
  const std::string mtreeDirectoryName(".");
  
  arl::hms::KrigingInterpolationDatabase 
    interpolationDatabase(strainInputFilter,
                          stressOutputFilter,
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
  
  //
  // read points
  //
  std::string numberPointsString;
  std::ifstream pointFile("strain_360.txt");
  pointFile >> numberPointsString;
  int numberPoints = std::atoi(numberPointsString.c_str());
  
  std::vector<std::vector<double> > strains(numberPoints,
                                            std::vector<double>(6));
 
  for(int i=0; i<numberPoints; ++i) {
    for(int j=0; j<6; ++j) {
      pointFile >> strains[i][j];
    }
  }

  //
  // read values
  //
  std::ifstream valueFile("stress_360.txt");
  valueFile >> numberPointsString;
  numberPoints = std::atoi(numberPointsString.c_str());
  
  std::vector<std::vector<double> > stresses(numberPoints,
                                             std::vector<double>(6));
 
  for(int i=0; i<numberPoints; ++i) {
    for(int j=0; j<6; ++j) {
      valueFile >> stresses[i][j];
    }
  }

  int numberWorkers = 1;

  std::vector<WorkerThreadData> workerThreadData(numberWorkers);
  
  for(int iWorker=0; iWorker<numberWorkers; ++iWorker) {

    //
    // initialize thread data
    //
    WorkerThreadData & data = workerThreadData[iWorker];
    data.numberPoints = numberPoints;
    data.threadId = iWorker;
    data.numberThreads = numberWorkers;
    data.strains = &strains;
    data.stresses = &stresses;
    data.interpolationDatabase = &interpolationDatabase;
    data.numberSuccessfulInterpolations = 0;
    data.numberExtraEvals = 0;
    data.totalError = 0.0;
    data.brokerCommunicator = brokerCommunicator;

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
            << static_cast<double>(numberSuccessfulInterpolations) / numberPoints << std::endl;

  std::cout << "Avg error of interpolated values: " 
            << totalError / numberSuccessfulInterpolations << std::endl;
  
  std::cout << "Number extra evals: " << numberExtraEvals << std::endl;

#endif // HAVE_PKG_asf

  //
  //
  //
  return 0;

}
