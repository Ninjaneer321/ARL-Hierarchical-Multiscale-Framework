dnl @synopsis ACX_MPI([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
dnl
dnl This macro tries to find out how to compile programs that
dnl use MPI (Message Passing Interface), a standard API for
dnl parallel process communication (see http://www-unix.mcs.anl.gov/mpi/)
dnl
dnl On success, it sets the MPICC, MPICXX, or MPIF77 output variable to
dnl the name of the MPI compiler, depending upon the current language.
dnl (This may just be $CC/$CXX/$F77, but is more often something like
dnl mpicc/mpiCC/mpif77.)  It also sets MPILIBS to any libraries that are
dnl needed for linking MPI (e.g. -lmpi, if a special MPICC/MPICXX/MPIF77
dnl was not found).
dnl
dnl If you want to compile everything with MPI, you should set:
dnl
dnl     CC="$MPICC" #OR# CXX="$MPICXX" #OR# F77="$MPIF77"
dnl     LIBS="$MPILIBS $LIBS"
dnl
dnl The user can force a particular library/compiler by setting the
dnl MPICC/MPICXX/MPIF77 and/or MPILIBS environment variables.
dnl
dnl ACTION-IF-FOUND is a list of shell commands to run if an MPI
dnl library is found, and ACTION-IF-NOT-FOUND is a list of commands
dnl to run it if it is not found.  If ACTION-IF-FOUND is not specified,
dnl the default action will define HAVE_MPI.
dnl
dnl @version $Id: acx_mpi.m4,v 1.2 2003/01/03 04:50:41 knap Exp $
dnl @author Steven G. Johnson <stevenj@alum.mit.edu>

AC_DEFUN([ACX_MPI], [
AC_PREREQ(2.50) dnl for AC_LANG_CASE

AC_LANG_CASE([C], [
	AC_REQUIRE([AC_PROG_CC])
	AC_CHECK_PROGS(MPICC, mpcc_r mpicc hcc mpcc mpxlc, $CC)
	acx_mpi_save_CC="$CC"
	CC="$MPICC"
	AC_SUBST(MPICC)
],
[C++], [
	AC_REQUIRE([AC_PROG_CXX])
	AC_CHECK_PROGS(MPICXX, mpCC_r mpiCC mpCC, $CXX)
	acx_mpi_save_CXX="$CXX"
	CXX="$MPICXX"
	AC_SUBST(MPICXX)
],
[Fortran 77], [
	AC_REQUIRE([AC_PROG_F77])
	AC_CHECK_PROGS(MPIF77, mpxlf95_r mpxlf90_r mpxlf_r mpif77 hf77 mpxlf mpf77 mpif90 mpf90 mpxlf90 mpxlf95 mpxlf_r, $F77)
	acx_mpi_save_F77="$F77"
	F77="$MPIF77"
	AC_SUBST(MPIF77)
])

dnl 
dnl hack to get AIX MPI libraries 
dnl

case "$host" in
  *-ibm-aix*)
    if test -d  /usr/lpp/ppe.poe/lib; then
      MPILIBS="-binitfini:poe_remote_main -R/usr/lpp/ppe.poe/lib/threads -R/usr/lpp/ppe.poe/lib -R/usr/lpp/ppe.poe/lib/ip -L/usr/lpp/ppe.poe/lib -lmpi_r -lvtd_r"
      CFLAGS="$CFLAGS -I/usr/lpp/ppe.poe/include"
      CXXFLAGS="$CXXFLAGS -I/usr/lpp/ppe.poe/include"
    fi	
    ;;
esac

dnl
dnl
dnl

if test x = x"$MPILIBS"; then
	AC_LANG_CASE([C], [AC_CHECK_FUNC(MPI_Init, [MPILIBS=" "])],
		[C++], [AC_CHECK_FUNC(MPI_Init, [MPILIBS=" "])],
		[Fortran 77], [AC_MSG_CHECKING([for MPI_Init])
			AC_TRY_LINK([],[      call MPI_Init], [MPILIBS=" "
				AC_MSG_RESULT(yes)], [AC_MSG_RESULT(no)])])
fi
if test x = x"$MPILIBS"; then
	AC_CHECK_LIB(mpi, MPI_Init, [MPILIBS="-lmpi"])
fi

dnl We have to use AC_TRY_COMPILE and not AC_CHECK_HEADER because the
dnl latter uses $CPP, not $CC (which may be mpicc).
AC_LANG_CASE([C], [if test x != x"$MPILIBS"; then
	AC_MSG_CHECKING([for mpi.h])
	AC_TRY_COMPILE([#include <mpi.h>],[],[AC_MSG_RESULT(yes)], [MPILIBS=""
		AC_MSG_RESULT(no)])
fi],
[C++], [if test x != x"$MPILIBS"; then
	AC_MSG_CHECKING([for mpi.h])
	AC_TRY_COMPILE([#include <mpi.h>],[],[AC_MSG_RESULT(yes)], [MPILIBS=""
		AC_MSG_RESULT(no)])
fi])

AC_LANG_CASE([C], [CC="$acx_mpi_save_CC"],
	[C++], [CXX="$acx_mpi_save_CXX"],
	[Fortran 77], [F77="$acx_mpi_save_F77"])

AC_SUBST(MPILIBS)

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x = x"$MPILIBS"; then
        $2
        :
else
        ifelse([$1],,[AC_DEFINE(HAVE_MPI,1,[Define if you have the MPI library.])],[$1])
        :
fi
])dnl ACX_MPI
