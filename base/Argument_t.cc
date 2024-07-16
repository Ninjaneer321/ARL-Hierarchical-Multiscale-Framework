//
// File:      Argument_t.cc
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
    // Serialize argument.
    //
    template <class Archive>
    void 
    Argument::serialize(Archive & archive, const unsigned int)
    {

      archive & d_toCleanupFlag;
      
      //
      //
      //
      return;
      
    }
  
  }
}
