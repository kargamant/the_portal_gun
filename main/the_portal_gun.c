#include "utils.h"
#include "constants.h"

static uint8_t state = 0;

void portal_toggled(void* arg)
{
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

        if(state)
            play_audio(portal_audio, portal_audio_size);
        else
            play_audio_inverse(portal_audio, portal_audio_size);    
    }
}

void app_main(void)
{
    // toggle init
    gpio_reset_pin(PORTAL_TOGGLE_PIN);
    
    // initing lights
    init_led(PORTAL_LIGHT_1_GREEN);
    init_led(PORTAL_LIGHT_2_GREEN);
    init_led(PORTAL_LIGHT_3_GREEN);
    
    // for debug only
    printf("glow mf!!!!!!!!!!!!\n");
    gpio_set_level(PORTAL_LIGHT_1_GREEN, 1);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    gpio_set_level(PORTAL_LIGHT_1_GREEN, 0);

    // button configuration
    gpio_config(&button_config);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(PORTAL_TOGGLE_PIN, portal_toggled, NULL);

    // audio task registration
    xTaskCreate(audio_task, "audio_task", 1024, NULL, tskIDLE_PRIORITY + 1, &audio_task_handle);
}
