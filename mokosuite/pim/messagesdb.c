#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include <sys/stat.h>
#include <phone-utils.h>

#include <freesmartphone-glib/freesmartphone-glib.h>
#include <freesmartphone-glib/opimd/messages.h>
#include <freesmartphone-glib/opimd/message.h>
#include <freesmartphone-glib/opimd/messagequery.h>

#include "messagesdb.h"
#include "../utils/misc.h"

static MessageThread* handle_message_for_thread(GHashTable* row)
{
    const char* _peer = map_get_string(row, "Peer");

    if (_peer != NULL) {
        char* peer = phone_utils_normalize_number(_peer);
        if (peer == NULL) peer = g_strdup(_peer);

        g_debug("Handling thread with %s", peer);

        const char* _direction = map_get_string(row, "Direction");
        int direction = !strcasecmp(_direction, "in") ? DIRECTION_INCOMING : DIRECTION_OUTGOING;

        MessageThread* t = g_new0(MessageThread, 1);
        t->peer = peer;
        t->content = g_strdup(map_get_string(row, "Content"));
        t->timestamp = map_get_int(row, "Timestamp");
        t->content = g_strdup(map_get_string(row, "Content"));
        t->direction = direction;
        t->unread_count = map_get_int(row, "UnreadCount");
        t->total_count = map_get_int(row, "TotalCount");

        return t;
    }

    return NULL;
}

typedef struct {
    /* liberati subito */
    GHashTable* query;
    /* mantenuti fino alla fine */
    GTimer* timer;
    MessageThreadFunc func;
    gpointer userdata;
    char* path;
} query_data_t;

static void _cb_thread_query(GError* error, const char* path, gpointer userdata);

static void _cb_thread_next(GError* error, GHashTable* row, gpointer userdata)
{
    query_data_t* data = userdata;

    if (error) {
        g_debug("[%s] message row error: %s", __func__, error->message);
        g_debug("[%s] message loading took %f seconds", __func__, g_timer_elapsed(data->timer, NULL));
        g_timer_destroy(data->timer);

        // distruggi query
        opimd_messagequery_dispose(data->path, NULL, NULL);

        // distruggi dati callback
        g_free(data->path);
        g_free(data);

        return;
    }

    MessageThread* t = handle_message_for_thread(row);
    if (t && data->func)
        (data->func)(t, data->userdata);

    opimd_messagequery_get_result(data->path, _cb_thread_next, data);
}

static gboolean _retry_thread_query(gpointer userdata)
{
    query_data_t* data = userdata;
    g_timer_start(data->timer);

    opimd_messages_query_threads(data->query, _cb_thread_query, data);
    return FALSE;
}

static void _cb_thread_query(GError* error, const char* path, gpointer userdata)
{
    query_data_t* data = userdata;
    g_debug("[messagesdb_foreach_thread] query took %f seconds", g_timer_elapsed(data->timer, NULL));

    if (error) {
        g_debug("[messagesdb_foreach_thread] Thread query error: (%d) %s", error->code, error->message);

        // opimd non ancora caricato? Riprova in 5 secondi
        if (FREESMARTPHONE_GLIB_IS_DBUS_ERROR(error, FREESMARTPHONE_GLIB_DBUS_ERROR_SERVICE_NOT_AVAILABLE)) {
            g_timeout_add_seconds(5, _retry_thread_query, data);
            return;
        }

        g_hash_table_destroy(data->query);
        g_timer_destroy(data->timer);
        g_free(data);
        return;
    }

    g_hash_table_destroy(data->query);

    data->path = g_strdup(path);
    opimd_messagequery_get_result(data->path, _cb_thread_next, data);
}

void messagesdb_foreach_thread(MessageThreadFunc func, gpointer data)
{
    g_return_if_fail(func != NULL);
    if (opimdMessagesBus == NULL) return;

    query_data_t* cbdata = g_new0(query_data_t, 1);

    cbdata->timer = g_timer_new();
    // no free functions because it's not used
    cbdata->query = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);
    cbdata->userdata = data;
    cbdata->func = func;

    opimd_messages_query_threads(cbdata->query, _cb_thread_query, cbdata);
}

void messagesdb_init(MessageThreadFunc func, gpointer userdata)
{
    opimd_messages_dbus_connect();

    if (opimdMessagesBus == NULL)
        g_warning("Unable to connect to messages database; will not be able to read messages");
}
