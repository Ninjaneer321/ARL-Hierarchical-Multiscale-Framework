ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

lib_LTLIBRARIES = libhmsmonitor.la

hmsmonitorincludedir = ${includedir}/hms/monitor

hmsmonitorinclude_HEADERS = BasicMonitor.h                                     \
                            Monitor.h

libhmsmonitor_la_SOURCES = BasicMonitor.cc                                     \
                           Monitor.cc

libhmsmonitor_la_LDFLAGS = ${top_builddir}/exception/libhmsexception.la        \
                           ${top_builddir}/base/libhmsbase.la                  \
                           ${BOOST_LDFLAGS}
