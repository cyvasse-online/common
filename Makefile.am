if HAVE_CPPUNIT
  UNIT_TESTS_DIR = unit-tests
endif

SUBDIRS = . $(UNIT_TESTS_DIR)

AUTOMAKE_OPTIONS = subdir-objects

noinst_LIBRARIES = \
	libcyvasse.a \
	libcyvws.a

libcyvasse_a_SOURCES = \
	src/cyvasse/bearing_table.cpp \
	src/cyvasse/match.cpp \
	src/cyvasse/piece.cpp \
	src/cyvasse/player.cpp \
	src/cyvasse/players_color.cpp

libcyvasse_a_CPPFLAGS = \
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
