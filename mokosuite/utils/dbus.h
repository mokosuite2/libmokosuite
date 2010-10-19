
#ifndef MOKOSUITE_UTILS_DBUS_H
#define MOKOSUITE_UTILS_DBUS_H

#include <mokosuite/utils/utils.h>
#include <glib.h>
#include <dbus/dbus-glib.h>

DBusGConnection* dbus_system_bus(void);
DBusGConnection* dbus_session_bus(void);
bool dbus_request_name(DBusGConnection* bus, const char* name);


#endif  /* MOKOSUITE_UTILS_DBUS_H */
