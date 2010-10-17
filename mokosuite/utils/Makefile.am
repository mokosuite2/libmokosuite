AM_CPPFLAGS = \
	@GLIB_CFLAGS@ 	\
	@EINA_CFLAGS@	\
	@ECORE_CFLAGS@	\
	@FSO_CFLAGS@	\
	-Wall

AM_LDFLAGS = \
	@GLIB_LIBS@		\
	@EINA_LIBS@ 	\
	@ECORE_LIBS@

VALAC_ARGS = \
	--pkg dbus-glib-1 \
	--pkg sqlite3 \
	--save-temps \
	--basedir $(top_srcdir)/mokosuite/utils

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = mokosuite-utils.pc

libmokosuite_utilsincludedir = $(includedir)/mokosuite-0/mokosuite/utils
libmokosuite_utilsinclude_HEADERS = \
	$(libmokosuite_utils_VALASOURCES:.vala=.h) \
	misc.h \
	utils.h

lib_LTLIBRARIES = libmokosuite-utils.la

BUILT_SOURCES = .vala.stamp

#sed -i 's/\#include <settingsdb.h>//' settingsdb.c
.vala.stamp: $(libmokosuite_utils_VALASOURCES)
	$(VALAC) -C $(VALAC_ARGS) -H $(^:.vala=.h) $^
	touch $@

libmokosuite_utils_VALASOURCES = \
	settingsdb.vala

libmokosuite_utils_la_SOURCES = \
	$(libmokosuite_utils_VALASOURCES:.vala=.c) \
	misc.c \
	misc.h \
	utils.c \
	utils.h \
	globals.h

libmokosuite_utils_la_LDFLAGS = -version-info 0:0:0

EXTRA_DIST = \
	mokosuite-utils.pc.in \
	$(BUILT_SOURCES) \
	$(libmokosuite_utils_VALASOURCES:.vala=.c) \
	$(libmokosuite_utils_VALASOURCES:.vala=.h)

CLEANFILES = \
	$(BUILT_SOURCES) \
	$(libmokosuite_utils_VALASOURCES:.vala=.c) \
	$(libmokosuite_utils_VALASOURCES:.vala=.h)

MAINTAINERCLEANFILES = \
	Makefile.in
