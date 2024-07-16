//
// File:      SSHTunnel.cc
// Package    comm
//
// Package for handling communication between individual components.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "SSHTunnel.h"

#include <exception/OSError.h>

#if defined(ENABLE_LOGGING)
#if defined(HAVE_BOOST_LOG)
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#else
#error boost logging not available
#endif // HAVE_BOOST_LOG
#endif // ENABLE_LOGGING

#if defined(HAVE_CERRNO)
#include <cerrno>
#else
#error cerrno header file not available
#endif // HAVE_CERRNO

#if defined(HAVE_CSTRING)
#include <cstring>
#else
#error cstring header file not available
#endif // HAVE_CSTRING

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

#if defined(HAVE_SYS_WAIT_H)
#include <sys/wait.h>
#else
#error sys/wait.h header file not available
#endif // HAVE_SYS_WAIT_H

#if defined(HAVE_VECTOR)
#include <vector>
#else
#error vector header file not available
#endif // HAVE_VECTOR

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#else
#error unistd.h header file not available
#endif // HAVE_UNISTD_H

namespace arl {
  namespace hms {

    //
    // local data
    //
    namespace {

      //
      // TODO: need to detect this somehow - but this may not be
      // executed on the same machine as configure
      //
      int localPort = 10000;
      const std::string sleepTime = "1";

    }

    //
    // Constructor.
    //
    SSHTunnel::SSHTunnel(const std::string & sshPath) :
      d_sshPath(sshPath)
    {
      
      //
      //
      //
      return;

    }
 
    //
    // Destructor.
    //
    SSHTunnel::~SSHTunnel()
    {
      
      //
      //
      //
      return;

    }

    //
    // Create an SSH tunnel.
    //
    std::string 
    SSHTunnel::create(const std::string & remoteHost,
                      const std::string & remotePort) const
    {
      
      //
      // continue trying to form tunnel
      //
      while(true) {
        
        //
        // create arguments for ssh tunnel
        //
        std::stringstream portForward;
        portForward << localPort << ":" << remoteHost << ":" << remotePort;
          
        std::vector<std::string> tunnelArguments;
        tunnelArguments.push_back(d_sshPath);
        tunnelArguments.push_back("-f"); // run ssh in background
        tunnelArguments.push_back("-o"); // fail if unable to bind
        tunnelArguments.push_back("ExitOnForwardFailure=yes"); 
        tunnelArguments.push_back("-L"); // tunnel
        tunnelArguments.push_back(portForward.str()); // tunnel address
        tunnelArguments.push_back(remoteHost);
        tunnelArguments.push_back("sleep");
        tunnelArguments.push_back(sleepTime);
          
#if defined(ENABLE_LOGGING)
        std::stringstream tunnelArgumentsString;
        for(std::vector<std::string>::const_iterator iter = 
              tunnelArguments.begin(); iter != tunnelArguments.end(); 
            ++iter) {
          tunnelArgumentsString << *iter << " ";
        }
        BOOST_LOG_TRIVIAL(info) << "Arguments for ssh tunnel: " 
                                << tunnelArgumentsString.str();
#endif // ENABLE_LOGGING
          
        //
        // convert to c strings
        //
        std::vector<const char *> tunnelArgv;
        for(std::vector<const char *>::size_type i=0; i<tunnelArguments.size(); ++i) {
          tunnelArgv.push_back(tunnelArguments[i].c_str());
        }
        tunnelArgv.push_back(NULL);
          
        //
        // fork child process
        //
        pid_t tunnelProcess = vfork();
          
        //
        // if child, exec ssh tunnel
        //
        if(tunnelProcess == 0) {
          
          if(execv(tunnelArgv[0],
                   const_cast<char * const *>(&(tunnelArgv[0]))) == -1) {
            std::stringstream message;
            message << "Error in execve of ssh tunnel: " 
                    << strerror(errno);
            throw OSError(message.str());
            
          }
          
        }
        else if(tunnelProcess == -1) {
          
          //
          // error forking, throw an exception.
          //
          std::stringstream message;
          message << "Error during fork() of ssh tunnel: " 
                  << strerror(errno);
          throw OSError(message.str());
          
        }
                        
        //
        // wait for ssh tunnel to complete
        //
        int tunnelStatus;
        if(waitpid(tunnelProcess,
                   &tunnelStatus,
                   0) == -1) {
          std::stringstream message;
          message << "Error on waitpid of ssh tunnel: " 
                  << strerror(errno);
          throw OSError(message.str());
        };
            
        //
        // check if exit status is successful
        //
        if(WIFEXITED(tunnelStatus) && WEXITSTATUS(tunnelStatus) == 0) {
            
          //
          // if tunnel exited normally, then we are done
          //
            
          //
          // modify address to use ssh tunnel
          //
          std::stringstream newAddress;
          newAddress << "0.0.0.0:" << localPort;
          ++localPort;
          return newAddress.str();

        } 
        else {
            
          //
          // tunnel failed - lets try to bind to another port
          //
          ++localPort;
            
        }
          
      }

      //
      //
      //
      return "";

    }

  }
   
}
