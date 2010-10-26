#ifndef __MOKOSUITE_UTILS_REMOTE_CONFIG_H
#define __MOKOSUITE_UTILS_REMOTE_CONFIG_H

#include <glib.h>
#include <dbus/dbus-glib.h>
#include <mokosuite/utils/utils.h>

#define REMOTE_CONFIG_INTERFACE     "org.mokosuite.Config"

/* GETTERS */
bool remote_config_get_int(DBusGProxy* proxy, const char* section, const char* key, int *value, GError** error);
bool remote_config_get_double(DBusGProxy* proxy, const char* section, const char* key, double *value, GError** error);
bool remote_config_get_bool(DBusGProxy* proxy, const char* section, const char* key, gboolean *value, GError** error);
bool remote_config_get_string(DBusGProxy* proxy, const char* section, const char* key, char **value, GError** error);

/* SETTERS */
bool remote_config_set_int(DBusGProxy* proxy, const char* section, const char* key, int value, GError** error);
bool remote_config_set_double(DBusGProxy* proxy, const char* section, const char* key, double value, GError** error);
bool remote_config_set_bool(DBusGProxy* proxy, const char* section, const char* key, gboolean value, GError** error);
bool remote_config_set_string(DBusGProxy* proxy, const char* section, const char* key, const char* value, GError** error);

/* CONTROL */
bool remote_config_save(DBusGProxy* proxy, GError** error);
bool remote_config_reload(DBusGProxy* proxy, GError** error);

/* SIGNALS */
void remote_config_changed_connect(DBusGProxy* proxy, GCallback callback, void* data);
void remote_config_changed_disconnect(DBusGProxy* proxy, GCallback callback, void* data);

/* CONSTRUCTOR */
DBusGProxy* remote_config_connect(const char* bus_name, const char* path);

#endif  /* __MOKOSUITE_UTILS_REMOTE_CONFIG_H */
