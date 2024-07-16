//
// File:      BrokerFactory.h
// Package    broker
//
// HMS Brokers.
//
#if !defined(broker_BrokerFactory_h)
#define broker_BrokerFactory_h

#include "Broker.h"

#include <string>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing broker factory - builds a broker from
     * a configuration file.
     */
    class BrokerFactory {

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
      BrokerFactory();
 
      /**
       * Destructor.
       */
      ~BrokerFactory();

      /**
       * Build a broker given a configuration file.
       *
       * @param configFile Configuration file. 
       * @param brokerProcessorId Id of this broker, must be between 0
       * and number of brokers.
       * @param communicatorRanks Client ranks broker communicates
       * with - the size of this vector is the number of communicators
       * created to the client for this broker.
       */
      BrokerPointer build(const std::string      & configFile,
                          unsigned int             brokerProcessorId,
                          const std::vector<int> & communicatorRanks);
                     
    private:
      //
      // copy constructor/assignment operator
      //
      BrokerFactory(const BrokerFactory &); // not implemented
      BrokerFactory & operator=(const BrokerFactory &); // not implemented

      //
      // data
      //
    private: 

    };

  }
}

#endif // broker_BrokerFactory_h
