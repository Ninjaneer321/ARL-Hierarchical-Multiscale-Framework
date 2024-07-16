//
// File:      MPICommunicator.cc
// Package    comm
//
// Package for handling communication between individual components.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "MPICommunicator.h"

#include <exception/ConnectionTerminationException.h>
#include <exception/NetworkError.h>

#if defined(ENABLE_LOGGING)
#if defined(HAVE_BOOST_LOG)
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#else
#error boost logging not available
#endif // HAVE_BOOST_LOG
#endif // ENABLE_LOGGING

#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#else
#error pthread.h header file not available
#endif // HAVE_PTHREAD_H

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available.
#endif // HAVE_SSTREAM

//
//
//

namespace arl {
  namespace hms {

    //
    // local data
    //
    namespace {

      //
      // Lock for all MPI calls
      //
      pthread_mutex_t mpiMutex = PTHREAD_MUTEX_INITIALIZER;
      
      //
      // Send / Receive Tags
      //
      const int sendReceiveTag = 0;
      const int terminationTag = 1;

    }

    //
    // Constructor.
    //
    MPICommunicator::MPICommunicator(const MPI_Comm & intercommunicator,
                                     bool             isServer,
                                     int              intercommunicatorRank) :
      d_intercommunicator(intercommunicator),
      d_isServer(isServer),
      d_remoteRank(intercommunicatorRank)
    {

      //
      //
      //
      return;
      
    }

    //
    // Destructor.
    //
    MPICommunicator::~MPICommunicator()
    {

#if defined(ENABLE_LOGGING)
      BOOST_LOG_FUNCTION();
#endif // ENABLE_LOGGING

      //
      // lock mpi
      //
      pthread_mutex_lock(&mpiMutex);

      //
      // copy outstanding send requests into contiguous storage
      //
      std::vector<MPI_Request> sendRequests;
      for(SendRequestContainer::const_iterator iter = d_sendRequests.begin();
          iter != d_sendRequests.end(); ++iter) {
        sendRequests.push_back(iter->first);
      }
    
      //
      // wait on all outstanding requests
      //
      std::vector<MPI_Status> sendStatuses(sendRequests.size());
      if(MPI_Waitall(sendRequests.size(),
                     &(sendRequests[0]),
                     &(sendStatuses[0])) != MPI_SUCCESS) {

#if defined(ENABLE_LOGGING)
        BOOST_LOG_TRIVIAL(fatal) << "Failure during MPI_Waitall";
#endif // ENABLE_LOGGING

      }
      
      //
      // if client, send a termination notice to the server
      //
      if(d_isServer == false) {

#if defined(ENABLE_LOGGING)
        BOOST_LOG_TRIVIAL(info) << "Sending termination notice to server "<< 
          d_remoteRank;
#endif // ENABLE_LOGGING
    
        char value = '\0';
        if(MPI_Send(static_cast<void *>(&value),
                    1,
                    MPI_CHAR,
                    d_remoteRank,
                    terminationTag,
                    d_intercommunicator) != MPI_SUCCESS) {

#if defined(ENABLE_LOGGING)
          BOOST_LOG_TRIVIAL(fatal) << "Failure sending termination notice";
#endif // ENABLE_LOGGING
        }

      }

      //
      // unlock mpi
      //
      pthread_mutex_unlock(&mpiMutex);
      
      //
      //
      //
      return;

    }

