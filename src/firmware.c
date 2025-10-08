#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);
}

static void delay(uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        __asm__("nop");
    }
}

int main(void) {
    gpio_setup();

    while (1) {
        gpio_toggle(GPIOA, GPIO5);
        delay(50000);
    }
}
