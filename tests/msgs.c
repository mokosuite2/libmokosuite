#include <glib.h>

#include <freesmartphone-glib/freesmartphone-glib.h>
#include <pim/messagesdb.h>
#include <phone-utils.h>

static void _cb_msgs(MessageThread* t, gpointer data)
{
    g_debug("Thread with %p, total=%d, unread=%d",
        t->peer, t->total_count, t->unread_count);
}

int main(int argc, char* argv[])
{
    freesmartphone_glib_init();
    phone_utils_init();

    messagesdb_init(NULL, NULL);

    messagesdb_foreach_thread(_cb_msgs, NULL);

    GMainLoop* m = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(m);

    return 0;
}
