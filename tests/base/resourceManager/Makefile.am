ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

bin_PROGRAMS = ResourceManager 
ResourceManager_SOURCES = main.cc                                         
ResourceManager_LDFLAGS = ${top_builddir}/base/libhmsbase.la                   \
                          ${BOOST_LDFLAGS}

#
# link against MPI library if MPI found
#
if COMPILE_MPI

ResourceManager_LDFLAGS += ${MPILIBS}

endif