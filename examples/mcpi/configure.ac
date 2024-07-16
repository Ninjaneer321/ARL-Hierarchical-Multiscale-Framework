dnl
dnl
dnl
AC_INIT([HMS_MC_PI],[1.0])
AC_CONFIG_MACRO_DIR([config])
AC_CONFIG_SRCDIR([unit_circle_indicator_function.py])
AC_CONFIG_AUX_DIR(./bin)
AM_INIT_AUTOMAKE([subdir-objects])
AM_MAINTAINER_MODE([])
AC_CANONICAL_HOST([])
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
AX_CXX_COMPILE_STDCXX([11], [], [mandatory])
dnl
dnl enable libtool builds
dnl
AC_DISABLE_SHARED([])
LT_INIT([])
dnl
dnl HMS
dnl
ARL_CHECK_HMS([])
dnl
dnl create outputs
dnl
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
