//
// File:      BatchQueueSettings.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "BatchQueueSettings.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    BatchQueueSettings::BatchQueueSettings()
    {

      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    BatchQueueSettings::BatchQueueSettings(const std::string & queue,
					   const std::string & walltime,
					   const int & numberNodes,
					   const int & numberCpus) :
      d_queue(queue),
      d_walltime(walltime),
      d_numberNodes(numberNodes),
      d_numberCpus(numberCpus)
    {
      
      //
      //
      //
      return;
      
    }

    //
    // Destructor.
    //
    BatchQueueSettings::~BatchQueueSettings()
    {

      //
      //
      //
      return;

    }

    //
    // Obtain queue.
    //
    const std::string & 
    BatchQueueSettings::getQueue() const
    {

      //
      //
      //
      return d_queue;

    }

    //
    // Obtain Walltime.
    //
    const std::string & 
    BatchQueueSettings::getWalltime() const
    {

      //
      //
      //
      return d_walltime;

    }
    
    //
    // Obtain NumberNodes.
    //
    const int & 
    BatchQueueSettings::getNumberNodes() const
    {

      //
      //
      //
      return d_numberNodes;
      
    }
   
    //
    // Obtain NumberCpus.
    //
    const int & 
    BatchQueueSettings::getNumberCpus() const
    {
      
      //
      //
      //
      return d_numberCpus;
      
    }

    //
    // output operator.
    //
    std::ostream & operator<<(std::ostream & outputStream,
                              const BatchQueueSettings & batchQueueSettings)
    {

      //
      // output queue
      //
      outputStream << "Queue: " << batchQueueSettings.getQueue() << std::endl;

      //
      // output walltime
      //
      outputStream << "Walltime: " << batchQueueSettings.getWalltime() 
		   << std::endl;

      //
      // output numberNodes
      //
      outputStream << "numberNodes: " << batchQueueSettings.getNumberNodes() 
		   << std::endl;

      //
      // output numberCpus
      //
      outputStream << "numberCpus: " << batchQueueSettings.getNumberCpus() 
		   << std::endl;


      //
      //
      //
      return outputStream;

    }

  }
}
