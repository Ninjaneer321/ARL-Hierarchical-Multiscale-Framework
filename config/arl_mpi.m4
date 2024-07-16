dnl
dnl ARL_CHECK_MPI() macro
dnl The following logic is used:
dnl i)  when --with-mpi is called without arguments rely on specialized
dnl     MPI-aware compilers (i.e. mpicc, mpiCC, etc.) or try to locate the
dnl     MPI headers and library in /usr/lib
dnl ii) when called with a path, assume that headers are in $path/include
dnl     and libraries in $path/lib
dnl

AC_DEFUN([ARL_CHECK_MPI],
[ AC_MSG_CHECKING([whether to add support for MPI])
  AC_ARG_WITH([mpi],
[AC_HELP_STRING([--with-mpi[[=PATH]]],[use MPI (default is no)])],
[ case "$withval" in
  no)
    AC_MSG_RESULT(no)
    ;;
  yes)
    ACX_MPI()
dnl
dnl check if mpi.h contains MPI_NO_CPPBIND
dnl
    AC_EGREP_HEADER(MPI_NO_CPPBIND, [mpi.h], [], [ AC_DEFINE(MPI_NO_CPPBIND)
	AH_TEMPLATE([MPI_NO_CPPBIND], [Define when SGI MPI implementation is used]) ])
    ;;
  *)
    if test '!' -f $withval/include/mpi.h; then
      AC_ERROR(MPI headers not found in $withval/include: please supply the correct path.)
    fi
    AC_MSG_RESULT(Assuming MPI headers and libraries are in $withval/include and $withval/lib.)dnl
    AC_DEFINE(HAVE_MPI,1,[Define if you have the MPI library.])
    CFLAGS="$CFLAGS -I$withval/include"
    CXXFLAGS="$CXXFLAGS -I$withval/include"
dnl
dnl try to locate the MPI library; choices are -lmpi_r, -lmpi, -lmpich
dnl
    if test x = x"$MPILIBS"; then
	ares_save_LDFLAGS="$LDFLAGS"
	LDFLAGS="$LDFLAGS -L$withval/lib"
	AC_CHECK_LIB(mpi, MPI_Init, [MPILIBS="-L$withval/lib -lmpi"])dnl
        LDFLAGS="$LDFLAGS -lmpi"
	AC_CHECK_LIB(mpi_cxx, ompi_mpi_cxx_throw_exception, [MPILIBS="$MPILIBS -lmpi_cxx"])dnl
	LDFLAGS="$ares_save_LDFLAGS"
    fi
    if test x = x"$MPILIBS"; then
	ares_save_LDFLAGS="$LDFLAGS"
	LDFLAGS="$LDFLAGS -L$withval/lib"
	AC_CHECK_LIB(mpi, MPI_Init, [MPILIBS="-L$withval/lib -lmpi"])dnl
	LDFLAGS="$ares_save_LDFLAGS"
    fi
    if test x = x"$MPILIBS"; then
	ares_save_LDFLAGS="$LDFLAGS"
	LDFLAGS="$LDFLAGS -L$withval/lib -lmpichcxx -lpmpich -lmpich -lopa -lmpl"
	AC_CHECK_LIB(mpich, MPI_Init, [MPILIBS="-L$withval/lib -lmpichcxx -lpmpich -lmpich -lopa -lmpl"])dnl
	LDFLAGS="$ares_save_LDFLAGS"
    fi
    AC_SUBST(MPILIBS)dnl
dnl
dnl
dnl
    if test x = x"$MPILIBS"; then
	$2
	:
    else
	ifelse([$1],,[AC_DEFINE(HAVE_MPI,1,[Define if you have the MPI library.])],[$1])
        :
    fi
  esac ],
  [AC_MSG_RESULT(no)]
)])dnl AC_CHECK_MPI
