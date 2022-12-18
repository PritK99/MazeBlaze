#include "mazeblaze2.h"

void app_main(void)
{

    ESP_ERROR_CHECK(enable_motor_driver());

    // code for turning - clockwise
    while (1)
    {
        //Code for acceleration and retardation
    for ( int i = 1 ; i < 99 ; i++ ) //acceleration
        {
            set_motor_speed(MOTOR_A_0, 1, i);
            set_motor_speed(MOTOR_A_1, 1, i);
            vTaskDelay(100 / portTICK_RATE_MS);
        }
        for (int i = 99; i > 1; i--) // retardation
        {
            set_motor_speed(MOTOR_A_0, 1, i);
            set_motor_speed(MOTOR_A_1, 1, i);
            vTaskDelay(100 / portTICK_RATE_MS);
        }
    }
}
