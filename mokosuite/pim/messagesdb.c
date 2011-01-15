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
#include "globals.h"
#include "../utils/misc.h"

typedef struct {
    /* liberati subito */
    GHashTable* query;
    /* mantenuti fino alla fine */
    GTimer* timer;
    gpointer func;
    gpointer userdata;
    char* path;
    bool cancel;
} query_data_t;

typedef struct {
    MessageFunc func;
    gpointer userdata;
} message_callback_t;

static GHashTable* callbacks = NULL;

/* -- QUERY THREADS -- */

static MessageThread* handle_message_for_thread(GHashTable* row)
{
    const char* _peer = map_get_string(row, "Peer");

    if (_peer != NULL) {
        char* peer = phone_utils_normalize_number(_peer);
        if (peer == NULL) peer = g_strdup(_peer);

        DEBUG("Handling thread with %s", peer);

        const char* _direction = map_get_string(row, "Direction");
        int direction = !strcasecmp(_direction, "in") ? DIRECTION_INCOMING : DIRECTION_OUTGOING;

        MessageThread* t = g_new0(MessageThread, 1);
        t->peer = peer;
        t->content = g_strdup(map_get_string(row, "Content"));
        t->timestamp = map_get_int(row, "Timestamp");
        t->direction = direction;
        t->unread_count = map_get_int(row, "UnreadCount");
        t->total_count = map_get_int(row, "TotalCount");

        return t;
    }

    return NULL;
}

static void _cb_thread_query(GError* error, const char* path, gpointer userdata);

static void _cb_thread_next(GError* error, GHashTable* row, gpointer userdata)
{
    query_data_t* data = userdata;

    if (error || data->cancel) {
        // error but query not aborted
        if (error && !data->cancel) {
            DEBUG("message row error: %s", error->message);
            DEBUG("message loading took %f seconds", g_timer_elapsed(data->timer, NULL));
            g_timer_destroy(data->timer);
        }

        // distruggi query
        opimd_messagequery_dispose(data->path, NULL, NULL);

        // distruggi dati callback
        g_free(data->path);
        g_free(data);

        return;
    }

    MessageThread* t = handle_message_for_thread(row);
    if (t && data->func)
        ((MessageThreadFunc)data->func)(t, data->userdata);

    opimd_messagequery_get_result(data->path, _cb_thread_next, data);
}

static gboolean _retry_thread_query(gpointer userdata)
{
    query_data_t* data = userdata;
    // query aborted
    if (data->cancel) {
        g_hash_table_destroy(data->query);
        g_timer_destroy(data->timer);
        g_free(data);
        return FALSE;
    }

    g_timer_start(data->timer);

    opimd_messages_query_threads(data->query, _cb_thread_query, data);
    return FALSE;
}

static void _cb_thread_query(GError* error, const char* path, gpointer userdata)
{
    query_data_t* data = userdata;
    DEBUG("query took %f seconds", g_timer_elapsed(data->timer, NULL));

    if (error || data->cancel) {
        if (error && !data->cancel) {
            DEBUG("thread query error: (%d) %s", error->code, error->message);

            // opimd non ancora caricato? Riprova in 5 secondi
            if (FREESMARTPHONE_GLIB_IS_DBUS_ERROR(error, FREESMARTPHONE_GLIB_DBUS_ERROR_SERVICE_NOT_AVAILABLE)) {
                g_timeout_add_seconds(5, _retry_thread_query, data);
                return;
            }
        }
        else {
            // no error, destroy query
            opimd_messagequery_dispose(path, NULL, NULL);
        }

        // no error, query aborted
        g_hash_table_destroy(data->query);
        g_timer_destroy(data->timer);
        g_free(data);
        return;
    }

    g_hash_table_destroy(data->query);

    data->path = g_strdup(path);
    opimd_messagequery_get_result(data->path, _cb_thread_next, data);
}

