//
// File:      Launcher.cc
// Package    comm
//
// Package for handling communication between individual components.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#if defined(HAVE_MPI)
#include <comm/MPICommunicator.h>
#endif // HAVE_MPI

#include "Launcher.h"

#include <comm/SSHTunnel.h>
#include <comm/SocketCommunicator.h>

#include <exception/IOError.h>
#include <exception/OSError.h>

#include <utils/HMSPaths.h>

#if defined(ENABLE_LOGGING)
#if defined(HAVE_BOOST_LOG)
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#else
#error boost logging not available
#endif // HAVE_BOOST_LOG
#endif // ENABLE_LOGGING

#if defined(HAVE_BOOST)
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#else
#error boost not available
#endif // HAVE_BOOST

#if defined(HAVE_CERRNO)
#include <cerrno>
#else
#error cerrno header file not available
#endif // HAVE_CERRNO

#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iostream header file not available
#endif // HAVE_IOSTREAM

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#else
#error unistd.h header file not available
#endif // HAVE_UNISTD_H

extern char ** environ;

namespace arl {
  namespace hms {

    //
    // local functions
    //
    namespace {

      //
      // split a string by whitespace
      //
      std::vector<std::string>
      splitString(const std::string & string) 
      {
        
        //
        // instantiate returnValue
        //
        std::vector<std::string> returnValue;

        //
        // build stream and use stream to split strings
        //
        std::istringstream stringStream(string);
        std::string splitString;
        while(stringStream >> splitString) {
          returnValue.push_back(splitString);
        }

        //
        //
        //
        return returnValue;

      }

      //
      // generate launch arguments - for each launched module, convert
      // clientCommunicatorRanks to argument strings
      //
      std::vector<std::string>
      generateModuleArguments(const std::string      & startModulePath,
                              int                      moduleId,
                              const std::vector<int> & clientRanks)
      
      {

        //
        // instantiate return value
        //
        std::vector<std::string> moduleArguments;

        //
        // first argument is executable name
        //
        moduleArguments.push_back(startModulePath);
     
        //
        // generate moduleIdString
        //
        std::stringstream moduleIdString;
        moduleIdString << moduleId;
        
        moduleArguments.push_back(moduleIdString.str());
                
        //
        // build string of client ranks and add to return value
        //
        if(clientRanks.size() > 0) {
          std::stringstream clientRanksString;
          for(std::vector<int>::size_type j=0; j<clientRanks.size() - 1; ++j) {
            clientRanksString << clientRanks[j] << ",";
          }
          clientRanksString << clientRanks.back();
          moduleArguments.push_back(clientRanksString.str());
        }

        //
        //
        //
        return moduleArguments;
        
      }

      //
      // generates ranks that launched modules (broker or adaptive
      // sampling) use to communicate to upper scale.
      //
      std::vector<std::vector<int> >
      generateCommunicatorRanks(const std::vector<std::string> & hostsVector,
                                int numberLaunchingProcessors)
      {

        //
        // instantiate return value of clientRanks
        //
        std::vector<std::vector<int> > clientRanks;
        
        //
        // size of hosts vector is number of servers that are to be
        // launched
        // 
        const int numberServerProcessors = hostsVector.size();
      
        //
        // number of communicators needed in total is equal to the max
        // of the number of client (algorithm or adaptive sampling)
        // processors and number of server processors.
        //
        const int totalNumberConnections = std::max(numberLaunchingProcessors,
                                                    numberServerProcessors);

        //
        // for now ensure that connections are divided equally - we can
        // fix later for uneven splittings, but uneven splitting is most
        // likely a mistake for how we plan to setup calculations
        // initially
        //
        if(totalNumberConnections % numberLaunchingProcessors != 0 ||
           totalNumberConnections % numberServerProcessors != 0) {
          const std::string message("Cannot assign connections between "
                                    "server and client processors equally - "
                                    "uneven assignment of client processors to "
                                    "servers is currently unsupported.");
          throw IOError(message);
        }

        //
        // now compute the number of connections per server and client
        //
        const int numberConnectionsPerServer =
          totalNumberConnections / numberServerProcessors;
        const int numberConnectionsPerLauncher =
          totalNumberConnections / numberLaunchingProcessors;
      
        //
        // generate ranks that servers will use to communicate to
        // client
        //
        for(int i=0; i<numberServerProcessors; ++i) {

          //
          // ranks for each server
          //
          std::vector<int> ranks;
          
          for(int j=0; j<numberConnectionsPerServer; ++j) {

            //
            // compute rank server uses to communicate to client
            //
            const int rank = numberConnectionsPerServer *
              i / numberConnectionsPerLauncher + j;
          
            //
            // add rank to list of ranks
            //
            ranks.push_back(rank);

          }
          
          //
          // add ranks to list of ranks for each launched module
          //
          clientRanks.push_back(ranks);
          
        }

        //
        //
        //
        return clientRanks;
        
      }
      
