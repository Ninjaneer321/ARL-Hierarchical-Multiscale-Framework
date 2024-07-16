//
// File:      AdaptiveSamplingLauncher.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm
//
#if !defined(adaptive_sampling_AdaptiveSamplingLauncher_h)
#define adaptive_sampling_AdaptiveSamplingLauncher_h

#include "InterpolationDatabaseInputFilter.h"
#include "InterpolationDatabaseOutputFilter.h"

#include <comm/Launcher.h>

#include <string>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class that launches and forms communicator to adaptive sampling.
     */
    class AdaptiveSamplingLauncher : public Launcher {

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
      AdaptiveSamplingLauncher();
 
      /**
       * Destructor.
       */
      virtual ~AdaptiveSamplingLauncher();

      /**
       * Launch and form a communicator to an HMS adaptive sampler.
       *
       * @param configFilePath Path to configuration file.
       * @param rank Rank of caller (algorithm).
       * @param size Size of caller (algorithm).
       * @param inputFilter Input filter for adaptive sampling database.
       * @param outputFilter Output filter for adaptive sampling database.
       */
      std::vector<CommunicatorPointer> 
      launch(const std::string & configFilePath,
             int rank,
             int size,
             const InterpolationDatabaseInputFilterPointer & inputFilter,
             const InterpolationDatabaseOutputFilterPointer & outputFilter);
                     
    private:
      //
      // copy constructor/assignment operator
      //
      AdaptiveSamplingLauncher(const AdaptiveSamplingLauncher &); // not implemented
      AdaptiveSamplingLauncher & operator=(const AdaptiveSamplingLauncher &); // not implemented

      //
      // data
      //
    private: 

    };

  }
}

#endif // adaptive_sampling_AdaptiveSamplingLauncher_h
