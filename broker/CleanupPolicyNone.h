//
// File:      CleanupPolicyNone.h
// Package    broker
//
// HMS Brokers.
//
#if !defined(broker_CleanupPolicyNone_h)
#define broker_CleanupPolicyNone_h

#include "CleanupPolicy.h"

#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief A cleanup policy which keeps all data (no cleanup)
     */
    class CleanupPolicyNone : public CleanupPolicy {

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
      CleanupPolicyNone();

      /**
       * Destructor.
       */
      virtual ~CleanupPolicyNone();

      /**
       * Apply policy to cleanup command directories.
       *
       * @param completedCommands Vector of commands which have
       * completed evaluation.
       */
      virtual void
      apply(const std::vector<CommandPointer> & completedCommands);
      
      //
      // methods
      // 
    protected:

    private:
      //
      // copy constructor/assignment operator
      //
      CleanupPolicyNone(const CleanupPolicyNone &); // not implemented
      CleanupPolicyNone & operator=(const CleanupPolicyNone &); // not implemented

      //
      // data
      //
    private:

    };

  }
}

#endif // broker_CleanupPolicyNone_h
