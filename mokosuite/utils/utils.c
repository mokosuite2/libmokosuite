#include <glib-object.h>
#include <Ecore.h>
#include <stdio.h>
#include "globals.h"
#include "utils.h"

int _mokosuite_utils_log_dom = -1;

void mokosuite_utils_init(void)
{
    _mokosuite_utils_log_dom = eina_log_domain_register(MOKOSUITE_UTILS_LOG_NAME, MOKOSUITE_UTILS_LOG_COLOR);
    if (_mokosuite_utils_log_dom < 0)
        printf("Cannot create log domain.\n");

    eina_log_domain_level_set(MOKOSUITE_UTILS_LOG_NAME, LOG_LEVEL);
}

bool mokosuite_utils_glib_init(bool init_gtype)
{
    if (init_gtype)
        g_type_init();

    /* GLib mainloop integration */
    if (!ecore_main_loop_glib_integrate()) {
        ERROR("Ecore/GLib integration failed!");
        return FALSE;
    }

    return TRUE;
}
