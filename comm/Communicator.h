//
// File:      Communicator.h
// Package    comm
//
// Package for handling communication between individual components.
//
#if !defined(comm_Communicator_h)
#define comm_Communicator_h

#include <utils/HMSMacros.h>

#include <pthread.h>
#include <string>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class for implementing communicators.
     */
    class Communicator {

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
      virtual ~Communicator() = 0;

      /**
       * Send an object.
       *
       * @param object An object to send.
       */
      template <typename Object>
      void send(const Object & object);

      /**
       * Receive objects.
       *
       * @return Received objects.
       */
      template <typename Object>
      std::vector<Object> receive();

    protected:

      /**
       * Constructor.
       */
      Communicator();

      /**
       * Send a serialized object.
       *
       * @param serializedObject A serialized object to send.
       */
      virtual void sendSerialized(const std::string & serializedObject) = 0;
      
      /**
       * Receive a serialized object.
       *
       * @return A serialized object.
       */
      virtual std::vector<std::string> receiveSerialized() = 0;

    private:
      //
      // copy constructor/assignment operator
      //
      Communicator(const Communicator &); // not implemented
      Communicator & operator=(const Communicator &); // not implemented

      //
      // data
      //
    private:

      pthread_mutex_t          d_failedDeserializationLock;
      std::vector<std::string> d_failedDeserializations;

    };

    HMS_SHARED_PTR(Communicator);

  }
}

#include "Communicator_t.cc"

#endif // comm_Communicator_h
