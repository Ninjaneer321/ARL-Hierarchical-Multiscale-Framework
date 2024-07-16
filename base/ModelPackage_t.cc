//
// Predicates for HMS commands.
//

#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

BOOST_CLASS_EXPORT_KEY(arl::hms::ModelPackage)
BOOST_CLASS_EXPORT_KEY(arl::hms::ModelPackagePointer)

namespace arl {
  namespace hms {

    //
    // Serialize model package.
    //
    template <class Archive>
    void 
    ModelPackage::serialize(Archive & archive, const unsigned int)
    {

      //
      // serialize model
      //
      archive & d_model;

      //
      // serialize input filter
      //
      archive & d_inputFilter;

      //
      // serialize output filter
      //
      archive & d_outputFilter;

      //
      // serialize argument
      //
      archive & d_argument;
      
      //
      // serialize value
      // 
      archive & d_value;

      //
      // serialize hint
      //
      archive & d_hint;
      
      //
      // serialize id
      // 
      archive & d_id;

      //
      // seralize wall clock time
      //
      archive & d_wallClockTime;

      //
      // serialize priority
      //
      archive & d_priority;

      //
      // seralize communicator ids
      //
      archive & d_communicatorIds;

      //
      // serialize timestamps
      //
      archive & d_timestamps;

      //
      // serialize number evaluations
      //
      archive & d_numberEvaluations;

      //
      // serialize batch queue settings
      //
      archive & d_batchQueueSettings;

      //
      // serialize status predicates
      //
      archive & d_statusPredicates;
      
      //
      // serialize end predicates
      //
      archive & d_endPredicates;
      
      //
      //
      //
      return;
      
    }
  
  }
}
