ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

bin_PROGRAMS = lsCommand 
lsCommand_SOURCES = main.cc                                                  
lsCommand_LDFLAGS = ${top_builddir}/base/libhmsbase.la                         \
                    ${BOOST_LDFLAGS}

#
# link against MPI library if MPI found
#
if COMPILE_MPI

lsCommand_LDFLAGS += ${MPILIBS}

endif