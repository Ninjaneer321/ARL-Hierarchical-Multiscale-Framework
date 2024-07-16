ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

bin_PROGRAMS = Warehouse 
Warehouse_SOURCES = main.cc                                                  \
                    ScalarValue.cc
Warehouse_LDFLAGS = ${top_builddir}/base/libhmsbase.la                       \
                    ${top_builddir}/comm/libhmscomm.la                       \
                    ${BOOST_LDFLAGS} -lpthread
