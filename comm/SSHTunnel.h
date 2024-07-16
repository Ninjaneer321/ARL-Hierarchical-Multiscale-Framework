//
// File:      SSHTunnel.h
// Package    comm
//
// Package for handling communication between individual components.
//
#if !defined(comm_SSHTunnel_h)
#define comm_SSHTunnel_h

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class that establishes an SSH tunnel.
     */
    class SSHTunnel {

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
       *
       * @param Path to ssh command used to create the tunnel
       */
      SSHTunnel(const std::string & sshPath);

      /**
       * Destructor.
       */
      ~SSHTunnel();
      
      /**
       * Create an SSH tunnel.
       *
       * @param remoteHost Remote host we are tunneling to.
       * @param remotePort Remote port we are tunneling to.
       *
       * @return Local address of tunneled connection.
       */
      std::string 
      create(const std::string & remoteHost,
             const std::string & remotePort) const;

    private:
      //
      // copy constructor/assignment operator
      //
      SSHTunnel(const SSHTunnel &); // not implemented
      SSHTunnel & operator=(const SSHTunnel &); // not implemented

      //
      // data
      //
    private: 

      const std::string d_sshPath;

    };

  }
}

#endif // comm_SSHTunnel_h
