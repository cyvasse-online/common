AC_INIT([cyvasse-common],[1.0],[mail@jonasplatte.de])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([1.11 foreign])
AM_SILENT_RULES([yes])

AC_PROG_CXX
AC_PROG_RANLIB
AM_PROG_AR

AC_LANG(C++)
AX_CHECK_COMPILE_FLAG([-std=c++14], [CXXFLAGS="$CXXFLAGS -std=c++14"],
	AX_CHECK_COMPILE_FLAG([-std=c++1y], [CXXFLAGS="$CXXFLAGS -std=c++1y"],
		AC_MSG_ERROR([Compiler doesn't support C++14])
	)
)
AX_CHECK_COMPILE_FLAG([-Wall], [CXXFLAGS="$CXXFLAGS -Wall"])
AX_CHECK_COMPILE_FLAG([-Wextra], [CXXFLAGS="$CXXFLAGS -Wextra"])
AX_CHECK_COMPILE_FLAG([-pedantic], [CXXFLAGS="$CXXFLAGS -pedantic"])

## only build cyvdb when compiling natively...
## This is probably not the best way to check whether emconfigure is used
AS_IF([test -n "$EMMAKEN_JUST_CONFIGURE"], [
	AM_CONDITIONAL([BUILD_CYVDB], false)
], [
	AM_CONDITIONAL([BUILD_CYVDB], true)

	AC_CHECK_HEADER([tntdb/connection.h], [], AC_MSG_ERROR([tntdb headers not found]))
])

PKG_CHECK_MODULES([JSONCPP], [jsoncpp])

PKG_CHECK_MODULES([CPPUNIT], [cppunit], [have_cppunit=yes], [have_cppunit=no])
AM_CONDITIONAL([HAVE_CPPUNIT], [test "$have_cppunit" = "yes"])

AC_CONFIG_FILES([
	Makefile
	unit-tests/Makefile
])

AC_OUTPUT
