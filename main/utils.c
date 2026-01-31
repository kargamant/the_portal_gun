#include "utils.h"
#include "driver/gpio.h"

void init_led(uint8_t led_n)
{
    gpio_reset_pin(led_n);
    gpio_set_direction(led_n, GPIO_MODE_OUTPUT);
}   