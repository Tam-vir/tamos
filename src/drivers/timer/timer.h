#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#define TIMER_INTERVAL 1000000ULL
extern volatile uint64_t tick_count;
extern volatile uint64_t next_timer_tick;
extern volatile int timer_fired;

void timer_boot_init(void);

void timer_enable(void);

uint64_t timer_get_ticks(void);

uint64_t timer_get_seconds(void);

#endif