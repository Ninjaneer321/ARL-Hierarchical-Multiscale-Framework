//
// File:      Logger.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#if defined(HAVE_MPI)
#include <mpi.h>
#endif // HAVE_MPI

#include "Logger.h"

#if defined(ENABLE_LOGGING)

#include <exception/NetworkError.h>

#if defined(HAVE_BOOST)
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#else 
#error boost not available
#endif // HAVE_BOOST

#if defined(HAVE_BOOST_LOG)
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#else
#error boost logging not available
#endif // HAVE_BOOST_LOG

#endif // ENABLE_LOGGING

#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iostream header file not available
#endif // HAVE_IOSTREAM

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    Logger::Logger(std::istream & configFile,
		   const std::string & logFileNameAppend)
    {

      //
      // initialize logging
      //
      
#if defined(ENABLE_LOGGING)
      
      boost::log::add_common_attributes();
      boost::log::core::get()->
        add_global_attribute("Scope",
                             boost::log::attributes::named_scope());

      std::string formatString("[%TimeStamp%] - %Message%");

#if defined(HAVE_MPI)
      
      //
      // get rank
      //
      int taskId;
      if(MPI_Comm_rank(MPI_COMM_WORLD,
                       &taskId) != MPI_SUCCESS) {
        const std::string message("Error getting rank");
        throw arl::hms::NetworkError(message);
      };
      
      std::stringstream format;
      format << "r@" << taskId << ": " << formatString; 
      
      formatString = format.str();
      
#endif // HAVE_MPI
      
      //
      // parse config file
      //
      boost::property_tree::ptree propertyTree;
      boost::property_tree::ini_parser::read_ini(configFile, 
                                                 propertyTree);

      //
      // go back to beginning of stream
      //
      configFile.clear();
      configFile.seekg(0);
         
      //
      // read required configuration options
      //
      const std::string loggingLevel = 
        propertyTree.get<std::string>("Logging.Level");
      std::string loggingFile = 
        propertyTree.get<std::string>("Logging.File");

      std::stringstream loggingFileName;
      loggingFileName << loggingFile << logFileNameAppend;
      loggingFile = loggingFileName.str();

      //
      // set logging to file
      //
      boost::log::add_file_log(boost::log::keywords::file_name = loggingFile,
                               boost::log::keywords::format = formatString,
			       boost::log::keywords::auto_flush = true);

      std::cerr << "Log file: " << loggingFile << std::endl; 

      //
      // set logging level
      // 
      boost::log::trivial::severity_level level;
      std::stringstream loggingLevelStream;
      loggingLevelStream << loggingLevel;
      loggingLevelStream >> level;
      std::cerr << "Level " << level << std::endl;
      boost::log::core::get()->set_filter(boost::log::trivial::severity >= 
                                          level);
      
      BOOST_LOG_TRIVIAL(trace) << "foo";

#endif // ENABLE_LOGGING

      //
      //
      //
      return;
      
    }

    //
    // Destructor.
    //
    Logger::~Logger()
    {

      //
      //
      //
      return;

    }
  
  }
}
