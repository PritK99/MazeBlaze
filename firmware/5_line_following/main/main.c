/*Understanding the flow of code :-
Here we create two tasks , namely line follow task and turn task simultanously .
The line follow task starts by getting the raw LSA values and then calculating the correction if any . Then it feeds this correction to motor by passing argument to set_motor_speed() . It also executes turning when a node is spotted .
The front LSA is used to note that a path exists and the back sensors are used to detect when to take sharp turn */

#include <stdio.h>
#include "mazeblaze2.h"
#include "line_following.h"
#include "tuning_http_server.h"

#define GOOD_DUTY_CYCLE 67
#define MIN_DUTY_CYCLE 55
#define MAX_DUTY_CYCLE 80

int actiavate_left_counter =0;
int actiavate_right_counter =0;
int Turn ;
float error=0, prev_error=0, difference, cumulative_error, correction; 
float left_duty_cycle = 0, right_duty_cycle = 0;
int prev_Turn ;
int prev_lsa1_counter = 0 ; 
int prev_lsa3_counter = 0 ; 
bool straight_possible = false ;

void circular_defn(int change_in_dir) ;
float bound(float val, float min, float max) ;
void calculate_correction() ;
void calculate_error() ;

void turn_task(void *arg)
{
    while(1)
    {
        get_raw_lsa() ;

        /* This part detects if there is a node and if there is , it reduces the speed to pwm 65 and works on counters*/

        if (lsa_reading[0] == 1) /*A path in left detected once */
        {
            if (lsa_reading[0] != prev_lsa0_value) /*records change in values i.e. increments if there is any difference between 2 values*/
            {
                actiavate_left_counter ++ ;
                prev_lsa0_value = lsa_reading[0] ;
            }
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;//Reduce pwm to 65 to slow down to bot 
            set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 65) ;
        }
        if (lsa_reading[4] == 1) /*A path in right detected first*/
        {
            actiavate_right_counter ++ ;    //we dont care about the number of paths in right , since it has to get the fist right it sees
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;//Reduce pwm to 65 to slow down to bot 
            set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 65) ;
        }
        if( (actiavate_left_counter > 0 )&& (actiavate_right_counter > 0 )&& ( lsa_reading[1] == 1) && ( lsa_reading[2] == 1 ) && ( lsa_reading[3] == 1 )) /*straight does exsist even though left or right counter are incremented*/
        {
            straight_possible = true ;
        }
        if ((lsa_reading[0] == 1 ) && (lsa_reading[1] == 1 ) &&  (lsa_reading[2] == 1 ) &&  (lsa_reading[3] == 1 ) &&  lsa_reading[4] == 1 )  //when the entire array of IR's read white
        {
            is_end = true ;
        }

        /*This part of the code takes action on the basis of the node detected*/

        if (actiavate_left_counter > 0 && lsa_reading[5] == 1)      //takes last left it sees
        {
            circular_defn(-2) ;     //for turning left , we do -2 
            do
            {
                set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ; //motor A_0 has to go forward and motor A_1 has to go backwards to take left 
                set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 65) ;
                if (lsa_reading[2] != prev_lsa2_value)
                    {
                        actiavate_left_counter -- ;     //decrement the counter and avoid the path since it not the leftmost path
                        prev_lsa2_value = lsa_reading[2] ;
                    }
            }
            while (( lsa_reading[1] == 0) && ( lsa_reading[2] == 0 ) && ( lsa_reading[3] == 0 ) && actiavate_left_counter <= 2) ;
        }
        else if ( straight_possible )
        {
            circular_defn(0) ;      //no change in direction
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 75) ;//Increase PWM since we have to go straight 
            set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 75) ;
        }
        else if (actiavate_right_counter > 0 && lsa_reading[6] == 1)    //takes first right it sees
        {
            circular_defn(+2) ;     //for turning right , we do +2 
            do
            {
                set_motor_speed(MOTOR_A_0 , MOTOR_BACKWARD , 65) ;  //motor A_0 has to go backward and motor A_1 has to go forwards to take right 
                set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 65) ;
                /*we dont care for actiavate_right_counter value here , since it has to take the fist possible path it gets in right as per left_follow*/
            }
            while ( (lsa_reading[1] == 0) && ( lsa_reading[2] == 0) && ( lsa_reading[3] == 0) ) ;
        } 
        else if (lsa_reading[0] == 0 && lsa_reading[1] == 0 && lsa_reading[2] == 0 && lsa_reading[3] == 0 && lsa_reading[4] == 0 )
        {
            /*dead end*/
            circular_defn(-4) ;
            do
            {
                set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ; //this will turn towards left
                set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 65) ;
            }
            while (( lsa_reading[1] == 0) && ( lsa_reading[2] == 0 ) && ( lsa_reading[3] == 0 ) ) ;
        }  
        else if (is_end)
        {
            led_blink_at_end() ;
        }

        /*resets all the counters*/
        actiavate_left_counter = 0 ;
        actiavate_left_counter = 0 ;
        straight_possible = 0 ;      
        
    }
    vTaskDelete(NULL);
}
//end of task

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

