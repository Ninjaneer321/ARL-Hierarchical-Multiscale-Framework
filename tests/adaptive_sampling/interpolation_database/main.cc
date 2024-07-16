//
// Test InterpolationDatabase
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

// only compile test if we have asf package linked
#if defined(HAVE_PKG_asf)

#include "XYAdaptiveSamplingInputFilter.h"
#include "XYAdaptiveSamplingOutputFilter.h"
#include "XYArgument.h"
#include "XYValue.h"

#include <adaptive_sampling/KrigingInterpolationDatabase.h>
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

#if defined(HAVE_CMATH)
#include <cmath>
#else
#error cmath header file not available
#endif // HAVE_CMATH

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

// local functions

namespace {

  struct WorkerThreadData {
    pthread_t                         thread;
    int                               numberPoints;
    int                               threadId;
    int                               numberThreads;
    std::vector<double>             * xCoordinates;
    std::vector<double>             * yCoordinates;
    arl::hms::InterpolationDatabase * interpolationDatabase;
    int                               numberSuccessfulInterpolations;
    double                            totalError;
  };

  //
  // 
  //
  double f(double x,
           double y) 
  {

    //
    //
    //
    return x*x + y*y;

  }

  void addNewArgumentValue(double x,
                           double y,
                           std::vector<arl::hms::ArgumentPointer> & arguments,
                           std::vector<arl::hms::ValuePointer> & values)
  {

    //
    // create new argument and value from x and y
    //
    arl::hms::ArgumentPointer argument(new arl::hms::XYArgument(x, y));
    arl::hms::ValuePointer value(new arl::hms::XYValue(f(x, y)));

    //
    // add to arguments and values
    //
    arguments.push_back(argument);
    values.push_back(value);
         
    //
    //
    //
    return;

  }

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
    const std::vector<double> & xCoordinates = *(data->xCoordinates);
    const std::vector<double> & yCoordinates = *(data->yCoordinates);
    arl::hms::InterpolationDatabase & interpolationDatabase = 
      *(data->interpolationDatabase);
    int & numberSuccessfulInterpolations = data->numberSuccessfulInterpolations;
    double & totalError = data->totalError;
    
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
      // get x and y
      //
      const double x = xCoordinates[i];
      const double y = yCoordinates[i];

      //
      // instantiate XYArgument from point
      //
      arl::hms::XYArgumentPointer xyArgument(new arl::hms::XYArgument(x,
                                                                      y));
    
      //
      // try to interpolate
      //
      const std::pair<bool, arl::hms::ValuePointer> interpolationResult = 
        interpolationDatabase.interpolate(xyArgument,
                                          hint);
      
