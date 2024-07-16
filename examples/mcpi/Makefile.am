ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir} ${HMS_CXXFLAGS}

lib_LTLIBRARIES = libhms_mc_pi.la
bin_PROGRAMS = main

hms_mc_piincludedir = ${includedir}

libhms_mc_pi_la_SOURCES = cxx/PiArgument.cc \
                          cxx/PiInputFilter.cc \
                          cxx/PiOutputFilter.cc \
                          cxx/PiValue.cc

libhms_mc_pi_la_LDFLAGS = ${HMS_LIBS} 

main_SOURCES = cxx/main.cc
main_LDFLAGS = libhms_mc_pi.la 
main_DEPENDENCIES = libhms_mc_pi.la
