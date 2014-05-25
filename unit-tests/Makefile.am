AM_CXXFLAGS = \
	-std=c++11

check_PROGRAMS = cyvmath-tests

cyvmath_tests_SOURCES = \
	hexagon_test.cpp \
	hexagon_test.hpp \
	main.cpp

cyvmath_tests_CPPFLAGS = \
	-I $(top_srcdir)/include

cyvmath_tests_CXXFLAGS = \
	$(AM_CXXFLAGS) \
	$(CPPUNIT_CFLAGS)

cyvmath_tests_LDFLAGS = \
	$(CPPUNIT_LIBS)
