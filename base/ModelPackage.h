//
// File:      ModelPackage.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_ModelPackage_h)
#define base_ModelPackage_h

#include "Argument.h"
#include "BatchQueueSettings.h"
#include "InputFilter.h"
#include "Model.h"
#include "OutputFilter.h"
#include "Priority.h"
#include "Value.h"
#include "predicate/StatusPredicate.h"
#include "predicate/EndPredicate.h"

#include <utils/HMSMacros.h>

#include <string>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing container storing Model, InputFilter,
     * OutputFilter, Argument, Value, and Communicator.
     */
    class ModelPackage {

      //
      // types
      //
    public:

      /**
       * Model package id.
       */
      typedef unsigned long long IdType;

      //
      // methods
      //
    public:

      /**
       * Constructor.
       */
      ModelPackage();

      /**
       * Constructor.
       *
       * @param model Model to store.
       * @param inputFilter InputFilter to store.
       * @param outputFilter OutputFilter to store.
       * @param argument Argument to store.
       */
      ModelPackage(const ModelPointer        & model,
                   const InputFilterPointer  & inputFilter,
                   const OutputFilterPointer & outputFilter,
                   const ArgumentPointer     & argument);

      /**
       * Constructor (enables ModelPackage to be submitted to a batch queue).
       *
       * @param model Model to store.
       * @param inputFilter InputFilter to store.
       * @param outputFilter OutputFilter to store.
       * @param argument Argument to store.
       * @param batchQueueSettings Settings for batch queue submission.
       */
      ModelPackage(const ModelPointer              & model,
                   const InputFilterPointer        & inputFilter,
                   const OutputFilterPointer       & outputFilter,
                   const ArgumentPointer           & argument,
		   const BatchQueueSettingsPointer & batchQueueSettings);

      /**
       * Destructor.
       */
      ~ModelPackage();

      /**
       * Add communicator id for routing of ModelPackage.
       *
       * @param communicatorId Id of communicator for routing of ModelPackage.
       */
      void addCommunicator(int communicatorId);

      /**
       * Add an end predicate for use when evaluating the ModelPackage.
       *
       * @param predicate a EndPredicate to add.
       */
      void addEndPredicate(const EndPredicatePointer & predicate);

      /**
       * Add a status predicate for use when evaluating the ModelPackage.
       *
       * @param predicate a StatusPredicate to add.
       */
      void addStatusPredicate(const StatusPredicatePointer & predicate);
 
      /**
       * Obtain argument.
       *
       * @return Argument stored by the ModelPackage.
       */
      const ArgumentPointer & getArgument() const;

      /**
       * Obtain batch queue settings.
       *
       * @return BatchQueueSettings stored by the ModelPackage.
       */
      const BatchQueueSettingsPointer & getBatchQueueSettings() const;

      /**
       * Get hint for adaptive sampling.
       *
       * @return Hint for adaptive sampling.
       */
      int getHint() const;

      /**
       * Obtain id.
       *
       * @return Id of ModelPackage
       */
      IdType getId() const;

      /**
       * Obtain input filter.
       *
       * @return InputFilter stored by the ModelPackage.
       */
      const InputFilterPointer & getInputFilter() const;

      /**
       * Obtain model.
       *
       * @return Model stored by the ModelPackage.
       */
      const ModelPointer & getModel() const;

      /**
       * Obtain number of evaluations.
       *
       * @return Number of times the ModelPackage has been evaluations.
       */
      int getNumberEvaluations() const;
      
      /**
       * Obtain output filter.
       *
       * @return OutputFilter stored by the ModelPackage.
       */
      const OutputFilterPointer & getOutputFilter() const;

      /**
       * Obtain priority for evaluation.
       *
       * @return Priority
       */
      const Priority & getPriority() const;

      /**
       * Obtain end predicates.
       *
       * @return Vector of end predicates.
       */
      const std::vector<EndPredicatePointer> & getEndPredicates() const;
      
      /**
       * Obtain status predicates.
       *
       * @return Vector of status predicates.
       */
      const std::vector<StatusPredicatePointer> & getStatusPredicates() const;
      
      /**
       * Get timestamp log messages.
       *
       * @return Timestamp log.
       */
      std::string getTimestampLog() const;

      /**
       * Obtain value.
       *
       * @return Value stored by the ModelPackage.
       */
      const ValuePointer & getValue() const;

      /**
       * Get elapsed wall clock time in seconds for model package evaluation.
       *
       * @return Wall clock time elapsed for model package evaluation.
       */
      double getWallClockTime() const;

      /**
       * Log a timestamp message (for timing HMS operations).
       *
       * @param message Log message associated with the timestamp.
       */
      void logTimestamp(const std::string & message);

      /**
       * Obtain communicator id for routing of ModelPackage. Remove
       * communicator from list of communicators.
       *
       * @return communicator id of most recently added communicator.
       */
      int popCommunicator();

      /**
       * Erase previous timestamp
       */
      void popTimestamp();

      /**
       * Serialize model package.
       *
       * @param archive Archive to store serialization.
       * @param version Version of serialization.
       */
      template <class Archive>
      void serialize(Archive & archive, const unsigned int version);

      /**
       * Set elapsed wall clock time in seconds for model package evaluation.
       *
       * @param wallClockTime Wall clock time for evaluating model package.
       */
      void setWallClockTime(double wallClockTime);
 
      /**
       * Set hint for adaptive sampling.
       *
       * @param hint Hint to store for adaptive sampling.
       */
      void setHint(int hint);
      
      /**
       * Set id.
       *
       * @param id Id to set.
       */
      void setId(IdType id);

      /**
       * Set number of evaluations.
       *
       * @param numberEvaluations The number of times the ModelPackage
       * has been evaluated.
       */
      void setNumberEvaluations(int numberEvaluations);

      /**
       * Set priority for evaluation.
       *
       * @param priority Priority to set.
       */
      void setPriority(const Priority & priority);

      /**
       * Set value.
       *
       * @param value Value to store.
       */
      void setValue(const ValuePointer & value);

      //
      // methods
      // 
    protected:

    private:
      //
      // copy constructor/assignment operator
      //
      ModelPackage(const ModelPackage &); // not implemented
      ModelPackage & operator=(const ModelPackage &); // not implemented

      //
      // data
      //
    private:

      ArgumentPointer                              d_argument;
      BatchQueueSettingsPointer                    d_batchQueueSettings;
      std::vector<int>                             d_communicatorIds;
      int                                          d_hint;
      IdType                                       d_id;
      InputFilterPointer                           d_inputFilter;
      ModelPointer                                 d_model;
      int                                          d_numberEvaluations;
      OutputFilterPointer                          d_outputFilter;
      Priority                                     d_priority;
      std::vector<StatusPredicatePointer>          d_statusPredicates;
      std::vector<EndPredicatePointer>             d_endPredicates;
      std::vector<std::pair<double, std::string> > d_timestamps;
      ValuePointer                                 d_value;
      double                                       d_wallClockTime;
      
    };

    HMS_SHARED_PTR(ModelPackage);

  }
}

#include "ModelPackage_t.cc"

#endif // base_ModelPackage_h