    //
    // Send a serialized object.
    //
    void 
    MPICommunicator::sendSerialized(const std::string & serializedObject)
    {

#if defined(ENABLE_LOGGING)
      BOOST_LOG_FUNCTION();
#endif // ENABLE_LOGGING

      //
      // lock mpi
      //
      pthread_mutex_lock(&mpiMutex);

      //
      // instantiate sendRequest
      //
      SendRequest sendRequest;
      
      //
      // deep copy of serialized object into local storage
      //
      sendRequest.second = serializedObject;
     
      //
      // insert sendRequest into collection of send requests
      //
      SendRequestContainer::iterator sendRequestsIter = 
        d_sendRequests.insert(d_sendRequests.end(),
                              sendRequest);
          
      //
      // perform a nonblocking send of serializedObject
      //
      const char * serializedObjectPtr = sendRequestsIter->second.c_str();
      if(MPI_Isend(static_cast<void *>(const_cast<char *>(serializedObjectPtr)),
                   sendRequestsIter->second.size(),
                   MPI_CHAR,
                   d_remoteRank,
                   sendReceiveTag,
                   d_intercommunicator,
                   &(sendRequestsIter->first)) != MPI_SUCCESS) {
        const std::string message("Error during MPI_Isend");
        throw NetworkError(message);
      }
      
      //
      // test outstanding send requests for completion
      //
      sendRequestsIter = d_sendRequests.begin();
      const SendRequestContainer::iterator sendRequestsEnd = 
        d_sendRequests.end();
      
      for( ; sendRequestsIter != sendRequestsEnd; ) {
      
        //
        // get send request
        //
        SendRequest & sendRequest = *sendRequestsIter;

        //
        // test whether send request has completed
        //
        int flag;   
        MPI_Status status;
        if(MPI_Test(&(sendRequest.first),
                    &flag,
                    &status) != MPI_SUCCESS) {
          const std::string message("Error during MPI_Test");
          throw NetworkError(message);
        }  

        //
        // increment sendRequestIter;
        //
        SendRequestContainer::iterator sendRequestIterCopy = sendRequestsIter++;

        //
        // check if request has completed
        //
        if(flag > 0) {
          
          //
          // erase completed request from d_sendRequests
          //
          d_sendRequests.erase(sendRequestIterCopy);
          
        }

      }

      //
      // unlock mpi
      //
      pthread_mutex_unlock(&mpiMutex);
      
      //
      //
      //
      return;

    }
      
    //
    // Receive a serialized object.
    //
    std::vector<std::string> 
    MPICommunicator::receiveSerialized()
    {

#if defined(ENABLE_LOGGING)
      BOOST_LOG_FUNCTION();
#endif // ENABLE_LOGGING

      //
      // lock mpi
      //
      pthread_mutex_lock(&mpiMutex);

      //
      // instantiate returnValue
      //
      std::vector<std::string> returnValue;
          
      //
      // receive messages while available
      //
      int flag;
      do {

        //
        // check if message is available
        //
        MPI_Status status;
        if(MPI_Iprobe(d_remoteRank,
                      sendReceiveTag,
                      d_intercommunicator,
                      &flag,
                      &status) != MPI_SUCCESS) {
          const std::string message("Error during MPI_Iprobe");
          throw NetworkError(message);
        }

        if (flag > 0) {
          
          //
          // get size of message
          //
          int messageSize;
          if(MPI_Get_count(&status,
                           MPI_CHAR,
                           &messageSize) != MPI_SUCCESS) {
            const std::string message("Error during MPI_Get_count");
            throw NetworkError(message);
          }
          
          //
          // allocate buffer
          //
          std::vector<char> serializedObject(messageSize);
                    
          //
          // receive message
          //
          if(MPI_Recv(static_cast<void *>(&serializedObject[0]),
                      messageSize,
                      MPI_CHAR,
                      d_remoteRank,
                      sendReceiveTag,
                      d_intercommunicator,
                      &status) != MPI_SUCCESS) {
            const std::string message("Error during MPI_Recv");
            throw NetworkError(message);
          }
          
          //
          // append serializedObject to returnValue
          //
          returnValue.push_back(std::string(serializedObject.begin(),
                                            serializedObject.end()));
          
        }

      } while (flag > 0);
      
      //
      // check if connection is still active. only check when the
      // number of deserialized objects is zero - this ensures we
      // return all possible deserialized objects before throwing an
      // exception
      //
      if(returnValue.size() == 0) {

        //
        // check if termination message is available
        //
        MPI_Status status;
        int terminationFlag;
        if(MPI_Iprobe(d_remoteRank,
                      terminationTag,
                      d_intercommunicator,
                      &terminationFlag,
                      &status) != MPI_SUCCESS) {
          const std::string message("Error during MPI_Iprobe for termination "
                                    "flag");
          throw NetworkError(message);
        }

        //
        // if termination message is available, receive it and throw an
        // exception.
        //
        if(terminationFlag > 0) {
      
          char receiveValue;
          MPI_Status status;
          if(MPI_Recv(&receiveValue,
                      1,
                      MPI_CHAR,
                      d_remoteRank,
                      terminationTag,
                      d_intercommunicator,
                      &status) != MPI_SUCCESS) {
            const std::string message("Error receiving termination flag");
            throw NetworkError(message);
          }

          //
          // throw exception indicating termination of communicator
          //
          const std::string message("Termination notice received");
          throw ConnectionTerminationException(message);

        }

      }

      //
      // unlock mpi
      //
      pthread_mutex_unlock(&mpiMutex);
      
      //
      //
      //
      return returnValue;

    }

  }
}
