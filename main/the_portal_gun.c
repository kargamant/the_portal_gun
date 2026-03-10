#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_intr_alloc.h"
#include "utils.h"
#include "driver/dac_continuous.h"
#include "constants.h"

TaskHandle_t audio_task_handle = NULL;

void portal_toggled(void* arg)
{
    static uint8_t state = 0;

    state = ~state;
    state &= 0x01;
    gpio_set_level(PORTAL_LIGHT_1_GREEN, state);
    gpio_set_level(PORTAL_LIGHT_2_GREEN, state);
    gpio_set_level(PORTAL_LIGHT_3_GREEN, state);

    BaseType_t status = pdTRUE;
    vTaskNotifyGiveFromISR(audio_task_handle, &status);
}

void audio_task(void* params)
{
    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        dac_continuous_config_t speaker_config = {
            .chan_mask = DAC_CHANNEL_MASK_CH1,
            .desc_num = 4,
            .buf_size = 2048,
            .freq_hz = 16000,
            .offset = 0,
            .clk_src = DAC_DIGI_CLK_SRC_APLL,
        };

        dac_continuous_new_channels(&speaker_config, &speaker_handle);
        dac_continuous_enable(speaker_handle);

        size_t bytes_loaded;
        dac_continuous_write(speaker_handle, portal_audio, portal_audio_size, &bytes_loaded, -1);

        dac_continuous_disable(speaker_handle);
        dac_continuous_del_channels(speaker_handle);
    }
}

void app_main(void)
{
    gpio_reset_pin(PORTAL_TOGGLE_PIN);
    
    // initing lights
    init_led(PORTAL_LIGHT_1_GREEN);
    init_led(PORTAL_LIGHT_2_GREEN);
    init_led(PORTAL_LIGHT_3_GREEN);
    
    printf("glow mf!!!!!!!!!!!!\n");
    gpio_set_level(PORTAL_LIGHT_1_GREEN, 1);
    vTaskDelay(10000 / portTICK_PERIOD_MS);
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

    xTaskCreate(audio_task, "audio_task", 1024, NULL, tskIDLE_PRIORITY + 1, &audio_task_handle);
}
