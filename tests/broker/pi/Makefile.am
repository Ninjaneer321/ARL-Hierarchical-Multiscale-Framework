ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

lib_LTLIBRARIES = libtest_broker.la
bin_PROGRAMS = StartAlgorithm

libtest_broker_la_SOURCES = PiArgument.cc                                      \
                            PicalcOutputFilter.cc                              \
                            PicalcInputFilter.cc                               \
                            PiValue.cc     
libtest_broker_la_LDFLAGS = ${top_builddir}/base/libhmsbase.la                 \
                            ${top_builddir}/comm/libhmscomm.la                 \
                            ${top_builddir}/monitor/libhmsmonitor.la           \
                            ${top_builddir}/broker/libhmsbroker.la             \
                            ${BOOST_LDFLAGS}

StartAlgorithm_SOURCES = StartAlgorithm.cc
StartAlgorithm_LDFLAGS = ${top_builddir}/tests/broker/pi/libtest_broker.la     \
                         ${top_builddir}/base/libhmsbase.la                    \
                         ${top_builddir}/comm/libhmscomm.la                    \
                         ${top_builddir}/monitor/libhmsmonitor.la              \
                         ${top_builddir}/broker/libhmsbroker.la                \
                         ${BOOST_LDFLAGS}
StartAlgorithm_DEPENDENCIES = libtest_broker.la

if COMPILE_MPI

bin_PROGRAMS += cpi

cpi_SOURCES = cpi.c

libtest_broker_la_LDFLAGS += ${MPILIBS}
StartAlgorithm_LDFLAGS += ${MPILIBS}
cpi_LDFLAGS = ${MPILIBS}

endif

