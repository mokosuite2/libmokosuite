#include <Ecore.h>
#include <glib.h>
#include <freesmartphone-glib/freesmartphone-glib.h>

void mokosuite_pim_init(void)
{
    /* GLib mainloop integration */
    if (!ecore_main_loop_glib_integrate())
        g_error("Ecore/GLib integration failed!");

    /* FSO init */
    freesmartphone_glib_init();
}
