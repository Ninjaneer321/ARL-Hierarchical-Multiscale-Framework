//
// File:      SocketCommunicator.cc
// Package    comm
//
// Package for handling communication between individual components.
//
//

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "SocketCommunicator.h"

#include <exception/ConnectionTerminationException.h>
#include <exception/NetworkError.h>
#include <exception/OSError.h>

#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#else
#error pthread.h header file not available
#endif // HAVE_PTHREAD_H

#if defined(HAVE_ARPA_INET_H)
#include <arpa/inet.h>
#else
#error arpa/inet.h header file not available
#endif // HAVE_ARPA_INET_H

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

#if defined(HAVE_CSTDIO)
#include <cstdio>
#else
#error cstdio header file not available
#endif // HAVE_CSTDIO

#if defined(HAVE_CSTDLIB)
#include <cstdlib>
#else
#error cstdlib header file not available
#endif // HAVE_CSTDLIB

#if defined(HAVE_CSTRING)
#include <cstring>
#else
#error cstring header file not available
#endif // HAVE_CSTRING

#if defined(HAVE_NETDB_H)
#include <netdb.h>
#else
#error netdb.h header file not available
#endif // HAVE_NETDB_H

#if defined(HAVE_NETINET_IN_H)
#include <netinet/in.h>
#else
#error netinet/in.h header file not available
#endif // HAVE_NETINET_IN_H

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

#if defined(HAVE_SYS_SOCKET_H)
#include <sys/socket.h>
#else
#error sys/socket.h header file not available
#endif // HAVE_SYS_SOCKET_H

#if defined(HAVE_SYS_UN_H)
#include <sys/un.h>
#else
#error sys/un.h header file not available
#endif // HAVE_SYS_UN_H

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#else
#error unistd.h header file not available
#endif // HAVE_UNISTD_H

//
//
//

namespace arl {
  namespace hms {

    //
    // types
    //
    struct SocketCommunicator::SocketCommunicatorData {
      pthread_t         d_threadId;
      std::vector<char> d_bucket;
      pthread_mutex_t   d_bucketLock;
      bool              d_isConnected;
      pthread_mutex_t   d_isConnectedLock;
      pthread_mutex_t   d_sendLock;
      int               d_socket;
      bool              d_isServer;
      int               d_serverSocket;
    };

   //
    // local datatypes, data, and functions
    //

    namespace {

      const size_t headerLength = 8; // length of header in char
      const size_t receiveBufferSize = 8192; // max size of receive buffer

      //
      // receive thread
      //
      void *
      receiveThread(void * argument)
      {

        //
        // cast argument to ReceiveThreadArgumentType
        //
        struct SocketCommunicator::SocketCommunicatorData * 
          d_data = 
          static_cast<struct SocketCommunicator::SocketCommunicatorData *>(argument);
        
        //
        // pull data from receiveThreadArgument
        //
        std::vector<char> & d_bucket = d_data->d_bucket;
        pthread_mutex_t & d_bucketLock = d_data->d_bucketLock;
        const int d_socket = d_data->d_socket;
        bool & d_isConnected = d_data->d_isConnected;
        pthread_mutex_t & d_isConnectedLock = d_data->d_isConnectedLock;

        //
        // instantiate receive buffer
        //    
        std::vector<char> receiveBuffer(receiveBufferSize);
        
        //
        // loop receiving data on socket
        //
        while (true) {
          
          //
          // receive data from socket into receiveBuffer
          //
          const ssize_t receivedSize = 
            recv(d_socket,
                 static_cast<void *>(&receiveBuffer[0]),
                 receiveBufferSize,
                 0);

          if(receivedSize > 0) {
            
            //
            // acquire lock to bucket
            //
            pthread_mutex_lock(&d_bucketLock);
            
            //
            // append receiveBuffer to bucket
            //
            d_bucket.insert(d_bucket.end(),
                            &(receiveBuffer[0]),
                            &(receiveBuffer[receivedSize]));
           
            //
            // unlock bucket
            //
            pthread_mutex_unlock(&d_bucketLock);
                                       
          }
     
          //
          // check if connection is closed
          //
          if(receivedSize == 0 || receivedSize == -1) {
             
            //
            // acquire lock to isConnected
            //
            pthread_mutex_lock(&d_isConnectedLock);
            
            //
            // set isConnected to false
            //
            d_isConnected = false;

            //
            // unlock isConnected
            //
            pthread_mutex_unlock(&d_isConnectedLock);
            
            //
            // connection is done - end thread
            //
            return NULL;
	    
          }

        }

        //
        //
        //
        return NULL;

      }