      //
      // convert a communicator type to appropriate socket type
      //
      SocketCommunicator::SocketType
      generateSocketType(const std::string & communicatorType)
      {

        //
        // convert communicatorType to appropriate socket type
        //
        if(communicatorType.compare("UnixSocket") == 0) {
          return SocketCommunicator::UNIX;
        }
        else if(communicatorType.compare("TCPIPSocket") == 0) {
          return SocketCommunicator::TCPIP;
        }
        else {
          const std::string message("Communicator type does not correspond to "
                                    "valid socket type.");
          throw IOError(message);
        }

        //
        // unreachable
        //
        return SocketCommunicator::UNIX;

      }

      //
      // read address from pipe
      //
      std::string
      readAddress(int pipe)
      {
        
        const std::string addressLine = "Address: ";
        
        std::stringstream allOutput;

        std::stringstream childOutput;
        char ch;
        while(read(pipe, &ch, 1) == 1) {
          childOutput << ch;
          if(strncmp(&ch, "\n", 1) == 0) {
            const std::string line = childOutput.str();
            if(line.size() > addressLine.size()) {
              if(line.substr(0, addressLine.size()).compare(addressLine) == 0) {
                return line.substr(addressLine.size(),
                                   line.size() - addressLine.size() - 1);
              }
            }
            allOutput << childOutput.str();
            childOutput.str(std::string());
          }
        }

        //
        // error reading address from launched module, throw an exception
        //
        std::stringstream message;
        message << "Cannot read address from launched module, received: " 
                << allOutput.str();
        throw OSError(message.str());

        //
        //
        //
        return "";

      }

