#ifndef __MOKOSUITE_PIM_GLOBALS_H
#define __MOKOSUITE_PIM_GLOBALS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Eina.h>

#ifdef DEBUG
#define LOG_LEVEL   EINA_LOG_LEVEL_DBG
#else
#define LOG_LEVEL   EINA_LOG_LEVEL_INFO
#endif

#define MOKOSUITE_PIM_LOG_NAME        "mokosuite_pim"

extern int _mokosuite_pim_log_dom;
#ifdef MOKOSUITE_PIM_LOG_COLOR
#undef MOKOSUITE_PIM_LOG_COLOR
#endif
#define MOKOSUITE_PIM_LOG_COLOR   EINA_COLOR_ORANGE

#define MOKOSUITE_PIM_LOG_DOM _mokosuite_pim_log_dom

#ifdef ERROR
#undef ERROR
#endif
#define ERROR(...) EINA_LOG_DOM_ERR(MOKOSUITE_PIM_LOG_DOM, __VA_ARGS__)
#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG(...) EINA_LOG_DOM_DBG(MOKOSUITE_PIM_LOG_DOM, __VA_ARGS__)
#ifdef INFO
#undef INFO
#endif
#define INFO(...) EINA_LOG_DOM_INFO(MOKOSUITE_PIM_LOG_DOM, __VA_ARGS__)
#ifdef WARN
#undef WARN
#endif
#define WARN(...) EINA_LOG_DOM_WARN(MOKOSUITE_PIM_LOG_DOM, __VA_ARGS__)

#endif  /* __MOKOSUITE_PIM_GLOBALS_H */
