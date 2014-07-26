if HAVE_CPPUNIT
  UNIT_TESTS_DIR = unit-tests
endif

SUBDIRS = . $(UNIT_TESTS_DIR)

AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = \
	-std=c++11

noinst_LIBRARIES = libcyvmath.a

libcyvmath_a_SOURCES = \
	include/enum_str.hpp \
	include/make_unique.hpp \
	include/server_message.hpp \
	include/cyvmath/coordinate.hpp \
	include/cyvmath/hexagon.hpp \
	include/cyvmath/match.hpp \
	include/cyvmath/piece.hpp \
	include/cyvmath/player.hpp \
	include/cyvmath/players_color.hpp \
	include/cyvmath/rule_sets.hpp \
	include/cyvmath/mikelepage/common.hpp \
	include/cyvmath/mikelepage/match.hpp \
	include/cyvmath/mikelepage/piece.hpp \
	include/cyvmath/mikelepage/player.hpp \
	include/cyvmath/terrain.hpp \
	src/players_color.cpp \
	src/rule_set_create.cpp \
	src/mikelepage/piece.cpp \
	src/mikelepage/player.cpp \
	src/mikelepage/terrain.cpp

libcyvmath_a_CPPFLAGS = \
	-I$(top_srcdir)/include
