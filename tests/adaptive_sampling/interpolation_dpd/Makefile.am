ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}
AM_CXXFLAGS = ${ASF_CXXFLAGS}

lib_LTLIBRARIES = libtest_interpolation_dpd.la
bin_PROGRAMS = InterpolationDPD 

libtest_interpolation_dpd_la_SOURCES = DPDArgument.cc                          \
                                       DPDInputFilter.cc                       \
                                       DPDOutputFilter.cc                      \
                                       DPDValue.cc

libtest_interpolation_dpd_la_LDFLAGS = ${top_builddir}/adaptive_sampling/libhmsadaptive_sampling.la \
                                       ${top_builddir}/broker/libhmsbroker.la  \
                                       ${top_builddir}/base/libhmsbase.la      \
                                       ${top_builddir}/exception/libhmsexception.la  \
                                       ${BOOST_LDFLAGS} ${ASF_LIBS}            \
                                       ${PYTHON_LIBS}

InterpolationDPD_SOURCES = main.cc
InterpolationDPD_LDFLAGS = ${top_builddir}/tests/adaptive_sampling/interpolation_dpd/libtest_interpolation_dpd.la \
                                 ${top_builddir}/adaptive_sampling/libhmsadaptive_sampling.la \
                                 ${top_builddir}/broker/libhmsbroker.la        \
                                 ${top_builddir}/base/libhmsbase.la            \
                                 ${top_builddir}/exception/libhmsexception.la  \
                                 ${BOOST_LDFLAGS} ${ASF_LIBS} ${PYTHON_LIBS}
InterpolationDPD_DEPENDENCIES = libtest_interpolation_dpd.la

#
# link against MPI library if MPI found
#
if COMPILE_MPI

libtest_interpolation_dpd_la_LDFLAGS += ${MPILIBS}
InterpolationDPD_LDFLAGS += ${MPILIBS}

endif
