#ifndef __MOKOSUITE_UTILS_UTILS_H
#define __MOKOSUITE_UTILS_UTILS_H

#include <Eina.h>

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

typedef Eina_Bool bool;

#define TRUE    EINA_TRUE
#define FALSE   EINA_FALSE

#define MOKOSUITE_DBUS_PATH     "/org/mokosuite"

void mokosuite_utils_init(void);

#endif  /* __MOKOSUITE_UTILS_UTILS_H */
