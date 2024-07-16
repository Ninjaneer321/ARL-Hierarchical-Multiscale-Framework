ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

lib_LTLIBRARIES = libhmsbroker.la
bin_PROGRAMS = StartBroker

hmsbrokerincludedir = ${includedir}/hms/broker

hmsbrokerinclude_HEADERS = BasicBroker.h                                       \
                           Broker.h                                            \
                           BrokerFactory.h                                     \
                           BrokerLauncher.h                                    \
                           CleanupPolicy.h                                     \
                           CleanupPolicyCustom.h                               \
                           CleanupPolicyFixed.h                                \
                           CleanupPolicyNone.h

libhmsbroker_la_SOURCES = BasicBroker.cc                                       \
                          Broker.cc                                            \
                          BrokerFactory.cc                                     \
                          BrokerLauncher.cc                                    \
                          CleanupPolicy.cc                                     \
                          CleanupPolicyCustom.cc                               \
                          CleanupPolicyFixed.cc                                \
                          CleanupPolicyNone.cc

libhmsbroker_la_LDFLAGS = ${top_builddir}/monitor/libhmsmonitor.la             \
                          ${top_builddir}/scheduler/libhmsscheduler.la         \
                          ${top_builddir}/base/libhmsbase.la                   \
                          ${top_builddir}/comm/libhmscomm.la                   \
                          ${top_builddir}/exception/libhmsexception.la         \
                          ${BOOST_LDFLAGS}

StartBroker_SOURCES = StartBroker.cc
StartBroker_LDFLAGS = ${top_builddir}/broker/libhmsbroker.la                   \
                      ${top_builddir}/monitor/libhmsmonitor.la                 \
                      ${top_builddir}/scheduler/libhmsscheduler.la             \
                      ${top_builddir}/base/libhmsbase.la                       \
                      ${top_builddir}/comm/libhmscomm.la                       \
                      ${top_builddir}/exception/libhmsexception.la             \
                      ${BOOST_LDFLAGS}
StartBroker_DEPENDENCIES = libhmsbroker.la

if COMPILE_MPI

libhmsbroker_la_LDFLAGS += ${MPILIBS}
StartBroker_LDFLAGS += ${MPILIBS}

endif
