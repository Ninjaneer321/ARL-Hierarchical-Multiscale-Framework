//
// File:      LoadQuery_t.cc
// Package    base
//
// Base classes for HMS.
//

//
//
//

BOOST_CLASS_EXPORT_KEY(arl::hms::LoadQuery)
BOOST_CLASS_EXPORT_KEY(arl::hms::LoadQueryPointer)

namespace arl {
  namespace hms {

    //
    // Serialize value.
    //
    template <class Archive>
    void 
    LoadQuery::serialize(Archive & archive, const unsigned int)
    {

      archive & d_numberFreeResources;
      archive & d_numberQueuedJobs;
      archive & d_numberQueuedJobsAllUsers;
      archive & d_numberUsedResources;
      archive & d_numberTotalResources;

      //
      //
      //
      return;
      
    }
  
  }
}
