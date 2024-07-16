//
// File:      LoadQuery.cc
// Package    scheduler
//
// HMS Job Schedulers.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "LoadQuery.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    LoadQuery::LoadQuery() :
      d_numberFreeResources(0),
      d_numberQueuedJobs(0),
      d_numberQueuedJobsAllUsers(0),
      d_numberUsedResources(0),
      d_numberTotalResources(0)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    LoadQuery::~LoadQuery()
    {

      //
      //
      //
      return;

    }

    //
    // Execute query.
    //
    void 
    LoadQuery::execute(const SchedulerPointer & scheduler,
		       const ResourceManager  & resourceManager,
                       const StateDB          & stateDB)
    {

      //
      // get number of free resources
      //
      d_numberFreeResources = 
        scheduler->getNumberFreeResources(resourceManager);
      d_numberQueuedJobs = scheduler->getNumberQueuedJobs(stateDB);
      d_numberQueuedJobsAllUsers = scheduler->getNumberQueuedJobsAllUsers(stateDB);
      d_numberUsedResources = scheduler->getNumberUsedResources(stateDB);
      d_numberTotalResources = scheduler->getNumberTotalResources(stateDB);

      //
      //
      //
      return;

    }

    //
    // Get number of free resources.
    //
    unsigned int
    LoadQuery::getNumberFreeResources() const
    {

      //
      //
      //
      return d_numberFreeResources;

    }

    //
    // Get number of queued jobs.
    //
    unsigned int
    LoadQuery::getNumberQueuedJobs() const
    {
      
      //
      //
      //
      return d_numberQueuedJobs;

    }

    //
    // Get number of queued jobs for all users.
    //
    unsigned int
    LoadQuery::getNumberQueuedJobsAllUsers() const
    {

      //
      //
      //
      return d_numberQueuedJobsAllUsers;

    }

    //
    // Get number of free resources.
    //
    unsigned int
    LoadQuery::getNumberUsedResources() const
    {

      //
      //
      //
      return d_numberUsedResources;

    }

    //
    // Get total number or resources.
    //
    unsigned int
    LoadQuery::getNumberTotalResources() const
    {

      //
      //
      //
      return d_numberTotalResources;

    }

    //
    // Set number of free resources.
    //
    void
    LoadQuery::setNumberFreeResources(unsigned int numberFreeResources)
    {

      //
      //
      //
      d_numberFreeResources = numberFreeResources;

    }

    //
    // Set number of queued jobs.
    //
    void
    LoadQuery::setNumberQueuedJobsAllUsers(unsigned int numberQueuedJobs)
    {
      
      //
      //
      //
      d_numberQueuedJobsAllUsers = numberQueuedJobs;

    }

  }
}
