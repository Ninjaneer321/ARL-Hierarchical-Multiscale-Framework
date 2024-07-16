//
// File:      ResourceType.I
// Package    base
//
// Base classes for HMS.
//

#include <exception/DomainError.h>

//
//
//

namespace arl {
  namespace hms {

    inline
    std::string
    resourceTypeToString(const ResourceType & resourceType)
    {

      //
      // convert resourceType to string 
      //
      switch(resourceType) {

      case CPU:
        return "CPU";
        break;

      case GPU:
        return "GPU";
        break;

      default: {
        const std::string message("Invalid resourceType");
        throw DomainError(message);
      }
      
      }
      
      //
      // unreachable
      //
      return "";

    }
  
  }
}