      //
      // map SocketType to socket domain type
      //
      int
      mapSocketType(SocketCommunicator::SocketType socketType)
      {

        switch (socketType) {

        case SocketCommunicator::UNIX:
          return PF_UNIX;
          break;

        case SocketCommunicator::TCPIP:
          return PF_INET;
          break;

        }

        //
        //
        //
        return -1;

      }

      //
      // initialize SocketCommunicator
      //
      void
      initialize(const std::string                          & address,
                 SocketCommunicator::SocketType               socketType,
                 bool                                         isServer,
                 SocketCommunicator::SocketCommunicatorData * d_data,
                 std::ostream                               * outputStream = NULL)
      {

#if defined(ENABLE_LOGGING)
      BOOST_LOG_FUNCTION();
#endif // ENABLE_LOGGING

        //
        // get socket domain
        //
        const int socketDomain = mapSocketType(socketType);

        //
        // fill socket address structure
        //
        struct sockaddr * socketAddress;
        struct sockaddr_un socketAddressUnix;
        struct sockaddr_in socketAddressTCP;
        size_t socketAddressSize;
        struct in_addr inaddr;
        struct hostent * hp;

        switch(socketType) {
            
        case SocketCommunicator::UNIX:
          bzero(&socketAddressUnix, sizeof(socketAddressUnix));
          socketAddressUnix.sun_family = PF_UNIX;
          strcpy(socketAddressUnix.sun_path, address.c_str());
          socketAddress = 
            (struct sockaddr *)(&socketAddressUnix);
          socketAddressSize = sizeof(socketAddressUnix);
          break;
            
        case SocketCommunicator::TCPIP:
          const size_t colonPosition = address.find(":"); 
          const std::string host = address.substr(0, colonPosition);
          const std::string port = address.substr(colonPosition + 1,
                                                  address.size());
          
#if defined(ENABLE_LOGGING)
          BOOST_LOG_TRIVIAL(info) << "Connecting to " << host << ":" << port;
#endif // ENABLE_LOGGING          

          bzero(&socketAddressTCP, sizeof(socketAddressTCP));
          socketAddressTCP.sin_family = AF_INET;
          
          if(inet_aton(host.c_str(), &inaddr) == 1)
            socketAddressTCP.sin_addr = inaddr;	/* it's dotted-decimal */
          else if( (hp = gethostbyname(host.c_str())) != NULL)
            bcopy(hp->h_addr, &socketAddressTCP.sin_addr, hp->h_length);
          else {
            const std::string message("Invalid hostname:" + host);
            throw IOError(message);
          }

          socketAddressTCP.sin_addr.s_addr = htonl(INADDR_ANY);    
          socketAddressTCP.sin_port = htons(std::atoi(port.c_str()));          
          socketAddress = 
            reinterpret_cast<struct sockaddr *>(&socketAddressTCP);
          socketAddressSize = sizeof(socketAddressTCP);

          break;
        }          
        
        if(isServer) {

          //
          // create server socket
          //
          if((d_data->d_serverSocket = socket(socketDomain,
                                              SOCK_STREAM,
                                              0)) == -1){
            std::stringstream message;
            message << "Failure creating socket: " << strerror(errno);
            throw NetworkError(message.str());
          }

          //
          // bind socket to address
          //
          if(bind(d_data->d_serverSocket,
                  socketAddress,
                  socketAddressSize) == -1) {
            std::stringstream message;
            message << "Failure on bind() of socket: " << strerror(errno);
            throw NetworkError(message.str());
          }

          if(outputStream) {
            
            std::ostream & stream = *outputStream;

            switch(socketType) {
              
            case SocketCommunicator::UNIX: {

              stream << "Address: " << address << std::endl;
              break;

            }
            case SocketCommunicator::TCPIP: {
              struct sockaddr_in address;
              socklen_t lenAddress;
              
              //
              // get (actual) socket name
              //
              lenAddress = sizeof(address);
              if(getsockname(d_data->d_serverSocket,
                             reinterpret_cast<struct sockaddr *>(&address),
                             &lenAddress) == -1) {
                std::stringstream message;
                message << "Failure on getsockname() of socket: " 
                        << strerror(errno);
                throw NetworkError(message.str());
              }
              
              //
              // convert address into string form
              //
              char * addressString = inet_ntoa(address.sin_addr);
              uint16_t port = ntohs(address.sin_port);
              stream << "Address: " << addressString << ":" << port 
                     << std::endl;
              
              break;

            }

            }

          }            

          //
          // listen on socket
          //
          if(listen(d_data->d_serverSocket, 
                    5) == -1) {
            std::stringstream message;
            message << "Failure on listen() of socket: " << strerror(errno);
            throw NetworkError(message.str());
          }
     
          //
          // accept connection from client
          //
          if((d_data->d_socket = accept(d_data->d_serverSocket, 
                                        NULL, 
                                        NULL)) == -1) {
            std::stringstream message;
            message << "Failure on accept() of client connection: " 
                    << strerror(errno);
            throw NetworkError(message.str());
          }
          
        }
        else {

          //
          // create socket
          //
          if((d_data->d_socket = socket(socketDomain,
                                        SOCK_STREAM,
                                        0)) == -1) {
            std::stringstream message;
            message << "Failure creating socket: " << strerror(errno);
            throw NetworkError(message.str());
          }
            
          //
          // connect to server
          //
          if(connect(d_data->d_socket,
                     socketAddress,
                     socketAddressSize) < 0) {
            std::stringstream message;
            message << "Failure to connect() to server at address: " 
                    << address << " with error: " << strerror(errno);
            throw NetworkError(message.str());
          }

          //
          // unlink address if UNIX socket
          //         
          if(socketType == SocketCommunicator::UNIX)
            if(unlink(address.c_str()) == -1) {
              std::stringstream message;
              message << "Error on unlink() of address: " << address 
                      << " with error: " << strerror(errno);
              throw NetworkError(message.str());
            }
          
        }

        //
        // initialize d_bucketLock
        //
        if(pthread_mutex_init(&(d_data->d_bucketLock), 
                              NULL) != 0) {
          std::stringstream message;
          message << "Error on pthread_mutex_init() of bucketLock: " 
                  << strerror(errno);
          throw OSError(message.str());
        }
        
        //
        // initialize d_isConnected and lock
        //
        d_data->d_isConnected = true;
        if(pthread_mutex_init(&(d_data->d_isConnectedLock), NULL) != 0) {
          std::stringstream message;
          message << "Error on pthread_mutex_init() of isConnectedLock: " 
                  << strerror(errno);
          throw OSError(message.str());
        };
	
	//
	// initialize send lock
	//
        if(pthread_mutex_init(&(d_data->d_sendLock), NULL) != 0) {
          std::stringstream message;
          message << "Error on pthread_mutex_init() of isConnectedLock: " 
                  << strerror(errno);
          throw OSError(message.str());
        };
        
        //
        // initialize thread attributes to detached
        //
        pthread_attr_t threadAttribute;
        
        if(pthread_attr_init(&threadAttribute) != 0) {
          std::stringstream message;
          message << "Error on pthread_attr_init() of threadAttribute: " 
                  << strerror(errno);
          throw OSError(message.str());
        }
       
        if(pthread_attr_setdetachstate(&threadAttribute, 
                                       PTHREAD_CREATE_DETACHED) != 0) {
          std::stringstream message;
          message << "Error on pthread_attr_setdetachstate of threadAttribute: "
                  << strerror(errno);
          throw OSError(message.str());
        };

        //
        // start receive thread
        //
        if(pthread_create(&d_data->d_threadId, 
                          &threadAttribute, 
                          receiveThread,
                          static_cast<void *>(d_data)) != 0) {
          std::stringstream message;
          message << "Error on pthread_create of receiveThread: "
                  << strerror(errno);
          throw OSError(message.str());
        }
        
        //
        //
        //
        return;

      }

    }

