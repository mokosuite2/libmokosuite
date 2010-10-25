AM_CPPFLAGS = \
	@GLIB_CFLAGS@ 	\
	@EINA_CFLAGS@	\
	@ECORE_CFLAGS@	\
	@DBUS_CFLAGS@	\
	@SQLITE_CFLAGS@	\
	-Wall

VALAC_ARGS = \
	--pkg dbus-glib-1 \
	--save-temps \
	--basedir $(top_srcdir)/mokosuite/utils

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = mokosuite-utils.pc

libmokosuite_utilsincludedir = $(includedir)/mokosuite-0/mokosuite/utils
libmokosuite_utilsinclude_HEADERS = \
	$(libmokosuite_utils_VALASOURCES:.vala=.h) \
	remoteconfig.h \
	misc.h \
	dbus.h \
	utils.h

lib_LTLIBRARIES = libmokosuite-utils.la

dbus_sources = \
	remoteconfig-glue.h

BUILT_SOURCES = $(dbus_sources) .vala.stamp

#sed -i 's/\#include <settingsdb.h>//' settingsdb.c
.vala.stamp: $(libmokosuite_utils_VALASOURCES)
	$(VALAC) -C $(VALAC_ARGS) -H $(^:.vala=.h) $^
	touch $@

libmokosuite_utils_VALASOURCES = \
	remoteconfig-service.vala

libmokosuite_utils_la_SOURCES = \
	$(dbus_sources) \
	$(libmokosuite_utils_VALASOURCES:.vala=.c) \
	$(libmokosuite_utils_VALASOURCES:.vala=.h) \
	remoteconfig.c \
	remoteconfig.h \
	misc.c \
	misc.h \
	utils.c \
	utils.h \
	dbus.c \
	dbus.h \
	globals.h

libmokosuite_utils_la_LDFLAGS = \
	-version-info 0:0:0 \
	@GLIB_LIBS@			\
	@EINA_LIBS@ 		\
	@ECORE_LIBS@		\
	@DBUS_LIBS@			\
	@SQLITE_LIBS@

remoteconfig-glue.h: remoteconfig.xml
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
