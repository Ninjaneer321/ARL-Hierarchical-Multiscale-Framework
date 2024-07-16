//
// File:      NumberProcessorsPerNode.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "NumberProcessorsPerNode.h"

#include <exception/IOError.h>
#include <exception/OSError.h>

#if defined(HAVE_CSTDLIB)
#include <cstdlib>
#else
#error cstdlib header file not available
#endif // HAVE_CSTDLIB

#if defined(HAVE_MAP)
#include <map>
#else
#error map header file not available
#endif // HAVE_MAP

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

#if defined(HAVE_STRING)
#include <string>
#else
#error string header file not available
#endif // HAVE_STRING

//
//
//

namespace arl {
  namespace hms {

    //
    // local functions
    //
    namespace {  
      
      //
      // get value from map, with error checking
      //
      int
      getValue(const std::string                & key,
	       const std::map<std::string, int> & mapKV)
      {
    
	//
	// find key in map
	//
	std::map<std::string, int>::const_iterator iter = mapKV.find(key);
	if(iter == mapKV.end()) {
	  std::stringstream message;
	  message << "Error finding: " << key 
		  << " in NumberProcessors Per Node";
	  throw arl::hms::IOError(message.str());
	}

	//
	//
	//
	return iter->second;

      }

    }


    //
    // Constructor.
    //
    NumberProcessorsPerNode::NumberProcessorsPerNode()
    {

      // machine to processors per node map
      std::map<std::string, int> machineToProcessorsPerNode;
      
      // customize this map to convert machine hostnames to number processors 
      // per node
      machineToProcessorsPerNode["hostname"] = 1;
      
      // get BC_Host name
      char * BCHOST = getenv("BC_HOST");
      
      if(!BCHOST) {
	const std::string message("Cannot find BC_HOST environment "
				  "variable");
	throw OSError(message);
      }

      const std::string bcHost = BCHOST;

      d_numberCpus = getValue(bcHost, 
			      machineToProcessorsPerNode);

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    NumberProcessorsPerNode::~NumberProcessorsPerNode()
    {

      //
      //
      //
      return;

    }

    //
    // obtain number of processors per node
    //
    int 
    NumberProcessorsPerNode::getNumberProcessorsPerNode() const
    {

      //
      //
      //
      return d_numberCpus;
  
    }
  }
}