      //
      // test whether interpolation was a success
      //
      const bool interpolationSuccess = interpolationResult.first;
      if(interpolationSuccess == true) {
        
        //
        //
        //
        //std::cout << "Interpolated Successfully" << std::endl;
        ++numberSuccessfulInterpolations;

        //
        // get interpolated value
        //
        const arl::hms::XYValuePointer interpolatedValue = 
          boost::dynamic_pointer_cast<arl::hms::XYValue>(interpolationResult.second);
        
        //
        // compute actual value of function
        //
        const double exactValue = f(x, y);
        
        //
        // compute error
        //
        const double error = 
          std::fabs(exactValue - interpolatedValue->getValue());
        
        //
        // output error
        //
        std::cout << "Interpolation Error: " << error << std::endl;

        if(error > 1.0) {
          std::cout << "ERROR TOO HIGH" << std::endl;
          exit(0);
        }

        totalError += error;
        
      }
      else {
        
        //
        // compute actual value of function
        //
        const double value = f(x, y);
        
        //
        // instantiate XYValue
        //
        arl::hms::XYValuePointer xyValue(new arl::hms::XYValue(value));
        
        //
        // insert (argument, value) into database
        //
        const bool insertSuccess = interpolationDatabase.insert(xyArgument,
                                                                xyValue,
                                                                hint);
        
        if(insertSuccess == false) {
          
          //
          // need to generate set of points and values for insert
          //
          const double hValue = 1.0e-1;
          
          std::vector<arl::hms::ArgumentPointer> arguments;
          std::vector<arl::hms::ValuePointer> values;
          
          arguments.push_back(xyArgument);
          values.push_back(xyValue);
          
          addNewArgumentValue(x - hValue,
                              y,
                              arguments,
                              values);
          
          addNewArgumentValue(x + hValue,
                              y,
                              arguments,
                              values);
          
          addNewArgumentValue(x,
                              y - hValue,
                              arguments,
                              values);
          
          addNewArgumentValue(x,
                              y + hValue,
                              arguments,
                              values);
          
          const bool addModelSuccess = 
            interpolationDatabase.addNewModel(arguments,
                                              values,
                                              hint);

          if(addModelSuccess == false) {
            std::cout << "Error adding new model" << std::endl;
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

  /*
  const int numberPoints = 40;
  const double xCoordinates[numberPoints] = {-.2, -.3, .15, .02, .8, -.8, .43, .99, -.3, -.4, -.4, -.7, .55, -.23, .67, .06, .12, -.05, .87, .21, .03, -.89, -.76, -.34, .04, -.22, .93, .023, .56, -.78, .8, .11, .39, .25, .44, .09, .022, -.67, -.67, .233};
  const double yCoordinates[numberPoints] = {.23, -.44, .44, -.67, .8, -.34, -.67, .34, .31, .09, -.89, -.97, -.32, .01, .1, .4, .56, .77, .32, .46, .457, .001, -.34, .56, .78, -.09, .56, -.899, .50, .35, -.23, .67, .24, -.09, .56, .789, .21, .11, -.09, .123};
  */

}

#endif // HAVE_PKG_asf

//
//
//
int
main(int,
     char * [])
{

#if defined(HAVE_PKG_asf)

  //
  // set tolerance
  //
  const double tolerance = 1.0e-4;

  //
  // instantiate XYInputFilter and XYOutputFilter
  //
  arl::hms::XYAdaptiveSamplingInputFilterPointer 
    xyInputFilter(new arl::hms::XYAdaptiveSamplingInputFilter);
  arl::hms::XYAdaptiveSamplingOutputFilterPointer 
    xyOutputFilter(new arl::hms::XYAdaptiveSamplingOutputFilter);
  
  //
  // instantiate interpolation database
  //
  const std::vector<double> deltaHs(2, 1.0e-1);
  const double theta = 1.0;
  const int maxKrigingModelSize = 50;
  const int maxNumberSearchModels = 4;
  const double meanErrorFactor = 1.0;
  const double maxQueryPointModelDistance = 1000.0;
   const int agingThreshold = 600000000;
  const std::string mtreeDirectoryName(".");
  arl::hms::KrigingInterpolationDatabase 
    interpolationDatabase(xyInputFilter,
                          xyOutputFilter,
                          deltaHs,
                          tolerance,
                          maxKrigingModelSize,
                          maxNumberSearchModels,
                          meanErrorFactor,
                          maxQueryPointModelDistance,
                          agingThreshold,
                          theta,
                          mtreeDirectoryName);
  
  //
  // read number of points
  //
  std::ifstream pointFile("gaussianRandom.dat");
  std::string numberPointsString;
  pointFile >> numberPointsString;
  const int numberPoints = std::atoi(numberPointsString.c_str());
  
  std::vector<double> xCoordinates(numberPoints);
  std::vector<double> yCoordinates(numberPoints);

  for(int i=0; i<numberPoints; ++i) {
    pointFile >> xCoordinates[i];
    pointFile >> yCoordinates[i];
  }

  int numberWorkers = 2;

  std::vector<WorkerThreadData> workerThreadData(numberWorkers);
  
  for(int iWorker=0; iWorker<numberWorkers; ++iWorker) {

    //
    // initialize thread data
    //
    WorkerThreadData & data = workerThreadData[iWorker];
    data.numberPoints = numberPoints;
    data.threadId = iWorker;
    data.numberThreads = numberWorkers;
    data.xCoordinates = &xCoordinates;
    data.yCoordinates = &yCoordinates;
    data.interpolationDatabase = &interpolationDatabase;
    data.numberSuccessfulInterpolations = 0;
    data.totalError = 0.0;

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

  int numberSuccessfulInterpolations = 0;  
  double totalError = 0.0;
  for(int iWorker = 0; iWorker < numberWorkers; ++iWorker) {
      WorkerThreadData & data = workerThreadData[iWorker];
      pthread_join(data.thread,
                   NULL);
      numberSuccessfulInterpolations += data.numberSuccessfulInterpolations;
      totalError += data.totalError;
  }

  std::cout << "Number Successful Interpolations: " 
            << numberSuccessfulInterpolations << " " 
            << static_cast<double>(numberSuccessfulInterpolations) / numberPoints << std::endl;

  std::cout << "Avg error of interpolated values: " 
            << totalError / numberSuccessfulInterpolations << std::endl;

#endif // HAVE_PKG_asf 
 
  //
  //
  //
  return 0;

}
