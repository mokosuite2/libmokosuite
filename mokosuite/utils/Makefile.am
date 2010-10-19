AM_CPPFLAGS = \
	@GLIB_CFLAGS@ 	\
	@EINA_CFLAGS@	\
	@ECORE_CFLAGS@	\
	@DBUS_CFLAGS@	\
	@SQLITE_CFLAGS@	\
	-Wall

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
	settings.h \
	misc.h \
	dbus.h \
	utils.h

lib_LTLIBRARIES = libmokosuite-utils.la

dbus_sources = \
	settings-glue.h

BUILT_SOURCES = $(dbus_sources) .vala.stamp

#sed -i 's/\#include <settingsdb.h>//' settingsdb.c
.vala.stamp: $(libmokosuite_utils_VALASOURCES)
	$(VALAC) -C $(VALAC_ARGS) -H $(^:.vala=.h) $^
	touch $@

libmokosuite_utils_VALASOURCES = \
	settingsdb.vala

libmokosuite_utils_la_SOURCES = \
	$(dbus_sources) \
	$(libmokosuite_utils_VALASOURCES:.vala=.c) \
	$(libmokosuite_utils_VALASOURCES:.vala=.h) \
	settings.c \
	settings.h \
	misc.c \
	misc.h \
	utils.c \
	utils.h \
	dbus.c \
	dbus.h \
	globals.h

libmokosuite_utils_la_LDFLAGS = \
	-version-info 0:0:0 \
	@GLIB_LIBS@		\
	@EINA_LIBS@ 	\
	@ECORE_LIBS@	\
	@DBUS_CFLAGS@	\
	@SQLITE_LIBS@

settings-glue.h: settings.xml
	dbus-binding-tool --mode=glib-client --prefix=moko_settings --output=$@ $<

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
