AM_CPPFLAGS = \
	@GLIB_CFLAGS@ 	\
	@EINA_CFLAGS@	\
	@ECORE_CFLAGS@	\
	@DBUS_CFLAGS@	\
	@SQLITE_CFLAGS@	\
	@NOTIFY_CFLAGS@ \
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
	remote-config.h \
	misc.h \
	dbus.h \
	utils.h \
	notify.h

lib_LTLIBRARIES = libmokosuite-utils.la

dbus-marshal.h: dbus-marshal.list
	( @GLIB_GENMARSHAL@ $< --prefix=dbus_glib_marshal --header > dbus-marshal.tmp \
	&& mv dbus-marshal.tmp dbus-marshal.h ) \
	|| ( rm -f dbus-marshal.tmp && exit 1 )

dbus-marshal.c: dbus-marshal.list dbus-marshal.h
	( @GLIB_GENMARSHAL@ $< --prefix=dbus_glib_marshal --body > dbus-marshal.tmp \
	&& mv dbus-marshal.tmp dbus-marshal.c ) \
	|| ( rm -f dbus-marshal.tmp && exit 1 )

dbus_sources = \
	dbus-marshal.c \
	dbus-marshal.h \
	remote-config-glue.h

BUILT_SOURCES = \
	$(dbus_sources) \
	.vala.stamp

#sed -i 's/\#include <settingsdb.h>//' settingsdb.c
.vala.stamp: $(libmokosuite_utils_VALASOURCES)
	$(VALAC) -C $(VALAC_ARGS) -H $(^:.vala=.h) $^
	touch $@

libmokosuite_utils_VALASOURCES = \
	remote-config-service.vala

libmokosuite_utils_la_SOURCES = \
	$(dbus_sources) \
	$(libmokosuite_utils_VALASOURCES:.vala=.c) \
	$(libmokosuite_utils_VALASOURCES:.vala=.h) \
	remote-config.c \
	remote-config.h \
	misc.c \
	misc.h \
	utils.c \
	utils.h \
	dbus.c \
	dbus.h \
	notify.c \
	notify.h \
	globals.h

libmokosuite_utils_la_LDFLAGS = \
	-version-info 0:0:0	\
	@GLIB_LIBS@		\
	@EINA_LIBS@ 		\
	@ECORE_LIBS@		\
	@DBUS_LIBS@		\
	@SQLITE_LIBS@		\
	@NOTIFY_LIBS@

remote-config-glue.h: remote-config.xml
	dbus-binding-tool --mode=glib-client --prefix=moko_settings --output=$@ $<

EXTRA_DIST = \
	mokosuite-utils.pc.in \
	dbus-marshal.list \
	$(BUILT_SOURCES) \
	$(libmokosuite_utils_VALASOURCES:.vala=.c) \
	$(libmokosuite_utils_VALASOURCES:.vala=.h)

CLEANFILES = \
	$(BUILT_SOURCES) \
	$(libmokosuite_utils_VALASOURCES:.vala=.c) \
	$(libmokosuite_utils_VALASOURCES:.vala=.h)

MAINTAINERCLEANFILES = \
	Makefile.in
