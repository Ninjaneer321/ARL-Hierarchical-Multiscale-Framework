//
// File:      ResourceType.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_ResourceType_h)
#define base_ResourceType_h

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * ResourceType.
     */
    typedef enum ResourceType {
      CPU = 0,
      GPU
    } ResourceType;

    std::string
    resourceTypeToString(const ResourceType & resourceType);
  
  }
}

#include "ResourceType.I"

#endif // base_ResourceType_h
