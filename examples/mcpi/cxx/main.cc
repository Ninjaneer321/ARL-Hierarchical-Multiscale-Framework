#include "PiArgument.h"
#include "PiInputFilter.h"
#include "PiOutputFilter.h"
#include "PiValue.h"

#include <broker/BrokerLauncher.h>

#include <cstdlib>
#include <iostream>

const std::string unitCircleIndicatorDirectory("");

int 
main(int ac, char * av[])
{

  if(ac < 3) {
    std::cout << "Usage: " << av[0] << " <hmsConf.ini> <num-samples>" << std::endl;
    exit(-1);
  }
  
  const std::string configFile(av[1]);
  const int numSamples(std::atoi(av[2]));
  const std::string pointFileName("point.txt");

  std::vector<arl::hms::CommunicatorPointer> communicators = 
    arl::hms::BrokerLauncher().launch(configFile, 0, 1);
  arl::hms::CommunicatorPointer communicator = communicators[0];

  std::stringstream unitCirclePath;
  unitCirclePath << unitCircleIndicatorDirectory << "unit_circle_indicator_function.py";
  
  arl::hms::ModelPointer 
    model(new arl::hms::Model("python",
			      unitCirclePath.str().c_str(),
			      std::vector<std::string>(1, pointFileName),
			      arl::hms::CPU,
			      1));

  for(int i=0; i<numSamples; ++i) {
    double x1 = 2.0 * static_cast<float>(rand())/static_cast<float>(RAND_MAX) - 1.0;
    double x2 = 2.0 * static_cast<float>(rand())/static_cast<float>(RAND_MAX) - 1.0;
 
    PiArgumentPointer argument(new PiArgument(x1, x2));
    PiInputFilterPointer inputFilter(new PiInputFilter(pointFileName));
    PiOutputFilterPointer outputFilter(new PiOutputFilter);

    arl::hms::ModelPackagePointer
      modelPackage(new arl::hms::ModelPackage(model, 
					      inputFilter,
					      outputFilter,
					      argument));
    communicator->send(modelPackage);
  }

  int counter = 0;
  double valueSum = 0.0;
  while(counter < numSamples){
    std::vector<arl::hms::ModelPackagePointer> modelPackages = 
      communicator->receive<arl::hms::ModelPackagePointer>();
    counter += modelPackages.size();
    for(std::vector<arl::hms::ModelPackagePointer>::const_iterator iter = 
	  modelPackages.begin(); iter != modelPackages.end(); ++iter) {
      arl::hms::ModelPackagePointer modelPackage = *iter;
      PiValuePointer value = 
	boost::dynamic_pointer_cast<PiValue>(modelPackage->getValue());
      valueSum += value->getValue();
    }
  }

  const double piEstimate = 4.0 * valueSum / static_cast<float>(numSamples);
  std::cout << "Estimate for pi: " << piEstimate << std::endl;

}
