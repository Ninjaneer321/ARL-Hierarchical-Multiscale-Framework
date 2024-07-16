//
// File:      Scheduler.h
// Package    scheduler
//
// HMS Job Schedulers.
//
#if !defined(scheduler_Scheduler_h)
#define scheduler_Scheduler_h

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
     * @brief Base class for scheduler.
     */
    class Scheduler {

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
      virtual ~Scheduler() = 0;
  
      /**
       * Process queue of queued commands.
       *
       * @param resourceManager Manager of available resources.
       * @param stateDB State of queued, running, and completed commands.
       */
      virtual void processQueue(ResourceManager & resourceManager,
                                StateDB         & stateDB) const = 0;

      /**
       * Get number of free resources.
       *
       * @param resourceManager Manager of available resources.
       *
       * @return number of free resources.
       */
      virtual unsigned int getNumberFreeResources(const ResourceManager & resourceManager) const = 0;

      /**
       * Get number of queued jobs.
       *
       * @param stateDB State of queued, running, and completed commands.
       *
       * @return number of queued jobs.
       */
      virtual unsigned int getNumberQueuedJobs(const StateDB & stateDB) const = 0;

      /**
       * Get number of queued jobs (for all users).
       *
       * @param stateDB State of queued, running, and completed commands.
       *
       * @return number of queued jobs for all users.
       */
      virtual unsigned int getNumberQueuedJobsAllUsers(const StateDB & stateDB) const = 0;

      /**
       * Get number of used resources.
       *
       * @param stateDB State of queued, running, and completed commands.
       *
       * @return number of used resources.
       */
      virtual unsigned int getNumberUsedResources(const StateDB & stateDB) const = 0;

      /**
       * Get total number of resources.
       *
       * @param stateDB StateDB State of total resources available on machine
       *
       * @return total number of resources
       */
      virtual unsigned int getNumberTotalResources(const StateDB & stateDB) const = 0;

      //
      // methods
      // 
    protected:

      /**
       * Constructor.
       */
      Scheduler();

    private:
      //
      // copy constructor/assignment operator
      //
      Scheduler(const Scheduler &); // not implemented
      Scheduler & operator=(const Scheduler &); // not implemented

      //
      // data
      //
    private:

    };

    HMS_SHARED_PTR(Scheduler);
    
  }
}

#endif // scheduler_Scheduler_h
