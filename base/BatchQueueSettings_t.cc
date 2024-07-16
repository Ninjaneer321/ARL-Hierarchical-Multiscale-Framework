//
// File:      BatchQueueSettings_t.cc
// Package    base
//
// Base classes for HMS.
//

//
//
//

namespace arl {
  namespace hms {

    //
    // Serialize model.
    //
    template <class Archive>
    void 
    BatchQueueSettings::serialize(Archive & archive, const unsigned int)
    {

      //
      // serialize contents of batch queue settings
      //
      archive & d_queue;
      archive & d_walltime;
      archive & d_numberNodes;
      archive & d_numberCpus;

      //
      //
      //
      return;
      
    }
  
  }
}
