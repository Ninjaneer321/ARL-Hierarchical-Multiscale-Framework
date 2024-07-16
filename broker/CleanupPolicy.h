//
// File:      CleanupPolicy.h
// Package    broker
//
// HMS Brokers.
//
#if !defined(broker_CleanupPolicy_h)
#define broker_CleanupPolicy_h

#include <base/Command.h>

#include <utils/HMSMacros.h>

#include <vector>

//
//
//

namespace arl {
  namespace hms {
    
    /**
     * @brief Base class for cleanup policies to decide when to remove
     * completed command evaluations from the filesystem.
     */
    class CleanupPolicy {

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
      virtual ~CleanupPolicy() = 0;
  
      /**
       * Apply policy to cleanup command directories.
       *
       * @param completedCommands Vector of commands which have
       * completed evaluation.
       */
      virtual void
      apply(const std::vector<CommandPointer> & completedCommands) = 0;
      
      //
      // methods
      // 
    protected:

      /**
       * Constructor.
       */
      CleanupPolicy();

      /**
       * Helper function to remove a directory.
       *
       * @param directory Path of directory to remove.
       */
      void removeDirectory(const std::string & directory) const;

    private:
      //
      // copy constructor/assignment operator
      //
      CleanupPolicy(const CleanupPolicy &); // not implemented
      CleanupPolicy & operator=(const CleanupPolicy &); // not implemented

      //
      // data
      //
    private:

    };

    HMS_SHARED_PTR(CleanupPolicy);

  }
}

#endif // broker_CleanupPolicy_h
