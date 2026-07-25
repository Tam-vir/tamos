#ifndef UNISTD_H
#define UNISTD_H

#include <stddef.h>
#include <stdint.h>

int64_t read(
    int fd,
    void *buf,
    size_t count);

int64_t write(
    int fd,
    const void *buf,
    size_t count);

int close(
    int fd);



void _exit(void);

void sleep(
    uint64_t ticks);

int getpid(void);

#endif