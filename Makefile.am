if HAVE_CPPUNIT
  UNIT_TESTS_DIR = unit-tests
endif

SUBDIRS = . $(UNIT_TESTS_DIR)

AUTOMAKE_OPTIONS = subdir-objects

noinst_LIBRARIES = \
	libcyvmath.a

libcyvmath_a_SOURCES = \
	include/cyvmath/coordinate.hpp \
	include/cyvmath/hexagon.hpp \
	include/cyvmath/match.hpp \
	include/cyvmath/piece_type.hpp \
	include/cyvmath/piece.hpp \
	include/cyvmath/player.hpp \
	include/cyvmath/players_color.hpp \
	include/cyvmath/rule_set_create.hpp \
	include/cyvmath/rule_sets.hpp \
	include/cyvmath/mikelepage/bearing_table.hpp \
	include/cyvmath/mikelepage/coordinate.hpp \
	include/cyvmath/mikelepage/fortress.hpp \
	include/cyvmath/mikelepage/match.hpp \
	include/cyvmath/mikelepage/piece.hpp \
	include/cyvmath/mikelepage/player.hpp \
	include/cyvmath/terrain_type.hpp \
	include/cyvmath/terrain.hpp \
	src/cyvmath/players_color.cpp \
	src/cyvmath/rule_set_create.cpp \
	src/cyvmath/rule_sets.cpp \
	src/cyvmath/mikelepage/bearing_table.cpp \
	src/cyvmath/mikelepage/match.cpp \
	src/cyvmath/mikelepage/piece.cpp \
	src/cyvmath/mikelepage/player.cpp

libcyvmath_a_CPPFLAGS = \
	-I$(top_srcdir)/include


if BUILD_CYVDB

noinst_LIBRARIES += \
    libcyvdb.a

libcyvdb_a_SOURCES = \
    include/cyvdb/config.hpp \
    include/cyvdb/match.hpp \
    include/cyvdb/match_manager.hpp \
    include/cyvdb/player.hpp \
    include/cyvdb/player_manager.hpp \
    src/cyvdb/match_manager.cpp \
    src/cyvdb/player_manager.cpp

libcyvdb_a_CPPFLAGS = \
    -I$(top_srcdir)/include

endif
