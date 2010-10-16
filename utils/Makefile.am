AM_CPPFLAGS = \
	@GLIB_CFLAGS@ 	\
	@EINA_CFLAGS@	\
	-Wall

AM_LDFLAGS = \
	@GLIB_LIBS@	\
	@EINA_LIBS@

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = mokosuite-utils.pc

libmokosuite_utilsincludedir = $(includedir)/mokosuite-0/mokosuite/utils
libmokosuite_utilsinclude_HEADERS = \
	misc.h \
	cfg.h \
	utils.h

lib_LTLIBRARIES = libmokosuite-utils.la

libmokosuite_utils_la_SOURCES = \
	misc.c \
	misc.h \
	cfg.c \
	cfg.h \
	utils.c \
	utils.h \
	globals.h

libmokosuite_utils_la_LDFLAGS = -version-info 0:0:0

EXTRA_DIST = \
	mokosuite-utils.pc.in

DISTCLEANFILES = 

MAINTAINERCLEANFILES = \
	Makefile.in
