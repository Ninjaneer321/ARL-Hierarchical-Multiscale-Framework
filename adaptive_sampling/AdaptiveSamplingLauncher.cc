//
// File:      AdaptiveSamplingLauncher.cc
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "AdaptiveSamplingLauncher.h"

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
    AdaptiveSamplingLauncher::AdaptiveSamplingLauncher()
    {
      
      //
      //
      //
      return;

    }
 
    //
    // Destructor.
    //
    AdaptiveSamplingLauncher::~AdaptiveSamplingLauncher()
    {
      
      //
      //
      //
      return;

    }

    //
    // Launch and form communicator to an HMS adaptive sampler.
    //
    std::vector<CommunicatorPointer> 
    AdaptiveSamplingLauncher::launch(const std::string & configFilePath,
                                     int                 rank,
                                     int                 size,
                                     const InterpolationDatabaseInputFilterPointer & inputFilter,
                                     const InterpolationDatabaseOutputFilterPointer & outputFilter)
    {

#if defined(ENABLE_LOGGING)
        BOOST_LOG_FUNCTION();
#endif // ENABLE_LOGGING

        //
        // open config file
        //
        std::ifstream configFileStream(configFilePath.c_str());
        if(!configFileStream) {
          std::stringstream message;
          message << "Cannot open configuration file: " << configFilePath;
          throw IOError(message.str());
        }
        
        //
        // read contents of config file
        //
        std::istreambuf_iterator<char> eos;
        const std::string 
          configFile(std::istreambuf_iterator<char>(configFileStream), 
                     eos);

        //
        // launch adaptive sampling
        //
        std::vector<CommunicatorPointer> communicators = 
          Launcher::launchModule(configFile,
                                 rank,
                                 size,
                                 "AdaptiveSampling.StartAdaptiveSampling",
                                 "AdaptiveSampling.Hosts",
                                 "AdaptiveSampling");

        //
        // send input and output filters to adaptive sampling
        //
        for(std::vector<CommunicatorPointer>::const_iterator iter = 
              communicators.begin(); iter != communicators.end(); ++iter) {
          
          //
          // get handle to communicator
          //
          const CommunicatorPointer & communicator = *iter;

          //
          // send input filter
          //
          communicator->send(inputFilter);
          
          //
          // receive ack
          //
          std::vector<int> ack;
          while(ack.size() == 0) {
            ack = communicator->receive<int>();
          }

          //
          // send output filter
          //
          communicator->send(outputFilter);

          //
          // receive ack
          // 
          ack.clear();
          while(ack.size() == 0) {
            ack = communicator->receive<int>();
          }

        }
        
        //
        //
        //
        return communicators;

    }
   
  }
}
