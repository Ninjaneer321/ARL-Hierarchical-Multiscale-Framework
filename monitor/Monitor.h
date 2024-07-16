//
// File:      Monitor.h
// Package    monitor
//
// HMS Job Monitors.
//
#if !defined(monitor_Monitor_h)
#define monitor_Monitor_h

#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    //
    // forward declarations
    //
    class ResourceManager;
    class StateDB;

    /**
     * @brief Base class for monitor.
     */
    class Monitor {

      //
      // types
      //
    public:

      //
      // methods
      //
    public:

      /**
       * Destructor.
       */
      virtual ~Monitor() = 0;
  
      /**
       * Process queue of running commands.
       *
       * @param resourceManager Manager of available resources.
       * @param stateDB State of queued, running, and completed commands.
       */
      virtual void processQueue(ResourceManager & resourceManager,
                                StateDB         & stateDB) const = 0;

      //
      // methods
      // 
    protected:

      /**
       * Constructor.
       */
      Monitor();

    private:
      //
      // copy constructor/assignment operator
      //
      Monitor(const Monitor &); // not implemented
      Monitor & operator=(const Monitor &); // not implemented

      //
      // data
      //
    private:

    };

    HMS_SHARED_PTR(Monitor);

  }
}

#endif // monitor_Monitor_h
