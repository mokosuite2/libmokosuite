#include <glib.h>
#include <dbus/dbus-glib.h>

#include "globals.h"
#include "utils.h"
#include "remote-config.h"
#include "remote-config-glue.h"

/* function templates macros */

#define remote_config_get(__type, __proxy, __section, __key, __value, __error) ({ \
    gboolean ret = FALSE; \
    if (!org_mokosuite_Config_get_##__type(__proxy, __section, __key, __value, &ret, __error)) { \
        g_warning("%s", (error != NULL && *error != NULL) ? (*error)->message : "unknown error"); \
        return FALSE; \
    } \
\
    return ret; \
})

#define remote_config_set(__type, __proxy, __section, __key, __value, __error) ({ \
    gboolean ret = FALSE; \
    if (!org_mokosuite_Config_set_##__type(__proxy, __section, __key, __value, &ret, error)) { \
        WARN("%s", (error != NULL && *error != NULL) ? (*error)->message : "unknown error"); \
        return FALSE; \
    } \
\
    return ret; \
})


/* GETTERS */

bool remote_config_get_int(DBusGProxy* proxy, const char* section, const char* key, int *value, GError** error)
{
    remote_config_get(int, proxy, section, key, value, error);
}

bool remote_config_get_double(DBusGProxy* proxy, const char* section, const char* key, double *value, GError** error)
{
    remote_config_get(double, proxy, section, key, value, error);
}

bool remote_config_get_bool(DBusGProxy* proxy, const char* section, const char* key, gboolean *value, GError** error)
{
    remote_config_get(bool, proxy, section, key, value, error);
}

bool remote_config_get_string(DBusGProxy* proxy, const char* section, const char* key, char **value, GError** error)
{
    remote_config_get(string, proxy, section, key, value, error);
}


/* SETTERS */

bool remote_config_set_int(DBusGProxy* proxy, const char* section, const char* key, int value, GError** error)
{
    remote_config_set(int, proxy, section, key, value, error);
}

bool remote_config_set_double(DBusGProxy* proxy, const char* section, const char* key, double value, GError** error)
{
    remote_config_set(double, proxy, section, key, value, error);
}

bool remote_config_set_bool(DBusGProxy* proxy, const char* section, const char* key, gboolean value, GError** error)
{
    remote_config_set(bool, proxy, section, key, value, error);
}

bool remote_config_set_string(DBusGProxy* proxy, const char* section, const char* key, const char* value, GError** error)
{
    remote_config_set(string, proxy, section, key, value, error);
}


/* CONTROL */

bool remote_config_save(DBusGProxy* proxy, GError** error)
{
    gboolean ret = FALSE;
    if (!org_mokosuite_Config_save(proxy, &ret, error)) {
        WARN("%s", (error != NULL && *error != NULL) ? (*error)->message : "unknown error");
        return FALSE;
    }

    return ret;
}

bool remote_config_reload(DBusGProxy* proxy, GError** error)
{
    gboolean ret = FALSE;
    if (!org_mokosuite_Config_reload(proxy, &ret, error)) {
        WARN("%s", (error != NULL && *error != NULL) ? (*error)->message : "unknown error");
        return FALSE;
    }

    return ret;
}


/* CONSTRUCTOR */

DBusGProxy* remote_config_connect(const char* bus_name, const char* path)
{
    GError* error = NULL;
    DBusGConnection* bus = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);

    if (!bus) {
        ERROR("Couldn't connect to system bus (%s)", error->message);
        g_error_free(error);
        return NULL;
    }

    DBusGProxy* proxy = dbus_g_proxy_new_for_name(bus, bus_name, path, REMOTE_CONFIG_INTERFACE);
    if (proxy == NULL) {
        WARN("Couln't connect to %s on bus %s (%s)", path, bus_name, REMOTE_CONFIG_INTERFACE);
        g_object_unref(bus);
    }

    return proxy;
}
