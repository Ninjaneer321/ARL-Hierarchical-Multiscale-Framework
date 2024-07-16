ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}
AM_CXXFLAGS = ${ASF_CXXFLAGS}

bin_PROGRAMS = kde

kde_SOURCES = main.cc
kde_LDFLAGS = ${top_builddir}/adaptive_sampling/libhmsadaptive_sampling.la     \
              ${top_builddir}/comm/libhmscomm.la                               \
              ${top_builddir}/base/libhmsbase.la                               \
              ${top_builddir}/exception/libhmsexception.la                     \
              ${BOOST_LDFLAGS} ${ASF_LIBS}

#
# link against MPI library if MPI found
#
if COMPILE_MPI

kde_LDFLAGS += ${MPILIBS}

endif
