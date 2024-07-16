ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}
AM_CXXFLAGS = ${ASF_CXXFLAGS}

lib_LTLIBRARIES = libtest_multiplexer.la
bin_PROGRAMS = Multiplexer

libtest_multiplexer_la_SOURCES = SleepArgument.cc                              \
                                 SleepInputFilter.cc

libtest_multiplexer_la_LDFLAGS = ${top_builddir}/adaptive_sampling/libhmsadaptive_sampling.la \
                                 ${top_builddir}/broker/libhmsbroker.la        \
                                 ${top_builddir}/base/libhmsbase.la            \
                                 ${top_builddir}/exception/libhmsexception.la  \
                                 ${BOOST_LDFLAGS} ${ASF_LIBS}                  \
                                 ${PYTHON_LIBS}

Multiplexer_SOURCES = main.cc
Multiplexer_LDFLAGS = ${top_builddir}/tests/adaptive_sampling/multiplexer/libtest_multiplexer.la \
                      ${top_builddir}/adaptive_sampling/libhmsadaptive_sampling.la \
                      ${top_builddir}/comm/libhmscomm.la                       \
                      ${top_builddir}/base/libhmsbase.la                       \
                      ${top_builddir}/exception/libhmsexception.la             \
                      ${BOOST_LDFLAGS} ${ASF_LIBS}
Multiplexer_DEPENDENCIES = libtest_multiplexer.la

#
# link against MPI library if MPI found
#
if COMPILE_MPI

libtest_multiplexer_la_LDFLAGS += ${MPILIBS}
Multiplexer_LDFLAGS += ${MPILIBS}

endif
