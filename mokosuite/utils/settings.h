#ifndef __MOKOSUITE_UTILS_SETTINGS_H
#define __MOKOSUITE_UTILS_SETTINGS_H

#include <glib.h>
#include <dbus/dbus-glib.h>

#define MOKO_SETTINGS_INTERFACE     "org.mokosuite.Settings"

#define settings_to_boolean(x)      (x != NULL && !strcasecmp(x, "true") ? TRUE : FALSE)
#define settings_from_boolean(x)    (x ? "true" : "false")

#define settings_to_integer(x)      (x != NULL ? atoi(x) : 0)

char* settings_get(DBusGProxy* proxy, const char* key, const char* default_val, GError** error);

void settings_get_async(DBusGProxy* proxy, const char* key, const char* default_val, void (*callback)
    (GError *, const char* ret_value, gpointer userdata), gpointer userdata);

gboolean settings_set(DBusGProxy* proxy, const char* key, const char* value, GError** error);

void settings_set_async(DBusGProxy* proxy, const char* key, const char* value, void (*callback)
    (GError *, gpointer userdata), gpointer userdata);

DBusGProxy* settings_connect(const char* bus_name, const char* path);

#endif  /* __MOKOSUITE_UTILS_SETTINGS_H */
