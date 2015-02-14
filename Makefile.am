if HAVE_CPPUNIT
  UNIT_TESTS_DIR = unit-tests
endif

SUBDIRS = . $(UNIT_TESTS_DIR)

AUTOMAKE_OPTIONS = subdir-objects

noinst_LIBRARIES = \
	libcyvmath.a \
	libcyvws.a

libcyvmath_a_SOURCES = \
	src/cyvmath/coordinate.cpp \
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
	src/cyvdb/match_manager.cpp \
	src/cyvdb/player_manager.cpp

libcyvdb_a_CPPFLAGS = \
	-I$(top_srcdir)/include

endif # BUILD_CYVDB


libcyvws_a_SOURCES = \
	src/cyvws/json_game_msg.cpp \
	src/cyvws/json_notification.cpp \
	src/cyvws/json_server_reply.cpp

libcyvws_a_CPPFLAGS = \
	-I$(top_srcdir)/include

libcyvws_a_CXXFLAGS = \
	$(JSONCPP_CFLAGS)
