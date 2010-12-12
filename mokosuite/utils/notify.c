#include <libnotify/notify.h>

#include "globals.h"
#include "notify.h"
#include "utils.h"

/**
 * Creates a new notification object.
 * @param category
 * @param summary
 * @param body
 * @param icon
 * @param flags flags from MokosuiteNotificationHints
 * @return a new notification object
 */
NotifyNotification* mokosuite_notification_new(const char* category, const char* summary, const char* body, const char* icon, int flags)
{
    NotifyNotification* n = notify_notification_new(summary, body, icon, NULL);
    notify_notification_set_category(n, category);

    if (flags & NOTIFICATION_HINT_DONT_PUSH)
        notify_notification_set_hint_byte(n, "x-mokosuite.flags.dont-push", TRUE);
    if (flags & NOTIFICATION_HINT_SHOW_ON_RESUME)
        notify_notification_set_hint_byte(n, "x-mokosuite.flags.show-on-resume", TRUE);
    if (flags & NOTIFICATION_HINT_INSISTENT)
        notify_notification_set_hint_byte(n, "x-mokosuite.flags.insistent", TRUE);
    if (flags & NOTIFICATION_HINT_ONGOING)
        notify_notification_set_hint_byte(n, "x-mokosuite.flags.ongoing", TRUE);
    if (flags & NOTIFICATION_HINT_NOCLEAR)
        notify_notification_set_hint_byte(n, "x-mokosuite.flags.noclear", TRUE);
    if (flags & NOTIFICATION_HINT_AUTODEL)
        notify_notification_set_hint_byte(n, "x-mokosuite.flags.autodel", TRUE);

    return n;
}