void* messagesdb_foreach_thread(MessageThreadFunc func, gpointer data)
{
    g_return_val_if_fail(func != NULL, NULL);
    if (opimdMessagesBus == NULL) return NULL;

    query_data_t* cbdata = g_new0(query_data_t, 1);

    cbdata->timer = g_timer_new();
    // no free functions because it's not used
    cbdata->query = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);
    cbdata->userdata = data;
    cbdata->func = func;

    opimd_messages_query_threads(cbdata->query, _cb_thread_query, cbdata);
    return cbdata;
}

/* -- QUERY -- */

static MessageEntry* handle_message_data(GHashTable* row)
{
    const char* _peer = map_get_string(row, "Peer");

    if (_peer != NULL) {
        char* peer = phone_utils_normalize_number(_peer);
        if (peer == NULL) peer = g_strdup(_peer);

        const char* _direction = map_get_string(row, "Direction");
        int direction = !strcasecmp(_direction, "in") ? DIRECTION_INCOMING : DIRECTION_OUTGOING;

        MessageEntry* e = g_new0(MessageEntry, 1);
        e->id = map_get_int(row, "EntryId");
        e->peer = peer;
        e->content = g_strdup(map_get_string(row, "Content"));
        e->timestamp = map_get_int(row, "Timestamp");
        e->direction = direction;
        e->is_new = map_get_bool(row, "New", TRUE);

        return e;
    }

    return NULL;
}

static void _cb_query(GError* error, const char* path, gpointer userdata);

static void _cb_next(GError* error, GHashTable* row, gpointer userdata)
{
    query_data_t* data = userdata;

    if (error || data->cancel) {
        // error but query not aborted
        if (error && !data->cancel) {
            DEBUG("message row error: %s", error->message);
            DEBUG("message loading took %f seconds", g_timer_elapsed(data->timer, NULL));
            g_timer_destroy(data->timer);
        }

        // call final callback only if query is not aborted
        if (data->func && !data->cancel)
            ((MessageFunc)data->func)(NULL, data->userdata);

        // distruggi query
        opimd_messagequery_dispose(data->path, NULL, NULL);

        // distruggi dati callback
        g_free(data->path);
        g_free(data);

        return;
    }

    MessageEntry* e = handle_message_data(row);
    if (e && data->func)
        ((MessageFunc)data->func)(e, data->userdata);

    opimd_messagequery_get_result(data->path, _cb_next, data);
}

static gboolean _retry_query(gpointer userdata)
{
    query_data_t* data = userdata;
    // query aborted
    if (data->cancel) {
        g_hash_table_destroy(data->query);
        g_timer_destroy(data->timer);
        g_free(data);
        return FALSE;
    }

    g_timer_start(data->timer);

    opimd_messages_query(data->query, _cb_query, data);
    return FALSE;
}

static void _cb_query(GError* error, const char* path, gpointer userdata)
{
    query_data_t* data = userdata;
    DEBUG("query took %f seconds", g_timer_elapsed(data->timer, NULL));

    if (error || data->cancel) {
        if (error && !data->cancel) {
            DEBUG("thread query error: (%d) %s", error->code, error->message);

            // opimd non ancora caricato? Riprova in 5 secondi
            if (FREESMARTPHONE_GLIB_IS_DBUS_ERROR(error, FREESMARTPHONE_GLIB_DBUS_ERROR_SERVICE_NOT_AVAILABLE)) {
                g_timeout_add_seconds(5, _retry_query, data);
                return;
            }
        }
        else {
            // no error, destroy query
            opimd_messagequery_dispose(path, NULL, NULL);
        }

        g_hash_table_destroy(data->query);
        g_timer_destroy(data->timer);
        g_free(data);
        return;
    }

    g_hash_table_destroy(data->query);

    data->path = g_strdup(path);
    opimd_messagequery_get_result(data->path, _cb_next, data);
}

void messagesdb_foreach_stop(void* query)
{
    query_data_t* cbdata = (query_data_t*) query;
    cbdata->cancel = TRUE;
}

