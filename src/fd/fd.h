#ifndef FD_H
#define FD_H

#include <stddef.h>
#include <stdint.h>

#include "task.h"

#define MAX_OPEN_FILES 64

typedef struct file file_t;

typedef struct
{
    int64_t (*read)(
        file_t *file,
        uint8_t *data,
        size_t len);

    int64_t (*write)(
        file_t *file,
        const uint8_t *data,
        size_t len);

    void (*close)(
        file_t *file);

} file_ops_t;

struct file
{
    const file_ops_t *ops;

    void *ctx;

    int in_use;

    
    
    
    
    int pinned;

    int closing;
};

void fd_init(void);




void fd_init_task(
    task_t *task);



int fd_open_for(
    task_t *task,
    const file_ops_t *ops,
    void *ctx);


int fd_open(
    const file_ops_t *ops,
    void *ctx);

int64_t fd_read(
    int fd,
    uint8_t *data,
    size_t len);

int64_t fd_write(
    int fd,
    const uint8_t *data,
    size_t len);

int fd_close(
    int fd);



void fd_close_all(
    task_t *task);


int fd_open_pipe(
    void *pipe);

#endif