#include "unistd.h"
#include "syscall.h"

static inline int64_t syscall3(
    int64_t nr,
    int64_t a0,
    int64_t a1,
    int64_t a2)
{
    register int64_t r_a7 asm("a7") = nr;
    register int64_t r_a0 asm("a0") = a0;
    register int64_t r_a1 asm("a1") = a1;
    register int64_t r_a2 asm("a2") = a2;

    asm volatile(
        "ecall"
        : "+r"(r_a0)
        : "r"(r_a7), "r"(r_a1), "r"(r_a2)
        : "memory");

    return r_a0;
}

static inline int64_t syscall1(
    int64_t nr,
    int64_t a0)
{
    return syscall3(nr, a0, 0, 0);
}

static inline int64_t syscall0(
    int64_t nr)
{
    return syscall3(nr, 0, 0, 0);
}

int64_t read(
    int fd,
    void *buf,
    size_t count)
{
    return syscall3(
        SYS_READ,
        fd,
        (int64_t)buf,
        (int64_t)count);
}

int64_t write(
    int fd,
    const void *buf,
    size_t count)
{
    return syscall3(
        SYS_WRITE,
        fd,
        (int64_t)buf,
        (int64_t)count);
}

int close(
    int fd)
{
    return (int)syscall1(
        SYS_CLOSE,
        fd);
}

void _exit(void)
{
    syscall0(
        SYS_EXIT);

    
    while (1)
    {
        asm volatile("wfi");
    }
}

void sleep(
    uint64_t ticks)
{
    syscall1(
        SYS_SLEEP,
        (int64_t)ticks);
}

int getpid(void)
{
    return (int)syscall0(
        SYS_GETPID);
}