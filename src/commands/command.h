#ifndef COMMAND_H
#define COMMAND_H

#define MAX_COMMANDS 32
#define MAX_ARGS 16

typedef void (*command_handler_t)(
    int argc,
    char **argv);

typedef struct
{
    const char *name;
    const char *description;

    command_handler_t handler;

} command_t;

void command_init(void);

void command_execute(
    char *input);

void command_register(
    const char *name,
    const char *description,
    command_handler_t handler);

#endif