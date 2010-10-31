#include <Ecore.h>
#include <glib.h>
#include <freesmartphone-glib/freesmartphone-glib.h>

#include "globals.h"

int _mokosuite_pim_log_dom = -1;

void mokosuite_pim_init(void)
{
    _mokosuite_pim_log_dom = eina_log_domain_register(MOKOSUITE_PIM_LOG_NAME, MOKOSUITE_PIM_LOG_COLOR);
    if (_mokosuite_pim_log_dom < 0)
        printf("Cannot create log domain.\n");

    eina_log_domain_level_set(MOKOSUITE_PIM_LOG_NAME, LOG_LEVEL);

    /* GLib mainloop integration */
    if (!ecore_main_loop_glib_integrate())
        ERROR("Ecore/GLib integration failed!");

    /* FSO init */
    freesmartphone_glib_init();
}
