#include <stdio.h>
#include "mazeblaze2.h"
/* 1 - w 
2 - nw
3 - n 
4 - ne 
5 - e 
6 - se 
7 - s
8 - sw */

#define NORTH 3
#define WEST 1
#define SOUTH 7
#define EAST 5

char path[200] = " ";
char dir[200] = " ";
float deg[200] = " ";
int turn;



char select_turn()
{
    if (lsa_reading[0]==1)
    {
      activate_left_counter++;
      deg = (state.position)*0.055;
      
    }

    if (lsa_reading[4]==1)
    {
        activate_right_counter++;
        
    }
}

void maze_explore(void *arg)
{
    ESP_LOGI("debug", "Initializing SPIFFS");
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE("debug", "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE("debug", "Failed to find SPIFFS partition");
        }
        else
        {
            ESP_LOGE("debug", "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK)
    {
        ESP_LOGE("debug", "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    }
    else
    {
        ESP_LOGI("debug", "Partition size: total: %d, used: %d", total, used);
    }
    while (1)
    {
        if (gpio_get_level(DEBUG_SWITCH) == 0)
        {
            int duty_cycle = 75;
            //        ESP_LOGI("debug", "ACTUAL: %d", _turn);
            ESP_LOGI("debug", "Reading file");
            f = fopen("/spiffs/hello.txt", "r");
            if (f == NULL)
            {
                ESP_LOGE("debug", "Failed to open file for reading");
                return;
            }
            char line[64];
            fgets(line, sizeof(line), f);
            fclose(f);
            // strip newline
            char *pos = strchr(line, '\n');
            if (pos)
            {
                *pos = '\0';
            }
            ESP_LOGI("debug", "Read from file: '%s'", line);
            // esp_vfs_spiffs_unregister(conf.partition_label);
            // ESP_LOGI("debug", "SPIFFS unmounted");

