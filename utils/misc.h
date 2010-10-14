#ifndef __MOKOSUITE_UTILS_MISC_H
#define __MOKOSUITE_UTILS_MISC_H

#include <glib.h>
#include <glib-object.h>
#include <time.h>

const char* map_get_attribute(GHashTable* properties, const char* key);
int map_get_attribute_int(GHashTable* properties, const char* key);
gboolean map_get_attribute_bool(GHashTable* properties, const char* key, gboolean fallback_int);

void g_value_free(gpointer data);

GValue* g_value_from_string(const char* string);
GValue* g_value_from_int(int number);

guint64 get_current_time(void);

char* get_time_repr_full(guint64 timestamp);
char* get_time_repr(guint64 timestamp);

time_t get_modification_time(const char* path);

#endif  /* __MOKOSUITE_UTILS_MISC_H */
