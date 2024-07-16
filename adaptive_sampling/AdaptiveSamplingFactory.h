//
// File:      AdaptiveSamplingFactory.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_AdaptiveSamplingFactory_h)
#define adaptive_sampling_AdaptiveSamplingFactory_h

#include "AdaptiveSampling.h"

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing adaptive sampling factory - build
     * adaptive sampling module from configuration file.
     */
    class AdaptiveSamplingFactory {

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
      AdaptiveSamplingFactory();
 
      /**
       * Destructor.
       */
      ~AdaptiveSamplingFactory();

      /**
       * Build an adaptive sampling module given a configuration file.
       *
       * @param configFile Configuration file.
       * @param adaptiveSamplingId Id of this adaptive sampling
       * module, must be between 0 and number of adaptive sampling
       * modules.
       * @param communicatorRanks Client ranks adaptive sampling
       * module communicates with - the size of this vector is the
       * number of communicators created to the client for this
       * adaptive sampling module.
       */
      AdaptiveSamplingPointer build(const std::string      & configFile,
                                    unsigned int             adaptiveSamplingId,
                                    const std::vector<int> & communicatorRanks);
                     
    private:
      //
      // copy constructor/assignment operator
      //
      AdaptiveSamplingFactory(const AdaptiveSamplingFactory &); // not implemented
      AdaptiveSamplingFactory & operator=(const AdaptiveSamplingFactory &); // not implemented

      //
      // data
      //
    private: 

    };

  }
}

#endif // adaptive_sampling_AdaptiveSamplingFactory_h
