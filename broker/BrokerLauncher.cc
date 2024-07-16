//
// File:      BrokerLauncher.cc
// Package    broker
//
// HMS Brokers
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "BrokerLauncher.h"

#include <exception/IOError.h>

#if defined(ENABLE_LOGGING)
#if defined(HAVE_BOOST_LOG)
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#else
#error boost logging not available
#endif // HAVE_BOOST_LOG
#endif // ENABLE_LOGGING

#if defined(HAVE_FSTREAM)
#include <fstream>
#else
#error fstream header file not available
#endif // HAVE_FSTREAM

#if defined(HAVE_ITERATOR)
#include <iterator>
#else
#error iterator header file not available
#endif // HAVE_ITERATOR

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    BrokerLauncher::BrokerLauncher()
    {
      
      //
      //
      //
      return;

    }
 
    //
    // Destructor.
    //
    BrokerLauncher::~BrokerLauncher()
    {
      
      //
      //
      //
      return;

    }

    //
    // Launch and form a communicator to an HMS broker.
    //
    std::vector<CommunicatorPointer> 
    BrokerLauncher::launch(std::istream & configFileStream,
                           int rank,
                           int size)
    {
      
      //
      // read contents of config file
      //
      std::istreambuf_iterator<char> eos;
      const std::string 
        configFile(std::istreambuf_iterator<char>(configFileStream), 
                   eos);
      
      //
      //
      //
      return Launcher::launchModule(configFile,
                                    rank,
                                    size,
                                    "Broker.StartBrokerPath",
                                    "Broker.Hosts",
                                    "Broker");
      
    }

    //
    // Launch and form communicator to an HMS broker.
    //
    std::vector<CommunicatorPointer> 
    BrokerLauncher::launch(const std::string & configFilePath,
                           int                 rank,
                           int                 size)
    {

#if defined(ENABLE_LOGGING)
        BOOST_LOG_FUNCTION();
#endif // ENABLE_LOGGING

        std::ifstream configFileStream(configFilePath.c_str());
        if(!configFileStream) {
          std::stringstream message;
          message << "Cannot open configuration file: " << configFilePath;
          throw IOError(message.str());
        }

        //
        //
        //
        return BrokerLauncher::launch(configFileStream,
                                      rank,
                                      size);

    }
   
  }
}
