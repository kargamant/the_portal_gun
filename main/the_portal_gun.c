#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "esp_intr_alloc.h"
#include "utils.h"
#include "driver/dac_oneshot.h"
#include "driver/dac_cosine.h"

#define PORTAL_TOGGLE_PIN 16
#define PORTAL_LIGHT_1_GREEN 17
#define PORTAL_LIGHT_2_GREEN 18
#define PORTAL_LIGHT_3_GREEN 19
#define SPEAKER_OUTPUT 26

//dac_oneshot_handle_t speaker_oneshot_handle;
dac_cosine_handle_t speaker_cosine_handle;
dac_cosine_handle_t* speaker_cosine_handle_p = NULL;

uint32_t notes_freq[7] = {261, 293, 329, 349, 392, 440, 493};
// dac_cosine_handle_t note_handlers[7];


void portal_toggled(void* arg)
{
    static uint8_t state = 0;
    static int8_t note_state = 0;
    static int8_t note_direction = 1;

    state = ~state;
    state &= 0x01;
    note_state += note_direction;
    gpio_set_level(PORTAL_LIGHT_1_GREEN, state);
    gpio_set_level(PORTAL_LIGHT_2_GREEN, state);
    gpio_set_level(PORTAL_LIGHT_3_GREEN, state);
    
    //dac_oneshot_output_voltage(speaker_oneshot_handle, state * 255);
    /*if(state)
        dac_cosine_start(speaker_cosine_handle);
    else
        dac_cosine_stop(speaker_cosine_handle);*/

    if(speaker_cosine_handle_p)
    {
        dac_cosine_stop(*speaker_cosine_handle_p);
        dac_cosine_del_channel(*speaker_cosine_handle_p);
    }
    else
        speaker_cosine_handle_p = &speaker_cosine_handle;
    
    dac_cosine_config_t speaker_cosine_config = {
            .chan_id = DAC_CHAN_1,
            .freq_hz = notes_freq[note_state]
    };
    dac_cosine_new_channel(&speaker_cosine_config, speaker_cosine_handle_p);
    
    dac_cosine_start(*speaker_cosine_handle_p);

    if(note_state == 6)
        note_direction = -1;
    if(note_state == 0)
        note_direction = 1;
}

void app_main(void)
{
    gpio_reset_pin(PORTAL_TOGGLE_PIN);
    
    // initing lights
    init_led(PORTAL_LIGHT_1_GREEN);
    init_led(PORTAL_LIGHT_2_GREEN);
    init_led(PORTAL_LIGHT_3_GREEN);

    // initing dac_oneshot
    // dac_oneshot_config_t speaker_oneshot_config = {
    //     .chan_id = DAC_CHAN_1
    // };

    // dac_oneshot_new_channel(&speaker_oneshot_config, &speaker_oneshot_handle);
    
    // initing dac_cosine
    // dac_cosine_config_t speaker_cosine_config = {
    //     .chan_id = DAC_CHAN_1,
    //     .freq_hz = 10000
    // };

    // dac_cosine_new_channel(&speaker_cosine_config, &speaker_cosine_handle);

    // notes LOL
    // for(int i=0; i<7; i++)
    // {
    //     dac_cosine_config_t speaker_cosine_config = {
    //         .chan_id = DAC_CHAN_1,
    //         .freq_hz = notes_freq[i],
    //         .force_set_freq = true
    //     };

    //     dac_cosine_new_channel(&speaker_cosine_config, note_handlers + i);
    // }

    
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
