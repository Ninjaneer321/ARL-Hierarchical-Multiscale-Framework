dnl
dnl Check for the eigen library.
dnl
AC_DEFUN([ARL_CHECK_EIGEN3],
[
dnl
dnl
dnl
AC_ARG_WITH([eigen3],
	    [  --with-eigen3[=DIR]            root directory of eigen3 installation],
            [ eigen3_path="/usr/local"
              case "$withval" in 
              no)
              AC_MSG_RESULT([no])
              AC_ERROR([Eigen3 library required.])
              ;;
              yes)
              ;;
              *)
              eigen3_path=${withval}
              ;;
              esac])
dnl
dnl set path to an eigen3 header to search for
dnl
eigen3_config_file_name="${eigen3_path}/include/eigen3/Eigen/Sparse"
dnl
dnl check if the eigen3_config_file_name exists
dnl
AC_CHECK_FILE(${eigen3_config_file_name}, 
              [
                dnl
                dnl
                dnl
                AC_MSG_CHECKING([checking for eigen3 library])
                dnl
                dnl obtain asf CPPFLAGS for eigen3
                dnl
                EIGEN3_CXXFLAGS="-I${eigen3_path}/include/eigen3"
                dnl
                dnl
                dnl
                AM_CONDITIONAL([HAVE_EIGEN3], true)
                AC_DEFINE([HAVE_PKG_eigen3], [], [Define if eigen3 headers are found])
                AC_SUBST(EIGEN3_CXXFLAGS)
                AC_MSG_RESULT([yes])
              ],
              [
                AC_MSG_WARN([Eigen/Sparse not found in ${eigen3_path}/include/eigen3])
                AM_CONDITIONAL([HAVE_EIGEN3], false)
              ])
])
