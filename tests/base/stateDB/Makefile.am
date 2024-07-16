ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

bin_PROGRAMS = StateDB 
StateDB_SOURCES = main.cc                                                  
StateDB_LDFLAGS = ${top_builddir}/base/libhmsbase.la                           \
                  ${BOOST_LDFLAGS}

#
# link against MPI library if MPI found
#
if COMPILE_MPI

StateDB_LDFLAGS += ${MPILIBS}

endif