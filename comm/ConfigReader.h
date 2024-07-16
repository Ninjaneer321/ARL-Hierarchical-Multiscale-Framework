//
// File:      ConfigReader.h
// Package    comm
//
// Package for handling communication between individual components.
//
#if !defined(comm_ConfigReader_h)
#define comm_ConfigReader_h

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class that reads config file from parent. 
     *
     * Socket - Config file is sent over a pipe.
     * MPI - Config file is sent using MPI communicator.
     */
    class ConfigReader {

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
      ConfigReader();

      /**
       * Destructor.
       */
      ~ConfigReader();
      
      /**
       * Read config file from parent.
       *
       * @return String containing contents of config file
       */
      std::string 
      read() const;

    private:
      //
      // copy constructor/assignment operator
      //
      ConfigReader(const ConfigReader &); // not implemented
      ConfigReader & operator=(const ConfigReader &); // not implemented

      //
      // data
      //
    private: 

    };

  }
}

#endif // comm_ConfigReader_h
