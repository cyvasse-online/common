AC_INIT([cyvasse-common],[0.1],[mail@jonasplatte.de])
AC_CONFIG_AUX_DIR([.])
AM_INIT_AUTOMAKE([1.11 foreign])
AM_SILENT_RULES([yes])
AC_LANG(C++)

AC_PROG_CXX
AC_PROG_RANLIB
AM_PROG_AR

## only build cyvdb when compiling natively...
## This is probably not the best way to check whether emconfigure is used
AS_IF([test -n "$EMMAKEN_JUST_CONFIGURE"], [
	AM_CONDITIONAL([BUILD_CYVDB], false)
], [
	AM_CONDITIONAL([BUILD_CYVDB], true)

	AC_CHECK_HEADER([tntdb/connection.h], [], AC_MSG_ERROR([tntdb headers not found]))
])

PKG_CHECK_MODULES([CPPUNIT], [cppunit], [have_cppunit=yes], [have_cppunit=no])
AM_CONDITIONAL([HAVE_CPPUNIT], [test "$have_cppunit" = "yes"])

AC_CONFIG_FILES([
	Makefile
	unit-tests/Makefile
])

AC_OUTPUT
