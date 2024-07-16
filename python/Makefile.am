if ENABLE_PYTHON_BINDINGS

ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir} ${PYTHON_CPPFLAGS}
AM_CXXFLAGS = 

pyexec_LTLIBRARIES = _hms.la

nodist__hms_la_SOURCES = HMSPython.cc

_hms_la_LDFLAGS = ${top_builddir}/broker/libhmsbroker.la                       \
                  ${top_builddir}/base/libhmsbase.la                           \
                  ${top_builddir}/exception/libhmsexception.la                 \
                  ${BOOST_LDFLAGS} ${PYTHON_LIBS}                              \
                  -avoid-version -module -shared -export-dynamic

nodist_python_PYTHON = hms.py

if COMPILE_MPI

_hms_la_LDFLAGS += ${MPILIBS}

endif

endif
