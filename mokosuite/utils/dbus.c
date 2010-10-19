#include <glib.h>
#include <dbus/dbus-glib-bindings.h>

#include "dbus.h"
#include "globals.h"

bool dbus_request_name(const char* name)
{
    GError *e = NULL;
    DBusGProxy *driver_proxy;
    guint request_ret;

    DBusGConnection* system_bus = dbus_g_bus_get(DBUS_BUS_SYSTEM, &e);
    if (e) {
        EINA_LOG_ERR("unable to connect to system bus: %s", e->message);
        g_error_free(e);
        return FALSE;
    }

    driver_proxy = dbus_g_proxy_new_for_name (system_bus,
            DBUS_SERVICE_DBUS,
            DBUS_PATH_DBUS,
            DBUS_INTERFACE_DBUS);
    if (!driver_proxy) {
        EINA_LOG_ERR("unable to connect to DBus interface");
        return FALSE;
    }

    if (!org_freedesktop_DBus_request_name (driver_proxy,
            name, 0, &request_ret, &e)) {
        EINA_LOG_ERR("unable to request name: %s", e->message);
        g_error_free(e);
        return FALSE;
    }
    g_object_unref(driver_proxy);

    return TRUE;
}