    //
    // Constructor.
    //
    SocketCommunicator::SocketCommunicator(const std::string & address,
                                           SocketType socketType,
                                           bool isServer) :
      d_data(new SocketCommunicatorData)
    {
      
      //
      // initialize SocketCommunicator
      //
      initialize(address,
                 socketType,
                 isServer,
                 d_data);        

      //
      //
      //
      return;
      
    }


    //
    // Constructor.
    // 
    SocketCommunicator::SocketCommunicator(const std::string & address,
                                           SocketType socketType,
                                           bool isServer,
                                           std::ostream & fileStream) :
      d_data(new SocketCommunicatorData)
    {

      //
      // initialize SocketCommunicator
      //
      initialize(address,
                 socketType,
                 isServer,
                 d_data,
                 &fileStream);        

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    SocketCommunicator::~SocketCommunicator()
    {

      //
      // shutdown socket
      //
      if(d_data->d_isServer)
        close(d_data->d_socket);
      else {
        shutdown(d_data->d_socket, SHUT_RDWR);
        close(d_data->d_socket);
      }

      //
      // destroy isConnectedLock
      //
      pthread_mutex_destroy(&(d_data->d_isConnectedLock));

      //
      // destroy bucketLock
      //
      pthread_mutex_destroy(&(d_data->d_bucketLock));

      //
      // destroy sendLock
      //
      pthread_mutex_destroy(&(d_data->d_sendLock));

      //
      // delete d_data
      //
      delete d_data;

    }

    //
    // Send a serialized object.
    //
    void 
    SocketCommunicator::sendSerialized(const std::string & serializedObject)
    {

#if defined(ENABLE_LOGGING)
      BOOST_LOG_FUNCTION();
#endif // ENABLE_LOGGING      

      //
      // create a string to send by prepending a header to serializedObject
      //
      std::ostringstream toSendStream;
      toSendStream << std::setw(headerLength)
                   << std::hex << serializedObject.size() << serializedObject;
       
      //
      // send object
      //
      const std::string toSend = toSendStream.str();
      const char * toSendString = toSend.c_str();
      ssize_t sizeToSend = toSend.size();

      pthread_mutex_lock(&d_data->d_sendLock);

      while (sizeToSend > 0) {

        //
        // send buffer using sockets
        //
        const ssize_t sentSize = 
          ::send(d_data->d_socket,
                 static_cast<const void *>(toSendString),
                 sizeToSend,
                 0);

        //
        // check for failure sending buffer
        //
        if (sentSize == -1) {
          std::stringstream message;
          message << "Error on send() of serialized object: " 
                  << strerror(errno);
          throw NetworkError(message.str());
        }

        //
        // update buffer
        //
        toSendString += sentSize;
        sizeToSend -= sentSize;

      }

      pthread_mutex_unlock(&d_data->d_sendLock);

    }
      
    //
    // Receive a serialized object.
    //
    std::vector<std::string> 
    SocketCommunicator::receiveSerialized()
    {
      
      //
      // return value
      //
      std::vector<std::string> returnValue;

      //
      // lock bucket
      //
      pthread_mutex_lock(&d_data->d_bucketLock);

      //
      // read as many objects off the bucket as possible
      //
      unsigned int headerStart = 0;
      while (true) {

        //
        // compute end of header
        //
        const unsigned int headerEnd = headerStart + headerLength;

        //
        // only read if there is a full header available
        //
        if(d_data->d_bucket.size() > headerEnd) {

          //
          // read header to get size of data
          //
          std::istringstream 
            header(std::string(&(d_data->d_bucket[headerStart]),
                               &(d_data->d_bucket[headerEnd])));
          std::size_t dataSize;
          header >> std::hex >> dataSize;
         
          const unsigned int dataEnd = headerEnd + dataSize;

          //
          // read data if possible
          //
          if(d_data->d_bucket.size() >= dataEnd) {
      
            //
            // add data string to returnValue
            //
            returnValue.push_back(std::string(&(d_data->d_bucket[headerEnd]),
                                              &(d_data->d_bucket[dataEnd])));
         
            //
            // move to next header
            //
            headerStart = dataEnd;

          }
          else {
            
            //
            // have not received full object
            //
            break;

          }
        }
        else {
          
          //
          // nothing to read
          //
          break;

        }

      }

      //
      // remove read entries off bucket
      //
      d_data->d_bucket.erase(d_data->d_bucket.begin(),
                             d_data->d_bucket.begin() + headerStart);

      //
      // unlock bucket
      //
      pthread_mutex_unlock(&d_data->d_bucketLock);
      
      //
      // check if connection is still active. only check when the
      // number of deserialized objects is zero - this ensures we
      // return all possible deserialized objects before throwing an
      // exception
      //
      if(returnValue.size() == 0) {
        bool isConnected;
        pthread_mutex_lock(&d_data->d_isConnectedLock);
        isConnected = d_data->d_isConnected;
        pthread_mutex_unlock(&d_data->d_isConnectedLock);

        //
        // throw exception if not connected
        //
        if(isConnected == false) {
          std::string message("Connection closed");
          throw ConnectionTerminationException(message);
        }
        
      }

      //
      //
      //
      return returnValue;

    }

  }
}
