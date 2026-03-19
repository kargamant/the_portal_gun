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
    // continous dma way
    dac_continuous_new_channels(&speaker_config, &speaker_handle);
    dac_continuous_enable(speaker_handle);

    size_t bytes_loaded;
    dac_continuous_write(speaker_handle, audio, size, &bytes_loaded, -1);

    dac_continuous_disable(speaker_handle);
    dac_continuous_del_channels(speaker_handle);
}

void play_audio_inverse(uint8_t* audio, size_t size)
{
    // continous dma way
    dac_continuous_new_channels(&speaker_config, &speaker_handle);
    dac_continuous_enable(speaker_handle);

    uint8_t chunk[256];
    uint16_t c = 0;

    for(int i=size-1; i>=0; i--)
    {
        chunk[c] = audio[i];
        c++;
        if(c == 256)
        {
            size_t bytes_loaded;
            dac_continuous_write(speaker_handle, chunk, 256, &bytes_loaded, -1);
            c = 0;
        }
    }
    
    dac_continuous_disable(speaker_handle);
    dac_continuous_del_channels(speaker_handle);

}