      //
      // launch module via fork and exec
      //
      int
      launch(const std::string      & startModulePath,
             const std::string      & configFile,
             const std::string      & host,
             int                      moduleId,
             const std::vector<int> & clientRanks,
             bool                     isRemoteSpawn)
      {

#if defined(ENABLE_LOGGING)
        BOOST_LOG_FUNCTION();
#endif // ENABLE_LOGGING

        int parentToChildPipe[2];
        
        if(pipe(parentToChildPipe) == -1) {
          std::stringstream message;
          message << "Error in creating pipe to module: " << strerror(errno);
          throw OSError(message.str());
        }

        int childToParentPipe[2];
        
        if(pipe(childToParentPipe) == -1) {
          std::stringstream message;
          message << "Error in creating pipe to module: " << strerror(errno);
          throw OSError(message.str());
        }

        //
        // generate arguments for exec
        //
        std::vector<std::string> execArguments = 
          generateModuleArguments(startModulePath,
                                  moduleId,
                                  clientRanks);

        //
        // check whether we are launching remotely - if so we need to
        // ssh to remote node
        //
        if(isRemoteSpawn) {
          execArguments.insert(execArguments.begin(),
                               host);
          execArguments.insert(execArguments.begin(),
                               HMS_SSH);
        }
        
#if defined(ENABLE_LOGGING)
        std::stringstream argumentsString;
        for(std::vector<std::string>::const_iterator iter = 
              execArguments.begin(); iter != execArguments.end(); ++iter) {
          argumentsString << *iter << " ";
        }
        BOOST_LOG_TRIVIAL(info) << "Arguments for launching: " 
                                << argumentsString.str();
#endif // ENABLE_LOGGING

        //
        // convert to c strings
        //
        std::vector<const char *> argv;
        for(std::vector<const char *>::size_type i=0; i<execArguments.size(); ++i) {
          argv.push_back(execArguments[i].c_str());
        }
        argv.push_back(NULL);
        
        const std::string mpiEnvPrefix = "PMI_";
        const std::string impiEnvPrefix = "I_MPI";
        const std::string mpichEnvPrefix = "MPICH";
        
        //
        // create environment for exec'd children (since we are forking
        // from an MPI process, we need to remove MPI environmental
        // variables in some cases)
        //
        std::vector<char *> environment;
        char ** environmentVariable = environ;
        while(*environmentVariable != NULL) {
          size_t size = strlen(*environmentVariable);
          if(size > mpiEnvPrefix.size()) {
            if(strncmp(*environmentVariable, 
                       mpiEnvPrefix.c_str(), 
                       mpiEnvPrefix.size()) != 0 && 
               strncmp(*environmentVariable, 
                       impiEnvPrefix.c_str(), 
                       impiEnvPrefix.size()) != 0 &&
               strncmp(*environmentVariable,
                       mpichEnvPrefix.c_str(),
                       mpichEnvPrefix.size()) != 0) {
              environment.push_back(*environmentVariable);
            }
          }
          ++environmentVariable;
        }  
        environment.push_back(NULL);
        
        //
        // fork child process
        //
        pid_t process = fork();

        //
        // if child, exec start module
        //
        if(process == 0) {

          // 
          // close other ends of pipe
          //
          close(childToParentPipe[0]);
          close(parentToChildPipe[1]);

          close(0); // close current stdin
          dup(parentToChildPipe[0]); // make stdin go to read end of pipe

          close(1); // close current stdout
          dup(childToParentPipe[1]); // make stdout go to write end of pipe

          if(execve(argv[0],
                   const_cast<char * const *>(&(argv[0])),
                   &(environment[0])) == -1) {
            std::stringstream message;
            message << "Error in execve of module: " << strerror(errno);
            throw OSError(message.str());
            
          }
         
        }
        else if(process == -1) {
          
          //
          // error forking, throw an exception.
          //
          std::stringstream message;
          message << "Error during fork() of module: " << strerror(errno);
          throw OSError(message.str());
          
        }

        //
        // write config file on pipe
        //
        char data;
        for(size_t i=0; i<configFile.size(); ++i) {
          data = configFile[i];
          while(const int written = write(parentToChildPipe[1],
                                          &data,
                                          1) < 1) {
            if(written == -1) {
              std::stringstream message;
              message << "Error writing config file to launched module: " << 
                strerror(errno);
              throw IOError(message.str());
            }
          }
        }
        
        //
        // close appropriate ends of pipes
        //
        if(close(parentToChildPipe[1]) != 0) {
          std::string message("Error closing write end of pipe to launched "
                              "module.");
          throw OSError(message);
        }
        if(close(parentToChildPipe[0]) != 0) {
          std::string message("Error closing read end of pipe to launched "
                              "module.");
          throw OSError(message);
        }
        if(close(childToParentPipe[1]) != 0) {
          std::string message("Error closing write end of pipe to launched "
                              "module.");
          throw OSError(message);
        }

        //
        //
        //
        return childToParentPipe[0];

      }

    }

    //
    // Constructor.
    //
    Launcher::Launcher()
    {
      
      //
      //
      //
      return;

    }
 
    //
    // Destructor.
    //
    Launcher::~Launcher()
    {
      
      //
      //
      //
      return;

    }

