#include <stdio.h>
#include "mazeblaze2.h"

int actiavate_left_counter =0;
int actiavate_right_counter =0;
char Turn ;

while(1)
{
    get_raw_lsa() ;
    turn_detection();
    turn();
               
    
}

/*********************************************************************************/

void calculate_error()
{
    int all_black_flag = 1; // assuming initially all black condition
    float weighted_sum = 0, sum = 0; 
    float pos = 0; 
    
    for(int i = 0; i < 4; i++)
    {
        if(line_sensor_readings.adc_reading[i] > BLACK_MARGIN)
        {
            all_black_flag = 0;
        }
        weighted_sum += (float)(weights[i]) * (line_sensor_readings.adc_reading[i]);
        sum = sum + line_sensor_readings.adc_reading[i];
    }

    if(sum != 0) // sum can never be 0 but just for safety purposes
    {
        pos = weighted_sum / sum; // This will give us the position wrt line. if +ve then bot is facing left and if -ve the bot is facing to right.
    }

    if(all_black_flag == 1)  // If all black then we check for previous error to assign current error.
    {
        if(prev_error > 0)
        {
            error = 2.5;
        }
        else
        {
            error = -2.5;
        }
    }
    else
    {
        error = pos;
    }
}

void calculate_correction()
{
    error = error*10;  // we need the error correction in range 0-100 so that we can send it directly as duty cycle paramete
    difference = error - prev_error;    //used for calcuating kd 
    cumulative_error += error;     //used for calculating ki

    cumulative_error = bound(cumulative_error, -30, 30);

    correction = read_pid_const().kp*error + read_pid_const().ki*cumulative_error + read_pid_const().kd*difference;


    prev_error = error; //update error
}

void line_follow_task(void* arg)
{
    ESP_ERROR_CHECK(enable_lsa()) ;
    ESP_ERROR_CHECK(enable_motor_driver()) ;
    
    while(1)
    {
        get_raw_lsa() ;

        calculate_error();
        calculate_correction();
        
        // left_duty_cycle = bound((optimum_duty_cycle - correction), lower_duty_cycle, higher_duty_cycle);
        // right_duty_cycle = bound((optimum_duty_cycle + correction), lower_duty_cycle, higher_duty_cycle);

        // set_motor_speed(MOTOR_A_0, MOTOR_FORWARD, left_duty_cycle);
        // set_motor_speed(MOTOR_A_1, MOTOR_FORWARD, right_duty_cycle);

        
        // //ESP_LOGI("debug","left_duty_cycle:  %f    ::  right_duty_cycle :  %f  :: error :  %f  correction  :  %f  \n",left_duty_cycle, right_duty_cycle, error, correction);
        // ESP_LOGI("error_correction", "KP: %f ::  KI: %f  :: KD: %f", read_pid_const().kp, read_pid_const().ki, read_pid_const().kd);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

void app_main()
{
    xTaskCreate(&line_follow_task, "line_follow_task", 4096, NULL, 1, NULL);    //creating a task to start line following
    start_tuning_http_server();    
}
