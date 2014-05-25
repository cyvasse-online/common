AC_INIT([cyvmath],[0.1],[mail@jonasplatte.de])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.11 foreign])
AM_SILENT_RULES([yes])
AC_PROG_CXX
AC_LANG(C++)

PKG_CHECK_MODULES([CPPUNIT], [cppunit], [have_cppunit=yes], [have_cppunit=no])
AM_CONDITIONAL([HAVE_CPPUNIT], [test "$have_cppunit" = "yes"])

AC_CONFIG_FILES([
	Makefile
	unit-tests/Makefile
])

AC_OUTPUT
