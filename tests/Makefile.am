INCLUDES = $(GLIB_CFLAGS) $(ELEMENTARY_CFLAGS) \
    -DLOCALEDIR=\""$(localedir)"\" -DDATADIR=\""$(datadir)"\" -DSYSCONFDIR=\""$(sysconfdir)"\" -Wall

noinst_PROGRAMS = mokowin

mokowin_SOURCES = \
    mokowin.c

mokowin_LDADD = \
    -L../mokosuite/ui -lmokosuite-ui \
    -L../mokosuite/utils -lmokosuite-utils

MAINTAINERCLEANFILES = \
    Makefile.in
