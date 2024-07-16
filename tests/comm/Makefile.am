ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

bin_PROGRAMS = SendReceive FunctionEval
SendReceive_SOURCES = sendreceive.cc
SendReceive_LDFLAGS = ${top_builddir}/base/libhmsbase.la                       \
                      ${top_builddir}/comm/libhmscomm.la                       \
                      ${BOOST_LDFLAGS} -lpthread

FunctionEval_SOURCES = functioneval.cc                                         \
                       ScalarArgument.cc                                       \
                       ScalarValue.cc 
FunctionEval_LDFLAGS = ${top_builddir}/base/libhmsbase.la                      \
                       ${top_builddir}/comm/libhmscomm.la                      \
                       ${BOOST_LDFLAGS} -lpthread
