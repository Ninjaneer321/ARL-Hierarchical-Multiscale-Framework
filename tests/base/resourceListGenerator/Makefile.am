ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

bin_PROGRAMS = ResourceListGenerator 
ResourceListGenerator_SOURCES = main.cc                                             
ResourceListGenerator_LDFLAGS = ${top_builddir}/base/libhmsbase.la             \
                                ${BOOST_LDFLAGS}

#
# link against MPI library if MPI found
#
if COMPILE_MPI

ResourceListGenerator_LDFLAGS += ${MPILIBS}

endif
