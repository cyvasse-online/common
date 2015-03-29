check_PROGRAMS = cyvasse-tests

cyvasse_tests_SOURCES = \
	hexagon_test.cpp \
	hexagon_test.hpp \
	main.cpp

cyvasse_tests_CPPFLAGS = \
	-I$(top_srcdir)/include

cyvasse_tests_CXXFLAGS = \
	$(CPPUNIT_CFLAGS)

cyvasse_tests_LDFLAGS = \
	$(CPPUNIT_LIBS)

cyvasse_tests_LDADD = \
	$(top_builddir)/libcyvasse.a
