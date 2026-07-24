#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>



#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERROR 3
#define LOG_FATAL 4



void logger_init(void);

void logger_enable(void);

void logger_disable(void);

void logger_set_level(
    uint8_t level);




void klog_debug(
    const char *fmt,
    ...);

void klog_info(
    const char *fmt,
    ...);

void klog_warn(
    const char *fmt,
    ...);

void klog_error(
    const char *fmt,
    ...);

void klog_fatal(
    const char *fmt,
    ...);

#endif