//
// File:      CleanupPolicyCustom.h
// Package    broker
//
// HMS Brokers.
//
#if !defined(broker_CleanupPolicyCustom_h)
#define broker_CleanupPolicyCustom_h

#include "CleanupPolicy.h"

#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief A cleanup policy which lets the user customize whether
     * directories are deleted by setting a flag on the returned value.
     */
    class CleanupPolicyCustom : public CleanupPolicy {

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
      CleanupPolicyCustom();

      /**
       * Destructor.
       */
      virtual ~CleanupPolicyCustom();

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
      CleanupPolicyCustom(const CleanupPolicyCustom &); // not implemented
      CleanupPolicyCustom & operator=(const CleanupPolicyCustom &); // not implemented

      //
      // data
      //
    private:
      
    };

  }
}

#endif // broker_CleanupPolicyCustom_h
