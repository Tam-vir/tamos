#include "command.h"

#include "stdio.h"
#include "string.h"

static command_t commands[MAX_COMMANDS];

static int command_count = 0;

void command_register(
    const char *name,
    const char *description,
    command_handler_t handler)
{
    if (command_count >= MAX_COMMANDS)
        return;

    commands[command_count].name = name;

    commands[command_count].description =
        description;

    commands[command_count].handler =
        handler;

    command_count++;
}

static command_t *find_command(
    const char *name)
{
    for (int i = 0;
         i < command_count;
         i++)
    {
        if (kstr_equal(
                name,
                commands[i].name))
        {
            return &commands[i];
        }
    }

    return 0;
}

static int tokenize(
    char *input,
    char **argv)
{
    int argc = 0;

    while (*input && argc < MAX_ARGS)
    {

        while (*input == ' ')
            input++;

        if (*input == '\0')
            break;

        argv[argc++] = input;

        while (*input &&
               *input != ' ')
        {
            input++;
        }

        if (*input)
        {
            *input = '\0';
            input++;
        }
    }

    return argc;
}

void command_execute(
    char *input)
{

    char *argv[MAX_ARGS];

    int argc =
        tokenize(
            input,
            argv);

    if (argc == 0)
        return;

    command_t *cmd =
        find_command(argv[0]);

    if (!cmd)
    {
        kprintf(
            "Unknown command: %s\n",
            argv[0]);

        return;
    }

    cmd->handler(
        argc,
        argv);
}