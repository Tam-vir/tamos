#include "console.h"

#include "uart.h"
#include "spinlock.h"



static spinlock_t console_lock;



void console_init(void)
{
    spinlock_init(
        &console_lock);

     

    uart_init();
}



void console_putc(
    char c)
{
    spinlock_lock(
        &console_lock);

    uart_putc(
        c);

    spinlock_unlock(
        &console_lock);
}



void console_puts(
    const char *str)
{
    spinlock_lock(
        &console_lock);

    uart_puts(
        str);

    spinlock_unlock(
        &console_lock);
}



void console_clear(void)
{
    spinlock_lock(
        &console_lock);

     

    uart_puts(
        "\033[2J\033[H");

    spinlock_unlock(
        &console_lock);
}



char console_getc(void)
{
    return uart_getc();
}



void console_gets(
    char *buffer,
    size_t size)
{
    size_t i = 0;

    while (1)
    {
        char c =
            console_getc();

        

        if (c == '\r' ||
            c == '\n')
        {
            buffer[i] = '\0';

            console_puts(
                "\n");

            return;
        }

        

        if (c == '\b' ||
            c == 127)
        {
            if (i > 0)
            {
                i--;

                console_puts(
                    "\b \b");
            }

            continue;
        }

        

        if (c >= ' ' &&
            c <= '~' &&
            i < (size - 1))
        {
            buffer[i++] = c;

            console_putc(
                c);
        }
    }
}