    //
    // Launch and form communicator to a HMS module.
    //
    std::vector<CommunicatorPointer> 
    Launcher::launchModule(const std::string & configFile,
                           int                 rank,
                           int                 size,
                           const std::string & startModulePathKeyword,
                           const std::string & hostsKeyword,
                           const std::string & moduleName)
    {

#if defined(ENABLE_LOGGING)
      BOOST_LOG_FUNCTION();
#endif // ENABLE_LOGGING
      
      //
      // helper class to establish ssh tunnels
      //
      SSHTunnel sshTunnel(HMS_SSH);

      //
      // parse config file
      //
      boost::property_tree::ptree propertyTree;
      std::stringstream configFileStream(configFile);
      boost::property_tree::ini_parser::read_ini(configFileStream, 
                                                 propertyTree);
     
      //
      // read required configuration options
      //
      const std::string communicatorType = 
        propertyTree.get<std::string>("Communicator.Type");
      const std::string hosts = 
        propertyTree.get<std::string>(hostsKeyword);
      const std::string startModulePath = 
        propertyTree.get<std::string>(startModulePathKeyword);
      
      //
      // split hosts into vector
      //
      const std::vector<std::string> hostsVector = splitString(hosts);
      const std::vector<std::string> startModulePathVector = splitString(startModulePath);

      //
      // sanity check
      //
      if(startModulePathVector.size() > 1) {
        if(hostsVector.size() != startModulePathVector.size()) {
	  std::stringstream message;
	  message << "Need a path to " << startModulePathKeyword 
		  << " for each host in config file.";
	  throw IOError(message.str());
	}
      }

      //
      // size of hosts vector is number of servers that are to be
      // launched
      // 
      const int numberServerProcessors = hostsVector.size();
      
      //
      // for each module to launch, generate ranks they will use to
      // communicate with us.
      //
      const std::vector<std::vector<int> > clientCommunicatorRanks = 
        generateCommunicatorRanks(hostsVector,
                                  size);

      //
      // instantiate communicators to client
      //
      std::vector<CommunicatorPointer> communicators; 
      if(communicatorType.compare("UnixSocket") == 0 ||
         communicatorType.compare("TCPIPSocket") == 0) {

        //
        // generate SocketCommunicator::SocketType
        //
        const SocketCommunicator::SocketType socketType = 
          generateSocketType(communicatorType);

        //
        // determine how many modules to launch
        //
        int numberModulesToLaunch = numberServerProcessors / size;

#if defined(ENABLE_LOGGING)
        BOOST_LOG_TRIVIAL(info) << "Launching " << numberModulesToLaunch << " " 
                                <<  moduleName;
#endif // ENABLE_LOGGING

        //
        // check if numberModulesToLaunch is 0, if so we cannot
        // currently do fan-in with sockets, this needs to be fixed
        //
        int ratioModulesToLaunch = 1;
        if(numberModulesToLaunch == 0) {
          
          //
          // determine if this should launch a module
          //
          ratioModulesToLaunch = size / numberServerProcessors;
          if(rank % ratioModulesToLaunch == 0) {
            numberModulesToLaunch = 1;
          }

        }
        
        //
        // launch each module
        //
        std::vector<std::string> remoteAddresses;

        int parentToChildPipe;
        for(std::vector<int>::size_type i=0; i<numberModulesToLaunch; ++i) {
         
          //
          // compute id of launched module
          //
          const int moduleId = (rank / ratioModulesToLaunch) * 
            numberModulesToLaunch + i;

          //
          // get host name
          //
          const std::string & host = hostsVector[moduleId];

          //
          // determine if we are spawning module remotely
          //
          const bool isRemoteSpawn = host.compare("localhost") != 0;

	  //
	  // get path to startModule
	  //
	  std::string hostStartModule = startModulePathVector[0];
	  if(startModulePathVector.size() > 1) {
	    hostStartModule = startModulePathVector[moduleId];
	  }

          //
          // launch module (adaptive sampling or broker)
          //
	  parentToChildPipe = launch(hostStartModule,
                                     configFile,
                                     host,
                                     moduleId,
                                     clientCommunicatorRanks[i],
                                     isRemoteSpawn);
         
          //
          // there is probably a better way to do this - read line by
          // line from child checking for addresses
          //
          const std::string address = readAddress(parentToChildPipe);

          if(ratioModulesToLaunch == 1) {
            if(close(parentToChildPipe) != 0) {
              std::string message("Error closing read end of pipe to launched "
                                  "module.");
              throw OSError(message);
            }
          }

          //
          // save address of launched module
          //
          const std::string remotePort = 
            address.substr(address.find(":") + 1);
          std::stringstream remoteAddress;
          remoteAddress << host << ":" << remotePort;
          remoteAddresses.push_back(remoteAddress.str());
                   
          //
          // if spawning remotely, need to create ssh tunnel to remote
          // host
          //
          std::string tunnelAddress = address;
          if(isRemoteSpawn) {           
            tunnelAddress = 
              sshTunnel.create(host,
                               remotePort);
          }

          //
          // instantiate communicator to module
          //
          CommunicatorPointer 
            communicator(new SocketCommunicator(tunnelAddress,
                                                socketType,
                                                false));
          
          //
          // add communicator to list of communicators
          //
          communicators.push_back(communicator);
          
        }

        //
        // take care of case where we are fanning in to module - we
        // need to communicate address to non-launchers
        //
        if(ratioModulesToLaunch > 1) {
          
          //
          // check whether we can do this over MPI - somehow we need
          // to get info on how to connect from launching process to
          // other processes. we will do it using MPI for now, but
          // there may be other & better ways
          //

#if defined(HAVE_MPI)

          //
          // perform sanity check to ensure launching processes share
          // MPI environment
          //
          int worldSize;
          MPI_Comm_size(MPI_COMM_WORLD,
                        &worldSize);
          if(worldSize != size) {
            const std::string message("Cannot do fan-in with sockets for "
                                      "this case - we cannot communicate "
                                      "connection info between launching "
                                      "processes.");
            throw IOError(message);
          }
          
          if(numberModulesToLaunch == 1) {

            //
            // ensure we launched a module
            //
            assert(remoteAddresses.size() == 1);
            
            //
            // we launched and need to send info to other connectors
            //
            for(int rankToSend = rank + 1; 
                rankToSend < rank + ratioModulesToLaunch; ++rankToSend) {
              
              //
              // read address from pipe
              //
              const std::string & remoteAddress = remoteAddresses[0];
              
              MPI_Send(const_cast<char *>(remoteAddress.c_str()),
                                          remoteAddress.size(),
                                          MPI_CHAR,
                                          rankToSend,
                                          0,
                                          MPI_COMM_WORLD);
            }

            //
            // close pipe
            //
            close(parentToChildPipe);

          }
          else {
            
            //
            // another processor launched and we need to receive info
            // and connect to module
            //
            int rankToReceive = rank - (rank % ratioModulesToLaunch);

            //
            // receive address
            //
            MPI_Status status;
            MPI_Probe(rankToReceive,
                      0,
                      MPI_COMM_WORLD,
                      &status);

            int addressSize;
            MPI_Get_count(&status,
                          MPI_CHAR,
                          &addressSize);

            std::vector<char> recvAddress(addressSize);
            MPI_Recv(&(recvAddress[0]),
                     addressSize,
                     MPI_CHAR,
                     rankToReceive,
                     0,
                     MPI_COMM_WORLD,
                     &status);

            std::string address(recvAddress.begin(),
                                recvAddress.end());
                            
            //
            // compute module id
            //
            const int moduleId = rank / ratioModulesToLaunch;
               
            //
            // get host name
            //
            const std::string & host = hostsVector[moduleId];
            
            //
            // determine if we are spawning module remotely
            //
            const bool isRemoteSpawn = host.compare("localhost") != 0;

            if(isRemoteSpawn) {             
              address = sshTunnel.create(host,
                                         address.substr(address.find(":") + 1));
            }

            //
            // instantiate communicator to module
            //
            CommunicatorPointer 
              communicator(new SocketCommunicator(address,
                                                  socketType,
                                                  false));
          
            //
            // add communicator to list of communicators
            //
            communicators.push_back(communicator);

          }
        
#else

          //
          // HMS not built with MPI - cannot communicate connection
          // info among sockets.
          //
          const std::string message("Cannot do fan-in with sockets when HMS "
                                    "is built without MPI- we cannot "
                                    "communicate connection info between "
                                    "launching processes.");
          throw IOError(message);

#endif // HAVE_MPI

        } 

        /*
        if(workStealing) {

          //
          // collect launched module addresses
          //
          std::stringstream launchedAddresses;
          for(std::vector<std::string>::const_iterator iter = 
                remoteAddresses.begin(); iter != remoteAddresses.end(); 
              ++iter) {
            launchedAddresses << *iter << " ";
          }
          const std::string launchedAddressesString = launchedAddresses.str();
          const int launchedAddressesLength = launchedAddressesString.size();
                
          //
          // gather lengths of strings from each process
          //
          int commSize;
          MPI_Comm_size(MPI_COMM_WORLD, &
                        commSize);
          std::vector<int> launchedAddressesLengths(commSize);
          MPI_Allgather(const_cast<int *>(&launchedAddressesLength),
                        1,
                        MPI_INT,
                        &(launchedAddressesLengths[0]),
                        1,
                        MPI_INT,
                        MPI_COMM_WORLD);
          std::vector<int> displs(commSize + 1);
          displs[0] = 0;
          for(std::vector<int>::size_type i=0; i<commSize; ++i) {
            displs[i+1] = displs[i] + launchedAddressesLengths[i];
          }
          const int totalSize = displs[commSize];
          std::vector<char> buffer(totalSize);        
          
          //
          // collect all addresses
          //
          MPI_Allgatherv(const_cast<char *>(launchedAddressesString.c_str()),
                         launchedAddressesLength,
                         MPI_CHAR,
                         &(buffer[0]),
                         &(launchedAddressesLengths[0]),
                         &(displs[0]),
                         MPI_CHAR,
                         MPI_COMM_WORLD);
          
          //
          // parse recv buffer and fill vector containing all addresses
          //
          std::vector<std::string> allAddresses;
          for(int i=0; i<commSize; ++i) {
            const int displ = displs[i];
            const int length = launchedAddressesLengths[i];
            const std::string data(&(buffer[displ]), length);
            std::stringstream dataStream(data);
            std::string token;
            while(dataStream >> token) {
              allAddresses.push_back(token);
            }
          }
          
          for(std::vector<std::string>::const_iterator iter = 
                allAddresses.begin(); iter != allAddresses.end(); ++iter) {
            std::cout << *iter << std::endl;
          }

          //
          // send all addresses to children so that they can form
          // connections between each other
          //
          for(std::vector<CommunicatorPointer>::const_iterator iter = 
                communicators.begin(); iter != communicators.end(); ++iter) {
            
            const CommunicatorPointer & communicator = *iter;
            communicator->send(allAddresses);
            
            //
            // receive ack
            //
            std::vector<int> ack;
            while(ack.size() == 0) {
              ack = communicator->receive<int>();
            }
            
          }

          }*/
        
      }
      else if(communicatorType.compare("MPI") == 0) {
        
#if defined(HAVE_MPI)

        //
        // generate arguments for each individual module
        //
        std::vector<std::vector<std::string> > spawnArguments;
        for(int i=0; i<numberServerProcessors; ++i) {
          
          std::vector<std::string> spawnArgument = 
            generateModuleArguments(startModulePath,
                                    i, 
                                    clientCommunicatorRanks[i]);
          

#if defined(ENABLE_LOGGING)
          
          if(rank == 0) {
            std::stringstream argumentsString;
            for(std::vector<std::string>::const_iterator iter = 
                  spawnArgument.begin(); iter != spawnArgument.end();
                ++iter) {
              argumentsString << *iter << " ";
            }
            BOOST_LOG_TRIVIAL(info) << "Arguments for launching " 
                                    << moduleName << " "  << i << " : " 
                                    << argumentsString.str();
          }
#endif // ENABLE_LOGGING

          //
          // remove executable name from argument list as
          // MPI_Comm_spawn_multiple needs them separately
          //
          spawnArgument.erase(spawnArgument.begin());

          spawnArguments.push_back(spawnArgument);
          
        }

        //
        // convert to c strings for spawn
        // 
        std::vector<const char *> argv;
        for(int i=0; i<numberServerProcessors; ++i) {

          //
          // get handle to argument
          //
          const std::vector<std::string> & spawnArgument = spawnArguments[i];

          //
          // convert argument to c strings
          //
          for(std::vector<std::string>::size_type j=0; 
              j<spawnArgument.size(); ++j) {
            argv.push_back(spawnArguments[i][j].c_str());
          }
          argv.push_back(NULL);
             
        }

        //
        // build argvs for each command
        //
        std::vector<const char **> argvs;
        int argvOffset = 0;
        for(int i=0; i<numberServerProcessors; ++i) {

          argvs.push_back(&(argv[argvOffset]));
          argvOffset += spawnArguments[i].size() + 1;

        }
        
        //
        // instantiate commands
        //
        std::vector<const char *> commands(numberServerProcessors,
                                           startModulePath.c_str());
        commands.push_back(NULL);
          
        //
        // spawn modules (brokers or adaptive sampling)
        //
        MPI_Comm intercommunicator;

#if defined(ENABLE_LOGGING)        
        if(rank == 0) {
          BOOST_LOG_TRIVIAL(info) << "Launching " << numberServerProcessors 
                                  << " " << moduleName << " via MPI";
        }
#endif // ENABLE_LOGGING

        //
        // we launch one module per host
        //
        std::vector<int> maxProcessors(numberServerProcessors,
                                       1);

        //
        // storage for mpi errors
        //
        int error;

        //
        // build infos and set hosts where modules are spawned
        //
        std::vector<MPI_Info> infos(numberServerProcessors,
                                    MPI_INFO_NULL);

        for(std::vector<MPI_Info>::size_type i=0; i<infos.size(); ++i) {

          //
          // get handle to host 
          //
          const std::string & host = hostsVector[i];
          
          //
          // get info handle
          //
          MPI_Info & info = infos[i];
         
          //
          // create info
          //
          error = MPI_Info_create(&info);
          
          if(error != MPI_SUCCESS) {
            const std::string message("Error creating info for MPI spawn.");
            throw IOError(message);
          }

          //
          // set host on info
          //
          const char * hostKey = "host";
          error = MPI_Info_set(info,
                               const_cast<char *>(hostKey),
                               const_cast<char *>(host.c_str()));

          if(error != MPI_SUCCESS) {
            const std::string message("Error setting host info for MPI spawn.");
            throw IOError(message);
          }
                    
        }
        
        std::vector<int> errorcodes(numberServerProcessors);

        //
        // spawn the modules
        //
        error =
          MPI_Comm_spawn_multiple(numberServerProcessors,
                                  const_cast<char **>(&(commands[0])),
                                  const_cast<char ***>(&(argvs[0])),
                                  &(maxProcessors[0]),
                                  &(infos[0]),
                                  0,
                                  MPI_COMM_WORLD,
                                  &intercommunicator,
                                  &(errorcodes[0]));

        if(error != MPI_SUCCESS) {
          const std::string message("Error launching module via MPI.");
          throw IOError(message);
        }

        //
        // check error codes
        //
        for(std::vector<int>::size_type i=0; i<errorcodes.size(); ++i) {
          
          //
          // get error code
          //
          const int errorCode = errorcodes[i];
        
          //
          // check error code
          //
          if(errorCode != MPI_SUCCESS) {
            std::stringstream message;
            message << "Error spawning " << moduleName << "# " << i << " on "
                    << hostsVector[i];
            throw IOError(message.str());
          }

        }

        //
        // cleanup info
        //
        for(std::vector<MPI_Info>::size_type i=0; i<infos.size(); ++i) {
          
          //
          // get info handle
          //
          MPI_Info & info = infos[i];
          
          //
          // cleanup info
          //
          error = MPI_Info_free(&info);

          if(error != MPI_SUCCESS) {
            const std::string message("Error freeing info for MPI spawn.");
            throw IOError(message);
          }

        }

        //
        // send config file to children over intercommunicator
        //
        if(rank == 0) {
          for(int i=0; i<numberServerProcessors; ++i) {
            error = MPI_Send(const_cast<char *>(configFile.c_str()),
                             configFile.size(),
                             MPI_CHAR,
                             i,
                             0,
                             intercommunicator);
          }
        }

        MPI_Barrier(MPI_COMM_WORLD);
        
        //
        // number of communicators needed in total is equal to the max
        // of the number of client (algorithm or adaptive sampling)
        // processors and number of server processors.
        //
        const int totalNumberConnections = std::max(size,
                                                    numberServerProcessors);
        
        
        //
        // determine how many connections to form per launcher
        //
        const int numberConnectionsPerLauncher = 
          totalNumberConnections / size;
        const int numberConnectionsPerModule =
          totalNumberConnections / numberServerProcessors;
        
        //
        // create communicators to each server
        //
        for(int i=0; i<numberConnectionsPerLauncher; ++i) {
      
          //
          // calculate connectionId
          //
          const int connectionId = 
            rank * numberConnectionsPerLauncher + i;
 
          //
          // convert connectionId to intercommunicatorRank
          //
          const int intercommunicatorRank = 
            connectionId / numberConnectionsPerModule;

          CommunicatorPointer 
            communicator(new MPICommunicator(intercommunicator,
                                             false,
                                             intercommunicatorRank));
       
          communicators.push_back(communicator);
          
        } 

#else

        //
        // cannot create MPI communicator when HMS is not built with
        // MPI
        //
        const std::string message("Cannot create MPI communicator because "
                                  "HMS was not compiled with HMS support - "
                                  "rebuild HMS or choose another communication "
                                  "mechanism");
        throw IOError(message);

#endif // HAVE_MPI

      }
      else {
        //
        // unknown communicator type, throw an exception
        //
        const std::string message("Invalid communicator type in broker "
                                  "configuration file: " + communicatorType);
        throw IOError(message);
      }

      //
      //
      //
      return communicators;

    }
   
  }
}
