#include <stdio.h>
#include <string.h>
#include "driver/i2c_master.h"

i2c_master_bus_config_t screen_mst_config = {
    .i2c_port = -1,
    .sda_io_num = 21,
    .scl_io_num = 22,
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .glitch_ignore_cnt = 7,
    .intr_priority = 0,
    //.trans_queue_depth = 4,
    .flags.enable_internal_pullup = true,
    //.allow_pd = false
};

i2c_device_config_t screen_slv_config = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = 0x3c,
    .scl_speed_hz = 100000,
    .scl_wait_us = 20000
};

void app_main(void)
{
    i2c_master_bus_handle_t master_handle;
    i2c_new_master_bus(&screen_mst_config, &master_handle);

    i2c_master_dev_handle_t device_handle;
    i2c_master_bus_add_device(master_handle, &screen_slv_config, &device_handle);

    const uint8_t init_sequence[18] = {
        0xA8, 0x1F, 0xD3, 0x00, 0x40, 0xA0, 0xC0, 0xDA, 0x02, 0x81, 0x7F, 0xA4, 0xA6, 0xD5, 0x80, 0x8D, 0x14, 0xAF
    };

    // initializing whole display
    // for(int i=0; i<18; i++)
    // {
    //     esp_err_t err_code = i2c_master_transmit(device_handle, init_sequence + i, 1, 5000);

    //     switch(err_code)
    //     {
    //         case ESP_OK:
    //             printf("success!\n");
    //             break;
    //         case ESP_ERR_INVALID_RESPONSE:
    //             printf("NACK recieved.\n");
    //             break;
    //         case ESP_ERR_INVALID_ARG:
    //             printf("invalid parameter.\n");
    //             break;
    //         case ESP_ERR_TIMEOUT:
    //             printf("operation timeout.\n");
    //     }
    // }
    esp_err_t err_code = i2c_master_transmit(device_handle, init_sequence, sizeof(init_sequence), 5000);

    switch(err_code)
    {
        case ESP_OK:
            printf("success!\n");
            break;
        case ESP_ERR_INVALID_RESPONSE:
            printf("NACK recieved.\n");
            break;
        case ESP_ERR_INVALID_ARG:
            printf("invalid parameter.\n");
            break;
        case ESP_ERR_TIMEOUT:
            printf("operation timeout.\n");
    }

    uint8_t set_window[] = {0x20, 0x00, 0x21, 0x00, 0x7F, 0x22, 0x00, 0x03};
    i2c_master_transmit(device_handle, set_window, sizeof(set_window), 1000);

    uint8_t white[512];
    memset(white, 0xFF, sizeof(white));
    i2c_master_transmit(device_handle, white, sizeof(white), 2000);

    printf("turn on pls\n");
    // 0x3c was correct
    // check the address of screen
    // uint16_t probes[3] = {0x78, 0x3c, 0x1e};
    
    // for(int i=0; i<3; i++)
    // {
    //     if(i2c_master_probe(master_handle, probes[i], 5000) == ESP_OK)
    //     {
    //         printf("Yes, %x is correct address\n", probes[i]);
    //         break;
    //     }
    //     else
    //         printf("No, %x is wrong\n", probes[i]);    
    // }
}
