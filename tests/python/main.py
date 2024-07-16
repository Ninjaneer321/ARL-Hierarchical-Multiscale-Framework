import hms
import random
import time
from pi_filters import *

if __name__ == "__main__":
    
    if len(sys.argv) < 3:
        print("Usage: " + str(sys.argv[0]) + " <hmsConf.ini> <num-samples>")
        sys.exit(-1)

    configFile = sys.argv[1]
    numSamples = int(sys.argv[2])
    pointFileName = "point.txt"
    
    communicator = hms.BrokerLauncher().launch(configFile,
                                               0,
                                               1)
    communicator = communicator[0]

    model = hms.Model("python",
                      "unit_circle_indicator_function.py",
                      hms.StringVector([pointFileName]),
                      hms.CPU,
                      1)

    for i in range(0, numSamples):
 
        x1 = 2.0 * random.random() - 1.0
        x2 = 2.0 * random.random() - 1.0
        
        argument = PiArgument(x1, x2)
        inputFilter = PiInputFilter(pointFileName)
        outputFilter = PiOutputFilter()
        
        modelPackage = hms.ModelPackage(model,
                                        inputFilter,
                                        outputFilter,
                                        argument)
        
        communicator.send(modelPackage)
        
    counter = 0
    valueSum = 0.0
    while counter < numSamples:
        modelPackages = communicator.receive()
        counter = counter + len(modelPackages)
        for modelPackage in modelPackages:
            value = modelPackage.getValue()
            valueSum += value.value

    piEstimate = 4.0 * valueSum / float(numSamples);
    print("Estimate for pi: " + str(piEstimate))
    
  
