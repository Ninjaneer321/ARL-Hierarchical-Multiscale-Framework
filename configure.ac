dnl
dnl
dnl
AC_INIT([HMS], [1.0])
AC_CONFIG_MACRO_DIR([config])
AC_CONFIG_SRCDIR([base/Model.h])
AC_CONFIG_AUX_DIR(./bin)
AM_INIT_AUTOMAKE([foreign subdir-objects])
AM_MAINTAINER_MODE([])
AC_CANONICAL_HOST([])
AM_CONFIG_HEADER(arlhms_config.h)
AC_PROG_MAKE_SET([])
dnl
dnl enable debugging
dnl
ARL_SET_DEBUG([])
dnl
dnl set compilers
dnl
ARL_PROG_CXX([])
AC_LANG([C++])
dnl
dnl enable libtool builds
dnl
AM_DISABLE_SHARED([])
AM_PROG_LIBTOOL([])
dnl
dnl ASF
dnl
ARL_CHECK_ASF([])
dnl
dnl Sparse Kriging
dnl
AM_CONDITIONAL([HAVE_SPARSEKRIGING], false)
ARL_CHECK_SPARSEKRIGING([])
dnl
dnl Eigen3
dnl
AM_CONDITIONAL([HAVE_EIGEN3], false)
ARL_CHECK_EIGEN3([])
AM_COND_IF([HAVE_SPARSEKRIGING], [AX_CXX_COMPILE_STDCXX([11], [], [mandatory])])
AM_COND_IF([HAVE_SPARSEKRIGING], [AM_COND_IF([HAVE_EIGEN3], [], [AC_MSG_ERROR("Eigen3 required when building with sparse kriging package")])])
dnl
dnl Boost
dnl
AX_BOOST_BASE
AX_BOOST_SERIALIZATION
AX_BOOST_FILESYSTEM
CPPFLAGS="$CPPFLAGS -DBOOST_ALL_DYN_LINK ${BOOST_CPPFLAGS}"
dnl
dnl Python
dnl
AX_PYTHON_DEVEL
AM_PATH_PYTHON
dnl
dnl check whether to compile python bindings
dnl
ARL_CHECK_PYTHON_BINDINGS()
dnl
dnl check whether to compile with pyAS package
dnl
ARL_CHECK_PYAS()
dnl
dnl check logging
dnl
ARL_CHECK_LOGGING()
BOOST_LDFLAGS="$BOOST_LDFLAGS $BOOST_LOG_LIB $BOOST_LOG_SETUP_LIB $BOOST_SYSTEM_LIB $BOOST_THREAD_LIB $BOOST_SERIALIZATION_LIB $BOOST_FILESYSTEM_LIB"
dnl
dnl check for headers
dnl
AC_CHECK_HEADERS([assert.h])
AC_CHECK_HEADERS([cassert climits cmath cstdlib cstddef cstring iomanip iosfwd])
AC_CHECK_HEADERS([iosfwd iostream utility fstream])
ARL_REQUIRED_HEADERS([algorithm arpa/inet.h boost/shared_ptr.hpp ctime cerrno])
ARL_REQUIRED_HEADERS([cstdio deque dlfcn.h exception functional list map])
ARL_REQUIRED_HEADERS([netdb.h netinet/in.h numeric pthread.h set sstream])
ARL_REQUIRED_HEADERS([string sys/socket.h vector signal.h sys/un.h sys/wait.h])
ARL_REQUIRED_HEADERS([iterator pwd.h])
dnl 
dnl
dnl check for libraries
dnl
dnl
dnl MPI
dnl
ARL_CHECK_MPI([])dnl
AM_CONDITIONAL([COMPILE_MPI], [test x != x"$MPILIBS"])dnl
dnl
dnl create outputs
dnl
AC_CONFIG_FILES([Makefile
                 adaptive_sampling/Makefile
                 base/Makefile
                 broker/Makefile
                 comm/Makefile
                 exception/Makefile 
                 monitor/Makefile
		 python/Makefile
                 scheduler/Makefile
                 tests/Makefile
                 tests/adaptive_sampling/Makefile
                 tests/adaptive_sampling/adaptive_sampling/Makefile 
                 tests/adaptive_sampling/interpolation_composite/Makefile 
                 tests/adaptive_sampling/interpolation_database/Makefile
                 tests/adaptive_sampling/interpolation_dpd/Makefile
                 tests/adaptive_sampling/kde/Makefile
                 tests/adaptive_sampling/multiplexer/Makefile
                 tests/base/Makefile
                 tests/base/lsCommand/Makefile
                 tests/base/machinefile/Makefile
                 tests/base/queue/Makefile
                 tests/base/queuePerf/Makefile
                 tests/base/resourceList/Makefile
                 tests/base/resourceListGenerator/Makefile
                 tests/base/resourceManager/Makefile
                 tests/base/stateDB/Makefile
                 tests/base/value/Makefile
                 tests/broker/Makefile
                 tests/broker/pi/Makefile
                 tests/broker/perf/Makefile
		 tests/broker/work_steal/Makefile
                 tests/comm/Makefile
                 tests/monitor/Makefile
                 tests/monitor/basicMonitor/Makefile
                 tests/warehouse/Makefile
                 config/hms-config])
AC_OUTPUT([])
