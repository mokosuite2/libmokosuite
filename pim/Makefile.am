AM_CPPFLAGS = \
	@ELEMENTARY_CFLAGS@ 	\
	@DBUS_CFLAGS@ 			\
	@FSO_CFLAGS@ 			\
	@PHONEUTILS_CFLAGS@ 	\
	-Wall

AM_LDFLAGS = \
	@ELEMENTARY_LIBS@ 		\
	@DBUS_LIBS@ 			\
	@FSO_LIBS@	 			\
	@PHONEUTILS_LIBS@ 		\
	-ldb

if ENABLE_CONTACTSDB_SQLITE
AM_CPPFLAGS += @SQLITE_CFLAGS@
AM_LDFLAGS += $(SQLITE_LIBS)
endif

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = mokosuite-pim.pc

libmokosuite_pimincludedir = $(includedir)/mokosuite/pim
libmokosuite_piminclude_HEADERS = \
	callsdb.h \
	contactsdb.h \
	messagesdb.h

lib_LTLIBRARIES = libmokosuite-pim.la

libmokosuite_pim_la_SOURCES = \
	callsdb.c \
	callsdb.h \
	contactsdb.c \
	contactsdb.h \
	messagesdb.c \
	messagesdb.h

libmokosuite_pim_la_LDFLAGS = -version-info 0:0:0 -L../utils -lmokosuite-utils

EXTRA_DIST = \
	mokosuite-pim.pc.in

DISTCLEANFILES = 

MAINTAINERCLEANFILES = \
	Makefile.in
