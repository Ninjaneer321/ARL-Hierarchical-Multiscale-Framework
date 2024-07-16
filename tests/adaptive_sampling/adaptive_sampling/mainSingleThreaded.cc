//
// Test InterpolationDatabase
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#include "XYArgument.h"
#include "XYInputFilter.h"
#include "XYOutputFilter.h"
#include "XYValue.h"

#include <adaptive_sampling/InterpolationDatabase.h>
#include <mtreedb/MTreeObject.h>

#include <fstream>

// local functions

namespace {

  struct WorkerThreadData {
    int                               numberPoints;
    int                               threadId;
    int                               numberThreads;
    std::vector<double>             * xCoordinates;
    std::vector<double>             * yCoordinates;
    arl::hms::InterpolationDatabase * database;
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
    arl::hms::InterpolationDatabase & database = *(data->database);
    
    //
    // compute share of work
    //
    const int threadDataSize = numberPoints / numberThreads;
    const int iStart = threadId * threadDataSize;
    const int iEnd = (threadId + 1) * threadDataSize;

    //
    //
    //
    for(
    
    //
    //
    //
    return;

  }

  /*
  const int numberPoints = 40;
  const double xCoordinates[numberPoints] = {-.2, -.3, .15, .02, .8, -.8, .43, .99, -.3, -.4, -.4, -.7, .55, -.23, .67, .06, .12, -.05, .87, .21, .03, -.89, -.76, -.34, .04, -.22, .93, .023, .56, -.78, .8, .11, .39, .25, .44, .09, .022, -.67, -.67, .233};
  const double yCoordinates[numberPoints] = {.23, -.44, .44, -.67, .8, -.34, -.67, .34, .31, .09, -.89, -.97, -.32, .01, .1, .4, .56, .77, .32, .46, .457, .001, -.34, .56, .78, -.09, .56, -.899, .50, .35, -.23, .67, .24, -.09, .56, .789, .21, .11, -.09, .123};
  */

}

//
//
//
int
main(int    ac,
     char * av[])
{

  //
  // set tolerance
  //
  const double tolerance = 1.0e-4;

  //
  // instantiate database hint
  //
  int hint = MPTCOUPLER::mtreedb::MTreeObject::getUndefinedId();

  //
  // instantiate XYInputFilter and XYOutputFilter
  //
  arl::hms::XYInputFilterPointer xyInputFilter(new arl::hms::XYInputFilter);
  arl::hms::XYOutputFilterPointer xyOutputFilter(new arl::hms::XYOutputFilter);
  
  //
  // instantiate interpolation database
  //
  const int maxKrigingModelSize = 50;
  const int maxNumberSearchModels = 4;
  const double meanErrorFactor = 1.0;
  const double maxQueryPointModelDistance = 1000.0;
  const int agingThreshold = 600000000;
  const std::string mtreeDirectoryName(".");
  arl::hms::InterpolationDatabase interpolationDatabase(xyInputFilter,
                                                        xyOutputFilter,
                                                        tolerance,
                                                        maxKrigingModelSize,
                                                        maxNumberSearchModels,
                                                        meanErrorFactor,
                                                        maxQueryPointModelDistance,
                                                        agingThreshold,
                                                        mtreeDirectoryName);
  
  //
  // read number of points
  //
  std::ifstream pointFile("grid.dat");
  std::string numberPointsString;
  pointFile >> numberPointsString;
  const int numberPoints = std::atoi(numberPointsString.c_str());
  
  std::vector<double> xCoordinates(numberPoints);
  std::vector<double> yCoordinates(numberPoints);

  for(int i=0; i<numberPoints; ++i) {
    pointFile >> xCoordinates[i];
    pointFile >> yCoordinates[i];
  }

  int numberSuccessfulInterpolations = 0;
  
  //
  // iterate over points, attempting to interpolate
  //
  for(int i=0; i<numberPoints; ++i) {
    
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
      std::cout << "Interpolated Successfully" << std::endl;
      ++numberSuccessfulInterpolations;

      //
      // get interpolated value
      //
      const arl::hms::XYValuePointer interpolatedValue = 
        dynamic_pointer_cast<arl::hms::XYValue>(interpolationResult.second);

      //
      // compute actual value of function
      //
      const double exactValue = f(x, y);
      
      //
      // compute error
      //
      const double error = std::abs(exactValue - interpolatedValue->getValue());
      
      //
      // output error
      //
      std::cout << "Interpolation Error: " << error << std::endl;
    
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
      interpolationDatabase.insert(xyArgument,
                                   xyValue,
                                   hint);

    }
    
  }

  std::cout << "Number Successful Interpolations: " 
            << numberSuccessfulInterpolations << " " 
            << static_cast<double>(numberSuccessfulInterpolations) / numberPoints << std::endl;

  //
  //
  //
  return 0;

}
