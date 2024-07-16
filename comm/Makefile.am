ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

lib_LTLIBRARIES = libhmscomm.la

hmscommincludedir = ${includedir}/hms/comm

hmscomminclude_HEADERS = Communicator.h                                        \
                         Communicator_t.cc                                     \
                         ConfigReader.h                                        \
                         Launcher.h                                            \
                         SSHTunnel.h                                           \
                         SocketCommunicator.h

libhmscomm_la_SOURCES = ConfigReader.cc                                        \
                        Communicator.cc                                        \
                        Launcher.cc                                            \
                        SocketCommunicator.cc                                  \
                        SSHTunnel.cc

libhmscomm_la_LDFLAGS = ${top_builddir}/exception/libhmsexception.la           \
                        ${BOOST_LDFLAGS}

#
# compile femmpi library if MPI found
#
if COMPILE_MPI

hmscomminclude_HEADERS += MPICommunicator.h
libhmscomm_la_SOURCES += MPICommunicator.cc
libhmscomm_la_LDFLAGS += ${MPILIBS}

endif
