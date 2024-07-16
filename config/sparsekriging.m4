dnl
dnl Check for the sparsekriging library.
dnl
AC_DEFUN([ARL_CHECK_SPARSEKRIGING],
[
dnl
dnl
dnl
AC_ARG_WITH([sparsekriging],
	    [  --with-sparsekriging[=DIR]            root directory of sparsekriging installation],
            [ sparsekriging_path="/usr/local"
              case "$withval" in 
              no)
              AC_MSG_RESULT([no])
              AC_ERROR([Sparse Kriging library required.])
              ;;
              yes)
              ;;
              *)
              sparsekriging_path=${withval}
              ;;
              esac])
dnl
dnl set path to the asf-config file in ${asf_path}/bin
dnl directory
dnl
sparsekriging_config_file_name="${sparsekriging_path}/include/GP_sparse_chol.h"
dnl
dnl check if the sparsekriging_config_file_name exists
dnl
AC_CHECK_FILE(${sparsekriging_config_file_name}, 
              [
                dnl
                dnl
                dnl
                AC_MSG_CHECKING([checking for sparse kriging library])
                dnl
                dnl obtain asf CPPFLAGS and LIBS from asf config file
                dnl
                SPARSEKRIGING_CXXFLAGS="-I${sparsekriging_path}/include -I/p/work2/projects/hms/dlib/include"
                SPARSEKRIGING_LIBS="-L${sparsekriging_path}/lib -lsparsekriging -L/p/work2/projects/hms/dlib/lib64 -ldlib"
                dnl
                dnl
                dnl
                AM_CONDITIONAL([HAVE_SPARSEKRIGING], true)
                AC_DEFINE([HAVE_PKG_sparsekriging], [], [Define if sparse kriging library is found])
                AC_SUBST(SPARSEKRIGING_CXXFLAGS)
                AC_SUBST(SPARSEKRIGING_LIBS)
                AC_MSG_RESULT([yes])
              ],
              [
                AC_MSG_WARN([GP_sparse_chol.h not found in ${sparsekriging_path}/include])
                AM_CONDITIONAL([HAVE_SPARSEKRIGING], false)
              ])
])
