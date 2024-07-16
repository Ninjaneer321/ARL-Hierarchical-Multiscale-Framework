//
// File:      TimeoutStatusPredicate.h
// Package    base/predicate
//
// Predicates for HMS commands.
//
#if !defined(predicate_TimeoutStatusPredicate_h)
#define predicate_TimeoutStatusPredicate_h

#include "StatusPredicate.h"

#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {
    
    /**
     * Predicate determining whether the Command has exceeded a
     * maximum runtime (wall-clock).
     */
    class TimeoutStatusPredicate : public StatusPredicate {

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
      TimeoutStatusPredicate();
      
      /**
       * Constructor.
       *
       * @param maximumWallClockTime The maximum wall clock time
       * allotted to the command. If the command exceeds the maximum
       * wall clock time, False is returned by the predicate, telling
       * Monitor to stop evaluation of the Command.
       */
      TimeoutStatusPredicate(int maximumWallClockTime);
      
      /**
       * Destructor.
       */
      virtual ~TimeoutStatusPredicate();
  
      /**
       * Evaluate predicate.
       *
       * @param command The command used to evaluate the predicate.
       *
       * @return True if command is still running, False otherwise.
       */
      virtual bool evaluate(const Command & command);

      /**
       * Serialize.
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
      TimeoutStatusPredicate(const TimeoutStatusPredicate &); // not implemented
      TimeoutStatusPredicate & operator=(const TimeoutStatusPredicate &); // not implemented

      //
      // data
      //
    private:

      int d_maximumWallClockTime;
      
    };

    HMS_SHARED_PTR(TimeoutStatusPredicate);

  }
}

#include "TimeoutStatusPredicate_t.cc"

#endif // predicate_TimeoutStatusPredicate_h
