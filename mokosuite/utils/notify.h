#ifndef __MOKOSUITE_UTILS_NOTIFY_H
#define __MOKOSUITE_UTILS_NOTIFY_H

#include <libnotify/notification.h>

enum {
    NOTIFICATION_HINT_DONT_PUSH = 1,
    NOTIFICATION_HINT_SHOW_ON_RESUME = 1 << 1,
    NOTIFICATION_HINT_INSISTENT = 1 << 2,
    NOTIFICATION_HINT_ONGOING = 1 << 3,
    NOTIFICATION_HINT_NOCLEAR = 1 << 4,
    NOTIFICATION_HINT_AUTODEL = 1 << 5
} MokosuiteNotificationHints;

NotifyNotification* mokosuite_notification_new(const char* category, const char* summary, const char* body, const char* icon, int flags);

#endif  /* __MOKOSUITE_UTILS_NOTIFY_H */
