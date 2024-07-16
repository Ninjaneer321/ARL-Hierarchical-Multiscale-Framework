//
// File:      SocketCommunicator.h
// Package    comm
//
// Package for handling communication between individual components.
//
#if !defined(comm_SocketCommunicator_h)
#define comm_SocketCommunicator_h

#include "Communicator.h"

#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Concrete class implementing communication over sockets.
     */
    class SocketCommunicator : public Communicator {

      //
      // types
      //
    public:
      typedef enum { UNIX, TCPIP } SocketType;  

      //
      // internal data type
      //
      struct SocketCommunicatorData;
      
      //
      // methods
      //
    public:

      /**
       * Constructor.
       *
       * @param address For server, address to bind to. For client,
       * address to connect to.
       * @param socketType Type of socket.
       * @param isServer Whether this communicator is a server or client.
       */
      SocketCommunicator(const std::string & address,
                         SocketType          socketType,
                         bool                isServer);

      /**
       * Constructor.
       *
       * @param address For server, address to bind to. For client,
       * address to connect to.
       * @param socketType Type of socket.
       * @param isServer Whether this communicator is a server or client.
       * @param fileStream File to write socket information to.
       */
      SocketCommunicator(const std::string & address,
                         SocketType          socketType,
                         bool                isServer,
                         std::ostream      & fileStream);

      /**
       * Destructor.
       */
      virtual ~SocketCommunicator();

    protected:

      /**
       * Send a serialized object.
       *
       * @param serializedObject A serialized object to send.
       */
      virtual void sendSerialized(const std::string & serializedObject);
      
      /**
       * Receive serialized objects.
       *
       * @return A vector of serialized objects.
       */
      virtual std::vector<std::string> receiveSerialized();

    private:
      //
      // copy constructor/assignment operator
      //
      SocketCommunicator(const SocketCommunicator &); // not implemented
      SocketCommunicator & operator=(const SocketCommunicator &); // not implemented

      //
      // data
      //
    private:
      
      SocketCommunicatorData * d_data;
      
    };

  }
}

#endif // comm_SocketCommunicator_h
