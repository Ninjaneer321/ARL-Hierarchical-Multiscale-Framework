//
// File:      EndPredicate.h
// Package    base/predicate
//
// Predicates for HMS model evaluations
//
#if !defined(predicate_EndPredicate_h)
#define predicate_EndPredicate_h

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
     * @brief Base class implementing EndPredicate - evaluates
     * whether a Command output satisfies a certain condition.
     */
    class EndPredicate {

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
      virtual ~EndPredicate() = 0;
  
      /**
       * Evaluate predicate.
       *
       * @param command The command used to evaluate the predicate.
       *
       * @return True if command completed successfully, False otherwise.
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
      EndPredicate();

    private:
      //
      // copy constructor/assignment operator
      //
      EndPredicate(const EndPredicate &); // not implemented
      EndPredicate & operator=(const EndPredicate &); // not implemented

      //
      // data
      //
    private:

    };

    HMS_SHARED_PTR(EndPredicate)

  }
}

#include "EndPredicate_t.cc"

#endif // predicate_EndPredicate_h
