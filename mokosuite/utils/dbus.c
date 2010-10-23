#include <glib.h>
#include <dbus/dbus-glib-bindings.h>

#include "dbus.h"
#include "globals.h"

static DBusGConnection* _dbus_bus(int bus_type)
{
    GError *e = NULL;
    DBusGConnection* bus = dbus_g_bus_get(bus_type, &e);
    if (e) {
        ERROR("unable to connect to %s bus: %s",
            (bus_type == DBUS_BUS_SYSTEM) ? "system" : "session",
            e->message);
        g_error_free(e);
        return NULL;
    }

    return bus;
}

DBusGConnection* dbus_system_bus(void)
{
    return _dbus_bus(DBUS_BUS_SYSTEM);
}

DBusGConnection* dbus_session_bus(void)
{
    return _dbus_bus(DBUS_BUS_SESSION);
}

bool dbus_request_name(DBusGConnection* bus, const char* name)
{
    GError *e = NULL;
    DBusGProxy *driver_proxy;
    guint request_ret;

    driver_proxy = dbus_g_proxy_new_for_name (bus,
            DBUS_SERVICE_DBUS,
            DBUS_PATH_DBUS,
            DBUS_INTERFACE_DBUS);
    if (!driver_proxy) {
        ERROR("unable to connect to DBus interface");
        return FALSE;
    }

    if (!org_freedesktop_DBus_request_name (driver_proxy,
            name, 0, &request_ret, &e)) {
        ERROR("unable to request name: %s", e->message);
        g_error_free(e);
        return FALSE;
    }
    g_object_unref(driver_proxy);

    return TRUE;
}
