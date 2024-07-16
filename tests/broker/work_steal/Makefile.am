ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

lib_LTLIBRARIES = libtest_broker.la
bin_PROGRAMS = StartAlgorithm

libtest_broker_la_SOURCES = SleepOutputFilter.cc                               \
                            SleepInputFilter.cc
libtest_broker_la_LDFLAGS = ${top_builddir}/base/libhmsbase.la                 \
                            ${top_builddir}/comm/libhmscomm.la                 \
                            ${top_builddir}/monitor/libhmsmonitor.la           \
                            ${top_builddir}/broker/libhmsbroker.la             \
                            ${BOOST_LDFLAGS}

StartAlgorithm_SOURCES = StartAlgorithm.cc
StartAlgorithm_LDFLAGS = ${top_builddir}/tests/broker/work_steal/libtest_broker.la     \
                         ${top_builddir}/base/libhmsbase.la                    \
                         ${top_builddir}/comm/libhmscomm.la                    \
                         ${top_builddir}/monitor/libhmsmonitor.la              \
                         ${top_builddir}/broker/libhmsbroker.la                \
                         ${BOOST_LDFLAGS}
StartAlgorithm_DEPENDENCIES = libtest_broker.la


if COMPILE_MPI

libtest_broker_la_LDFLAGS += ${MPILIBS}
StartAlgorithm_LDFLAGS += ${MPILIBS}

endif

