dnl
dnl Set C++ compiler and compiler flags
dnl
dnl reset the order of checks for C++ to native then GNU
dnl
AC_DEFUN([ARL_PROG_CXX],
[
AC_REQUIRE([ARL_SET_DEBUG])
AC_PROG_CXX(pgCC icpc CC aCC cxx xlC_r c++ gcc cc++ cl g++ clang++)dnl
AC_PROG_CXXCPP([])
dnl
dnl arch dependend
dnl

case "$host" in
  *-hp-hpux*)
  if test -z "$GCC"; then
    if test $debug_defined = "yes"; then
      CXXFLAGS="-AA -g -D_HPUX_SOURCE -D_RWSTD_MULTI_THREAD -D_REENTRANT"
    else
      CXXFLAGS="-AA -O -D_HPUX_SOURCE -D_RWSTD_MULTI_THREAD -D_REENTRANT"
    fi
  else
    if test $debug_defined = "yes"; then
      CXXFLAGS="-g"
    else
      CXXFLAGS="-O"
    fi
  fi
  ;;

  *-dec-osf*)
  if test $debug_defined = "yes"; then
    CXXFLAGS="-g"
  else
    CXXFLAGS="-O"
  fi
  ;;

  *-sun-solaris*)
  if test $debug_defined = "yes"; then
    CXXFLAGS="-g"
  else
    CXXFLAGS="-O"
  fi
  ;;

  *-sgi-irix*)
  if test -z "$GCC"; then
    if test $debug_defined = "yes"; then
      CXXFLAGS="-g -LANG:std"
    else
      CXXFLAGS="-O -LANG:std"
    fi
  else
    if test $debug_defined = "yes"; then
      CXXFLAGS="-g"
    else
      CXXFLAGS="-O"
    fi
  fi
  ;;

  *-ibm-aix*)
  if test -z "$GCC"; then
    if test $debug_defined = "yes"; then
      CXXFLAGS="-g -qthreaded"
    else
      CXXFLAGS="-O -qthreaded"
    fi
    CPPFLAGS="-qlanglvl=extended"
  else
    if test $debug_defined = "yes"; then
      CXXFLAGS="-g"
    else
      CXXFLAGS="-O"
    fi
  fi
  ;;

  alpha*-linux-*)
  if test -z "$GXX"; then
    if test $debug_defined = "yes"; then
      CXXFLAGS="-std strict_ansi -g -w0"
    else
      CXXFLAGS="-std strict_ansi -O4 -fast -w0"
    fi
  else
    if test $debug_defined = "yes"; then
      CXXFLAGS="-g -Wall -mieee"
    else
      CXXFLAGS="-O3 -Wall -mieee"
    fi
  fi
  ;;
  
  *86*-linux-*)
  if test -z "$GXX"; then
    if test $debug_defined = "yes"; then
      CXXFLAGS="-g"
    else
      CXXFLAGS="-g -O2"
    fi
  else
    if test $debug_defined = "yes"; then
      CXXFLAGS="-g -Wall"
    else
      CXXFLAGS="-g -O2 -Wall"
    fi
  fi
  ;;

  *86*-freebsd*)
  if test -z "$GXX"; then
    if test $debug_defined = "yes"; then
      CXXFLAGS="-g"
    else
      CXXFLAGS="-O3"
    fi
  else
    if test $debug_defined = "yes"; then
      CXXFLAGS="-g -Wall"
    else
      CXXFLAGS="-O3 -Wall"
    fi
  fi
  ;;

  *86*-apple-darwin*)
  if test -z "$GXX"; then
    if test $debug_defined = "yes"; then
      CXXFLAGS="-g"
    else
      CXXFLAGS="-O3"
    fi
  else
    if test $debug_defined = "yes"; then
      CXXFLAGS="-g -Wall -ftemplate-depth-1024"
    else
      CXXFLAGS="-O3 -Wall -ftemplate-depth-1024"
    fi
  fi
  ;;
  
  *)
  ;;
esac
])
