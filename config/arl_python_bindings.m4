dnl
dnl ARL_CHECK_PYTHON_BINDINGS() macro
dnl
AC_DEFUN([ARL_CHECK_PYTHON_BINDINGS],
  [AC_MSG_CHECKING([whether to enable building python bindings])
    AC_ARG_ENABLE([python-bindings],
                  [AC_HELP_STRING([--enable-python-bindings], [Enable python bindings])],
                  [AC_MSG_RESULT(yes)] 
                  [AC_DEFINE(ENABLE_PYTHON_BINDINGS,
                             [1],
                             [Define if python bindings are enabled.])]
                  [enable_python_bindings=yes],
                  [AC_MSG_RESULT(no)]
    )
  ]
  AM_CONDITIONAL([ENABLE_PYTHON_BINDINGS], [test "x$enable_python_bindings" = "xyes"])
)

