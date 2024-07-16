ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

bin_PROGRAMS = Queue 
Queue_SOURCES = main.cc                                                  
Queue_LDFLAGS = ${top_builddir}/base/libhmsbase.la                             \
                ${BOOST_LDFLAGS}