void* messagesdb_foreach(MessageFunc func, const char* peer, bool sort_desc, int start, int limit, gpointer data)
{
    g_return_val_if_fail(func != NULL, NULL);
    if (opimdMessagesBus == NULL) return NULL;

    query_data_t* cbdata = g_new0(query_data_t, 1);

    cbdata->timer = g_timer_new();

    cbdata->query = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_value_free);
    if (peer)
        g_hash_table_insert(cbdata->query, g_strdup("Peer"), g_value_from_string(peer));

    // sorting
    g_hash_table_insert(cbdata->query, g_strdup("_sortby"), g_value_from_string("Timestamp"));
    if (sort_desc)
        g_hash_table_insert(cbdata->query, g_strdup("_sortdesc"), g_value_from_int(1));

    // start
    if (start >= 0)
        g_hash_table_insert(cbdata->query, g_strdup("_limit_start"), g_value_from_int(start));

    // limit
    if (limit >= 0)
        g_hash_table_insert(cbdata->query, g_strdup("_limit"), g_value_from_int(limit));

    cbdata->userdata = data;
    cbdata->func = func;

    opimd_messages_query(cbdata->query, _cb_query, cbdata);
    return cbdata;
}

typedef struct {
    MessageFunc func;
    gpointer userdata;
} new_message_data_t;

static void _new_message_data(GError* error, GHashTable* row, gpointer userdata)
{
    g_return_if_fail(error == NULL);

    new_message_data_t* data = userdata;
    MessageEntry* e = handle_message_data(row);

    if (e != NULL) {
        // main callback
        if (data->func) {
            // double call for last message
            (data->func)(e, data->userdata);
            (data->func)(NULL, data->userdata);
        }

        // peer callback
        message_callback_t* c = (message_callback_t*) g_hash_table_lookup(callbacks, e->peer);
        if (c && c->func) {
            // double call for last message
            (c->func)(e, c->userdata);
            (c->func)(NULL, c->userdata);
        }
    }
}

static void _new_message(gpointer userdata, const char* path)
{
    DEBUG("new message created %s", path);

    // ottieni informazioni sul messaggio
    opimd_message_get_content(path, _new_message_data, userdata);
}

void messagesdb_free_entry(MessageEntry* e)
{
    g_free(e->content);
    g_free(e);
}

void messagesdb_connect(MessageFunc func, const char* peer, gpointer userdata)
{
    if (!callbacks)
        callbacks = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

    if (!func) {
        messagesdb_disconnect(peer);
    }
    else {
        message_callback_t* c = g_new(message_callback_t, 1);
        c->func = func;
        c->userdata = userdata;
        g_hash_table_insert(callbacks, g_strdup(peer), c);
    }
}

void messagesdb_disconnect(const char* peer)
{
    g_hash_table_remove(callbacks, peer);
}

typedef struct {
    GHashTable* query;
    GTimer* timer;
} set_message_new_data_t;

static void _cb_set_call_new(GError* error, gpointer userdata)
{
    if (error) {
        EINA_LOG_WARN("error: %s", error->message);
    }

    set_message_new_data_t* data = userdata;
    EINA_LOG_DBG("query took %f seconds", g_timer_elapsed(data->timer, NULL));

    g_timer_destroy(data->timer);
    g_hash_table_destroy(data->query);
    g_free(data);
}

void messagesdb_set_message_new(int id, gboolean is_new)
{
    if (id < 0) return;

    if (opimdMessagesBus == NULL) return;

    char* path = messagesdb_get_message_path(id);

    set_message_new_data_t* cbdata = g_new0(set_message_new_data_t, 1);
    cbdata->timer = g_timer_new();
    cbdata->query = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_value_free);

    g_hash_table_insert(cbdata->query, g_strdup("New"), g_value_from_int(is_new));

    opimd_message_update(path, cbdata->query, _cb_set_call_new, cbdata);
    g_free(path);
}

char* messagesdb_get_message_path(int id)
{
    return g_strdup_printf("/org/freesmartphone/PIM/Messages/%d", id);
}

void messagesdb_init(MessageFunc func, gpointer userdata)
{
    opimd_messages_dbus_connect();

    if (opimdMessagesBus == NULL)
        WARN("unable to connect to messages database; will not be able to read messages");

    new_message_data_t* data = g_new(new_message_data_t, 1);
    data->func = func;
    data->userdata = userdata;
    opimd_messages_new_message_connect(_new_message, data);
}
