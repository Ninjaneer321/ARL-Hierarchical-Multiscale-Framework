//
// File:      Communicator_t.cc
// Package    comm
//
// Package for handling communication between individual components.
//

#include <base/ModelPackage.h>
#include <exception/Exception.h>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <sstream>

//
//
//

namespace arl {
  namespace hms {

    //
    // local methods
    //
    namespace {
 
      template <typename Object>
      void
      logTimestamp(const Object      & /* object */,
		   const std::string & /* message */) 
      {
      }

//      template <>
//      void
//      logTimestamp(const ModelPackagePointer & modelPackage,
//		   const std::string         & message)
//      {
//	modelPackage->logTimestamp(message);
//      }

      template <typename Object>
      void
      deserialize(const std::vector<std::string> & serializedObjects,
                  std::vector<Object>            & objects,
                  std::vector<std::string>       & /*failedDeserializations*/) 
      {
	
        for(std::vector<std::string>::const_iterator iter = 
              serializedObjects.begin(); 
            iter != serializedObjects.end(); ++iter) {
          
          //
          // get handle to serializedObject
          //
          const std::string & serializedObject = *iter;

          //std::cerr << "received" << serializedObject << std::endl;

          //
          // instantiate object
          //
          Object object;
        
          //
          // deserialize object
          //
          std::stringstream stream;
          stream << serializedObject;
          //boost::archive::text_iarchive archive(stream);
          boost::archive::binary_iarchive archive(stream);

          //
          // first check tag
          //
/*
          std::string tag;
          archive >> tag;

          const char * guid = boost::serialization::guid<Object>();

          std::cerr << guid << std::endl;
          std::cerr << tag << std::endl;

          if(guid && tag.compare(guid) != 0) {

	    //
	    // check if this is an exception
	    //

	    if(tag.compare(boost::serialization::guid<Exception>()) == 0) {
            
	      Exception exception;
	      archive >> exception;
	      throw exception;
	      
	    }
	    else {

	      //
	      // not the expected type - add object to list of failed
	      // deserializations
	      //
	      failedDeserializations.push_back(serializedObject);

	    }

          }
          else {
*/
            archive >> object;

	    //
	    // log data if appropriate
	    //
	    logTimestamp(object, 
			 "Received");
	    
            //
            // add object to objects
            // 
            objects.push_back(object);
            
          }
/*         
        }
*/

        //
        //
        //
        return;
        
      }

    }

    //
    // Send an object.
    //
    template <typename Object>
    void 
    Communicator::send(const Object & object)
    {

      //
      // log data if appropriate
      //
      logTimestamp(object, 
		   "Sent");

      //
      // serialize object to boost archive
      //
      std::ostringstream stream;
      //boost::archive::text_oarchive archive(stream);
      boost::archive::binary_oarchive archive(stream);
 
      //
      // prepend string with guid from boost so we can figure out how
      // to deserialize objects
      //
/*
      const char * guid = boost::serialization::guid<Object>();

      std::string guidString;
      if(guid) {
        guidString = guid;
      }
      std::cerr << "BLAH: " << guidString << std::endl;
*/
      archive << /*guidString <<*/ object;
      
      //
      // extract string from stream
      //
      const std::string serializedObject = stream.str();
      
      //std::cerr << "Sending: " << serializedObject << std::endl;

      //
      // send serialized object
      //
      sendSerialized(serializedObject);

      //
      //
      //
      return;

    }

    //
    // Receive an object.
    //
    template <typename Object>
    std::vector<Object> 
    Communicator::receive()
    {

      //
      // instantiate return value
      //
      std::vector<Object> objects;

      //
      // receive serialized object strings
      //
      const std::vector<std::string> serializedObjects = 
        receiveSerialized();

      //
      // local storage for failed deserializations
      //
      std::vector<std::string> failedDeserializations;

      pthread_mutex_lock(&d_failedDeserializationLock);
  
      //
      // first attempt to deserialize previous failed deserializations
      //
      deserialize(d_failedDeserializations,
                  objects,
                  failedDeserializations);

      //
      // next attempt to deserialize newly received objects
      //
      deserialize(serializedObjects,
                  objects,
                  failedDeserializations);
      
      //
      // store failed deserializations locally
      //
      d_failedDeserializations.swap(failedDeserializations);
      
      pthread_mutex_unlock(&d_failedDeserializationLock);

      //
      //
      //
      return objects;

    }
    
  }
}
