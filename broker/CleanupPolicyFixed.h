//
// File:      CleanupPolicyFixed.h
// Package    broker
//
// HMS Brokers.
//
#if !defined(broker_CleanupPolicyFixed_h)
#define broker_CleanupPolicyFixed_h

#include "CleanupPolicy.h"

#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief A cleanup policy which keeps a fixed number of most recently
     * completed evaluations.
     */
    class CleanupPolicyFixed : public CleanupPolicy {

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
       *
       * @param numberToKeep The number of most recent evaluation
       * directories to keep. Once this number of directories is
       * reached, the oldest directories are removed to keep the
       * number of stored directories less than or equal to
       * numberToKeep.
       */
      CleanupPolicyFixed(int numberToKeep);

      /**
       * Destructor.
       */
      virtual ~CleanupPolicyFixed();

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
      CleanupPolicyFixed(const CleanupPolicyFixed &); // not implemented
      CleanupPolicyFixed & operator=(const CleanupPolicyFixed &); // not implemented

      //
      // data
      //
    private:

      const int d_numberToKeep;
      std::vector<std::string> d_completedDirectories;
      
    };

  }
}

#endif // broker_CleanupPolicyFixed_h
