//
// File:      BrokerLauncher.h
// Package    broker
//
// HMS Brokers
//
#if !defined(broker_BrokerLauncher_h)
#define broker_BrokerLauncher_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <comm/Launcher.h>

#if defined(HAVE_IOSFWD)
#include <iosfwd>
#else
#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iosfwd or iostream header file not available
#endif // HAVE_IOSTREAM
#endif // HAVE_IOSFWD

#include <string>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class that launches and forms communicator to a Broker.
     */
    class BrokerLauncher : public Launcher {

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
      BrokerLauncher();
 
      /**
       * Destructor.
       */
      virtual ~BrokerLauncher();

      /**
       * Launch and form a communicator to an HMS broker.
       *
       * @param configFile Configuration file.
       * @param rank Rank of caller (algorithm).
       * @param size Size of caller (algorithm).
       */
      std::vector<CommunicatorPointer> 
      launch(std::istream & configFileStream,
             int rank,
             int size);

      /**
       * Launch and form a communicator to an HMS broker.
       *
       * @param configFilePath Path to configuration file.
       * @param rank Rank of caller (algorithm).
       * @param size Size of caller (algorithm).
       */
      std::vector<CommunicatorPointer> 
      launch(const std::string & configFilePath,
             int rank,
             int size);
                     
    private:
      //
      // copy constructor/assignment operator
      //
      BrokerLauncher(const BrokerLauncher &); // not implemented
      BrokerLauncher & operator=(const BrokerLauncher &); // not implemented

      //
      // data
      //
    private: 

    };

  }
}

#endif // comm_BrokerLauncher_h
