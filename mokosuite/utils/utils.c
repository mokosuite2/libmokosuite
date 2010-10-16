#include <stdio.h>
#include "globals.h"

int _mokosuite_utils_log_dom = -1;

void mokosuite_utils_init(void)
{
    _mokosuite_utils_log_dom = eina_log_domain_register(MOKOSUITE_UTILS_LOG_NAME, MOKOSUITE_UTILS_LOG_COLOR);
    if (_mokosuite_utils_log_dom < 0)
        printf("Cannot create log domain.\n");

    eina_log_domain_level_set(MOKOSUITE_UTILS_LOG_NAME, LOG_LEVEL);
}
