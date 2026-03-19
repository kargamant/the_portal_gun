#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include "driver/dac_oneshot.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_intr_alloc.h"
#include "driver/dac_continuous.h"


void init_led(uint8_t led_n);

void play_audio(uint8_t* audio, size_t size);

void play_audio_inverse(uint8_t* audio, size_t size);

#endif