#include <stdio.h>
#include "mazeblaze2.h"
#include "rotary_encoder.h"
/* 1 - w 
2 - nw
3 - n 
4 - ne 
5 - e 
6 - se 
7 - s
8 - sw */


#define WEST 1
#define SOUTH 7
#define EAST 5
#define NORTH 3

char path[200] = "";
char dir[200] = "";
float deg[200] = "";
int turn;
int actiavate_left_counter =0;
int actiavate_right_counter =0;
int prev_lsa1_counter = 0 ; 
int prev_lsa3_counter = 0 ; 
int count = 0;
int straight;


int dirFunc(int a) // for determining direction
{
    if (a == 1 || a == -5) // for first left turn, the bot will be facing WEST(from NORTH initially). And one counter less for 3rd left turn
    {
        printf("\tWEST");
    }
    if (a == 7 || a == -7) // similarly for 2nd turn and so on
    {
        printf("\tSOUTH");
    }
    if (a == 5 || a == -1)
    {
        printf("\tEAST");
    }
    if (a == 3 || a == -3 || a == 0)
    {
        printf("\tNORTH");
        count = 0;
    }
    return 0;
}

char select_turn()
{
    if (lsa_reading[0] == 1) //left turn possible
    {
      actiavate_left_counter++;
      deg = (state.position)*0.055; //this is not degree (dist actually), have to add some constraints and formulae in order to store it as deg
      count += 2;
      dirFunc(count);
      
    }

    if (lsa_reading[4]==1 && lsa_reading[0] != 1 && lsa_reading[1]!=1 && lsa_reading[2]!=1 && lsa_reading[3]!=1 && lsa_reading[6] != 1) //right turn possible
    {
        set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;//Reduce pwm to 65 to slow down to bot 
        set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 65) ;

        if (lsa_reading[1] == 1 && lsa_reading[2] == 1 && lsa_reading[3] == 1 && lsa_reading[6] == 1)
        {
            straight++;
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 75) ;//Increase PWM since we have to go straight 
            set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 75) ;

        }

        else if (lsa_reading[6] == 1 && lsa_reading[1] != 1 && lsa_reading[2] != 1 && lsa_reading[3] != 1)
        {
            actiavate_right_counter++;
            count -= 2;
            dirFunc(count);
            set_motor_speed(MOTOR_A_0, MOTOR_FORWARD, 65);
            set_motor_speed(MOTOR_A_1, MOTOR_BACKWARD, 65);
        }
        
    }

    if (lsa_reading[4] == 1)
    {
        actiavate_right_counter++;
        count -= 2;
        dirFunc(count);
    }
}

void redundant_path()
{

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

            

            if (_turn != E)
            {
                ESP_LOGI("debug", "Itterations: %d", prev);
                prev = 0;
                take_turn(palat);
                ESP_LOGI("debug", "Turn: %d", _turn);

                selection();
                unsigned char dir = select_turn();
                path[path_length] = dir;
                path_length++;

                // You should check to make sure that the path_length does not
                // exceed the bounds of the array.  We'll ignore that in this
                // example.

                // Simplify the learned path.
                simplify_path();
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
}