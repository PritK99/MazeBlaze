#include <stdio.h>
#include "mazeblaze2.h"

#define GOOD_DUTY_CYCLE 67
#define MIN_DUTY_CYCLE 55
#define MAX_DUTY_CYCLE 80

int actiavate_left_counter =0;
int actiavate_right_counter =0;
char Turn ;
float error=0, prev_error=0, difference, cumulative_error, correction; 
float left_duty_cycle = 0, right_duty_cycle = 0;

<<<<<<< HEAD
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
=======
/*
possible cases of errors :-
1) All the front sensor read black
2) if lsa_radings [1] != lsa_radings [2] != lsa_radings [3]

error to right of line is positive while left of line is negative
*/
void calculate_error()
{
    if (lsa_reading[1] == 0 && lsa_reading[2] == 0 && lsa_reading[3] == 0 )
    {

        if(prev_error > 0) //we use prev_error to extract the sign to give to error
        {
            error = 2.5;
        }
        else 
        {
            error = -2.5;
        }
    }
    else if (lsa_readings[1] == 0 && lsa_readings[3] == 1)
>>>>>>> 253bccb81c0329507b01baa090ef205fcd3da804
    {
        /* turn right to nullify */
        error = 1 ;
    }
    else if (lsa_readings[1] == 1 && lsa_readings[3] == 0)
    {
        /* turn left to nullify */
        error = -1 ;
    }
    else if (lsa_radings[1] == lsa_radings[2] == lsa_radings[3] == 1)
    {
        /* no error */
        error = 0 ; 
    }
    else
    {
        error = prev_error ; //this case is for safety
    }   
}

void calculate_correction()
{
    error = error*10;  // we need the error correction in range 0-100 so that we can send it directly as duty cycle paramete
    difference = error - prev_error;    //used for calcuating kd 
    cumulative_error += error;     //used for calculating ki

    cumulative_error = bound(cumulative_error, -30, 30); //bounding cumulative_error to avoid the issue of cumulative_error being very large

    correction = read_pid_const().kp*error + read_pid_const().ki*cumulative_error + read_pid_const().kd*difference; //defined in http_server.c

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

        left_duty_cycle = bound((GOOD_DUTY_CYCLE - correction), MIN_DUTY_CYCLE , MAX_DUTY_CYCLE);
        right_duty_cycle = bound((GOOD_DUTY_CYCLE + correction), MIN_DUTY_CYCLE , MAX_DUTY_CYCLE);

        set_motor_speed(MOTOR_A_0, MOTOR_FORWARD, left_duty_cycle);
        set_motor_speed(MOTOR_A_1, MOTOR_FORWARD, right_duty_cycle);

        /*prints on terminal*/
        ESP_LOGI("debug","left_duty_cycle:  %f    ::  right_duty_cycle :  %f  :: error :  %f  correction  :  %f  \n",left_duty_cycle, right_duty_cycle, error, correction);
        ESP_LOGI("error_correction", "KP: %f ::  KI: %f  :: KD: %f", read_pid_const().kp, read_pid_const().ki, read_pid_const().kd);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

float bound(float val, float min, float max) //To bound a certain value in range MAX to MIN 
{
    if (val > max)
        val = max;
    else if (val < min)
        val = min;
    return val;
}

void app_main()
{
    xTaskCreate(&line_follow_task, "line_follow_task", 4096, NULL, 1, NULL);    //creating a task to start line following
    start_tuning_http_server();    
}
