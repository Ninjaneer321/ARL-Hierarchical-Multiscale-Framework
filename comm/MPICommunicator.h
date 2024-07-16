//
// File:      MPICommunicator.h
// Package    comm
//
// Package for handling communication between individual components.
//
#if !defined(comm_MPICommunicator_h)
#define comm_MPICommunicator_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#if defined(HAVE_MPI)
#include <mpi.h>
#else
#error mpi.h header file not available
#endif // HAVE_MPI

#include "Communicator.h"

#include <list>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Concrete class implementing communication over MPI.
     */
    class MPICommunicator : public Communicator {

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
       * @param intercommunicator MPI intercommunicator.
       * @param isServer Whether this communicator is a server or client.
       * @param intercommunicatorRank Rank to communicate on
       * intercommunicator.
       */
      MPICommunicator(const MPI_Comm & intercommunicator,
                      bool             isServer,
                      int              intercommunicatorRank);

      /**
       * Destructor.
       */
      virtual ~MPICommunicator();

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
      MPICommunicator(const MPICommunicator &); // not implemented
      MPICommunicator & operator=(const MPICommunicator &); // not implemented

      //
      // types
      //
    private:

      typedef std::pair<MPI_Request, std::string> SendRequest;
      typedef std::list<SendRequest> SendRequestContainer;

      //
      // data
      //
    private:

      MPI_Comm             d_intercommunicator;
      bool                 d_isServer;
      std::string          d_name;
      int                  d_remoteRank;
      SendRequestContainer d_sendRequests;

    };

  }
}

#endif // comm_MPICommunicator_h
