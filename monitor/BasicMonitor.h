//
// File:      BasicMonitor.h
// Package    monitor
//
// HMS Job Monitors.
//
#if !defined(monitor_BasicMonitor_h)
#define monitor_BasicMonitor_h

#include "Monitor.h"

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing a basic monitor.
     */
    class BasicMonitor : public Monitor {

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
       */
      BasicMonitor();

      /**
       * Destructor.
       */
      virtual ~BasicMonitor();
  
      /**
       * Process queue of running commands.
       *
       * @param resourceManager Manager of available resources.
       * @param stateDB State of queued, running, and completed commands.
       */
      virtual void processQueue(ResourceManager & resourceManager,
                                StateDB         & stateDB) const;

      //
      // methods
      // 
    protected:

    private:
      //
      // copy constructor/assignment operator
      //
      BasicMonitor(const BasicMonitor &); // not implemented
      BasicMonitor & operator=(const BasicMonitor &); // not implemented

      //
      // data
      //
    private:

    };

  }
}

#endif // monitor_BasicMonitor_h
