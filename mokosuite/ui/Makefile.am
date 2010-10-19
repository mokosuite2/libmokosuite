AM_CPPFLAGS = \
	@GLIB_CFLAGS@ 			\
	@ELEMENTARY_CFLAGS@		\
	-Wall -DDATADIR=\""$(datadir)"\"

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = mokosuite-ui.pc

libmokosuite_uiincludedir = $(includedir)/mokosuite-0/mokosuite/ui
libmokosuite_uiinclude_HEADERS = \
	gui.h

lib_LTLIBRARIES = libmokosuite-ui.la

libmokosuite_ui_la_SOURCES = \
	mokowin.c \
	mokoinwin.c \
	gui.c \
	gui.h

libmokosuite_ui_la_LDFLAGS = \
	-version-info 0:0:0 	\
	@GLIB_LIBS@		\
	@ELEMENTARY_LIBS@

EXTRA_DIST = \
	mokosuite-ui.pc.in

DISTCLEANFILES = 

MAINTAINERCLEANFILES = \
	Makefile.in
