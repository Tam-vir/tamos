#include <stdarg.h>
#include <stdint.h>

#include "logger.h"
#include "stdio.h"
#include "spinlock.h"



static uint8_t logger_level =
    LOG_DEBUG;

static int logger_enabled = 1;

static spinlock_t logger_lock;



static void logger_begin(
    const char *level)
{
    kprintf(
        "[%s] ",
        level);
}

static void logger_end(void)
{
    kprintf(
        "\n");
}



void logger_init(void)
{
    spinlock_init(
        &logger_lock);

    logger_enabled = 1;

    logger_level =
        LOG_DEBUG;
}



void logger_enable(void)
{
    spinlock_lock(
        &logger_lock);

    logger_enabled = 1;

    spinlock_unlock(
        &logger_lock);
}

void logger_disable(void)
{
    spinlock_lock(
        &logger_lock);

    logger_enabled = 0;

    spinlock_unlock(
        &logger_lock);
}

void logger_set_level(
    uint8_t level)
{
    spinlock_lock(
        &logger_lock);

    logger_level =
        level;

    spinlock_unlock(
        &logger_lock);
}



static void logger_write(
    const char *level,
    const char *fmt,
    va_list args)
{
    logger_begin(
        level);

    kvprintf(
        fmt,
        args);

    logger_end();
}



void klog_debug(
    const char *fmt,
    ...)
{
    spinlock_lock(
        &logger_lock);

    if (!logger_enabled ||
        logger_level > LOG_DEBUG)
    {
        spinlock_unlock(
            &logger_lock);

        return;
    }

    va_list args;

    va_start(
        args,
        fmt);

    logger_write(
        "DEBUG",
        fmt,
        args);

    va_end(
        args);

    spinlock_unlock(
        &logger_lock);
}



void klog_info(
    const char *fmt,
    ...)
{
    spinlock_lock(
        &logger_lock);

    if (!logger_enabled ||
        logger_level > LOG_INFO)
    {
        spinlock_unlock(
            &logger_lock);

        return;
    }

    va_list args;

    va_start(
        args,
        fmt);

    logger_write(
        "INFO",
        fmt,
        args);

    va_end(
        args);

    spinlock_unlock(
        &logger_lock);
}



void klog_warn(
    const char *fmt,
    ...)
{
    spinlock_lock(
        &logger_lock);

    if (!logger_enabled ||
        logger_level > LOG_WARN)
    {
        spinlock_unlock(
            &logger_lock);

        return;
    }

    va_list args;

    va_start(
        args,
        fmt);

    logger_write(
        "WARN",
        fmt,
        args);

    va_end(
        args);

    spinlock_unlock(
        &logger_lock);
}



void klog_error(
    const char *fmt,
    ...)
{
    spinlock_lock(
        &logger_lock);

    if (!logger_enabled ||
        logger_level > LOG_ERROR)
    {
        spinlock_unlock(
            &logger_lock);

        return;
    }

    va_list args;

    va_start(
        args,
        fmt);

    logger_write(
        "ERROR",
        fmt,
        args);

    va_end(
        args);

    spinlock_unlock(
        &logger_lock);
}



void klog_fatal(
    const char *fmt,
    ...)
{
    spinlock_lock(
        &logger_lock);

    if (!logger_enabled)
    {
        spinlock_unlock(
            &logger_lock);

        return;
    }

    va_list args;

    va_start(
        args,
        fmt);

    logger_write(
        "FATAL",
        fmt,
        args);

    va_end(
        args);

    spinlock_unlock(
        &logger_lock);
}