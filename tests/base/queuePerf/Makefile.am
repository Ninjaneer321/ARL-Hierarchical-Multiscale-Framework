ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

bin_PROGRAMS = QueuePerf 
QueuePerf_SOURCES = main.cc                                                  
QueuePerf_LDFLAGS = ${top_builddir}/base/libhmsbase.la                         \
                    ${BOOST_LDFLAGS}

#
# link against MPI library if MPI found
#
if COMPILE_MPI

QueuePerf_LDFLAGS += ${MPILIBS}

endif