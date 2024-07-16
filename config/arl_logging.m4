dnl
dnl ARL_CHECK_LOGGING() macro
dnl
AC_DEFUN([ARL_CHECK_LOGGING],
  [AC_MSG_CHECKING([whether to disable logging])
    AC_ARG_ENABLE([logging],
                  [AC_HELP_STRING([--disable-logging], [Disable logging])],
                  [AC_MSG_RESULT(yes)],
                  [AC_DEFINE(ENABLE_LOGGING, 
                             [1], 
                             [Define if logging is enabled.])
                   AC_MSG_RESULT(no)
                   AX_BOOST_LOG
                   AS_IF([test "x$have_boost_log" == xno],
                         [AC_MSG_ERROR("Cannot find boost log library")]
                   )
                   AX_BOOST_LOG_SETUP
                   AS_IF([test "x$have_boost_log_setup" == xno],
                         [AC_MSG_ERROR("Cannot find boost log setup library")]
                   )
                   AX_BOOST_SYSTEM
                   AS_IF([test "x$have_boost_system" == xno],
                         [AC_MSG_ERROR("Cannot find boost system library")]
                   )
                   AX_BOOST_THREAD
                   AS_IF([test "x$have_boost_thread" == xno],
                         [AC_MSG_ERROR("Cannot find boost thread library")]
                   )
      ]
    )
  ]
)
