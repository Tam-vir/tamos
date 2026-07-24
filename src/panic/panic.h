#ifndef PANIC_H
#define PANIC_H

#include <stdarg.h>
#include "timer.h"


void halt(void);

void panic(
    const char *message);

void panicf(
    const char *fmt,
    ...);

void warnf(
    const char *fmt,
    ...);



#define ASSERT(expr)                 \
    do                               \
    {                                \
        if (!(expr))                 \
        {                            \
            panicf(                  \
                "ASSERTION FAILED\n" \
                "Expression : %s\n"  \
                "File       : %s\n"  \
                "Function   : %s\n"  \
                "Line       : %u",   \
                #expr,               \
                __FILE__,            \
                __func__,            \
                (unsigned)__LINE__); \
        }                            \
    } while (0)



#define BUG()                       \
    do                              \
    {                               \
        panicf(                     \
            "KERNEL BUG DETECTED\n" \
            "File     : %s\n"       \
            "Function : %s\n"       \
            "Line     : %u",        \
            __FILE__,               \
            __func__,               \
            (unsigned)__LINE__);    \
    } while (0)

#define BUG_ON(expr)                    \
    do                                  \
    {                                   \
        if (expr)                       \
        {                               \
            panicf(                     \
                "KERNEL BUG DETECTED\n" \
                "Expression : %s\n"     \
                "File       : %s\n"     \
                "Function   : %s\n"     \
                "Line       : %u",      \
                #expr,                  \
                __FILE__,               \
                __func__,               \
                (unsigned)__LINE__);    \
        }                               \
    } while (0)



#define WARN(message)            \
    do                           \
    {                            \
        warnf(                   \
            "WARNING\n"          \
            "Message  : %s\n"    \
            "File     : %s\n"    \
            "Function : %s\n"    \
            "Line     : %u",     \
            message,             \
            __FILE__,            \
            __func__,            \
            (unsigned)__LINE__); \
    } while (0)

#define WARN_ON(expr)                \
    do                               \
    {                                \
        if (expr)                    \
        {                            \
            warnf(                   \
                "WARNING\n"          \
                "Expression : %s\n"  \
                "File       : %s\n"  \
                "Function   : %s\n"  \
                "Line       : %u",   \
                #expr,               \
                __FILE__,            \
                __func__,            \
                (unsigned)__LINE__); \
        }                            \
    } while (0)

#endif