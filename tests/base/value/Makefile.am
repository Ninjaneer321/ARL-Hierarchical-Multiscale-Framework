ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

bin_PROGRAMS = ScalarValue 
ScalarValue_SOURCES = main.cc                                                  \
                      ScalarValue.cc
ScalarValue_LDFLAGS = ${top_builddir}/base/libhmsbase.la                       \
                      ${BOOST_LDFLAGS}
