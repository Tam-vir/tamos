#include "fd.h"

#include "spinlock.h"
#include "string.h"
#include "errno.h"
#include "console.h"
#include "pipe.h"

static file_t files[MAX_OPEN_FILES];

static spinlock_t fd_lock;

void fd_init(void)
{
    spinlock_init(
        &fd_lock);

    kmemzero(
        files,
        sizeof(files));
}

static int alloc_file_slot(
    const file_ops_t *ops,
    void *ctx)
{
    for (int i = 0; i < MAX_OPEN_FILES; i++)
    {
        if (!files[i].in_use)
        {
            files[i].ops = ops;
            files[i].ctx = ctx;
            files[i].in_use = 1;
            files[i].pinned = 0;
            files[i].closing = 0;

            return i;
        }
    }

    return -1;
}

static int alloc_fd_slot(
    task_t *task,
    int file_index)
{
    for (int i = 0; i < MAX_FDS_PER_TASK; i++)
    {
        if (task->fds[i] == -1)
        {
            task->fds[i] = file_index;
            return i;
        }
    }

    return -1;
}

int fd_open_for(
    task_t *task,
    const file_ops_t *ops,
    void *ctx)
{
    if (!task || !ops)
    {
        errno = EINVAL;
        return -1;
    }

    spinlock_lock(
        &fd_lock);

    int idx =
        alloc_file_slot(
            ops,
            ctx);

    if (idx < 0)
    {
        spinlock_unlock(
            &fd_lock);

        errno = EMFILE;
        return -1;
    }

    int fd =
        alloc_fd_slot(
            task,
            idx);

    if (fd < 0)
    {
        files[idx].in_use = 0;

        spinlock_unlock(
            &fd_lock);

        errno = EMFILE;
        return -1;
    }

    spinlock_unlock(
        &fd_lock);

    errno = SUCCESS;

    return fd;
}

int fd_open(
    const file_ops_t *ops,
    void *ctx)
{
    return fd_open_for(
        task_current(),
        ops,
        ctx);
}

int64_t fd_read(
    int fd,
    uint8_t *data,
    size_t len)
{
    task_t *task =
        task_current();

    if (!task ||
        fd < 0 ||
        fd >= MAX_FDS_PER_TASK ||
        !data)
    {
        errno = EBADF;
        return -1;
    }

    spinlock_lock(
        &fd_lock);

    int idx =
        task->fds[fd];

    if (idx < 0 ||
        !files[idx].in_use ||
        files[idx].closing)
    {
        spinlock_unlock(
            &fd_lock);

        errno = EBADF;
        return -1;
    }

    files[idx].pinned++;

    file_t *file =
        &files[idx];

    spinlock_unlock(
        &fd_lock);

    
    
    
    int64_t result =
        file->ops->read(
            file,
            data,
            len);

    spinlock_lock(
        &fd_lock);

    files[idx].pinned--;

    if (files[idx].pinned == 0 &&
        files[idx].closing)
    {
        files[idx].ops->close(
            &files[idx]);

        files[idx].in_use = 0;
    }

    spinlock_unlock(
        &fd_lock);

    errno = SUCCESS;

    return result;
}

int64_t fd_write(
    int fd,
    const uint8_t *data,
    size_t len)
{
    task_t *task =
        task_current();

    if (!task ||
        fd < 0 ||
        fd >= MAX_FDS_PER_TASK ||
        !data)
    {
        errno = EBADF;
        return -1;
    }

    spinlock_lock(
        &fd_lock);

    int idx =
        task->fds[fd];

    if (idx < 0 ||
        !files[idx].in_use ||
        files[idx].closing)
    {
        spinlock_unlock(
            &fd_lock);

        errno = EBADF;
        return -1;
    }

    files[idx].pinned++;

    file_t *file =
        &files[idx];

    spinlock_unlock(
        &fd_lock);

    int64_t result =
        file->ops->write(
            file,
            data,
            len);

    spinlock_lock(
        &fd_lock);

    files[idx].pinned--;

    if (files[idx].pinned == 0 &&
        files[idx].closing)
    {
        files[idx].ops->close(
            &files[idx]);

        files[idx].in_use = 0;
    }

    spinlock_unlock(
        &fd_lock);

    errno = SUCCESS;

    return result;
}

int fd_close(
    int fd)
{
    task_t *task =
        task_current();

    if (!task ||
        fd < 0 ||
        fd >= MAX_FDS_PER_TASK)
    {
        errno = EBADF;
        return -1;
    }

    spinlock_lock(
        &fd_lock);

    int idx =
        task->fds[fd];

    if (idx < 0 ||
        !files[idx].in_use)
    {
        spinlock_unlock(
            &fd_lock);

        errno = EBADF;
        return -1;
    }

    
    
    task->fds[fd] = -1;

    if (files[idx].pinned > 0)
    {
        
        
        
        
        files[idx].closing = 1;
    }
    else
    {
        files[idx].ops->close(
            &files[idx]);

        files[idx].in_use = 0;
    }

    spinlock_unlock(
        &fd_lock);

    errno = SUCCESS;

    return 0;
}

void fd_close_all(
    task_t *task)
{
    if (!task)
    {
        return;
    }

    for (int i = 0; i < MAX_FDS_PER_TASK; i++)
    {
        if (task->fds[i] != -1)
        {
            fd_close(i);
        }
    }
}



static int64_t console_file_read(
    file_t *file,
    uint8_t *data,
    size_t len)
{
    (void)file;

    for (size_t i = 0; i < len; i++)
    {
        data[i] =
            (uint8_t)console_getc();
    }

    return (int64_t)len;
}

static int64_t console_file_write(
    file_t *file,
    const uint8_t *data,
    size_t len)
{
    (void)file;

    for (size_t i = 0; i < len; i++)
    {
        console_putc(
            (char)data[i]);
    }

    return (int64_t)len;
}

static void console_file_close(
    file_t *file)
{
    (void)file;

    
    
}

static const file_ops_t console_ops = {
    console_file_read,
    console_file_write,
    console_file_close,
};

void fd_init_task(
    task_t *task)
{
    for (int i = 0; i < MAX_FDS_PER_TASK; i++)
    {
        task->fds[i] = -1;
    }

    fd_open_for(task, &console_ops, 0); 
    fd_open_for(task, &console_ops, 0); 
    fd_open_for(task, &console_ops, 0); 
}



static int64_t pipe_file_read(
    file_t *file,
    uint8_t *data,
    size_t len)
{
    return (int64_t)pipe_read(
        (pipe_t *)file->ctx,
        data,
        len);
}

static int64_t pipe_file_write(
    file_t *file,
    const uint8_t *data,
    size_t len)
{
    return (int64_t)pipe_write(
        (pipe_t *)file->ctx,
        data,
        len);
}

static void pipe_file_close(
    file_t *file)
{
    pipe_close(
        (pipe_t *)file->ctx);
}

static const file_ops_t pipe_file_ops = {
    pipe_file_read,
    pipe_file_write,
    pipe_file_close,
};

int fd_open_pipe(
    void *pipe)
{
    return fd_open(
        &pipe_file_ops,
        pipe);
}