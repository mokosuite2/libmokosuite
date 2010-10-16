#ifndef __MOKOSUITE_UTILS_GLOBALS_H
#define __MOKOSUITE_UTILS_GLOBALS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Eina.h>

#ifdef DEBUG
#define LOG_LEVEL   EINA_LOG_LEVEL_DBG
#else
#define LOG_LEVEL   EINA_LOG_LEVEL_INFO
#endif

#define MOKOSUITE_UTILS_LOG_NAME        "mokosuite_utils"

extern int _mokosuite_utils_log_dom;
#ifdef MOKOSUITE_UTILS_LOG_COLOR
#undef MOKOSUITE_UTILS_LOG_COLOR
#endif
#define MOKOSUITE_UTILS_LOG_COLOR   EINA_COLOR_BLUE

#define MOKOSUITE_UTILS_LOG_DOM _mokosuite_utils_log_dom

#ifdef ERROR
#undef ERROR
#endif
#define ERROR(...) EINA_LOG_DOM_ERR(MOKOSUITE_UTILS_LOG_DOM, __VA_ARGS__)
#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG(fmt, ...) EINA_LOG_DOM_DBG(MOKOSUITE_UTILS_LOG_DOM, fmt, __VA_ARGS__)
#ifdef INFO
#undef INFO
#endif
#define INFO(...) EINA_LOG_DOM_INFO(MOKOSUITE_UTILS_LOG_DOM, __VA_ARGS__)
#ifdef WARN
#undef WARN
#endif
#define WARN(...) EINA_LOG_DOM_WARN(MOKOSUITE_UTILS_LOG_DOM, __VA_ARGS__)

#endif  /* __MOKOSUITE_UTILS_GLOBALS_H */
