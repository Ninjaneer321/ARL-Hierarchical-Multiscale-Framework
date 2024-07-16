ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir} ${PYTHON_CPPFLAGS}
AM_CXXFLAGS = ${ASF_CXXFLAGS} ${SPARSEKRIGING_CXXFLAGS} ${EIGEN3_CXXFLAGS} 

lib_LTLIBRARIES = libhmsadaptive_sampling.la
bin_PROGRAMS = StartAdaptiveSampling

hmsasincludedir = ${includedir}/hms/adaptive_sampling

hmsasinclude_HEADERS = AdaptiveSampling.h                                      \
                       AdaptiveSamplingFactory.h                               \
                       AdaptiveSamplingLauncher.h                              \
                       AdaptiveSamplingLoadScheduler.h                         \
                       AdaptiveSamplingPerformanceScheduler.h                  \
                       AdaptiveSamplingRoundRobinScheduler.h                   \
                       AdaptiveSamplingScheduler.h                             \
                       InterpolationDatabase.h                                 \
                       InterpolationDatabaseInputFilter.h                      \
                       InterpolationDatabaseInputFilter_t.cc                   \
                       InterpolationDatabaseOutputFilter.h                     \
                       InterpolationDatabaseOutputFilter_t.cc                  \
                       KrigingInterpolationDatabase.h                          \
                       MonteCarloGenerator.h                                   \
                       NullInterpolationDatabase.h                             \
                       NullGenerator.h                                         \
                       PyasInterpolationDatabase.h                             \
                       SpeculativeGenerator.h 

libhmsadaptive_sampling_la_SOURCES = AdaptiveSampling.cc                       \
                                     AdaptiveSamplingFactory.cc                \
                                     AdaptiveSamplingLauncher.cc               \
                                     AdaptiveSamplingLoadScheduler.cc          \
                                     AdaptiveSamplingPerformanceScheduler.cc   \
                                     AdaptiveSamplingRoundRobinScheduler.cc    \
                                     AdaptiveSamplingScheduler.cc              \
                                     InterpolationDatabase.cc                  \
                                     InterpolationDatabaseInputFilter.cc       \
                                     InterpolationDatabaseOutputFilter.cc      \
                                     KrigingInterpolationDatabase.cc           \
                                     MonteCarloGenerator.cc                    \
                                     NullInterpolationDatabase.cc              \
                                     NullGenerator.cc                          \
                                     PyasInterpolationDatabase.cc              \
                                     SpeculativeGenerator.cc 

libhmsadaptive_sampling_la_LDFLAGS = ${top_builddir}/broker/libhmsbroker.la    \
                                     ${top_builddir}/base/libhmsbase.la        \
                                     ${top_builddir}/exception/libhmsexception.la \
                                     ${BOOST_LDFLAGS} \
                                     ${ASF_LIBS} \
                                     ${PYTHON_LIBS}


StartAdaptiveSampling_SOURCES = StartAdaptiveSampling.cc
StartAdaptiveSampling_LDFLAGS = ${top_builddir}/adaptive_sampling/libhmsadaptive_sampling.la \
                                ${top_builddir}/base/libhmsbase.la             \
                                ${top_builddir}/exception/libhmsexception.la   \
                                ${BOOST_LDFLAGS} ${ASF_LIBS}
StartAdaptiveSampling_DEPENDENCIES = libhmsadaptive_sampling.la

if COMPILE_MPI

StartAdaptiveSampling_LDFLAGS += ${MPILIBS}
libhmsadaptive_sampling_la_LDFLAGS += ${MPILIBS}

endif
