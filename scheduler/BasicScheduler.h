//
// File:      BasicScheduler.h
// Package    scheduler
//
// HMS Job Schedulers.
//
#if !defined(scheduler_BasicScheduler_h)
#define scheduler_BasicScheduler_h

#include "Scheduler.h"

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing a basic scheduler.
     */
    class BasicScheduler : public Scheduler {

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
      BasicScheduler(bool isSpeculative);

      /**
       * Destructor.
       */
      virtual ~BasicScheduler();
  
      /**
       * Process queue of queued commands.
       *
       * @param resourceManager Manager of available resources.
       * @param stateDB State of queued, running, and completed commands.
       */
      virtual void processQueue(ResourceManager & resourceManager,
                                StateDB         & stateDB) const;

      /**
       * Get number of free resources.
       *
       * @param resourceManager Manager of available resources.
       *
       * @return number of free resources.
       */
      virtual unsigned int getNumberFreeResources(const ResourceManager & resourceManager) const;

      /**
       * Get number of queued jobs.
       *
       * @param stateDB State of queued, running, and completed commands.
       *
       * @return number of queued jobs.
       */
      virtual unsigned int getNumberQueuedJobs(const StateDB & stateDB) const;

      /**
       * Get number of queued jobs (for all users).
       *
       * @param stateDB State of queued, running, and completed commands.
       *
       * @return number of queued jobs for all users.
       */
      virtual unsigned int getNumberQueuedJobsAllUsers(const StateDB & stateDB) const;

      /**
       * Get number of used resources.
       *
       * @param stateDB State of queued, running, and completed commands.
       *
       * @return number of used resources.
       */
      virtual unsigned int getNumberUsedResources(const StateDB & stateDB) const;

      /**
      * Get total number of resources.
      * 
      * @param stateDB StateDB State of total resources available on machine
      * 
      *  @return total number of resources
      **/
      virtual unsigned int getNumberTotalResources(const StateDB & stateDB) const;


      //
      // methods
      // 
    protected:

    private:
      //
      // copy constructor/assignment operator
      //
      BasicScheduler(const BasicScheduler &); // not implemented
      BasicScheduler & operator=(const BasicScheduler &); // not implemented

      //
      // data
      //
    private:

      bool d_isSpeculative;

    };

  }
}

#endif // scheduler_BasicScheduler_h
