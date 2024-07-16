//
// File:      Logger.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_Logger_h)
#define base_Logger_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#if defined(HAVE_IOSFWD)
#include <iosfwd>
#else
#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iosfwd or iostream header file not available
#endif // HAVE_IOSTREAM
#endif // HAVE_IOSFWD

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief HMS logger
     */
    class Logger {

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
       * @param configFile Input stream to configuration file.
       */
      Logger(std::istream      & configFile,
	     const std::string & logFileNameAppend = "");

      /**
       * Destructor.
       */
      virtual ~Logger();
  
    private:
      //
      // copy constructor/assignment operator
      //
      Logger(const Logger &); // not implemented
      Logger & operator=(const Logger &); // not implemented

      //
      // data
      //
    private:

    };
  
  }
}

#endif // base_Logger_h
