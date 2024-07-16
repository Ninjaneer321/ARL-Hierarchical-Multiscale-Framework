//
// File:      BasicBroker.h
// Package    broker
//
// HMS Brokers.
//
#if !defined(broker_BasicBroker_h)
#define broker_BasicBroker_h

#include "Broker.h"
#include "CleanupPolicy.h"

#include <base/CommandFactory.h>
#include <base/ResourceManager.h>
#include <base/StateDB.h>
#include <monitor/Monitor.h>
#include <scheduler/Scheduler.h>
#include <warehouse/ModelPackageWarehouse.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing a basic single-threaded Broker.
     */
    class BasicBroker : public Broker {

      //
      // types
      //
    public:

      //
      // methods
      //
    public:

      /**
       * Constructor.
       *
       * @param clientCommunicators Communicators to client algorithm.
       * @param scheduler Scheduler for command execution.
       * @param monitor Monitor for command monitoring.
       * @param cleanupPolicy Policy for cleaning up command directories.
       * @param commandFactory Factory for building commands.
       * @param resourceManager ResourceManager
       * @param outputDirectory Output directory where model
       * executions are stored and run.
       * @param handleLoadQuery Whether this broker is expected to handle
       * load queries from the client algorithm.
       */
      BasicBroker(const std::vector<CommunicatorPointer> & clientCommunicators,
                  const SchedulerPointer                 & scheduler,
                  const MonitorPointer                   & monitor,
		  const CleanupPolicyPointer             & cleanupPolicy,
                  const CommandFactoryPointer            & commandFactory,
                  const ResourceManagerPointer           & resourceManager,
                  const std::string                      & outputDirectory,
		  bool                                     handleLoadQuery);

      /**
       * Destructor.
       */
      virtual ~BasicBroker();
  
      /**
       * Execute cycle of receiving model packages, creating commands,
       * scheduling them to run, monitoring their execution, and
       * sending values back.
       */
      virtual void run();

    private:
      //
      // copy constructor/assignment operator
      //
      BasicBroker(const BasicBroker &); // not implemented
      BasicBroker & operator=(const BasicBroker &); // not implemented

      //
      // data
      //
    private:

      ModelPackageWarehousePointer     d_modelPackageWarehouse;
      std::vector<CommunicatorPointer> d_clientCommunicators;
      SchedulerPointer                 d_scheduler;
      MonitorPointer                   d_monitor;
      CleanupPolicyPointer             d_cleanupPolicy;
      CommandFactoryPointer            d_commandFactory;
      ResourceManagerPointer           d_resourceManager;
      std::string                      d_outputDirectory;
      long unsigned int                d_commandCounter;
      bool                             d_handleLoadQuery;
      StateDB                          d_stateDB;
   
    };

  }
}

#endif // broker_BasicBroker_h
