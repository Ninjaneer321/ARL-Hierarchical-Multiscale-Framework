//
// File:      StatusPredicate.h
// Package    base/predicate
//
// Predicates for HMS model evaluations
//
#if !defined(predicate_StatusPredicate_h)
#define predicate_StatusPredicate_h

#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    //
    // Forward declaration
    //
    class Command;
    
    /**
     * @brief Base class implementing StatusPredicate - evaluates
     * whether a Command output satisfies a certain condition.
     */
    class StatusPredicate {

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
      virtual ~StatusPredicate() = 0;
  
      /**
       * Evaluate predicate.
       *
       * @param command The command used to evaluate the predicate.
       *
       * @return True if command is still running, False otherwise.
       */
      virtual bool evaluate(const Command & command) = 0;

      /**
       * Serialize predicate.
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

      /**
       * Constructor.
       */
      StatusPredicate();

    private:
      //
      // copy constructor/assignment operator
      //
      StatusPredicate(const StatusPredicate &); // not implemented
      StatusPredicate & operator=(const StatusPredicate &); // not implemented

      //
      // data
      //
    private:

    };

    HMS_SHARED_PTR(StatusPredicate);

  }
}

#include "StatusPredicate_t.cc"

#endif // predicate_StatusPredicate_h
