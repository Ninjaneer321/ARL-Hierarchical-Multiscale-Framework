ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}
AM_CXXFLAGS = ${ASF_CXXFLAGS}

lib_LTLIBRARIES = libtest_adaptive_sampling.la
bin_PROGRAMS = AdaptiveSampling  xy

libtest_adaptive_sampling_la_SOURCES = XYAdaptiveSamplingInputFilter.cc        \
                                       XYAdaptiveSamplingOutputFilter.cc       \
                                       XYArgument.cc                           \
                                       XYInputFilter.cc                        \
                                       XYOutputFilter.cc                       \
                                       XYValue.cc
libtest_adaptive_sampling_la_LDFLAGS = ${top_builddir}/adaptive_sampling/libhmsadaptive_sampling.la \
                                       ${top_builddir}/broker/libhmsbroker.la  \
                                       ${top_builddir}/comm/libhmscomm.la      \
                                       ${top_builddir}/base/libhmsbase.la      \
                                       ${top_builddir}/exception/libhmsexception.la \
                                       ${BOOST_LDFLAGS} ${ASF_LIBS}

AdaptiveSampling_SOURCES = main.cc
AdaptiveSampling_LDFLAGS = ${top_builddir}/tests/adaptive_sampling/adaptive_sampling/libtest_adaptive_sampling.la                                              \
                           ${top_builddir}/adaptive_sampling/libhmsadaptive_sampling.la \
                           ${top_builddir}/comm/libhmscomm.la                  \
                           ${top_builddir}/base/libhmsbase.la                  \
                           ${top_builddir}/exception/libhmsexception.la        \
                           ${BOOST_LDFLAGS} ${ASF_LIBS}
AdaptiveSampling_DEPENDENCIES = libtest_adaptive_sampling.la

xy_SOURCES = xy.cc

#
# link against MPI library if MPI found
#
if COMPILE_MPI

libtest_adaptive_sampling_la_LDFLAGS += ${MPILIBS}
AdaptiveSampling_LDFLAGS += ${MPILIBS}

endif
