//
// File:      ModelPackage.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "ModelPackage.h"

#include "TimeUtils.h"

#if defined(HAVE_CERRNO)
#include <cerrno>
#else
#error cerrno header file not available
#endif // HAVE_CERRNO

#if defined(HAVE_CSTRING)
#include <cstring>
#else
#error cstring header file not available
#endif // HAVE_CSTRING

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    ModelPackage::ModelPackage()
    {

      //
      //
      //
      return;
      
    }

    //
    // Constructor.
    //
    ModelPackage::ModelPackage(const ModelPointer        & model,
                               const InputFilterPointer  & inputFilter,
                               const OutputFilterPointer & outputFilter,
                               const ArgumentPointer     & argument) :
      d_argument(argument),
      d_batchQueueSettings(BatchQueueSettingsPointer()),
      d_hint(-1),
      d_id(0),
      d_inputFilter(inputFilter),
      d_model(model),
      d_numberEvaluations(0),
      d_outputFilter(outputFilter),
      d_priority(NORMAL),
      d_wallClockTime(0.0)
    {

      //
      //
      //
      return;

    }
    
    //
    // Constructor (enables ModelPackage to be submitted to a batch queue).
    //
    ModelPackage::ModelPackage(const ModelPointer              & model,
			       const InputFilterPointer        & inputFilter,
			       const OutputFilterPointer       & outputFilter,
			       const ArgumentPointer           & argument,
			       const BatchQueueSettingsPointer & batchQueueSettings) :
      d_argument(argument),
      d_batchQueueSettings(batchQueueSettings),
      d_hint(-1),
      d_id(0),
      d_inputFilter(inputFilter),
      d_model(model),
      d_numberEvaluations(0),
      d_outputFilter(outputFilter),
      d_priority(NORMAL),
      d_wallClockTime(0.0)
    {

      //
      //
      //
      return;

    }      

    //
    // Destructor.
    //
    ModelPackage::~ModelPackage()
    {
      
      //
      //
      //
      return;

    }

    //
    // Add communicator id for routing of ModelPackage.
    //
    void 
    ModelPackage::addCommunicator(int communicatorId)
    {

      //
      // add communicator to stack of communicator ids
      //
      d_communicatorIds.push_back(communicatorId);
      
      //
      //
      //
      return;

    }

    //
    // Add an end predicate for use when evaluating the ModelPackage.
    //
    void
    ModelPackage::addEndPredicate(const EndPredicatePointer & predicate)
    {

      //
      //
      //
      d_endPredicates.push_back(predicate);

      //
      //
      //
      return;
      
    }

    //
    // Add a status predicate for use when evaluating the ModelPackage.
    //
    void
    ModelPackage::addStatusPredicate(const StatusPredicatePointer & predicate)
    {

      //
      //
      //
      d_statusPredicates.push_back(predicate);

      //
      //
      //
      return;
      
    }
      
    //
    // Obtain argument.
    //
    const ArgumentPointer & 
    ModelPackage::getArgument() const
    {

      //
      //
      //
      return d_argument;
      
    }

    //
    // Obtain batch queue settings.
    //
    const BatchQueueSettingsPointer & 
    ModelPackage::getBatchQueueSettings() const
    {

      //
      //
      //
      return d_batchQueueSettings;

    }

    //
    // Obtain end predicates.
    //
    const std::vector<EndPredicatePointer> &
    ModelPackage::getEndPredicates() const
    {

      //
      //
      //
      return d_endPredicates;
      
    }
          
    //
    // Get hint for adaptive sampling.
    //
    int 
    ModelPackage::getHint() const
    {

      //
      //
      //
      return d_hint;

    }
   
    //
    // Obtain id.
    //
    // @return Id of ModelPackage
    //
    ModelPackage::IdType 
    ModelPackage::getId() const
    {

      //
      //
      //
      return d_id;

    }

    //
    // Obtain input filter.
    //
    const InputFilterPointer &
    ModelPackage::getInputFilter() const
    {

      //
      //
      //
      return d_inputFilter;
      
    }

    //
    // Obtain model.
    //
    const ModelPointer &
    ModelPackage::getModel() const
    {

      //
      //
      //
      return d_model;

    }

    //
    // Obtain number of evaluations.
    //
    int 
    ModelPackage::getNumberEvaluations() const
    {
      
      //
      //
      //
      return d_numberEvaluations;

    }
 
    //
    // Obtain output filter.
    //
    const OutputFilterPointer & 
    ModelPackage::getOutputFilter() const
    {

      //
      //
      //
      return d_outputFilter;

    }

    //
    // Obtain priority for evaluation.
    //
    const Priority & 
    ModelPackage::getPriority() const
    {
      
      //
      //
      //
      return d_priority;
      
    }

    //
    // Obtain status predicates.
    //
    const std::vector<StatusPredicatePointer> &
    ModelPackage::getStatusPredicates() const
    {

      //
      //
      //
      return d_statusPredicates;
      
    }

    //
    // Get timestamp log messages.
    //
    std::string 
    ModelPackage::getTimestampLog() const
    {
      
      std::stringstream log;
      log << std::setprecision(18);
      for(std::vector<std::pair<double, std::string> >::const_iterator iter = 
	    d_timestamps.begin(); iter != d_timestamps.end(); ++iter) {
	log << iter->first << " " << iter->second << std::endl;
      }
      
      //
      //
      //
      return log.str();

    }

    //
    // Obtain value.
    //
    const ValuePointer & 
    ModelPackage::getValue() const
    {

      //
      //
      //
      return d_value;
      
    }

    //
    // Get elapsed wall clock time in seconds for model package evaluation.
    //
    double 
    ModelPackage::getWallClockTime() const
    {

      //
      //
      //
      return d_wallClockTime;

    }

    //
    // Log a timestamp message (for timing HMS operations).
    //
    void 
    ModelPackage::logTimestamp(const std::string & message)
    {
      
      //
      // get time
      //
      const struct timespec currentTime = getTime();
      
      //
      // compute elapsed wall clock time
      //
      const double time = timespecToDouble(currentTime);
      
      //
      // add to timestamps
      //
      d_timestamps.push_back(std::make_pair(time, message));

      //
      //
      //
      return;
      
    }

    //
    // Obtain communicator id for routing of ModelPackage. Remove
    // communicator from list of communicators.
    //
    int 
    ModelPackage::popCommunicator()
    {

      //
      // get last inserted communicator
      //
      const int communicatorId = d_communicatorIds.back();

      //
      // pop last inserted communicator
      //
      d_communicatorIds.pop_back();

      //
      //
      //
      return communicatorId;

    }

    //
    // Erase previous timestamp
    //
    void 
    ModelPackage::popTimestamp()
    {

      //
      // 
      //
      d_timestamps.pop_back();

      //
      //
      //
      return;
      
    }

    //
    // Set elapsed wall clock time in seconds for model package evaluation.
    //
    void 
    ModelPackage::setWallClockTime(double wallClockTime)
    {

      //
      //
      //
      d_wallClockTime = wallClockTime;
        
      //
      //
      //
      return;

    }

    //
    // Set hint for adaptive sampling.
    //
    void 
    ModelPackage::setHint(int hint)
    {

      //
      // store hint
      //
      d_hint = hint;

      //
      //
      //
      return;

    }

    //
    // Set id.
    //
    void 
    ModelPackage::setId(ModelPackage::IdType id)
    {

      //
      // store id
      //
      d_id = id;
      
      //
      //
      //
      return;

    }

    //
    // Set number of evaluations.
    //
    void 
    ModelPackage::setNumberEvaluations(int numberEvaluations)
    {
      
      //
      // store numberEvaluations
      //
      d_numberEvaluations = numberEvaluations;

      //
      //
      //
      return;

    }

    //
    // Set priority for evaluation.
    //
    void 
    ModelPackage::setPriority(const Priority & priority)
    {

      //
      // store priority
      //
      d_priority = priority;

      //
      //
      //
      return;

    }

    //
    // Set value.
    //
    void 
    ModelPackage::setValue(const ValuePointer & value)
    {

      //
      // store value
      //
      d_value = value;
      
      //
      //
      //
      return;

    }
            
  }

}
