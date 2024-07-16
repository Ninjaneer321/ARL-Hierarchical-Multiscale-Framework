dnl
dnl check for required header files
dnl
AC_DEFUN([ARL_REQUIRED_HEADERS], [
  AC_CHECK_HEADERS([$1], [], [AC_ERROR([$ac_header header required but not present])], [$2]) 
])