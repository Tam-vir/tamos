#ifndef SYSCALL_H
#define SYSCALL_H

#include "trap.h"




#define SYS_YIELD 0
#define SYS_READ 1
#define SYS_WRITE 2
#define SYS_CLOSE 3
#define SYS_EXIT 4
#define SYS_SLEEP 5
#define SYS_GETPID 6
#define SYS_COUNT 7

void syscall_dispatch(
    trap_context_t *ctx);

#endif