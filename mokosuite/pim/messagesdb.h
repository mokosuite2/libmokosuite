#ifndef __MESSAGESDB_H
#define __MESSAGESDB_H

#include <glib.h>
#include <time.h>

#include <mokosuite/utils/utils.h>

typedef enum
{
    DIRECTION_OUTGOING,
    DIRECTION_INCOMING
} MessageDirection;

struct _MessageThread {
    /* numero del contatto */
    char* peer;

    /* contenuto ultimo messaggio */
    char* content;

    /* direzione ultimo messaggio */
    MessageDirection direction;

    /* timestamp ultimo messaggio */
    guint64 timestamp;

    /* numero di messaggi non letti */
    int unread_count;

    /* numero di messaggi totali */
    int total_count;

    /* dati utente (mokomessages: ListItem, ContactEntry, MsgList window) */
    gpointer* data;
};

struct _MessageEntry {
    /* message id */
    int id;

    /* peer number */
    char* peer;

    /* content */
    char* content;

    /* direction */
    MessageDirection direction;

    /* timestamp */
    guint64 timestamp;

    /* New flag */
    gboolean is_new;

    /* user data */
    gpointer* data;
};

typedef struct _MessageEntry    MessageEntry;
typedef struct _MessageThread   MessageThread;

typedef void (*MessageThreadFunc)(MessageThread*, gpointer);
typedef void (*MessageFunc)(MessageEntry*, gpointer);

void messagesdb_free_entry(MessageEntry* e);

void* messagesdb_foreach_thread(MessageThreadFunc func, gpointer data);
void* messagesdb_foreach(MessageFunc func, const char* peer, bool sort_desc, int start, int limit, gpointer data);
void messagesdb_foreach_stop(void* query);

void messagesdb_connect(MessageFunc func, const char* peer, gpointer userdata);
void messagesdb_disconnect(const char* peer);

void messagesdb_set_message_new(int id, gboolean is_new);
void messagesdb_delete_message(int id);
char* messagesdb_get_message_path(int id);

void messagesdb_init(MessageFunc new_message_func, MessageFunc delete_message_func, gpointer userdata);

#endif  /* __MESSAGESDB_H */
