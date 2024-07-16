ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

bin_PROGRAMS = BasicMonitor
BasicMonitor_SOURCES = main.cc                                                 \
                       PicalcEndPredicate.cc                                   \
                       PicalcOutputFilter.cc                                   \
                       PiValue.cc
BasicMonitor_LDFLAGS = ${top_builddir}/monitor/libhmsmonitor.la                \
                       ${top_builddir}/scheduler/libhmsscheduler.la            \
                       ${top_builddir}/base/libhmsbase.la                      \
                       ${BOOST_LDFLAGS} -lpthread

#
# link against MPI library if MPI found
#
if COMPILE_MPI

bin_PROGRAMS += cpi

cpi_SOURCES = cpi.c

BasicMonitor_LDFLAGS += ${MPILIBS}
cpi_LDFLAGS = ${MPILIBS}

endif
