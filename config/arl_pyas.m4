dnl
dnl Check for the numpy header required for the pyAS package
dnl
AC_DEFUN([ARL_CHECK_PYAS],
[
AC_MSG_CHECKING([whether to build pyAS surrogate model])
dnl
dnl
dnl
AC_ARG_WITH([pyas],
            [AC_HELP_STRING([--with-pyas], [Include pyAS surrogate model package])],
            [with_pyas='$withval'],
            [AC_MSG_RESULT(no)
             with_pyas=no])
dnl
dnl Only additional requirement for pyAS is the numpy/arrayobjec.h
dnl header.
dnl
AM_CONDITIONAL([HAVE_PYAS], false)

if test "x$with_pyas" != "xno"; then
     
     AC_MSG_RESULT(yes)
             
     numpy_include_dir=`$PYTHON -c "import numpy; \
                      print(numpy.get_include())"`

    pyas_numpy_header="${numpy_include_dir}/numpy/arrayobject.h"

    AC_CHECK_FILE(${pyas_numpy_header},
                  [ AC_MSG_CHECKING([checking numpy header for pyAS])
                    PYTHON_CPPFLAGS="${PYTHON_CPPFLAGS} -I${numpy_include_dir}"
                    AM_CONDITIONAL([HAVE_PYAS], true)
                    AC_DEFINE([HAVE_PKG_pyas], 1, [Define if pyas package is found])
                    AC_MSG_RESULT([yes]) ],
                  [ AC_MSG_WARN([numpy header for pyas not found in ${numpy_include_idr}])
                    AM_CONDITIONAL([HAVE_PYAS], false)
                  ])
fi

])
