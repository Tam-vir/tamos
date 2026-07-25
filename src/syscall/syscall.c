#include "syscall.h"

#include "fd.h"
#include "task.h"
#include "errno.h"

static int64_t sys_read(
    trap_context_t *ctx)
{
    return fd_read(
        (int)ctx->a0,
        (uint8_t *)ctx->a1,
        (size_t)ctx->a2);
}

static int64_t sys_write(
    trap_context_t *ctx)
{
    return fd_write(
        (int)ctx->a0,
        (const uint8_t *)ctx->a1,
        (size_t)ctx->a2);
}

static int64_t sys_close(
    trap_context_t *ctx)
{
    return fd_close(
        (int)ctx->a0);
}

static int64_t sys_exit(
    trap_context_t *ctx)
{
    (void)ctx;

    
    
    
    
    task_exit();

    return 0;
}

static int64_t sys_sleep(
    trap_context_t *ctx)
{
    task_sleep(
        (uint64_t)ctx->a0);

    return 0;
}

static int64_t sys_getpid(
    trap_context_t *ctx)
{
    (void)ctx;

    task_t *task =
        task_current();

    return task
               ? (int64_t)task->pid
               : -1;
}

typedef int64_t (*syscall_fn_t)(
    trap_context_t *ctx);

static syscall_fn_t syscall_table[SYS_COUNT] = {
    0, 
    sys_read,
    sys_write,
    sys_close,
    sys_exit,
    sys_sleep,
    sys_getpid,
};

void syscall_dispatch(
    trap_context_t *ctx)
{
    uint64_t nr =
        ctx->a7;

    if (nr >= SYS_COUNT ||
        syscall_table[nr] == 0)
    {
        errno = ENOSYS;

        ctx->a0 =
            (uint64_t)-1;

        return;
    }

    ctx->a0 =
        (uint64_t)
            syscall_table[nr](ctx);
}