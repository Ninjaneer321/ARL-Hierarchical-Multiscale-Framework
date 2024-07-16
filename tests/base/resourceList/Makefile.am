ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

bin_PROGRAMS = ResourceList 
ResourceList_SOURCES = main.cc                                                  
ResourceList_LDFLAGS = ${top_builddir}/base/libhmsbase.la                      \
                       ${BOOST_LDFLAGS}

#
# link against MPI library if MPI found
#
if COMPILE_MPI

ResourceList_LDFLAGS += ${MPILIBS}

endif