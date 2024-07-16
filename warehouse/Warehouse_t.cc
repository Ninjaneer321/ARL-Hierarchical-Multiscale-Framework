//
// File:      Warehouse_t.cc
// Package    warehouse
//
// HMS warehouse.
//

#include <exception/ConnectionTerminationException.h>

//
//
//

namespace arl {
  namespace hms {
    
    //
    // Constructor.
    //
    template <typename Object,
              template<typename, typename> class STLContainer>
    Warehouse<Object, STLContainer>::Warehouse(const CommunicatorContainer & communicators) :
      d_communicators(communicators)
    {
      
      //
      //
      //
      return;

    }
    
    //
    // Destructor.
    //
    template <typename Object,
              template<typename, typename> class STLContainer>
    Warehouse<Object, STLContainer>::~Warehouse()
    {

      //
      //
      //
      return;
      
    }
    
    //
    // Receive objects.
    //
    template <typename Object,
              template<typename, typename> class STLContainer>
    std::vector<Object> 
    Warehouse<Object, STLContainer>::receive()
    {
     
      //
      // instantiate return value.
      //
      std::vector<Object> receivedObjects;

      //
      // if we have no communicators, we are done processing and need
      // to issue shutdown notice
      //
      if(d_communicators.empty()) {
        
        //
        //
        //
        const std::string message("Connection Terminated.");
        throw ConnectionTerminationException(message);
        
      }

      //
      // iterate over communicators
      //
      typename CommunicatorContainer::iterator iter = 
        d_communicators.begin();
    
      for(typename CommunicatorContainer::size_type communicatorId = 0; 
          communicatorId < d_communicators.size(); ++communicatorId) {

        //
        // get handle to communicator
        //
        const CommunicatorPointer & communicator = 
          d_communicators[communicatorId];

        //
        // try to receive on communicator
        //
        std::vector<Object> objects;
        
        //
        // flag to indicate connection terminated
        //
        bool isConnectionTerminated = false;

        try {
        
          //
          // receive vector of objects on communicator
          //
          objects = communicator->receive<Object>();

        }
        catch (ConnectionTerminationException & exception) {
        
          //
          // termination received on communicator, set
          // isConnectionTerminated to true.
          //
          isConnectionTerminated = true;
          
        }

        //
        // if connection is terminated remove communicator
        //
        iter = isConnectionTerminated ? d_communicators.erase(iter) : ++iter;
       
        //
        // set communicator for all received objects
        //
        typename std::vector<Object>::iterator objectIter = objects.begin();
        const typename std::vector<Object>::const_iterator objectEndIter =
          objects.end();
        
        for(; objectIter != objectEndIter; ++objectIter) {
        
          //
          // get handle to object
          //
          Object & object = *objectIter;

          //
          // set communicator
          //
          object->addCommunicator(communicatorId);
                   
        }

        //
        // add to received objects
        //
        receivedObjects.insert(receivedObjects.end(),
                               objects.begin(),
                               objects.end());
         
      }

      //
      // return all received objects
      //
      return receivedObjects;
 
    }

    //
    // Ship objects.
    //
    template <typename Object,
              template<typename, typename> class STLContainer>
    void 
    Warehouse<Object, STLContainer>::ship(const std::vector<Object> & objects)
    {

      //
      // iterate over objects
      //
      typename std::vector<Object>::const_iterator iter = objects.begin();
      const typename std::vector<Object>::const_iterator iterEnd = 
        objects.end();

      for(; iter != iterEnd; ++iter) {
        
        //
        // get handle to object
        //
        const Object & object = *iter;
        
        //
        // get communicator id from object
        //
        const int communicatorId = object->popCommunicator();

        //
        // get handle to communicator
        //
        CommunicatorPointer & communicator = d_communicators[communicatorId];

        //
        // send object on communicator
        // 
        communicator->send(object);
      
      }

      //
      //
      //
      return;

    }
          
  }
}
