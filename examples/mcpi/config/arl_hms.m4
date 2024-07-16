dnl
dnl Check for the hms library.
dnl
AC_DEFUN([ARL_CHECK_HMS],
[
dnl
dnl
dnl
AC_ARG_WITH([hms],
	    [  --with-hms[=DIR]            root directory of hms installation],
            [ hms_path="/usr/local"
              case "$withval" in 
              no)
              AC_MSG_RESULT([no])
              AC_ERROR([HMS library required.])
              ;;
              yes)
              ;;
              *)
              hms_path=${withval}
              ;;
              esac])
dnl
dnl set path to the hms-config file in ${hms_path}/bin
dnl directory
dnl
hms_config_file_name="${hms_path}/bin/hms-config"
dnl
dnl check if the hms_config_file_name exists
dnl
AC_CHECK_FILE(${hms_config_file_name}, 
              [],
              AC_ERROR([hms-config not found in ${hms_path}/bin]))
dnl
dnl
dnl
AC_MSG_CHECKING([checking for hms library])
dnl
dnl obtain hms CPPFLAGS and LIBS from hms config file
dnl
HMS_CXXFLAGS=`${hms_config_file_name} --cxxflags`
HMS_LIBS=`${hms_config_file_name} --libs`
dnl
dnl
dnl
AC_DEFINE([HAVE_PKG_hms], [], [Define if hms library is found])
AC_SUBST(HMS_CXXFLAGS)
AC_SUBST(HMS_LIBS)
AC_MSG_RESULT([yes])
])
