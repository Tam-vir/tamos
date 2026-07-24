#include "shell.h"

#include "uart.h"
#include "stdio.h"

#include "command.h"


extern void cmd_help(
    int argc,
    char **argv);

extern void cmd_echo(
    int argc,
    char **argv);

extern void cmd_time(
    int argc,
    char **argv);

extern void cmd_ticks(
    int argc,
    char **argv);

extern void cmd_clear(
    int argc,
    char **argv);

extern void cmd_exit(
    int argc,
    char **argv);

extern void cmd_cpuinfo(
    int argc,
    char **argv);

extern void cmd_mem(
    int argc,
    char **argv);

void shell_init(void)
{

    command_register(
        "help",
        "Show commands",
        cmd_help);

    command_register(
        "echo",
        "Print text",
        cmd_echo);

    command_register(
        "time",
        "Show uptime",
        cmd_time);

    command_register(
        "ticks",
        "Show timer ticks",
        cmd_ticks);

    command_register(
        "clear",
        "Clear terminal",
        cmd_clear);

    command_register(
        "cpuinfo",
        "Show CPU information",
        cmd_cpuinfo);

    command_register(
        "mem",
        "Show memory information",
        cmd_mem);

    command_register(
        "exit",
        "Shutdown CPU",
        cmd_exit);

    kprintf(
        "\nTamOS shell ready\n");
}

void shell_run(void)
{

    char buffer[128];

    while (1)
    {

        kprintf(">> ");

        kscanf(
            buffer,
            sizeof(buffer));

        command_execute(
            buffer);
    }
}