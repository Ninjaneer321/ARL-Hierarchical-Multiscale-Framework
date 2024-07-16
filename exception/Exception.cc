//
// File:      Exception.cc
// Package    exception
//
// Base class for exceptions.
//

#ifdef HAVE_CONFIG_H
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#if defined(HAVE_MPI)
#include <mpi.h>
#include <sstream>
#endif // HAVE_MPI

#include "Exception.h"

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available
#endif // HAVE_SSTREAM

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor
    //
    Exception::Exception() throw () :
      std::exception()
    {
      return;
    }

    //
    // Constructor
    //
    Exception::Exception( const std::string & what_arg ) throw ():
      std::exception(),
      d_message( what_arg )
    {

#if defined(HAVE_MPI)
      //
      // prepend task identifier to d_message
      //
    
      //
      // get task id
      //
      int taskId;
      if(MPI_Comm_rank(MPI_COMM_WORLD,
                       &taskId) != MPI_SUCCESS) {
        // ignore (already throwing exception)
      }
    
      //
      // instantiate string stream
      //
      std::ostringstream messageStream;

      //
      // fill in messageStream
      //
      messageStream << "task@" << taskId << ": " << d_message;
    
      //
      // rewrite d_message
      //
      d_message.clear();
      d_message.assign( messageStream.str() );
    
#endif // HAVE_MPI
    

      //
      // 
      //
      return;

    }

    //
    // Destructor
    //
    Exception::~Exception() throw()
    {
      return;
    }

    //
    // Provide a message associated with the Exception object.
    //
    const char *
    Exception::what() const throw()
    {

      return d_message.c_str();

    }

  }
}
