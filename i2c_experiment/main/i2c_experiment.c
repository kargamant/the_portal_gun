#include <stdio.h>
#include "driver/i2c_master.h"

i2c_master_bus_config_t screen_mst_config = {
    .i2c_port = -1,
    .sda_io_num = 21,
    .scl_io_num = 22,
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .glitch_ignore_cnt = 7,
    //.intr_priority = 0,
    //.trans_queue_depth = 4,
    .flags.enable_internal_pullup = true,
    //.allow_pd = false
};

// i2c_device_config_t screen_slv_config = {
//     .dev_addr_length = I2C_ADDR_BIT_LEN_7,
//     .device_address = 0x3c, // or 0x78 or 0x1e
//     .scl_speed_hz = 100000,
//     .scl_wait_us = 20000
// };

void app_main(void)
{
    i2c_master_bus_handle_t master_handle;
    i2c_new_master_bus(&screen_mst_config, &master_handle);

    // i2c_master_dev_handle_t device_handle;
    // i2c_master_bus_add_device(master_handle, &screen_slv_config, &device_handle);

    // check the address of screen
    uint16_t probes[3] = {0x78, 0x3c, 0x1e};
    
    for(int i=0; i<3; i++)
    {
        if(i2c_master_probe(master_handle, probes[i], 5000) == ESP_OK)
        {
            printf("Yes, %x is correct address\n", probes[i]);
            break;
        }
        else
            printf("No, %x is wrong\n", probes[i]);    
    }
}
