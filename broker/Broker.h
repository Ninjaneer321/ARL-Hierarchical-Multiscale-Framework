//
// File:      Broker.h
// Package    broker
//
// HMS Brokers.
//
#if !defined(broker_Broker_h)
#define broker_Broker_h

#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class for broker, which executes cycle of receiving
     * model packages, scheduling them to run, monitoring execution,
     * and sending values back.
     */
    class Broker {

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
      virtual ~Broker() = 0;
  
      /**
       * Execute cycle of receiving model packages, creating commands,
       * scheduling them to run, monitoring their execution, and
       * sending values back.
       */
      virtual void run() = 0;

      //
      // methods
      // 
    protected:

      /**
       * Constructor.
       */
      Broker();

    private:
      //
      // copy constructor/assignment operator
      //
      Broker(const Broker &); // not implemented
      Broker & operator=(const Broker &); // not implemented

      //
      // data
      //
    private:

    };

    HMS_SHARED_PTR(Broker);

  }
}

#endif // broker_Broker_h