void app_main()
{
    xTaskCreate(&turn_task, "line_follow_task", 4096, NULL, 1, NULL);    //creating a task to start line following
    xTaskCreate(&line_follow_task, "line_follow_task", 4096, NULL, 1, NULL);    //creating a task to start line following
    start_tuning_http_server();    
}

/*
possible cases of errors :-
1) All the front sensor read black
2) if lsa_reading [1] != lsa_reading [2] != lsa_reading [3]

error to right of line is positive while left of line is negative
*/

// void turns(void *args)
// {
//     turns();
// }

void calculate_error()
{
    if (lsa_reading[1] == 0 && lsa_reading[2] == 0 && lsa_reading[3] == 0 )
    {

        if(prev_error > 0) //we use prev_error to extract the sign to give to error
        {
            error = 2.5;    //arbitrary value
        }
        else 
        {
            error = -2.5;   //arbitrary value
        }
    }
    else if (lsa_reading[1] == 0 && lsa_reading[3] == 1000)
    {
        /* turn right to nullify */
        error = 1 ;     //arbitrary value
    }
    else if (lsa_reading[1] == 1000 && lsa_reading[3] == 0)
    {
        /* turn left to nullify */
        error = -1 ;    //arbitrary value
    }
    else if (lsa_reading[1] == 1000 && lsa_reading[2] == 1000 && lsa_reading[3] == 1000)
    {
        /* no error */
        error = 0 ;     //arbitrary value
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

void circular_defn(int change_in_dir)
{
    /*The use of this function is have a circular defination from 1 to 8 
    1 - W , 2 - NW , 3 - N , 4 - NE , 5 - E , 6 - SE , 7 - S , 8 - SW 
    The difference of 4 stands for u turn i.e. dead end . this will help us to delete that path from char array 
    subtracting 4 from 1 will give 5 according tio this function */
    int temp = Turn ;
    Turn = prev_Turn + change_in_dir ; //for LFR , change_in_dir = -2 means left
    prev_Turn = temp ; 

    if (Turn <= 0)
    {
        Turn = Turn + 8 ;
        /*fpr eg . if you were moving in west i.e. 1 and then you were to take left . As per LFR , your new direction is 1 - 2 = -1 ;
        now -1 + 8 is 7 which is 7 . Now using LFR you are facing lsouth which is also 7*/
    }
    if (Turn <= 0)
    {
        Turn = Turn - 8 ;
        /*fpr eg . if you were moving in south i.e. 7 and then you were to take right . As per LFR , your new direction is 7 + 2 = 9 ;
        now 9 - 8 is 1 which is 1 . Now using LFR you are facing west which is also 1*/
    }
}

float bound(float val, float min, float max) //To bound a certain value in range MAX to MIN 
{
    if (val > max)
        val = max;
    else if (val < min)
        val = min;
    return val;
}



