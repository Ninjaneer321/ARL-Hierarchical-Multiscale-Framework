//
// File:      Launcher.h
// Package    comm
//
// Package for handling communication between individual components.
//
#if !defined(comm_Launcher_h)
#define comm_Launcher_h

#include "Communicator.h"

#include <string>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class that launches and forms communicator to an
     * independently executing HMS module.
     */
    class Launcher {

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
      virtual ~Launcher() = 0;

    protected:

      /**
       * Constructor.
       */
      Launcher();
 
      /**
       * Launch and form a communicator to a HMS module.
       *
       * @param configFile String containing contents of configuration file.
       * @param rank Rank of caller (algorithm).
       * @param size Size of caller (algorithm).
       * @param startModulePathKeyword Keyword in configuration file
       * giving path to executable of module to launch.
       * @param hostsKeyword Keyword in configuration file giving
       * hosts where module should be launched.
       * @param moduleName Name of module being launched.
       */
      std::vector<CommunicatorPointer> 
      launchModule(const std::string & configFile,
                   int                 rank,
                   int                 size,
                   const std::string & startModulePathKeyword,
                   const std::string & hostsKeyword,
                   const std::string & moduleName);
                     
    private:
      //
      // copy constructor/assignment operator
      //
      Launcher(const Launcher &); // not implemented
      Launcher & operator=(const Launcher &); // not implemented

      //
      // data
      //
    private: 

    };

  }
}

#endif // comm_Launcher_h
