dnl
dnl enable debug build 
dnl
AC_DEFUN([ARL_SET_DEBUG],
[
AH_TEMPLATE([DEBUG], [Define if compiling for debug])
AH_TEMPLATE([DEBUG_NO_INLINE], [Define if inlinining should be disabled])
dnl
dnl enable debugging flag
dnl
AC_MSG_CHECKING([whether to enable debugging])
debug_defined=no
AC_ARG_ENABLE([debug],
[  --enable-debug          enable debugging],
[ case "$enableval" in
  yes)
    AC_MSG_RESULT(yes)
    AC_DEFINE(DEBUG)
    debug_defined=yes
    ;;
  *)
    AC_MSG_RESULT(no)
    ;;
  esac ], [AC_MSG_RESULT(no)])
dnl
dnl enable setting of DEBUG_NO_INLINE
dnl
AC_MSG_CHECKING([whether to disable function inlining])
AC_ARG_ENABLE([debug_no_inline],
[  --enable-debug-no-inline	 disable function inlining],
[ case "$enableval" in
  yes)
    AC_MSG_RESULT(yes)
    AC_DEFINE(DEBUG_NO_INLINE)
    ;;
  *)
    AC_MSG_RESULT(no)
    ;;
  esac ], [AC_MSG_RESULT(no)])
dnl
dnl
dnl
])
