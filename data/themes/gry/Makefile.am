MAINTAINERCLEANFILES = Makefile.in

filesdir = $(datadir)/mokosuite/common/themes
files_DATA = gry.edj

EDC_DEPS = keypad_normal.png keypad_pressed.png

EXTRA_DIST = $(files_DATA) $(EDC_DEPS) theme.edc

clean-local:
	rm -f $(files_DATA)

gry.edj: theme.edc $(EDC_DEPS)
	@edje_cc@ $< $@
