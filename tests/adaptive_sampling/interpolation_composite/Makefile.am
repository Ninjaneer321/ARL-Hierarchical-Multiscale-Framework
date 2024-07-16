ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}
AM_CXXFLAGS = ${ASF_CXXFLAGS}

lib_LTLIBRARIES = libtest_interpolation_composite.la
bin_PROGRAMS = InterpolationComposite 

libtest_interpolation_composite_la_SOURCES = PowellInputFilter.cc              \
                                             PowellOutputFilter.cc             \
                                             StrainArgument.cc                 \
                                             StrainInputFilter.cc              \
                                             StressOutputFilter.cc             \
                                             StressValue.cc

libtest_interpolation_composite_la_LDFLAGS = ${top_builddir}/adaptive_sampling/libhmsadaptive_sampling.la \
                                             ${top_builddir}/broker/libhmsbroker.la \
                                             ${top_builddir}/base/libhmsbase.la \
                                             ${top_builddir}/exception/libhmsexception.la  \
                                             ${BOOST_LDFLAGS} ${ASF_LIBS}      \
                                             ${PYTHON_LIBS}

InterpolationComposite_SOURCES = main.cc
InterpolationComposite_LDFLAGS = ${top_builddir}/tests/adaptive_sampling/interpolation_composite/libtest_interpolation_composite.la \
                                 ${top_builddir}/adaptive_sampling/libhmsadaptive_sampling.la \
                                 ${top_builddir}/broker/libhmsbroker.la        \
                                 ${top_builddir}/base/libhmsbase.la            \
                                 ${top_builddir}/exception/libhmsexception.la  \
                                 ${BOOST_LDFLAGS} ${ASF_LIBS} ${PYTHON_LIBS}
InterpolationComposite_DEPENDENCIES = libtest_interpolation_composite.la

#
# link against MPI library if MPI found
#
if COMPILE_MPI

libtest_interpolation_composite_la_LDFLAGS += ${MPILIBS}
InterpolationComposite_LDFLAGS += ${MPILIBS}

endif
