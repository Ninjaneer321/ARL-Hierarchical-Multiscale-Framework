//
// File:      LoadQuery.h
// Package    scheduler
//
// HMS Job Schedulers.
//
#if !defined(scheduler_LoadQuery_h)
#define scheduler_LoadQuery_h

#include <base/ResourceManager.h>
#include <base/StateDB.h>

#include <scheduler/Scheduler.h>

#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Query load of broker.
     */
    class LoadQuery {

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
      LoadQuery();

      /**
       * Destructor.
       */
      virtual ~LoadQuery();

      /**
       * Execute query.
       *
       * @param scheduler Scheduler of ModelPackages.
       * @param resourceManager ResourceManager.
       * @param stateDB StateDB.
       */
      void execute(const SchedulerPointer & scheduler,
                   const ResourceManager  & resourceManager,
                   const StateDB          & stateDB);

      /**
       * Get number of free resources.
       *
       * @return number of free resources.
       */
      unsigned int
      getNumberFreeResources() const;

      /**
       * Get number of used resources.
       *
       * @return number of used resources.
       */
      unsigned int
      getNumberUsedResources() const;

      /**
       * Get number of queued jobs.
       *
       * @return number of queued jobs.
       */
      unsigned int
      getNumberQueuedJobs() const;

      /**
       * Get number of queued jobs for all users.
       *
       * @return number of queued jobs.
       */
      unsigned int
      getNumberQueuedJobsAllUsers() const;

      /**
       * Get total number of resources a machine has
       *
       * @return number of total resources
       */
      unsigned int
      getNumberTotalResources() const;

      /**
       * Set number of free resources.
       *
       * @param numberFreeResources New number of free resources.
       */
      void
      setNumberFreeResources(unsigned int numberFreeResources);

      /**
       * Set number of queued jobs for all users.
       *
       * @param numberQueuedJobs New number of queued jobs.
       */
      void
      setNumberQueuedJobsAllUsers(unsigned int numberQueuedJobs);

      /**
       * Serialize LoadQuery.
       *
       * @param archive Archive to store serialization.
       * @param version Version of serialization.
       */
      template <class Archive>
      void serialize(Archive & archive, const unsigned int version);
  
      //
      // methods
      // 
    protected:

    private:
      //
      // copy constructor/assignment operator
      //
      LoadQuery(const LoadQuery &); // not implemented
      LoadQuery & operator=(const LoadQuery &); // not implemented

      //
      // data
      //
    private:

      unsigned int d_numberFreeResources;
      unsigned int d_numberQueuedJobs;
      unsigned int d_numberQueuedJobsAllUsers;
      unsigned int d_numberUsedResources;
      unsigned int d_numberTotalResources;

    };

    HMS_SHARED_PTR(LoadQuery);

  }
}

#include "LoadQuery_t.cc"

#endif // scheduler_LoadQuery_h
