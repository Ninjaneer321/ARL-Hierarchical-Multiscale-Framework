dnl
dnl Check for the asf library.
dnl
AC_DEFUN([ARL_CHECK_ASF],
[
dnl
dnl
dnl
AC_ARG_WITH([asf],
	    [  --with-asf[=DIR]            root directory of asf installation],
            [ asf_path="/usr/local"
              case "$withval" in 
              no)
              AC_MSG_RESULT([no])
              AC_ERROR([ASF library required.])
              ;;
              yes)
              ;;
              *)
              asf_path=${withval}
              ;;
              esac])
dnl
dnl set path to the asf-config file in ${asf_path}/bin
dnl directory
dnl
asf_config_file_name="${asf_path}/bin/asf-config"
dnl
dnl check if the asf_config_file_name exists
dnl
AC_CHECK_FILE(${asf_config_file_name}, 
              [
                dnl
                dnl
                dnl
                AC_MSG_CHECKING([checking for asf library])
                dnl
                dnl obtain asf CPPFLAGS and LIBS from asf config file
                dnl
                ASF_CXXFLAGS=`${asf_config_file_name} --cxxflags`
                ASF_LIBS=`${asf_config_file_name} --libs`
                dnl
                dnl
                dnl
                AM_CONDITIONAL([HAVE_ASF], true)
                AC_DEFINE([HAVE_PKG_asf], [], [Define if asf library is found])
                AC_SUBST(ASF_CXXFLAGS)
                AC_SUBST(ASF_LIBS)
                AC_MSG_RESULT([yes])
              ],
              [
                AC_MSG_WARN([asf-config not found in ${asf_path}/bin])
                AM_CONDITIONAL([HAVE_ASF], false)
              ])
])
