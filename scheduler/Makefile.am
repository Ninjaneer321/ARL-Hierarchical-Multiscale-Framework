ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

lib_LTLIBRARIES = libhmsscheduler.la

hmsschedulerincludedir = ${includedir}/hms/scheduler

hmsschedulerinclude_HEADERS = BasicScheduler.h                                 \
                              BatchQueueScheduler.h                            \
                              LoadQuery.h                                      \
                              LoadQuery_t.cc                                   \
                              Scheduler.h

libhmsscheduler_la_SOURCES = BasicScheduler.cc                                 \
                             BatchQueueScheduler.cc                            \
                             LoadQuery.cc                                      \
                             Scheduler.cc

libhmsscheduler_la_LDFLAGS = ${top_builddir}/exception/libhmsexception.la      \
                             ${top_builddir}/base/libhmsbase.la                \
                             ${BOOST_LDFLAGS}
