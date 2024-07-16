//
// File:      StateDB.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_StateDB_h)
#define base_StateDB_h

#include "Command.h"
#include "Queue.h"

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing state of queued, running, and
     * completed commands.
     */
    class StateDB {

      //
      // types
      //
    public:
      
      typedef arl::hms::Queue<CommandPointer> Queue;

      //
      // methods
      //
    public:

      /**
       * Constructor.
       */
      StateDB();

      /**
       * Destructor.
       */
      ~StateDB();
  
      /**
       * Get queued commands.
       *
       * @return Queue of queued commands.
       */
      Queue & getQueued();

      /**
       * Get queued commands.
       *
       * @return Queue of queued commands.
       */
      const Queue & getQueued() const;

      /**
       * Get running commands.
       *
       * @return Queue of running commands.
       */
      Queue & getRunning();

      /**
       * Get running commands.
       *
       * @return Queue of running commands.
       */
      const Queue & getRunning() const;

      /**
       * Get completed commands.
       *
       * @return Queue of completed commands.
       */
      Queue & getCompleted();

      /**
       * Get completed commands.
       *
       * @return Queue of completed commands.
       */
      const Queue & getCompleted() const;

      //
      // methods
      // 
    protected:

 
    private:
      //
      // copy constructor/assignment operator
      //
      StateDB(const StateDB &); // not implemented
      StateDB & operator=(const StateDB &); // not implemented

      //
      // data
      //
    private:

      Queue d_queued;
      Queue d_running;
      Queue d_completed;
      
    };

    /**
     * Output operator.
     *
     * @param outputStream Output stream to use for output.
     * @param stateDB StateDB object.
     *
     * @return Updated output stream.
     */
    std::ostream & operator<<(std::ostream  & outputStream,
                              const StateDB & stateDB);


  }
}

#endif // base_StateDB_h
