#include "uart.h"

#include "ring.h"
#include "spinlock.h"
#include "interrupt.h"
#include "plic.h"
#include "waitqueue.h"

#define UART_BASE 0x10000000UL

#define UART_RBR 0
#define UART_THR 0
#define UART_IER 1
#define UART_FCR 2
#define UART_LCR 3
#define UART_MCR 4
#define UART_LSR 5

#define UART_IRQ 10

#define UART_LSR_DATA_READY 0x01
#define UART_LSR_THR_EMPTY 0x20

static volatile uint8_t *uart =
    (volatile uint8_t *)UART_BASE;

static ring_t uart_rx_buffer;

static spinlock_t uart_lock;

static wait_queue_t uart_waitqueue;

void uart_init(void)
{
    ring_init(
        &uart_rx_buffer);

    spinlock_init(
        &uart_lock);

    waitqueue_init(
        &uart_waitqueue);

    uart[UART_FCR] =
        0x07;

    uart[UART_IER] =
        0x00;
}

void uart_enable_interrupts(void)
{

    interrupt_register(
        UART_IRQ,
        uart_interrupt_handler);

    plic_set_priority(
        UART_IRQ,
        1);

    plic_enable_irq(
        UART_IRQ);

    uart[UART_IER] =
        0x01;
}

void uart_putc(
    char c)
{

    while (!(uart[UART_LSR] &
             UART_LSR_THR_EMPTY))
    {
        asm volatile(
            "wfi");
    }

    uart[UART_THR] =
        c;
}

void uart_puts(
    const char *str)
{

    while (*str)
    {

        if (*str == '\n')
        {
            uart_putc('\r');
        }

        uart_putc(
            *str++);
    }
}

void uart_interrupt_handler(void)
{

    spinlock_lock(
        &uart_lock);

    while (uart[UART_LSR] &
           UART_LSR_DATA_READY)
    {

        uint8_t c =
            uart[UART_RBR];

        ring_push(
            &uart_rx_buffer,
            c);
    }

    spinlock_unlock(
        &uart_lock);

    waitqueue_wake_all(
        &uart_waitqueue);
}

char uart_getc(void)
{
    uint8_t c;

    while (1)
    {
        spinlock_lock(
            &uart_lock);

        if (!ring_empty(
                &uart_rx_buffer))
        {
            ring_pop(
                &uart_rx_buffer,
                &c);

            spinlock_unlock(
                &uart_lock);

            return (char)c;
        }

        spinlock_unlock(
            &uart_lock);

        if (uart[UART_LSR] &
            UART_LSR_DATA_READY)
        {
            c = uart[UART_RBR];
            return (char)c;
        }

        asm volatile(
            "wfi");
    }
}

void uart_gets(
    char *buffer,
    size_t max_len)
{

    size_t i = 0;

    while (1)
    {

        char c =
            uart_getc();

        if (c == '\r' ||
            c == '\n')
        {

            buffer[i] =
                '\0';

            uart_puts(
                "\n");

            return;
        }

        if (c == '\b' ||
            c == 127)
        {

            if (i > 0)
            {

                i--;

                uart_puts(
                    "\b \b");
            }

            continue;
        }

        if (c >= ' ' &&
            c <= '~' &&
            i < max_len - 1)
        {

            buffer[i++] =
                c;

            uart_putc(
                c);
        }
    }
}