DESCRIPTION = "Desktop environment and phone stack GUI for SHR - core library"
HOMEPAGE = "http://gitorious.org/mokosuite2"
AUTHOR = "Daniele Ricci"
LICENSE = "GPLv3"
DEPENDS = "elementary libfreesmartphone-glib libphone-utils eggdbus glib-2.0 dbus-glib alsa-lib db sqlite3 vala-native edje-native"
SECTION = "misc/utils"

PV = "1.0.99+gitr${SRCPV}"
#SRCREV = "4c3a2f890987a23880c7b1809e360908f85e30fa"

SRC_URI = "git://gitorious.org/mokosuite2/libmokosuite.git;protocol=git"
S = "${WORKDIR}/git"

PARALLEL_MAKE = ""

EXTRA_OECONF = " --enable-contactsdb-sqlite --with-edje-cc=${STAGING_BINDIR_NATIVE}/edje_cc --enable-debug"
FILES_${PN} += "${datadir}/mokosuite"

inherit pkgconfig autotools
