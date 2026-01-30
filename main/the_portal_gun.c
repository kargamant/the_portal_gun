#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "esp_intr_alloc.h"

#define PORTAL_TOGGLE_PIN 16
#define PORTAL_LIGHT_1_GREEN 17
#define PORTAL_LIGHT_2_GREEN 18
#define PORTAL_LIGHT_3_GREEN 19

void portal_toggled(void* arg)
{
    static uint8_t state = 0;

    state = ~state;
    state &= 0x01;
    gpio_set_level(PORTAL_LIGHT_1_GREEN, state);
    gpio_set_level(PORTAL_LIGHT_2_GREEN, state);
    gpio_set_level(PORTAL_LIGHT_3_GREEN, state);
}

void app_main(void)
{
    gpio_reset_pin(PORTAL_TOGGLE_PIN);
    gpio_reset_pin(PORTAL_LIGHT_1_GREEN);
    gpio_set_direction(PORTAL_LIGHT_1_GREEN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(PORTAL_LIGHT_2_GREEN);
    gpio_set_direction(PORTAL_LIGHT_2_GREEN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(PORTAL_LIGHT_3_GREEN);
    gpio_set_direction(PORTAL_LIGHT_3_GREEN, GPIO_MODE_OUTPUT);

    
    
    printf("glow mf!!!!!!!!!!!!\n");
    gpio_set_level(PORTAL_LIGHT_1_GREEN, 1);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    gpio_set_level(PORTAL_LIGHT_1_GREEN, 0);

    gpio_config_t button_config = {
        .pin_bit_mask = (1ULL << PORTAL_TOGGLE_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };

    gpio_config(&button_config);
    
    gpio_install_isr_service(0);
    
    gpio_isr_handler_add(PORTAL_TOGGLE_PIN, portal_toggled, NULL);

    //gpio_isr_register(&portal_toggled, NULL, 0, &gpio_handler);


}
