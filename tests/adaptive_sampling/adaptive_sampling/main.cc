//
// Test InterpolationDatabase
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#if defined(HAVE_MPI)
#include <mpi.h>
#endif // HAVE_MPI

#include <adaptive_sampling/AdaptiveSamplingLauncher.h>
#include <base/Model.h>
#include <base/ModelPackage.h>
#include <exception/IOError.h>

#include "XYArgument.h"
#include "XYInputFilter.h"
#include "XYAdaptiveSamplingInputFilter.h"
#include "XYAdaptiveSamplingOutputFilter.h"
#include "XYOutputFilter.h"
#include "XYValue.h"

#if defined(HAVE_FSTREAM)
#include <fstream>
#else
#error fstream header file not available
#endif // HAVE_FSTREAM

#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iostream header file not available
#endif // HAVE_IOSTREAM

#if defined(HAVE_STRING)
#include <string>
#else
#error string header file not available
#endif // HAVE_STRING

const int chunkSize = 10;

const std::string configFilePath = "hmsConf.ini";
const std::string xyExecutable = "xy";

//
//
//
int
main(int  ac ,
     char **  av )
{

#if defined(HAVE_MPI)
  
  MPI_Init(&ac,
           &av);
  
#endif // HAVE_MPI
    
  //
  // read number of points
  //
  std::ifstream pointFile("gaussianRandom.dat");

  if(!pointFile) {
    const std::string message("Error opening file containing points to "
			      "interpolate.");
    throw arl::hms::IOError(message);
  }

  std::string numberPointsString;
  pointFile >> numberPointsString;
  const int numberPoints = std::atoi(numberPointsString.c_str());
  
  std::vector<double> xCoordinates(numberPoints);
  std::vector<double> yCoordinates(numberPoints);

  for(int i=0; i<numberPoints; ++i) {
    pointFile >> xCoordinates[i];
    pointFile >> yCoordinates[i];
  }

  //
  // launch adaptive sampling module
  //
  arl::hms::InterpolationDatabaseInputFilterPointer
    asInputFilter(new arl::hms::XYAdaptiveSamplingInputFilter);
  arl::hms::InterpolationDatabaseOutputFilterPointer
    asOutputFilter(new arl::hms::XYAdaptiveSamplingOutputFilter);
  std::vector<arl::hms::CommunicatorPointer> communicators = 
    arl::hms::AdaptiveSamplingLauncher().launch(configFilePath,
                                                0,
                                                1,
                                                asInputFilter,
                                                asOutputFilter);

  //
  // check to ensure we only launched one AS module
  //
  if(communicators.size() != 1) {
    const std::string message("Error launching AS module.");
    throw arl::hms::IOError(message);
  }

  arl::hms::CommunicatorPointer & communicator = communicators[0];
  
  //
  // instantiate input filter
  //
  arl::hms::InputFilterPointer inputFilter(new arl::hms::XYInputFilter);

  //
  // instantiate output filter
  //
  arl::hms::OutputFilterPointer outputFilter(new arl::hms::XYOutputFilter);
  
  //
  // instantiate model
  //
  arl::hms::ModelPointer 
    model(new arl::hms::Model("",
                              xyExecutable,
                              std::vector<std::string>(),
                              arl::hms::CPU,
                              1));
    
  int numberProcessedPoints = 0;
  while(numberProcessedPoints < numberPoints) {
    
    //
    // calculate index to end of points
    //
    const int endIndex = std::min(numberPoints,
                                  numberProcessedPoints + chunkSize);

    //
    // iterate over points, sending them to HMS for evaluation
    //
    for(int i=numberProcessedPoints; i<endIndex; ++i) {
      
      //
      // get x and y coordinate of point
      //
      const double x = xCoordinates[i];
      const double y = yCoordinates[i];
      
      //
      // instantiate argument
      //
      arl::hms::XYArgumentPointer argument(new arl::hms::XYArgument(x,
                                                                    y));
      
      //
      // instantiate model package
      //
      arl::hms::ModelPackagePointer 
        modelPackage(new arl::hms::ModelPackage(model,
                                                inputFilter,
                                                outputFilter,
                                                argument));
      
      //
      // send model package for evaluation
      //
      communicator->send(modelPackage);
      
    }

    //
    // receive model packages
    //
    int numberReceivedPackages = 0;
    while(numberReceivedPackages < endIndex - numberProcessedPoints) {
  
      //
      // receive model packages on communicator
      //
      std::vector<arl::hms::ModelPackagePointer> modelPackages = 
        communicator->receive<arl::hms::ModelPackagePointer>();
      
      //
      // increment number of received packages
      //
      numberReceivedPackages += modelPackages.size();
      
      //
      // iterate over model packages, computing error
      //
      for(std::vector<arl::hms::ModelPackagePointer>::size_type i=0; 
          i<modelPackages.size(); ++i) {
        
        //
        // get handle to model package
        //
        const arl::hms::ModelPackagePointer & modelPackage = modelPackages[i];
        
        //
        // get argument and value from model package
        //
        arl::hms::XYArgumentPointer argument =
          boost::dynamic_pointer_cast<arl::hms::XYArgument>(modelPackage->getArgument());
        arl::hms::XYValuePointer value =
          boost::dynamic_pointer_cast<arl::hms::XYValue>(modelPackage->getValue());
        
        //
        // get x and y and compute exact value of function
        //
        const double x = argument->getX();
        const double y = argument->getY();
        const double exactValue = x*x + y*y;
        
        //
        // get evaluated value
        //
        const double evaluatedValue = value->getValue();
        
        //
        // compute error
        //
        const double error = (exactValue - evaluatedValue) * 
          (exactValue - evaluatedValue); 

        std::cout << "Error: " << error << std::endl;
        
      }

    }

    numberProcessedPoints = endIndex;
   
  }

 
}
