#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/vector.h>

#define CPU_FREQ (2000000)
#define SYSTICK_FREQ (1000)

volatile uint64_t ticks = 0;
void sys_tick_handler(void) {
    ticks++;
    gpio_toggle(GPIOA, GPIO5);
}

static uint64_t get_ticks(void) {
    return ticks;
}

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);
}

static void systick_setup(void) {
    systick_set_frequency(SYSTICK_FREQ, CPU_FREQ);
    systick_counter_enable();
    systick_interrupt_enable();
}

int main(void) {
    gpio_setup();
    systick_setup();

    uint64_t start_time = get_ticks();

    while (1) {
        if (get_ticks() - start_time >= SYSTICK_FREQ) {
            start_time = get_ticks();
        }
    }

    return 0;
}
