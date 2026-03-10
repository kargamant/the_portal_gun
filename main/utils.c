#include "utils.h"
extern dac_continuous_handle_t speaker_handle;
extern dac_continuous_config_t speaker_config;

void init_led(uint8_t led_n)
{
    gpio_reset_pin(led_n);
    gpio_set_direction(led_n, GPIO_MODE_OUTPUT);
}   

void play_audio(uint8_t* audio, size_t size)
{
    dac_continuous_new_channels(&speaker_config, &speaker_handle);
    dac_continuous_enable(speaker_handle);

    size_t bytes_loaded;
    dac_continuous_write(speaker_handle, audio, size, &bytes_loaded, -1);

    dac_continuous_disable(speaker_handle);
    dac_continuous_del_channels(speaker_handle);
}
